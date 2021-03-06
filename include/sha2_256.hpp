#pragma once
#include "sha2.hpp"

namespace sha2_256 {

// SHA2-256 specific input/ output width constants, taken from
// section 1's figure 1 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
constexpr size_t IN_LEN_BITS = 512;
constexpr size_t IN_LEN_BYTES = IN_LEN_BITS >> 3;

constexpr size_t OUT_LEN_BITS = IN_LEN_BITS >> 1;
constexpr size_t OUT_LEN_BYTES = IN_LEN_BYTES >> 1;

// Initial hash values for SHA2-256, as specified in section 5.3.3 of Secure
// Hash Standard http://dx.doi.org/10.6028/NIST.FIPS.180-4
constexpr sycl::uint IV_0[8] = {
  0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
  0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

// Given 512 -bits of input ( two SHA2-256 digests concatenated ), this function
// appends 1 -bit, required 447 -many 0 -bit and finally 64 -bits representing
// input message length ( = 512 ) in bits, making total of 1024 bits padded
// input, which will be now parsed into words & hashed.
//
// See section 5.1.1 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
void
pad_input_message(const sycl::uchar* __restrict in,
                  sycl::uchar* const __restrict out)
{
  // copy 512 -bits of original input to output memory allocation
  //
  // try to (partially) parallelize this loop execution, due to absense of
  // loop carried dependencies
#pragma unroll 16
  for (size_t i = 0; i < 64; i++) {
    *(out + i) = *(in + i);
  }

  constexpr size_t offset = 64;

  // then setting 65 -th byte, as defined in section 5.1.1 of sha2 specification
  *(out + offset) = 0b10000000;

  // now setting next 61 bytes to zero
  //
  // 61 being prime number, I can't partially unroll this loop
  // execution, so relying on compiler to decide what should/ can be done
#pragma unroll
  for (size_t i = 1; i < 62; i++) {
    *(out + offset + i) = 0;
  }

  // setting last two bytes of total 128 -bytes output, where
  // original length of input ( in bits ) is kept
  //
  // so last two bytes denote 512 = length of input being hashed
  *(out + 126) = 0b00000010;
  *(out + 127) = 0b00000000;
}

// When input message is interpreted in terms of SHA2-256 words ( = 32 -bit wide
// ) input padding involves adding 16 new words, which is just same as done in ????
// `pad_input_message`, but 4 consecutive big endian bytes are now interpreted
// as one word
//
// Output of this function is 32 -words, where first 16 words are original input
// ( = 512 bits ) and last 16 words are padding
//
// See section 5.1.1 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
void
pad_input_message(const sycl::uint* __restrict in,
                  sycl::uint* const __restrict out)
{
  // copy first 64 -bytes = 16 words ( sha2-256 words are 32 -bit wide )
  // from input to output memory allocation
  //
  // this loop execution can be fully parallelized
#pragma unroll 16
  for (size_t i = 0; i < 16; i++) {
    *(out + i) = *(in + i);
  }

  constexpr size_t offset = 16;

  // setting 16 -th word
  *(out + offset) = 0b10000000 << 24;

  // next 14 words are set to 0, due to padding requirement
#pragma unroll 14
  for (size_t i = 1; i < 15; i++) {
    *(out + offset + i) = 0;
  }

  // finally last word set to length of input in bits ( = 512 )
  *(out + 31) = 0 | 0b00000010 << 8;
}

// Each of four consecutive big endian bytes of 1024 -bit padded input are
// interpreted as SHA2-256 word ( = 32 -bit ), making total of 32 words as
// output
//
// See section 5.2.1 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
void
parse_message_words(const sycl::uchar* __restrict in,
                    sycl::uint* const __restrict out)
{
  // attempt to partially parallelize this loop execution
  //
  // no loop carried dependency !
#pragma unroll 16
  for (size_t i = 0; i < 32; i++) {
    *(out + i) = from_be_bytes_to_u32_words(in + i * 4);
  }
}

// Takes two padded, parsed input message blocks ( = 1024 -bit ) and computes
// SHA2-256 digest ( = 256 -bit ) on input in two rounds ( because two message
// blocks are processed sequentially )
//
// See algorithm defined in section 6.2.2 of Secure Hash Standard
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
    sha2::word_32::prepare_message_schedule(in + 16 * i, msg_schld);

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
      sycl::uint tmp0 = h + sha2::word_32::??_1(e) + sha2::word_32::ch(e, f, g) +
                        sha2::word_32::K[t] + msg_schld[t];
      sycl::uint tmp1 = sha2::word_32::??_0(a) + sha2::word_32::maj(a, b, c);
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

  // write 256 -bit digest back to allocated memory --- final hash state after
  // consuming two message blocks
#pragma unroll 8
  for (size_t i = 0; i < 8; i++) {
    *(digest + i) = hash_state[i];
  }
}

}
