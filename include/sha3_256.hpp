#pragma once
#include "sha3.hpp"

namespace sha3_256 {

// SHA3-256 specific input/ output width constants, taken from
// table 4's in section A.1 of http://dx.doi.org/10.6028/NIST.FIPS.202
constexpr size_t IN_LEN_BITS = 512;
constexpr size_t IN_LEN_BYTES = IN_LEN_BITS >> 3;

constexpr size_t OUT_LEN_BITS = IN_LEN_BITS >> 1;
constexpr size_t OUT_LEN_BYTES = IN_LEN_BYTES >> 1;

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
