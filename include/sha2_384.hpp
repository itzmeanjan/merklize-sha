#pragma once
#include "utils.hpp"
#include <CL/sycl.hpp>

namespace sha2_384 {

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

}
