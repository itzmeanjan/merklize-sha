#pragma once
#include <CL/sycl.hpp>

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

}
