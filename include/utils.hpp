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

// Profile execution time of some command, whose submission resulted into
// provided SYCL event
//
// Ensure that queue has profiling enabled, otherwise this function should panic
inline sycl::cl_ulong
time_event(sycl::event& evt)
{
  sycl::cl_ulong start =
    evt.get_profiling_info<sycl::info::event_profiling::command_start>();
  sycl::cl_ulong end =
    evt.get_profiling_info<sycl::info::event_profiling::command_end>();

  return end - start;
}

// This function can be used for converting four contiguous big endian bytes
// into 32 -bit word
inline sycl::uint
from_be_bytes_to_words(const sycl::uchar* in)
{
  return (static_cast<sycl::uint>(*(in + 0)) << 24) |
         (static_cast<sycl::uint>(*(in + 1)) << 16) |
         (static_cast<sycl::uint>(*(in + 2)) << 8) |
         (static_cast<sycl::uint>(*(in + 3)) << 0);
}

// This function can be used for converting 32 -bit word to four
// consecutive big endian bytes
inline void
from_words_to_be_bytes(const sycl::uint word, sycl::uchar* const out)
{
  *(out + 0) = static_cast<sycl::uchar>((word >> 24) & 0xff);
  *(out + 1) = static_cast<sycl::uchar>((word >> 16) & 0xff);
  *(out + 2) = static_cast<sycl::uchar>((word >> 8) & 0xff);
  *(out + 3) = static_cast<sycl::uchar>((word >> 0) & 0xff);
}
