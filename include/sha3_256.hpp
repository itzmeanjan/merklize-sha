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

// From absorbed hash state array of dimension 5 x 5 x 64, produces 32 -bytes
// digest using method defined in section 3.1.3 of
// http://dx.doi.org/10.6028/NIST.FIPS.202 and algorithm 11 defined in section
// B.1 of above hyperlinked document
void
to_digest_bytes(const sycl::ulong* __restrict in,
                sycl::uchar* const __restrict digest)
{
#pragma unroll 4
  for (size_t i = 0; i < 4; i++) {
    const sycl::ulong lane = in[i];

    digest[(i << 3) + 0] = static_cast<sycl::uchar>((lane >> 0) & 0xffull);
    digest[(i << 3) + 1] = static_cast<sycl::uchar>((lane >> 8) & 0xffull);
    digest[(i << 3) + 2] = static_cast<sycl::uchar>((lane >> 16) & 0xffull);
    digest[(i << 3) + 3] = static_cast<sycl::uchar>((lane >> 24) & 0xffull);
    digest[(i << 3) + 4] = static_cast<sycl::uchar>((lane >> 32) & 0xffull);
    digest[(i << 3) + 5] = static_cast<sycl::uchar>((lane >> 40) & 0xffull);
    digest[(i << 3) + 6] = static_cast<sycl::uchar>((lane >> 48) & 0xffull);
    digest[(i << 3) + 7] = static_cast<sycl::uchar>((lane >> 56) & 0xffull);
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
  sycl::ulong state[25];

  to_state_array(in, state);
  keccak_p(state);
  to_digest_bytes(state, digest);
}

}
