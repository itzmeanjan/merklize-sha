#pragma once
#include "utils.hpp"
#include <CL/sycl.hpp>

namespace sha2_384 {

// SHA2-384 specific input/ output width constants, taken from
// section 1's figure 1 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
constexpr size_t IN_LEN_BITS = 768;
constexpr size_t IN_LEN_BYTES = 96;

constexpr size_t OUT_LEN_BITS = IN_LEN_BITS >> 1;
constexpr size_t OUT_LEN_BYTES = IN_LEN_BYTES >> 1;

constexpr size_t WORD_SIZE_BITS = 64;
constexpr size_t WORD_SIZE_BYTES = 8;

// SHA2-{384, 512, 512/ 224, 512/ 256} constants
//
// See section 4.2.3 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
constexpr sycl::ulong K[80] = {
  0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f,
  0xe9b5dba58189dbbc, 0x3956c25bf348b538, 0x59f111f1b605d019,
  0x923f82a4af194f9b, 0xab1c5ed5da6d8118, 0xd807aa98a3030242,
  0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
  0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235,
  0xc19bf174cf692694, 0xe49b69c19ef14ad2, 0xefbe4786384f25e3,
  0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65, 0x2de92c6f592b0275,
  0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
  0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f,
  0xbf597fc7beef0ee4, 0xc6e00bf33da88fc2, 0xd5a79147930aa725,
  0x06ca6351e003826f, 0x142929670a0e6e70, 0x27b70a8546d22ffc,
  0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
  0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6,
  0x92722c851482353b, 0xa2bfe8a14cf10364, 0xa81a664bbc423001,
  0xc24b8b70d0f89791, 0xc76c51a30654be30, 0xd192e819d6ef5218,
  0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
  0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99,
  0x34b0bcb5e19b48a8, 0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb,
  0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3, 0x748f82ee5defb2fc,
  0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
  0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915,
  0xc67178f2e372532b, 0xca273eceea26619c, 0xd186b8c721c0c207,
  0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, 0x06f067aa72176fba,
  0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
  0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc,
  0x431d67c49c100d4c, 0x4cc5d4becb3e42b6, 0x597f299cfc657e2a,
  0x5fcb6fab3ad6faec, 0x6c44198c4a475817
};

// SHA2-384 -bit variant's initial hash state values
//
// Taken from section 5.3.4 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
constexpr sycl::ulong IV_0[8] = { 0xcbbb9d5dc1059ed8, 0x629a292a367cd507,
                                  0x9159015a3070dd17, 0x152fecd8f70e5939,
                                  0x67332667ffc00b31, 0x8eb44a8768581511,
                                  0xdb0c2e0d64f98fa7, 0x47b5481dbefa4fa4 };

// SHA2 function, used for 384 -bit, 512 -bit, 512/ 224 -bit and 512/ 256 -bit
// variants
//
// See section 4.1.3 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
inline sycl::ulong
ch(sycl::ulong x, sycl::ulong y, sycl::ulong z)
{
  return (x & y) ^ (~x & z);
}

// SHA2 function, used for 384 -bit, 512 -bit, 512/ 224 -bit and 512/ 256 -bit
// variants
//
// See section 4.1.3 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
inline sycl::ulong
maj(sycl::ulong x, sycl::ulong y, sycl::ulong z)
{
  return (x & y) ^ (x & z) ^ (y & z);
}

// SHA2 function, used for 384 -bit, 512 -bit, 512/ 224 -bit and 512/ 256 -bit
// variants
//
// See section 4.1.3 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
inline sycl::ulong
Σ_0(sycl::ulong x)
{
  return rotr(x, 28) ^ rotr(x, 34) ^ rotr(x, 39);
}

// SHA2 function, used for 384 -bit, 512 -bit, 512/ 224 -bit and 512/ 256 -bit
// variants
//
// See section 4.1.3 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
inline sycl::ulong
Σ_1(sycl::ulong x)
{
  return rotr(x, 14) ^ rotr(x, 18) ^ rotr(x, 41);
}

// SHA2 function, used for 384 -bit, 512 -bit, 512/ 224 -bit and 512/ 256 -bit
// variants
//
// See section 4.1.3 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
inline sycl::ulong
σ_0(sycl::ulong x)
{
  return rotr(x, 1) ^ rotr(x, 8) ^ (x >> 7);
}

// SHA2 function, used for 384 -bit, 512 -bit, 512/ 224 -bit and 512/ 256 -bit
// variants
//
// See section 4.1.3 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
inline sycl::ulong
σ_1(sycl::ulong x)
{
  return rotr(x, 19) ^ rotr(x, 61) ^ (x >> 6);
}

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

// From sixteen message words in a mesage block, prepares message
// schedule of 80 message words, for mixing into hash state
//
// See step 1 of algorithm defined in section 6.4.2 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
void
prepare_message_schedule(const sycl::ulong* __restrict in,
                         sycl::ulong* const __restrict out)
{
#pragma unroll 8
  for (size_t i = 0; i < 16; i++) {
    *(out + i) = *(in + i);
  }

#pragma unroll 8
  for (size_t i = 16; i < 80; i++) {
    const sycl::ulong tmp0 = σ_1(*(out + (i - 2))) + *(out + (i - 7));
    const sycl::ulong tmp1 = σ_0(*(out + (i - 15))) + *(out + (i - 16));

    *(out + i) = tmp0 + tmp1;
  }
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
  prepare_message_schedule(in, msg_schld);

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
    sycl::ulong tmp0 = h + Σ_1(e) + ch(e, f, g) + K[i] + msg_schld[i];
    sycl::ulong tmp1 = Σ_0(a) + maj(a, b, c);

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
