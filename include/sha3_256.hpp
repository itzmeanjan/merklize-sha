#pragma once
#include "sha3.hpp"

namespace sha3_256 {

// SHA3-256 specific input/ output width constants, taken from
// table 4's in section A.1 of http://dx.doi.org/10.6028/NIST.FIPS.202
constexpr size_t IN_LEN_BITS = 512;
constexpr size_t IN_LEN_BYTES = IN_LEN_BITS >> 3;

constexpr size_t OUT_LEN_BITS = IN_LEN_BITS >> 1;
constexpr size_t OUT_LEN_BYTES = IN_LEN_BYTES >> 1;

// From input byte array ( = 64 bytes ) preparing 5 x 5 x 64 keccak state array
// as twenty five 64 -bit unsigned integers
//
// Combined techniques adapted from section 3.1.2 of
// http://dx.doi.org/10.6028/NIST.FIPS.202; algorithm 10
// defined in section B.1 of above linked document
void
to_state_array(const sycl::uchar* __restrict in,
               sycl::ulong* const __restrict state)
{
#pragma unroll 8
  for (size_t i = 0; i < 8; i++) {
    state[i] = static_cast<sycl::ulong>(in[(i << 3) + 7]) << 56 |
               static_cast<sycl::ulong>(in[(i << 3) + 6]) << 48 |
               static_cast<sycl::ulong>(in[(i << 3) + 5]) << 40 |
               static_cast<sycl::ulong>(in[(i << 3) + 4]) << 32 |
               static_cast<sycl::ulong>(in[(i << 3) + 3]) << 24 |
               static_cast<sycl::ulong>(in[(i << 3) + 2]) << 16 |
               static_cast<sycl::ulong>(in[(i << 3) + 1]) << 8 |
               static_cast<sycl::ulong>(in[(i << 3) + 0]) << 0;
  }

  // see how 0b01 is appended to input message bits in section
  // 6.1 of http://dx.doi.org/10.6028/NIST.FIPS.202
  //
  // ! read right to left !
  //
  // also notice left most 1 added due to padding requirement
  // as specified in section 5.1 of above linked specification
  state[8] = 0b110ull;

#pragma unroll 7
  for (size_t i = 9; i < 16; i++) {
    state[i] = 0ull;
  }

  // this 1 is added to input message bits due to padding requirement
  // ( pad10*1 ) written in section 5.1 of
  // http://dx.doi.org/10.6028/NIST.FIPS.202
  //
  // ! read right to left, so it's actually 1 << 63 !
  state[16] = 9223372036854775808ull;

#pragma unroll 8
  for (size_t i = 17; i < 25; i++) {
    state[i] = 0ull;
  }
}

// Given two ( byte concatenated ) SHA3-256 digests ( i.e. 64 -bytes ), prepares
// bit string of length 1600, which can be passed to keccak_p[b, n_r]
// permutation function, in later phase
//
// See algorithm 10 ( h2b ) defined in section B.1 of
// http://dx.doi.org/10.6028/NIST.FIPS.202
void
prepare_bit_string(const sycl::uchar* in, std::bitset<1600>& s)
{
  s.reset();

  size_t s_idx = 1599;
  for (size_t i = 0; i < 64; i++) {
    s[s_idx--] = get_bit_at<0>(*(in + i));
    s[s_idx--] = get_bit_at<1>(*(in + i));
    s[s_idx--] = get_bit_at<2>(*(in + i));
    s[s_idx--] = get_bit_at<3>(*(in + i));
    s[s_idx--] = get_bit_at<4>(*(in + i));
    s[s_idx--] = get_bit_at<5>(*(in + i));
    s[s_idx--] = get_bit_at<6>(*(in + i));
    s[s_idx--] = get_bit_at<7>(*(in + i));
  }

  s[s_idx--] = 0;
  s[s_idx--] = 1;

  s[s_idx--] = 1;
  for (size_t i = 0; i < 572; i++) {
    s[s_idx--] = 0;
  }
  s[s_idx--] = 1;
}

// From keccak-p[n. b_r] state bit string, extracting first 256 -bits
// as output digest of SHA3-256 2-to-1 hasher
void
prepare_digest_bytes(std::bitset<1600>& s, sycl::uchar* const out)
{
  size_t s_idx = 1599;

  for (size_t i = 0; i < 32; i++) {
    *(out + i) = set_bit_at<7>(s[s_idx - 7]) | set_bit_at<6>(s[s_idx - 6]) |
                 set_bit_at<5>(s[s_idx - 5]) | set_bit_at<4>(s[s_idx - 4]) |
                 set_bit_at<3>(s[s_idx - 3]) | set_bit_at<2>(s[s_idx - 2]) |
                 set_bit_at<1>(s[s_idx - 1]) | set_bit_at<0>(s[s_idx - 0]);
    s_idx -= 8;
  }
}

// SHA3-256 2-to-1 hasher, where input is 64 contiguous bytes which is hashed
// to produce 32 -bytes output
//
// This function itself doesn't do much instead of calling other functions
// which actually
// - prepares state bit array from input byte array
// - permutes input using `keccak-p[b, n_r]`
// - truncates first 256 -bits from state bit array
//
// See section 6.1 of http://dx.doi.org/10.6028/NIST.FIPS.202
void
hash(const sycl::uchar* __restrict in, sycl::uchar* const __restrict digest)
{
  std::bitset<1600> s;

  prepare_bit_string(in, s);
  keccak_p(s);
  prepare_digest_bytes(s, digest);
}

}
