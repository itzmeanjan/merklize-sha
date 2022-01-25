#pragma once
#include "sha2_256.hpp"
#include <cassert>

void
test_sha2_256(sycl::queue& q)
{
  // obtained by executing following snippet in python3 shell
  //
  // >>> import hashlib
  // >>> list(hashlib.sha256(bytes([i for i in range(64)])).digest())
  //
  // note, same input is prepared inside 👇 for loop
  constexpr sycl::uchar expected[32] = { 253, 234, 185, 172, 243, 113, 3,   98,
                                         189, 38,  88,  205, 201, 162, 158, 143,
                                         156, 117, 127, 207, 152, 17,  96,  58,
                                         140, 68,  124, 209, 217, 21,  17,  8 };

  // acquire resources
  sycl::uchar* in_0 = static_cast<sycl::uchar*>(sycl::malloc_shared(64, q));
  sycl::uint* in_1 = static_cast<sycl::uint*>(sycl::malloc_shared(64, q));
  sycl::uchar* out_0 = static_cast<sycl::uchar*>(sycl::malloc_shared(32, q));
  sycl::uchar* out_1 = static_cast<sycl::uchar*>(sycl::malloc_shared(32, q));

#pragma unroll 16
  for (size_t i = 0; i < 64; i++) {
    // preparing input for testing 2-to-1 SHA2-256 hash
    *(in_0 + i) = i;
  }

#pragma unroll 16
  for (size_t i = 0; i < 16; i++) {
    sycl::uint v = static_cast<sycl::uint>(i << 2);

    // preparing input to SHA2-256 hash function as words, instead of big endian
    // byte array, which is already prepared above
    //
    // so that I can test it both ways --- see below, two kernels dispatched
    *(in_1 + i) = (v + 0) << 24 | (v + 1) << 16 | (v + 2) << 8 | (v + 3) << 0;
  }

  // enqueue kernel execution in single work-item model
  q.single_task<class kernelTestSHA2_256_v0>([=]() {
    sycl::uchar padded[128];
    sycl::uint parsed[32];
    sycl::uint digest[8];

    sha2_256::pad_input_message(in_0, padded);
    sha2_256::parse_message_words(padded, parsed);
    sha2_256::hash(parsed, digest);

    // converting each message word of digest into four consecutive big endian
    // bytes
#pragma unroll 8
    for (size_t i = 0; i < 8; i++) {
      from_words_to_be_bytes(*(digest + i), out_0 + i * 4);
    }
  });
  q.wait();

  q.single_task<class kernelTestSHA2_256_v1>([=]() {
    sycl::uint padded[32];
    sycl::uint digest[8];

    sha2_256::pad_input_message(in_1, padded);
    sha2_256::hash(padded, digest);

    // converting each message word of digest into four consecutive big endian
    // bytes
#pragma unroll 8
    for (size_t i = 0; i < 8; i++) {
      from_words_to_be_bytes(*(digest + i), out_1 + i * 4);
    }
  });
  q.wait();

  // check result !
  for (size_t i = 0; i < 32; i++) {
    assert(*(out_0 + i) == expected[i]);
    assert(*(out_1 + i) == expected[i]);
  }

  // ensure resources are deallocated
  sycl::free(in_0, q);
  sycl::free(in_1, q);
  sycl::free(out_0, q);
  sycl::free(out_1, q);
}
