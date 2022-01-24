#pragma once
#include "utils.hpp"
#include <CL/sycl.hpp>

namespace sha1 {

// SHA1 specific input/ output width constants, taken from
// section 1's figure 1 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
constexpr size_t IN_LEN_BITS = 320;
constexpr size_t IN_LEN_BYTES = 40;

constexpr size_t OUT_LEN_BITS = IN_LEN_BITS >> 1;
constexpr size_t OUT_LEN_BYTES = IN_LEN_BYTES >> 1;

constexpr size_t WORD_SIZE_BITS = 32;
constexpr size_t WORD_SIZE_BYTES = 4;

// See section 4.2.1 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
//
// Total 80 of them !
constexpr sycl::uint K_0 = 0x5a827999; // 0 <= t <= 19
constexpr sycl::uint K_1 = 0x6ed9eba1; // 20 <= t <= 39
constexpr sycl::uint K_2 = 0x8f1bbcdc; // 40 <= t <= 59
constexpr sycl::uint K_3 = 0xca62c1d6; // 60 <= t <= 79

// Initial hash values for SHA1
//
// See section 5.3.1 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
constexpr sycl::uint IV_0[5] = { 0x67452301,
                                 0xefcdab89,
                                 0x98badcfe,
                                 0x10325476,
                                 0xc3d2e1f0 };

// SHA1 functions, see section 4.1.1 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
//
// For 0 <= t <= 19
inline sycl::uint
ch(sycl::uint x, sycl::uint y, sycl::uint z)
{
  return (x & y) ^ (~x & z);
}

// SHA1 functions, see section 4.1.1 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
//
// For 20 <= t <= 39
// and 60 <= t <= 79
inline sycl::uint
parity(sycl::uint x, sycl::uint y, sycl::uint z)
{
  return x ^ y ^ z;
}

// SHA1 functions, see section 4.1.1 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
//
// For 40 <= t <= 59
inline sycl::uint
maj(sycl::uint x, sycl::uint y, sycl::uint z)
{
  return (x & y) ^ (x & z) ^ (y & z);
}

// Given 40 -bytes input array, where two SHA1 digests are concatenated
// produces output byte array of 64 -bytes, which is message block length of
// SHA1, by padding input with 1 bit, required many zero bits and finally length
// of input bit array ( = 320 -bits )
//
// In next step output byte array can be processed to have 16 SHA1 words ( each
// word 32 -bit wide, obtained from four consecutive big-endian bytes )
//
// See section 5.1.1 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
void
pad_input_message(const sycl::uchar* __restrict in,
                  sycl::uchar* const __restrict out)
{
  // attempt to parallelize copying of input to output byte array
  //
  // copies first 40 -bytes as it's
  //
  // note, this loop will be partially unrolled
#pragma unroll 8
  for (size_t i = 0; i < 40; i++) {
    *(out + i) = *(in + i);
  }

  constexpr size_t offset = 40;

  // then set 41 -st byte, as defined in sha1 specification
  *(out + offset) = 0b10000000;

  // now set next 21 -bytes to zero, it can be parallelized, due to
  // absense of any loop carried dependencies
  //
  // note, this loop will be partially unrolled, though unroll factor is not
  // power of 2 !
#pragma unroll 7
  for (size_t i = 1; i < 22; i++) {
    *(out + offset + i) = 0;
  }

  // set last two bytes to length of input ( in terms of bits )
  // which is 320 -bit
  *(out + 62) = 0b00000001;
  *(out + 63) = 0b01000000;
}

// Given input message in terms of SHA1 words ( note, each word is of 32 -bit
// width ) this function pads input such that a SHA1 message block ( 16 message
// words ) is returned, which will be consumed into hash state for producing 20
// -bytes output ( in next step )
//
// Note, this function is almost same as 👆 `pad_input_message`, just that
// instead of working with byte arrays, this function operates on SHA1 message
// words ( 32 -bit wide )
//
// I plan to use this function when computing intermediate nodes ( other than
// those just above leaf nodes ) of Binary Merkle Tree
void
pad_input_message(const sycl::uint* __restrict in,
                  sycl::uint* const __restrict out)
{
  // attempt to parallelize copying of input to output word array
  //
  // copies first 10 -words as it's
  //
  // note, this loop will be partially unrolled
#pragma unroll 10
  for (size_t i = 0; i < 10; i++) {
    *(out + i) = *(in + i);
  }

  constexpr size_t offset = 10;

  // then set 11 -th word, as defined in sha1 specification
  *(out + offset) = 0b10000000u << 24;

  // fully parallelize execution of this loop
#pragma unroll 4
  for (size_t i = 1; i < 5; i++) {
    *(out + offset + i) = 0;
  }

  // set last word to length of input ( in terms of bits )
  // which is 320 -bit
  *(out + 15) = 0 | 0b00000001u << 8 | 0b01000000u << 0;
}

// Given 512 -bit wide input message block ( read 64 -bytes )
// this function takes each of four consecutive bytes ( in big-endian order )
// and produces sixteen 32 -bit message words
//
// See section 5.2.1 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
void
parse_message_words(const sycl::uchar* __restrict in,
                    sycl::uint* const __restrict out)
{
  // attempt to fully parallelize this loop execution
  //
  // no loop carried dependency !
#pragma unroll 16
  for (size_t i = 0; i < 16; i++) {
    *(out + i) = (static_cast<sycl::uint>(*(in + i * 4 + 0)) << 24) |
                 (static_cast<sycl::uint>(*(in + i * 4 + 1)) << 16) |
                 (static_cast<sycl::uint>(*(in + i * 4 + 2)) << 8) |
                 (static_cast<sycl::uint>(*(in + i * 4 + 3)) << 0);
  }
}

// Five SHA1 message words are converted into equivalent length ( 20 -bytes )
// big endian byte array
//
// This function will be used for converting SHA1 digest to byte array form
//
// It does opposite of `parse_message_words` defined 👆
void
words_to_be_bytes(const sycl::uint* __restrict in,
                  sycl::uchar* const __restrict out)
{
#pragma unroll 5
  for (size_t i = 0; i < 5; i++) {
    const sycl::uint num = *(in + i);

    *(out + i * 4 + 0) = static_cast<sycl::uchar>((num >> 24) & 0xff);
    *(out + i * 4 + 1) = static_cast<sycl::uchar>((num >> 16) & 0xff);
    *(out + i * 4 + 2) = static_cast<sycl::uchar>((num >> 8) & 0xff);
    *(out + i * 4 + 3) = static_cast<sycl::uchar>((num >> 0) & 0xff);
  }
}

// Given sixteen 32 -bit words as input to SHA1 hash function
// prepares eighty 32 -bit words ( as message consumption schedule ) which are
// consumed into hash state
//
// See step 1 of algorithm defined in section 6.1.2 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
void
prepare_message_schedule(const sycl::uint* __restrict in,
                         sycl::uint* const __restrict out)
{
  // fully parallelize the loop execution
#pragma unroll 16
  for (size_t i = 0; i < 16; i++) {
    *(out + i) = *(in + i);
  }

  // total 64 iteration rounds to be executed, attempting to partially
  // unroll
#pragma unroll 16
  for (size_t i = 16; i < 80; i++) {
    const sycl::uint tmp0 = *(out + (i - 3)) ^ *(out + (i - 8));
    const sycl::uint tmp1 = *(out + (i - 14)) ^ *(out + (i - 16));

    *(out + i) = rotl(tmp0 ^ tmp1, 1);
  }
}

// This function takes a padded & parsed message block ( 512 -bit ) as input and
// produces 160 -bit SHA1 digest i.e. it computes 2-to-1 hash, which is useful
// during binary merklization
//
// See section 6.1.2 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
void
hash(const sycl::uint* __restrict in, sycl::uint* const __restrict digest)
{
  sycl::uint msg_schld[80];
  prepare_message_schedule(in, msg_schld);

  // initialize working variables with initial hash state values
  sycl::uint a = IV_0[0];
  sycl::uint b = IV_0[1];
  sycl::uint c = IV_0[2];
  sycl::uint d = IV_0[3];
  sycl::uint e = IV_0[4];

  // following four loops can't be unrolled due to (somewhat) complex nature of
  // loop carried dependencies !
  //
  // input message words are consumed into hash state in total 80 rounds
  for (size_t i = 0; i < 20; i++) {
    sycl::uint tmp = rotl(a, 5) + ch(b, c, d) + e + K_0 + msg_schld[i];
    e = d;
    d = c;
    c = rotl(b, 30);
    b = a;
    a = tmp;
  }

  for (size_t i = 20; i < 40; i++) {
    sycl::uint tmp = rotl(a, 5) + parity(b, c, d) + e + K_1 + msg_schld[i];
    e = d;
    d = c;
    c = rotl(b, 30);
    b = a;
    a = tmp;
  }

  for (size_t i = 40; i < 60; i++) {
    sycl::uint tmp = rotl(a, 5) + maj(b, c, d) + e + K_2 + msg_schld[i];
    e = d;
    d = c;
    c = rotl(b, 30);
    b = a;
    a = tmp;
  }

  for (size_t i = 60; i < 80; i++) {
    sycl::uint tmp = rotl(a, 5) + parity(b, c, d) + e + K_3 + msg_schld[i];
    e = d;
    d = c;
    c = rotl(b, 30);
    b = a;
    a = tmp;
  }

  // compute 20 -bytes sha1 digest of 64 -bytes message block
  *(digest + 0) = IV_0[0] + a;
  *(digest + 1) = IV_0[1] + b;
  *(digest + 2) = IV_0[2] + c;
  *(digest + 3) = IV_0[3] + d;
  *(digest + 4) = IV_0[4] + e;
}

}
