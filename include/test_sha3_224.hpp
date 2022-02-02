#pragma once
#include "sha3_224.hpp"
#include <cassert>

void
test_sha3_224(sycl::queue& q)
{
  // obtained by executing following snippet in python3 shell
  //
  // >>> import hashlib
  // >>> list(hashlib.sha3_224(bytes([i for i in range(56)])).digest())
  //
  // note, same input is prepared inside 👇 for loop
  constexpr sycl::uchar expected[28] = { 252, 149, 212, 78,  128, 108, 187,
                                         212, 132, 227, 121, 136, 34,  56,
                                         245, 85,  253, 169, 35,  135, 140,
                                         68,  58,  190, 76,  228, 205, 214 };

  // acquire resources
  sycl::uchar* in = static_cast<sycl::uchar*>(sycl::malloc_shared(56, q));
  sycl::uchar* out = static_cast<sycl::uchar*>(sycl::malloc_shared(28, q));

#pragma unroll 8
  for (size_t i = 0; i < 56; i++) {
    // preparing input for testing 2-to-1 SHA3-224 hash
    *(in + i) = i;
  }

  // enqueue kernel execution in single work-item model
  q.single_task<class kernelTestSHA3_224>([=]() { sha3_224::hash(in, out); });
  q.wait();

  // check result !
  for (size_t i = 0; i < 28; i++) {
    assert(*(out + i) == expected[i]);
  }

  // ensure resources are deallocated
  sycl::free(in, q);
  sycl::free(out, q);
}
