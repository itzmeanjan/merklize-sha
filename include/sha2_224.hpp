#pragma once
#include "utils.hpp"
#include <CL/sycl.hpp>

namespace sha2_224 {

// SHA2-224 variant uses 64 words as constants, which are
// specified in section 4.2.2 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
constexpr sycl::uint K[64] = {
  0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
  0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
  0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
  0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
  0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
  0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
  0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
  0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
  0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
  0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
  0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

// Initial hash values for SHA2-224, as specified in section 5.3.2 of Secure
// Hash Standard http://dx.doi.org/10.6028/NIST.FIPS.180-4
constexpr sycl::uint IV_0[8] = {
  0xc1059ed8, 0x367cd507, 0x3070dd17, 0xf70e5939,
  0xffc00b31, 0x68581511, 0x64f98fa7, 0xbefa4fa4
};

// SHA2 function, defined in section 4.1.2 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
//
// Same function is also used in SHA1
inline sycl::uint
ch(sycl::uint x, sycl::uint y, sycl::uint z)
{
  return (x & y) ^ (~x & z);
}

// SHA2 function, defined in section 4.1.2 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
//
// Same function is also used in SHA1
inline sycl::uint
maj(sycl::uint x, sycl::uint y, sycl::uint z)
{
  return (x & y) ^ (x & z) ^ (y & z);
}

// SHA2 function, defined in section 4.1.2 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
inline sycl::uint
Σ_0(sycl::uint x)
{
  return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22);
}

// SHA2 function, defined in section 4.1.2 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
inline sycl::uint
Σ_1(sycl::uint x)
{
  return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25);
}

// SHA2 function, defined in section 4.1.2 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
inline sycl::uint
σ_0(sycl::uint x)
{
  return rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3);
}

// SHA2 function, defined in section 4.1.2 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
inline sycl::uint
σ_1(sycl::uint x)
{
  return rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10);
}

// Given 448 -bits of input ( two SHA2-224 digests concatenated ), this function
// appends 1 -bit, required 511 -many 0 -bit and finally 64 -bits representing
// input message length ( = 448 ) in bits, making total of 1024 bits padded
// input, which will be now parsed into words & hashed.
//
// See section 5.1.1 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
void
pad_input_message(const sycl::uchar* __restrict in,
                  sycl::uchar* const __restrict out)
{
  // copy 448 -bits of original input to output memory allocation
  //
  // try to (partially) parallelize this loop execution, due to absense of
  // loop carried dependencies
#pragma unroll 8
  for (size_t i = 0; i < 56; i++) {
    *(out + i) = *(in + i);
  }

  constexpr size_t offset = 56;

  // then setting 57 -th byte, as defined in section 5.1.1 of sha2 specification
  *(out + offset) = 0b10000000;

  // now setting next 69 bytes to zero
  //
  // relying on compiler to decide if this loop can be parallelized
#pragma unroll
  for (size_t i = 1; i < 70; i++) {
    *(out + offset + i) = 0;
  }

  // setting last two bytes of total 128 -bytes output, where
  // original length of input ( in bits ) is kept
  //
  // so last two bytes denote 448 = length of input being hashed
  *(out + 126) = 0b00000001;
  *(out + 127) = 0b11000000;
}

// When input message is interpreted in terms of SHA2-224 words ( = 32 -bit wide
// ) input padding involves adding 18 new words, which is just same as done in 👆
// `pad_input_message`, but 4 consecutive big endian bytes are now interpreted
// as one word
//
// Output of this function is 32 -words, where first 14 words are original input
// ( = 448 bits ) and last 18 words are padding
//
// See section 5.1.1 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
void
pad_input_message(const sycl::uint* __restrict in,
                  sycl::uint* const __restrict out)
{
  // copy first 56 -bytes = 14 words ( sha2-224 words are 32 -bit wide )
  // from input to output memory allocation
  //
  // this loop execution can be fully parallelized
#pragma unroll 14
  for (size_t i = 0; i < 14; i++) {
    *(out + i) = *(in + i);
  }

  constexpr size_t offset = 14;

  // setting 15 -th word
  *(out + offset) = 0b10000000 << 24;

  // next 16 words are set to 0, due to padding requirement
#pragma unroll 16
  for (size_t i = 1; i < 17; i++) {
    *(out + offset + i) = 0;
  }

  // finally last word set to length of input in bits ( = 448 )
  *(out + 31) = 0 | 0b00000001 << 8 | 0b11000000;
}

// Given 512 -bit input message block, it prepares 64 message schedules
// for consuming input message into hash state
//
// See step 1 of algorithm defined in section 6.2.2 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
void
prepare_message_schedule(const sycl::uint* __restrict in,
                         sycl::uint* const __restrict out)
{
  // first 16 message schedules are same as original message words
  // of 512 -bit message block
#pragma unroll 16
  for (size_t i = 0; i < 16; i++) {
    *(out + i) = *(in + i);
  }

  // 48 iteration rounds, preparing 48 remaining message schedules
#pragma unroll 16
  for (size_t i = 16; i < 64; i++) {
    const sycl::uint tmp0 = σ_1(*(out + (i - 2))) + *(out + (i - 7));
    const sycl::uint tmp1 = σ_0(*(out + (i - 15))) + *(out + (i - 16));

    *(out + i) = tmp0 + tmp1;
  }
}

// Takes two padded, parsed input message blocks ( = 1024 -bit ) and computes
// SHA2-224 digest ( = 224 -bit ) on input in two rounds ( because two message
// blocks are processed sequentially )
//
// See algorithm defined in section 6.3 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
void
hash(const sycl::uint* __restrict in, sycl::uint* const __restrict digest)
{
  sycl::uint msg_schld[64];
  sycl::uint hash_state[8];

  // fully parallelize hash state initialization
#pragma unroll 8
  for (size_t i = 0; i < 8; i++) {
    hash_state[i] = IV_0[i];
  }

  // two message blocks to be processed
  //
  // can't unroll this loop, too many logic inside loop body with
  // somewhat complex nature of loop carried dependencies
  for (size_t i = 0; i < 2; i++) {
    // see step 1 of algorithm defined in section  6.2.2 of Secure Hash Standard
    // http://dx.doi.org/10.6028/NIST.FIPS.180-4
    prepare_message_schedule(in + 16 * i, msg_schld);

    // see step 2 of algorithm defined in section  6.2.2 of Secure Hash Standard
    // http://dx.doi.org/10.6028/NIST.FIPS.180-4
    sycl::uint a = hash_state[0];
    sycl::uint b = hash_state[1];
    sycl::uint c = hash_state[2];
    sycl::uint d = hash_state[3];
    sycl::uint e = hash_state[4];
    sycl::uint f = hash_state[5];
    sycl::uint g = hash_state[6];
    sycl::uint h = hash_state[7];

    // can't parallelize loop execution, due to complex nature of loop carried
    // dependencies
    //
    // see step 3 of algorithm defined in section  6.2.2 of Secure Hash Standard
    // http://dx.doi.org/10.6028/NIST.FIPS.180-4
    for (size_t t = 0; t < 64; t++) {
      sycl::uint tmp0 = h + Σ_1(e) + ch(e, f, g) + K[t] + msg_schld[t];
      sycl::uint tmp1 = Σ_0(a) + maj(a, b, c);
      h = g;
      g = f;
      f = e;
      e = d + tmp0;
      d = c;
      c = b;
      b = a;
      a = tmp0 + tmp1;
    }

    // see step 4 of algorithm defined in section  6.2.2 of Secure Hash Standard
    // http://dx.doi.org/10.6028/NIST.FIPS.180-4
    hash_state[0] += a;
    hash_state[1] += b;
    hash_state[2] += c;
    hash_state[3] += d;
    hash_state[4] += e;
    hash_state[5] += f;
    hash_state[6] += g;
    hash_state[7] += h;
  }

  // write 224 -bit digest back to allocated memory --- final hash state after
  // consuming two message blocks
#pragma unroll 7
  for (size_t i = 0; i < 7; i++) {
    *(digest + i) = hash_state[i];
  }
}

}
