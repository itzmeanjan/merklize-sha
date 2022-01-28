#pragma once
#include "sha2.hpp"

namespace sha2_384 {

// SHA2-384 specific input/ output width constants, taken from
// section 1's figure 1 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
constexpr size_t IN_LEN_BITS = 768;
constexpr size_t IN_LEN_BYTES = 96;

constexpr size_t OUT_LEN_BITS = IN_LEN_BITS >> 1;
constexpr size_t OUT_LEN_BYTES = IN_LEN_BYTES >> 1;

// SHA2-384 -bit variant's initial hash state values
//
// Taken from section 5.3.4 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
constexpr sycl::ulong IV_0[8] = { 0xcbbb9d5dc1059ed8, 0x629a292a367cd507,
                                  0x9159015a3070dd17, 0x152fecd8f70e5939,
                                  0x67332667ffc00b31, 0x8eb44a8768581511,
                                  0xdb0c2e0d64f98fa7, 0x47b5481dbefa4fa4 };

// Padding input message ( = 768 -bits ), which is nothing but
// two SHA2-384 digests ( read each digest is 48 -bytes ) concatenated,
// such that padded message becomes a 1024 -bit block, which can be parsed in
// later phase
//
// See section 5.1.2 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
void
pad_input_message(const sycl::uchar* __restrict in,
                  sycl::uchar* const __restrict out)
{
  // partially parallelize copying of 96 input bytes to output
#pragma unroll 16
  for (size_t i = 0; i < 96; i++) {
    *(out + i) = *(in + i);
  }

  constexpr size_t offset = 96;

  // then append a 1 bit, at 97 -th bytes position
  *(out + offset) = 0b10000000;

  // after that append 29 zero bytes
  //
  // letting compiler decide how to unroll this loop, because loop iteration
  // count ( = 29 ) is a prime number
#pragma unroll
  for (size_t i = 1; i < 30; i++) {
    *(out + offset + i) = 0;
  }

  // finally last two bytes keeping length of input ( = 768 -bit )
  *(out + 126) = 0b00000011;
  *(out + 127) = 0b00000000;
}

// When two SHA2-384 digests are represented in word form ( = 64 -bit word size
// ), they're input as 12 consecutive words, which are now padded such that
// output is 16 words, making total of a single message block ( = 1024 bit )
//
// See section 5.1.2 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
void
pad_input_message(const sycl::ulong* __restrict in,
                  sycl::ulong* const __restrict out)
{
  // copying first 12 words ( = SHA2-384 64 -bit wide words )
  // from input to padded output allocation
#pragma unroll 12
  for (size_t i = 0; i < 12; i++) {
    *(out + i) = *(in + i);
  }

  constexpr size_t offset = 12;

  // then appending 1 -bit at end of input message words
  //
  // note, here I'm working with 64 -bit word
  *(out + offset) = 0b10000000ul << 56;

  // then add two zero words, making first 15 words of output
  // message block set
#pragma unroll 2
  for (size_t i = 1; i < 3; i++) {
    *(out + offset + i) = 0;
  }

  // finally last message word ( = 16 -th ) keeps length of input
  // in bits ( = 768 )
  *(out + 15) = 0ul | 0b00000011ul << 8;
}

// Given two concatenated SHA2-384 digests are padded ( to 1024 bit message
// block ) and parsed ( to sixteen 64 -bit words ), this function produces 48
// -bytes SHA2-384 digest, as six 64 -bit words
//
// This is 2-to-1 hash function, which computes hash of two SHA2-384 digests
//
// See section 6.5 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
void
hash(const sycl::ulong* __restrict in, sycl::ulong* const __restrict digest)
{
  // step 1) prepare 80 message words
  sycl::ulong msg_schld[80];
  sha2::word_64::prepare_message_schedule(in, msg_schld);

  // step 2) initialize eight working variables
  sycl::ulong a = IV_0[0];
  sycl::ulong b = IV_0[1];
  sycl::ulong c = IV_0[2];
  sycl::ulong d = IV_0[3];
  sycl::ulong e = IV_0[4];
  sycl::ulong f = IV_0[5];
  sycl::ulong g = IV_0[6];
  sycl::ulong h = IV_0[7];

  // step 3) mix scheduled message words into initial hash state
  for (size_t i = 0; i < 80; i++) {
    sycl::ulong tmp0 = h + sha2::word_64::Σ_1(e) + sha2::word_64::ch(e, f, g) +
                       sha2::word_64::K[i] + msg_schld[i];
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

  // step 4) finally write 48 -bytes digest back to allocated memory
  *(digest + 0) = IV_0[0] + a;
  *(digest + 1) = IV_0[1] + b;
  *(digest + 2) = IV_0[2] + c;
  *(digest + 3) = IV_0[3] + d;
  *(digest + 4) = IV_0[4] + e;
  *(digest + 5) = IV_0[5] + f;
}

}
