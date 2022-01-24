#pragma once
#include <CL/sycl.hpp>
#include <cassert>

namespace sha1 {

// See section 4.2.1 of Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
//
// Total 80 of them !
constexpr sycl::uint K_0 = 0x5a827999; // 0 <= t <= 19
constexpr sycl::uint K_1 = 0x6ed9eba1; // 20 <= t <= 39
constexpr sycl::uint K_2 = 0x8f1bbcdc; // 40 <= t <= 59
constexpr sycl::uint K_3 = 0xca62c1d6; // 60 <= t <= 79

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
                  size_t in_bit_len,
                  sycl::uchar* const __restrict out,
                  size_t out_bit_len)
{
  // input is always 40 -bytes i.e. two sha1 digests concatenated
  assert(in_bit_len == 320);
  // output is 64 -bytes i.e. a message block of sha1
  assert(out_bit_len == 512);

  // attempt to parallelize copying of input to output byte array
  //
  // copies first 40 -bytes as it's
  //
  // note, this loop will be partially unrolled
#pragma unroll 8
  for (size_t i = 0; i < (in_bit_len >> 3); i++) {
    *(out + i) = *(in + i);
  }

  const sycl::uint offset = in_bit_len >> 3;

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
                 (static_cast<sycl::uint>(*(in + i * 4 + 2)) << 28) |
                 (static_cast<sycl::uint>(*(in + i * 4 + 3)) << 0);
  }
}

}
