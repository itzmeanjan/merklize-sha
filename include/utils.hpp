#pragma once
#include <CL/sycl.hpp>

// Circular right shift of 32 -bit word, by n bit places
//
// Make sure n < 32
//
// See section 3.2's point 4 in Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
inline sycl::uint
rotr(sycl::uint x, size_t n)
{
  return (x >> n) | (x << (32 - n));
}

// Circular left shift of 32 -bit word, by n bit places
//
// Make sure n < 32
//
// See section 3.2's point 5 in Secure Hash Standard
// http://dx.doi.org/10.6028/NIST.FIPS.180-4
inline sycl::uint
rotl(sycl::uint x, size_t n)
{
  return (x << n) | (x >> (32 - n));
}
