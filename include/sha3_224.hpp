#pragma once
#include "sha3.hpp"

namespace sha3_224 {

// SHA3-224 specific input/ output width constants, taken from
// table 4's in section A.1 of http://dx.doi.org/10.6028/NIST.FIPS.202
constexpr size_t IN_LEN_BITS = 448;
constexpr size_t IN_LEN_BYTES = IN_LEN_BITS >> 3;

constexpr size_t OUT_LEN_BITS = IN_LEN_BITS >> 1;
constexpr size_t OUT_LEN_BYTES = IN_LEN_BYTES >> 1;

// From input byte array ( = 56 bytes ) preparing 5 x 5 x 64 keccak state array
// as twenty five 64 -bit unsigned integers
//
// Combined techniques adapted from section 3.1.2 of
// http://dx.doi.org/10.6028/NIST.FIPS.202; algorithm 10
// defined in section B.1 of above linked document
void
to_state_array(const sycl::uchar* __restrict in,
               sycl::ulong* const __restrict state)
{ 
#pragma unroll 7
  for (size_t i = 0; i < (IN_LEN_BYTES >> 3); i++) {
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
  state[7] = 0b110ull;

#pragma unroll 9
  for (size_t i = 8; i < 17; i++) {
    state[i] = 0ull;
  }

  // this 1 is added to input message bits due to padding requirement
  // ( pad10*1 ) written in section 5.1 of
  // http://dx.doi.org/10.6028/NIST.FIPS.202
  //
  // ! read right to left, so it's actually 1 << 63 !
  state[17] = 9223372036854775808ull;

#pragma unroll 7
  for (size_t i = 18; i < 25; i++) {
    state[i] = 0ull;
  }
}

// From absorbed hash state array of dimension 5 x 5 x 64, produces 28 -bytes
// digest using method defined in section 3.1.3 of
// http://dx.doi.org/10.6028/NIST.FIPS.202 and algorithm 11 defined in section
// B.1 of above hyperlinked document
void
to_digest_bytes(const sycl::ulong* __restrict in,
                sycl::uchar* const __restrict digest)
{
  // writing first 24 -bytes of digest from first three lanes
  // of state array i.e. lane(0, 0), lane(1, 0),lane(2, 0)
#pragma unroll 3
  for (size_t i = 0; i < 3; i++) {
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

  // and then computing final 4 bytes of digest from fourth lane of
  // state array i.e. lane(3, 0)
  //
  // for understanding meaning of lane notation, you want to see
  // section 3.1.1 and section 2.2 of http://dx.doi.org/10.6028/NIST.FIPS.202
  const sycl::ulong lane = in[3];
  digest[(3 << 3) + 0] = static_cast<sycl::uchar>((lane >> 0) & 0xffull);
  digest[(3 << 3) + 1] = static_cast<sycl::uchar>((lane >> 8) & 0xffull);
  digest[(3 << 3) + 2] = static_cast<sycl::uchar>((lane >> 16) & 0xffull);
  digest[(3 << 3) + 3] = static_cast<sycl::uchar>((lane >> 24) & 0xffull);
}

// SHA3-224 2-to-1 hasher, where input is 56 contiguous bytes which is hashed
// to produce 28 -bytes output
//
// This function itself doesn't do much instead of calling other functions
// which actually
// - prepares state bit array from input byte array
// - permutes input using `keccak-p[b, n_r]`
// - truncates first 224 -bits from state bit array
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
