#pragma once
#include "sha2.hpp"

namespace sha2_512_224 {

// SHA2-512/224 specific input/ output width constants, taken from
// section 1's figure 1 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
constexpr size_t IN_LEN_BITS = 448;
constexpr size_t IN_LEN_BYTES = IN_LEN_BITS >> 3;

constexpr size_t OUT_LEN_BITS = IN_LEN_BITS >> 1;
constexpr size_t OUT_LEN_BYTES = IN_LEN_BYTES >> 1;

// SHA2-512/224 -bit variant's initial hash state values
//
// Taken from section 5.3.6.1 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
constexpr sycl::ulong IV_0[8] = { 0x8C3D37C819544DA2, 0x73E1996689DCD4D6,
                                  0x1DFAB7AE32FF9C82, 0x679DD514582F9FCF,
                                  0x0F6D2B697BD44DA8, 0x77E36F7304C48942,
                                  0x3F9D85A86A1D36C8, 0x1112E6AD91D692A1 };

// Padding input message ( = 448 -bits ), which is nothing but
// two SHA2-512/224 digests ( read each digest is 28 -bytes ) concatenated,
// such that padded message becomes one 1024 -bit message block, which can be
// parsed in later phase
//
// See section 5.1.2 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
void
pad_input_message(const sycl::uchar* __restrict in,
                  sycl::uchar* const __restrict out)
{
  // partially parallelize copying of 56 input bytes to output
#pragma unroll 8
  for (size_t i = 0; i < 56; i++) {
    *(out + i) = *(in + i);
  }

  constexpr size_t offset = 56;

  // then append a 1 bit, at 57 -th bytes position
  *(out + offset) = 0b10000000;

  // after that append 69 zero bytes
  // letting compiler decide what to do about unrolling this loop
#pragma unroll
  for (size_t i = 1; i < 70; i++) {
    *(out + offset + i) = 0;
  }

  // finally last two bytes keeping length of input ( = 448 -bit )
  *(out + 126) = 0b00000001;
  *(out + 127) = 0b11000000;
}

// When two SHA2-512/224 digests are represented in word form ( = 64 -bit word
// size
// ), they're input as 7 consecutive words, which are now padded such that
// output is 16 words, making total of one message block ( = 1024 bit )
//
// See section 5.1.2 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
void
pad_input_message(const sycl::ulong* __restrict in,
                  sycl::ulong* const __restrict out)
{
  // copying first 7 words ( = SHA2-512/224 uses 64 -bit wide words )
  // from input to padded output allocation
#pragma unroll 7
  for (size_t i = 0; i < 7; i++) {
    *(out + i) = *(in + i);
  }

  constexpr size_t offset = 7;

  // then appending 1 -bit at end of input message words
  //
  // note, here I'm working with 64 -bit word
  *(out + offset) = 0b10000000ul << 56;

  // then add 7 zero words
#pragma unroll 7
  for (size_t i = 1; i < 8; i++) {
    *(out + offset + i) = 0;
  }

  // finally last message word ( = 16 -th ) keeps length of input
  // in bits ( = 448 )
  *(out + 15) = 0ul | 0b00000001ul << 8 | 0b11000000ul;
}

// Given two concatenated SHA2-512/224 digests are padded ( to single 1024 bit
// message block ) and parsed ( to sixteen 64 -bit words ), this function
// produces 28 -bytes SHA2-512/224 digest, as four 64 -bit words, where last
// word's only upper ( MSB ) 32 -bits are of importance
//
// This is 2-to-1 hash function, which computes hash of two SHA2-512/224 digests
//
// See section 6.6 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
void
hash(const sycl::ulong* __restrict in, sycl::ulong* const __restrict digest)
{
  // prepare 80 message words
  sycl::ulong msg_schld[80];

  // step 1) prepare message schedule
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
  for (size_t t = 0; t < 80; t++) {
    sycl::ulong tmp0 = h + sha2::word_64::Σ_1(e) + sha2::word_64::ch(e, f, g) +
                       sha2::word_64::K[t] + msg_schld[t];
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

  // step 4 ) computing final hash values ( = 28 -bytes digest )
  *(digest + 0) = IV_0[0] + a;
  *(digest + 1) = IV_0[1] + b;
  *(digest + 2) = IV_0[2] + c;
  *(digest + 3) = IV_0[3] + d; // last word's LSB 32 -bits to be dropped !
}

}
