#pragma once
#include "sha3_512.hpp"
#include <cassert>

void
test_sha3_512(sycl::queue& q)
{
  // obtained by executing following snippet in python3 shell
  //
  // >>> import hashlib
  // >>> list(hashlib.sha3_512(bytes([i for i in range(128)])).digest())
  //
  // note, same input is prepared inside 👇 for loop
  constexpr sycl::uchar expected[64] = {
    152, 156, 25,  149, 218, 157, 45,  52,  31,  153, 60,  46,  44,
    166, 149, 243, 71,  112, 117, 6,   27,  251, 210, 205, 240, 190,
    117, 207, 123, 169, 159, 190, 51,  216, 210, 196, 220, 195, 31,
    168, 153, 23,  120, 107, 136, 62,  108, 157, 91,  2,   237, 129,
    183, 72,  58,  76,  179, 234, 152, 103, 21,  136, 247, 69
  };

  // acquire resources
  sycl::uchar* in = static_cast<sycl::uchar*>(sycl::malloc_shared(128, q));
  sycl::uchar* out = static_cast<sycl::uchar*>(sycl::malloc_shared(64, q));

#pragma unroll 16
  for (size_t i = 0; i < 128; i++) {
    // preparing input for testing 2-to-1 SHA3-512 hash
    *(in + i) = i;
  }

  // enqueue kernel execution in single work-item model
  q.single_task<class kernelTestSHA3_512>([=]() { sha3_512::hash(in, out); });
  q.wait();

  // check result !
  for (size_t i = 0; i < 64; i++) {
    assert(*(out + i) == expected[i]);
  }

  // ensure resources are deallocated
  sycl::free(in, q);
  sycl::free(out, q);
}
