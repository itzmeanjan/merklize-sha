#pragma once
#include "sha3_256.hpp"
#include <cassert>

void
test_sha3_256(sycl::queue& q)
{
  // obtained by executing following snippet in python3 shell
  //
  // >>> import hashlib
  // >>> list(hashlib.sha3_256(bytes([i for i in range(64)])).digest())
  //
  // note, same input is prepared inside 👇 for loop
  constexpr sycl::uchar expected[32] = { 200, 173, 71,  143, 78,  29,  217, 212,
                                         125, 252, 59,  152, 87,  8,   217, 45,
                                         177, 248, 219, 72,  254, 156, 221, 212,
                                         89,  230, 60,  50,  31,  73,  4,   2 };

  // acquire resources
  sycl::uchar* in = static_cast<sycl::uchar*>(sycl::malloc_shared(64, q));
  sycl::uchar* out = static_cast<sycl::uchar*>(sycl::malloc_shared(32, q));

#pragma unroll 16
  for (size_t i = 0; i < 64; i++) {
    // preparing input for testing 2-to-1 SHA3-256 hash
    *(in + i) = i;
  }

  // enqueue kernel execution in single work-item model
  q.single_task<class kernelTestSHA3_256>([=]() { sha3_256::hash(in, out); });
  q.wait();

  // check result !
  for (size_t i = 0; i < 32; i++) {
    assert(*(out + i) == expected[i]);
  }

  // ensure resources are deallocated
  sycl::free(in, q);
  sycl::free(out, q);
}
