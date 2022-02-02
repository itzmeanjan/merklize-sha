#pragma once
#include "sha3.hpp"

namespace sha3_512 {

// SHA3-512 specific input/ output width constants, taken from
// table 4's in section A.1 of http://dx.doi.org/10.6028/NIST.FIPS.202
constexpr size_t IN_LEN_BITS = 1024;
constexpr size_t IN_LEN_BYTES = IN_LEN_BITS >> 3;

constexpr size_t OUT_LEN_BITS = IN_LEN_BITS >> 1;
constexpr size_t OUT_LEN_BYTES = IN_LEN_BYTES >> 1;

constexpr size_t RATE_LEN_BITS = 576;
constexpr size_t RATE_LEN_BYTES = RATE_LEN_BITS >> 3;

// First absorb starting 576 -bits ( = 72 -bytes, note this is RATE for SHA3-512
// ) of input byte array ( = 128 bytes ) preparing 5 x 5 x 64 keccak state array
// as twenty five 64 -bit unsigned integers
//
// Combined techniques adapted from section 3.1.2 of
// http://dx.doi.org/10.6028/NIST.FIPS.202; algorithm 10
// defined in section B.1 of above linked document
//
// Note total input is 1024 -bits wide
void
process_first_576_bits(const sycl::uchar* __restrict in,
                       sycl::ulong* const __restrict state)
{
#pragma unroll 3
  for (size_t i = 0; i < (RATE_LEN_BYTES >> 3); i++) {
    state[i] = static_cast<sycl::ulong>(in[(i << 3) + 7]) << 56 |
               static_cast<sycl::ulong>(in[(i << 3) + 6]) << 48 |
               static_cast<sycl::ulong>(in[(i << 3) + 5]) << 40 |
               static_cast<sycl::ulong>(in[(i << 3) + 4]) << 32 |
               static_cast<sycl::ulong>(in[(i << 3) + 3]) << 24 |
               static_cast<sycl::ulong>(in[(i << 3) + 2]) << 16 |
               static_cast<sycl::ulong>(in[(i << 3) + 1]) << 8 |
               static_cast<sycl::ulong>(in[(i << 3) + 0]) << 0;
  }

  // finally 1024 ( = capacity ) zero bits
#pragma unroll 8
  for (size_t i = 9; i < 25; i++) {
    state[i] = 0ull;
  }
}

// Then absorb remaining 448 -bits ( = 56 -bytes ) of input byte array ( = 128
// -bytes ) preparing 5 x 6 x 64 keccak state array as twenty five 64 -bit
// unsigned integers
//
// Combined techniques adapted from section 3.1.2 of
// http://dx.doi.org/10.6028/NIST.FIPS.202; algorithm 10
// defined in section B.1 of above linked document
//
// Note total input is 1024 -bits wide
void
process_remaining_448_bits(const sycl::uchar* __restrict in,
                           sycl::ulong* const __restrict state)
{
#pragma unroll 7
  for (size_t i = 0; i < ((IN_LEN_BITS - RATE_LEN_BITS) >> 6); i++) {
    state[i] = static_cast<sycl::ulong>(in[(i << 3) + 7]) << 56 |
               static_cast<sycl::ulong>(in[(i << 3) + 6]) << 48 |
               static_cast<sycl::ulong>(in[(i << 3) + 5]) << 40 |
               static_cast<sycl::ulong>(in[(i << 3) + 4]) << 32 |
               static_cast<sycl::ulong>(in[(i << 3) + 3]) << 24 |
               static_cast<sycl::ulong>(in[(i << 3) + 2]) << 16 |
               static_cast<sycl::ulong>(in[(i << 3) + 1]) << 8 |
               static_cast<sycl::ulong>(in[(i << 3) + 0]) << 0;
  }

  // following two write ops actually pushing
  // 0b10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000110
  // bit pattern ( read in opposite order i.e. right to left ascending )
  // at end of 448 -bit input message bits
  //
  // = 0b0000000000000000000000000000000000000000000000000000000000000110
  state[7] = 0b110ull;
  // = 0b1000000000000000000000000000000000000000000000000000000000000000
  // = 1 << 63
  state[8] = 9223372036854775808ull;

  // finally 1024 ( = capacity ) zero bits
#pragma unroll 8
  for (size_t i = 9; i < 25; i++) {
    state[i] = 0ull;
  }
}

// Because RATE is small enough ( = 576 -bits ) that I'm required to absorb
// whole padded input bits in two phases, after performing keccak-p[b, n_r]
// permutation on first 576 input -bits, before second permutation can be
// applied for absorbing remaining 448 input -bits, I'm required to mix current
// state array ( in size 5 x 5 x 64 ) with previous state array
//
// Note total input is 1024 -bits wide
static inline void
mix_prev_into_cur_state(const sycl::ulong* __restrict prev,
                        sycl::ulong* const __restrict cur)
{
#pragma unroll 8
  for (size_t i = 0; i < 24; i++) {
    cur[i] ^= prev[i];
  }

  cur[24] ^= prev[24];
}

// From absorbed hash state array of dimension 5 x 5 x 64, produces 64 -bytes
// digest using method defined in section 3.1.3 of
// http://dx.doi.org/10.6028/NIST.FIPS.202 and algorithm 11 defined in section
// B.1 of above hyperlinked document
void
to_digest_bytes(const sycl::ulong* __restrict in,
                sycl::uchar* const __restrict digest)
{
#pragma unroll 4
  for (size_t i = 0; i < 8; i++) {
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

// SHA3-512 2-to-1 hasher, where input is 128 contiguous bytes which is hashed
// to produce 64 -bytes output
//
// This function itself doesn't do much instead of calling other functions
// which actually
// - prepares state bit array from first 576 input bits
// - permutes input using `keccak-p[b, n_r]`
// - then prepares another state bit array from remaining 448 input bits
// - mixes ( read XORs ) previous state bit array into current state bit array
// - permutes current input bit array using `keccak-p[b, n_r]`
// - truncates first 512 -bits from final state bit array
//
// See section 6.1 of http://dx.doi.org/10.6028/NIST.FIPS.202
void
hash(const sycl::uchar* __restrict in, sycl::uchar* const __restrict digest)
{
  sycl::ulong state_0[25];
  sycl::ulong state_1[25];

  process_first_576_bits(in, state_0);
  keccak_p(state_0);

  process_remaining_448_bits(in + 72, state_1);
  mix_prev_into_cur_state(state_0, state_1);
  keccak_p(state_1);

  to_digest_bytes(state_1, digest);
}

}
