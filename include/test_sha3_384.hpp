#pragma once
#include "sha3_384.hpp"
#include <cassert>

void
test_sha3_384(sycl::queue& q)
{
  // obtained by executing following snippet in python3 shell
  //
  // >>> import hashlib
  // >>> list(hashlib.sha3_384(bytes([i for i in range(96)])).digest())
  //
  // note, same input is prepared inside 👇 for loop
  constexpr sycl::uchar expected[48] = {
    214, 226, 102, 151, 10,  63,  220, 212, 168, 51,  218, 134,
    21,  153, 23,  154, 6,   11,  87,  105, 89,  233, 147, 180,
    105, 133, 41,  48,  78,  227, 140, 35,  199, 16,  42,  112,
    132, 196, 213, 104, 177, 217, 85,  35,  209, 64,  119, 231
  };

  // acquire resources
  sycl::uchar* in = static_cast<sycl::uchar*>(sycl::malloc_shared(96, q));
  sycl::uchar* out = static_cast<sycl::uchar*>(sycl::malloc_shared(48, q));

#pragma unroll 16
  for (size_t i = 0; i < 96; i++) {
    // preparing input for testing 2-to-1 SHA3-384 hash
    *(in + i) = i;
  }

  // enqueue kernel execution in single work-item model
  q.single_task<class kernelTestSHA3_384>([=]() { sha3_384::hash(in, out); });
  q.wait();

  // check result !
  for (size_t i = 0; i < 48; i++) {
    assert(*(out + i) == expected[i]);
  }

  // ensure resources are deallocated
  sycl::free(in, q);
  sycl::free(out, q);
}
