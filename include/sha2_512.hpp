#pragma once
#include "sha2.hpp"

namespace sha2_512 {

// SHA2-512 specific input/ output width constants, taken from
// section 1's figure 1 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
constexpr size_t IN_LEN_BITS = 1024;
constexpr size_t IN_LEN_BYTES = IN_LEN_BITS >> 3;

constexpr size_t OUT_LEN_BITS = IN_LEN_BITS >> 1;
constexpr size_t OUT_LEN_BYTES = IN_LEN_BYTES >> 1;

// SHA2-512 -bit variant's initial hash state values
//
// Taken from section 5.3.4 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
constexpr sycl::ulong IV_0[8] = { 0x6a09e667f3bcc908, 0xbb67ae8584caa73b,
                                  0x3c6ef372fe94f82b, 0xa54ff53a5f1d36f1,
                                  0x510e527fade682d1, 0x9b05688c2b3e6c1f,
                                  0x1f83d9abfb41bd6b, 0x5be0cd19137e2179 };

// Padding input message ( = 1024 -bits ), which is nothing but
// two SHA2-512 digests ( read each digest is 64 -bytes ) concatenated,
// such that padded message becomes two 1024 -bit blocks, which can be parsed in
// later phase
//
// See section 5.1.2 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
void
pad_input_message(const sycl::uchar* __restrict in,
                  sycl::uchar* const __restrict out)
{
  // partially parallelize copying of 128 input bytes to output
#pragma unroll 32
  for (size_t i = 0; i < 128; i++) {
    *(out + i) = *(in + i);
  }

  constexpr size_t offset = 128;

  // then append a 1 bit, at 128 -th bytes position
  *(out + offset) = 0b10000000;

  // after that append 125 zero bytes
#pragma unroll 25
  for (size_t i = 1; i < 126; i++) {
    *(out + offset + i) = 0;
  }

  // finally last two bytes keeping length of input ( = 1024 -bit )
  *(out + 254) = 0b00000100;
  *(out + 255) = 0b00000000;
}

// When two SHA2-512 digests are represented in word form ( = 64 -bit word size
// ), they're input as 16 consecutive words, which are now padded such that
// output is 32 words, making total of two message blocks ( = 2048 bit )
//
// See section 5.1.2 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
void
pad_input_message(const sycl::ulong* __restrict in,
                  sycl::ulong* const __restrict out)
{
  // copying first 16 words ( = SHA2-512 64 -bit wide words )
  // from input to padded output allocation
#pragma unroll 8
  for (size_t i = 0; i < 16; i++) {
    *(out + i) = *(in + i);
  }

  // after this point, we're working with second message block
  // which is 1024 -bit wide
  constexpr size_t offset = 16;

  // then appending 1 -bit at end of input message words
  //
  // note, here I'm working with 64 -bit word
  *(out + offset) = 0b10000000ul << 56;

  // then add 14 zero words, making first 31 words of output
  // message blocks
#pragma unroll 7
  for (size_t i = 1; i < 15; i++) {
    *(out + offset + i) = 0;
  }

  // finally last message word ( = 32 -nd ) keeps length of input
  // in bits ( = 1024 )
  *(out + 31) = 0ul | 0b00000100ul << 8;
}

// Given two concatenated SHA2-512 digests are padded ( to total 2048 bit
// message blocks ) and parsed ( to thirty-two 64 -bit words ), this function
// produces 64 -bytes SHA2-512 digest, as eight 64 -bit words
//
// This is 2-to-1 hash function, which computes hash of two SHA2-512 digests
//
// See section 6.4 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
void
hash(const sycl::ulong* __restrict in, sycl::ulong* const __restrict digest)
{
  // prepare 80 message words
  sycl::ulong msg_schld[80];
  sycl::ulong hash_state[8];

  // setting initial hash state
#pragma unroll 8
  for (size_t i = 0; i < 8; i++) {
    hash_state[i] = IV_0[i];
  }

  for (size_t i = 0; i < 2; i++) {
    // step 1) prepare message schedule
    sha2::word_64::prepare_message_schedule(in + (i << 4), msg_schld);

    // step 2) initialize eight working variables
    sycl::ulong a = hash_state[0];
    sycl::ulong b = hash_state[1];
    sycl::ulong c = hash_state[2];
    sycl::ulong d = hash_state[3];
    sycl::ulong e = hash_state[4];
    sycl::ulong f = hash_state[5];
    sycl::ulong g = hash_state[6];
    sycl::ulong h = hash_state[7];

    // step 3) mix scheduled message words into initial hash state
    for (size_t t = 0; t < 80; t++) {
      sycl::ulong tmp0 = h + sha2::word_64::Σ_1(e) +
                         sha2::word_64::ch(e, f, g) + sha2::word_64::K[t] +
                         msg_schld[t];
      sycl::ulong tmp1 = sha2::word_64::Σ_0(a) + sha2::word_64::maj(a, b, c);

      h = g;
      g = f;
      f = e;
      e = d + tmp0;
      d = c;
      c = b;
      b = a;
      a = tmp0 + tmp1;
    }

    // step 4 ) computing intermediate hash values
    hash_state[0] += a;
    hash_state[1] += b;
    hash_state[2] += c;
    hash_state[3] += d;
    hash_state[4] += e;
    hash_state[5] += f;
    hash_state[6] += g;
    hash_state[7] += h;
  }

#pragma unroll 8
  for (size_t i = 0; i < 8; i++) {
    *(digest + i) = hash_state[i];
  }
}

}
