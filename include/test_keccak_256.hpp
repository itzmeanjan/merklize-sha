#pragma once
#include "keccak_256.hpp"
#include <cassert>

void
test_keccak_256(sycl::queue& q)
{
  // obtained by executing following snippet in python3 shell
  //
  // $ python3 -m pip install --user pysha3
  // $ python3
  // >>> import sha3
  // >>> list(sha3.keccak_256(bytes([i for i in range(64)])).digest())
  //
  // note, same input is prepared inside 👇 for loop
  constexpr sycl::uchar expected[32] = {
    0,   32,  48,  189, 227, 212, 207, 137, 145, 150, 73,
    119, 92,  215, 24,  117, 196, 208, 171, 23,  8,   163,
    128, 224, 63,  239, 195, 162, 138, 162, 72,  49
  };

  // acquire resources
  sycl::uchar* in = static_cast<sycl::uchar*>(sycl::malloc_shared(64, q));
  sycl::uchar* out_0 = static_cast<sycl::uchar*>(sycl::malloc_shared(32, q));
  sycl::uchar* out_1 = static_cast<sycl::uchar*>(sycl::malloc_shared(32, q));

#pragma unroll 16
  for (size_t i = 0; i < 64; i++) {
    // preparing input for testing 2-to-1 Keccak-256 hash
    in[i] = i;
  }

  // enqueue kernel execution in single work-item model
  q.single_task<class kernelTestKeccak_256>([=]() {
    keccak_256::hash(in, out_0);
    keccak_256::hash_u32(in, out_1);
  });
  q.wait();

  // check result !
  for (size_t i = 0; i < 32; i++) {
    assert(out_0[i] == expected[i]);
    assert(out_1[i] == expected[i]);
  }

  // ensure resources are deallocated
  sycl::free(in, q);
  sycl::free(out_0, q);
  sycl::free(out_1, q);
}
