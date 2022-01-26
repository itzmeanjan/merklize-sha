#pragma once
#include "sha2_224.hpp"
#include <cassert>

void
test_sha2_224(sycl::queue& q)
{
  // obtained by executing following snippet in python3 shell
  //
  // >>> import hashlib
  // >>> list(hashlib.sha224(bytes([i for i in range(56)])).digest())
  //
  // note, same input is prepared inside 👇 for loop
  constexpr sycl::uchar expected[28] = { 43,  44,  214, 55,  193, 106, 215,
                                         41,  11,  176, 103, 173, 125, 143,
                                         208, 78,  32,  79,  164, 58,  132,
                                         54,  106, 252, 113, 48,  244, 239 };

  // acquire resources
  sycl::uchar* in_0 = static_cast<sycl::uchar*>(sycl::malloc_shared(56, q));
  sycl::uint* in_1 = static_cast<sycl::uint*>(sycl::malloc_shared(56, q));
  sycl::uchar* out_0 = static_cast<sycl::uchar*>(sycl::malloc_shared(28, q));
  sycl::uchar* out_1 = static_cast<sycl::uchar*>(sycl::malloc_shared(28, q));

#pragma unroll 8
  for (size_t i = 0; i < 56; i++) {
    // preparing input for testing 2-to-1 SHA2-224 hash
    *(in_0 + i) = i;
  }

#pragma unroll 14
  for (size_t i = 0; i < 14; i++) {
    sycl::uint v = static_cast<sycl::uint>(i << 2);

    // preparing input to SHA2-224 hash function as words, instead of big endian
    // byte array, which is already prepared above
    //
    // so that I can test it both ways --- see below, two kernels dispatched
    *(in_1 + i) = (v + 0) << 24 | (v + 1) << 16 | (v + 2) << 8 | (v + 3) << 0;
  }

  // enqueue kernel execution in single work-item model
  q.single_task<class kernelTestSHA2_224_v0>([=]() {
    sycl::uchar padded[128];
    sycl::uint parsed[32];
    sycl::uint digest[7];

    sha2_224::pad_input_message(in_0, padded);

    // parsing message words such that each of four consecutive
    // big endian bytes are interpreted as SHA2-224 word ( = 32 -bit )
#pragma unroll 16
    for (size_t i = 0; i < 32; i++) {
      parsed[i] = from_be_bytes_to_words(padded + i * 4);
    }

    sha2_224::hash(parsed, digest);

    // converting each message word of digest into four consecutive big endian
    // bytes
#pragma unroll 7
    for (size_t i = 0; i < 7; i++) {
      from_words_to_be_bytes(*(digest + i), out_0 + i * 4);
    }
  });
  q.wait();

  q.single_task<class kernelTestSHA2_224_v1>([=]() {
    sycl::uint padded[32];
    sycl::uint digest[7];

    sha2_224::pad_input_message(in_1, padded);
    sha2_224::hash(padded, digest);

    // converting each message word of digest into four consecutive big endian
    // bytes
#pragma unroll 7
    for (size_t i = 0; i < 7; i++) {
      from_words_to_be_bytes(*(digest + i), out_1 + i * 4);
    }
  });
  q.wait();

  // check result !
  for (size_t i = 0; i < 28; i++) {
    assert(*(out_0 + i) == expected[i]);
    assert(*(out_1 + i) == expected[i]);
  }

  // ensure resources are deallocated
  sycl::free(in_0, q);
  sycl::free(in_1, q);
  sycl::free(out_0, q);
  sycl::free(out_1, q);
}
