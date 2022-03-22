#pragma once
#include "sha3.hpp"

namespace keccak_256 {

// Keccak-256 specific input/ output width constants
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
//
// Note, in SHA3 specification padding rule is different than what it's for
// keccak-256 i.e. to be very spcific, compare
// https://github.com/itzmeanjan/merklize-sha/blob/e421134ea4c9b1a832458bc870c3e79dc2849ecc/include/sha3_256.hpp#L43
// ( this is sha3-256 implementation ) and write to 9-th state lane ( at index 8
// of state array ) in following function body
//
// I suggest you read https://keccak.team/files/Keccak-implementation-3.2.pdf 's
// section 1.1 where padding rule is defined under `Keccak[r, c](M)` definition
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

  // see how 0b01 is padded to input message; following keccak-256
  // implementation guide
  // https://keccak.team/files/Keccak-implementation-3.2.pdf 's section 1.1
  // where `Keccak[r, c](M)` is defined ( spcifically padding rule block in
  // pseudocode, at very end of mentioned section )
  //
  // ! read right to left !
  state[8] = 0b1ull;

#pragma unroll 7
  for (size_t i = 9; i < 16; i++) {
    state[i] = 0ull;
  }

  // this 1 is added to input message bits due to padding requirement
  // defined in keccak-256 implementation guide
  // https://keccak.team/files/Keccak-implementation-3.2.pdf 's section 1.1
  // where `Keccak[r, c](M)` is defined ( spcifically padding rule block in
  // pseudocode, at very end of mentioned section )
  //
  // ! read right to left, so it's actually 1 << 63 !
  state[16] = 9223372036854775808ull;

#pragma unroll 8
  for (size_t i = 17; i < 25; i++) {
    state[i] = 0ull;
  }
}

// From input byte array ( = 64 bytes ) preparing 5 x 5 x 64 keccak state array
// as twenty five 64 -bit unsigned integers
//
// Combined techniques adapted from section 3.1.2 of
// http://dx.doi.org/10.6028/NIST.FIPS.202; algorithm 10
// defined in section B.1 of above linked document
//
// Note, in SHA3 specification padding rule is different than what it's for
// keccak-256 i.e. to be very spcific, compare
// https://github.com/itzmeanjan/merklize-sha/blob/e421134ea4c9b1a832458bc870c3e79dc2849ecc/include/sha3_256.hpp#L43
// ( this is sha3-256 implementation ) and write to 9-th state lane ( at index 8
// of state array ) in following function body
//
// I suggest you read https://keccak.team/files/Keccak-implementation-3.2.pdf 's
// section 1.1 where padding rule is defined under `Keccak[r, c](M)` definition
//
// Standard representation of keccak state array is converted to bit interleaved
// representation so that each lane of state array is represented in terms two
// limbs, each of 32 -bit wide; this will help us in using only 32 -bit bitwise
// operations while computing keccak-p[1600, 24] permutation
void
to_state_array(const sycl::uchar* __restrict in,
               sycl::uint* const __restrict state)
{
#pragma unroll 8
  for (size_t i = 0; i < 8; i++) {
    const uint64_t word = static_cast<sycl::ulong>(in[(i << 3) + 7]) << 56 |
                          static_cast<sycl::ulong>(in[(i << 3) + 6]) << 48 |
                          static_cast<sycl::ulong>(in[(i << 3) + 5]) << 40 |
                          static_cast<sycl::ulong>(in[(i << 3) + 4]) << 32 |
                          static_cast<sycl::ulong>(in[(i << 3) + 3]) << 24 |
                          static_cast<sycl::ulong>(in[(i << 3) + 2]) << 16 |
                          static_cast<sycl::ulong>(in[(i << 3) + 1]) << 8 |
                          static_cast<sycl::ulong>(in[(i << 3) + 0]) << 0;

    uint32_t even, odd;
    to_bit_interleaved(word, &even, &odd);

    state[(i << 1) + 0] = even;
    state[(i << 1) + 1] = odd;
  }

  // see how 0b01 is padded to input message; following keccak-256
  // implementation guide
  // https://keccak.team/files/Keccak-implementation-3.2.pdf 's section 1.1
  // where `Keccak[r, c](M)` is defined ( spcifically padding rule block in
  // pseudocode, at very end of mentioned section )
  //
  // ! read right to left !
  uint64_t word = 0b1ull;
  uint32_t even, odd;
  to_bit_interleaved(word, &even, &odd);

  state[16] = even;
  state[17] = odd;

#pragma unroll 7
  for (size_t i = 9; i < 16; i++) {
    state[(i << 1) + 0] = 0u;
    state[(i << 1) + 1] = 0u;
  }

  // this 1 is added to input message bits due to padding requirement
  // defined in keccak-256 implementation guide
  // https://keccak.team/files/Keccak-implementation-3.2.pdf 's section 1.1
  // where `Keccak[r, c](M)` is defined ( spcifically padding rule block in
  // pseudocode, at very end of mentioned section )
  //
  // ! read right to left, so it's actually 1 << 63 !
  word = 9223372036854775808ull;
  to_bit_interleaved(word, &even, &odd);

  state[32] = even;
  state[33] = odd;

#pragma unroll 8
  for (size_t i = 17; i < 25; i++) {
    state[(i << 1) + 0] = 0u;
    state[(i << 1) + 1] = 0u;
  }
}

// From absorbed hash state array of dimension 5 x 5 x 64, produces 32 -bytes
// digest using method defined in section 3.1.3 of
// http://dx.doi.org/10.6028/NIST.FIPS.202 and algorithm 11 defined in section
// B.1 of above hyperlinked document
//
// Note, digest preparation method is same for both sha3-256 and keccak-256
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

// From absorbed hash state array of dimension 5 x 5 x 64, produces 32 -bytes
// digest using method defined in section 3.1.3 of
// http://dx.doi.org/10.6028/NIST.FIPS.202 and algorithm 11 defined in section
// B.1 of above hyperlinked document
//
// Note, digest preparation method is same for both sha3-256 and keccak-256
//
// Bit interleaved representation of keccak state array is converted to standard
// representation for generating 2-to-1 hash digest
void
to_digest_bytes(const uint32_t* __restrict in,
                sycl::uchar* const __restrict digest)
{
#pragma unroll 4
  for (size_t i = 0; i < 4; i++) {
    const sycl::uint lane_even = in[(i << 1) + 0];
    const sycl::uint lane_odd = in[(i << 1) + 1];

    const uint64_t word = from_bit_interleaved(lane_even, lane_odd);

    digest[(i << 3) + 0] = static_cast<sycl::uchar>((word >> 0) & 0xffull);
    digest[(i << 3) + 1] = static_cast<sycl::uchar>((word >> 8) & 0xffull);
    digest[(i << 3) + 2] = static_cast<sycl::uchar>((word >> 16) & 0xffull);
    digest[(i << 3) + 3] = static_cast<sycl::uchar>((word >> 24) & 0xffull);
    digest[(i << 3) + 4] = static_cast<sycl::uchar>((word >> 32) & 0xffull);
    digest[(i << 3) + 5] = static_cast<sycl::uchar>((word >> 40) & 0xffull);
    digest[(i << 3) + 6] = static_cast<sycl::uchar>((word >> 48) & 0xffull);
    digest[(i << 3) + 7] = static_cast<sycl::uchar>((word >> 56) & 0xffull);
  }
}

// Keccak-256 2-to-1 hasher, where input is 64 contiguous bytes which is hashed
// to produce 32 -bytes output
//
// This function itself doesn't do much instead of calling other functions
// which actually
// - prepares state bit array from input byte array ( difference lies here, when
// comparing between sha3-256 & keccak-256 )
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

// Keccak-256 2-to-1 hasher, where input is 64 contiguous bytes which is hashed
// to produce 32 -bytes output
//
// This function itself doesn't do much instead of calling other functions
// which actually
// - prepares bit interleaved state bit array from input byte array ( using two
// 32 -bit words for each lane )
// - permutes input using `keccak-p[b, n_r]`
// - truncates first 256 -bits from state bit array and converts to standard
// representation from bit interleaved form
//
// See section 6.1 of http://dx.doi.org/10.6028/NIST.FIPS.202
//
// For more info on bit interleaved representation, see section 2.1 of
// https://keccak.team/files/Keccak-implementation-3.2.pdf
void
hash_u32(const sycl::uchar* __restrict in, sycl::uchar* const __restrict digest)
{
  // holds bit interleaved representation of state array i.e. each lane will be
  // splitted into two limbs ( each of 32 -bit wide )
  uint32_t state[50];

  to_state_array(in, state);
  keccak_p(state);
  to_digest_bytes(state, digest);
}

}
