#pragma once
#include "sha2_512.hpp"
#include <cassert>

void
test_sha2_512(sycl::queue& q)
{
  // obtained by executing following snippet in python3 shell
  //
  // >>> import hashlib
  // >>> list(hashlib.sha512(bytes([i for i in range(128)])).digest())
  //
  // note, same input is prepared inside 👇 for loops
  constexpr sycl::uchar expected[64] = {
    29,  255, 213, 227, 173, 183, 29,  69,  210, 36,  89,  57,  102,
    85,  33,  174, 0,   26,  49,  122, 3,   114, 10,  69,  115, 43,
    161, 144, 12,  163, 184, 53,  31,  197, 201, 180, 202, 81,  62,
    186, 111, 128, 188, 123, 29,  31,  218, 212, 171, 209, 52,  145,
    203, 130, 77,  97,  176, 141, 140, 14,  21,  97,  179, 247
  };

  // acquire resources
  sycl::uchar* in_0 = static_cast<sycl::uchar*>(sycl::malloc_shared(128, q));
  sycl::ulong* in_1 = static_cast<sycl::ulong*>(sycl::malloc_shared(128, q));
  sycl::uchar* out_0 = static_cast<sycl::uchar*>(sycl::malloc_shared(64, q));
  sycl::uchar* out_1 = static_cast<sycl::uchar*>(sycl::malloc_shared(64, q));

#pragma unroll 32
  for (size_t i = 0; i < 128; i++) {
    // preparing input for testing 2-to-1 SHA2-512 hash
    *(in_0 + i) = i;
  }

#pragma unroll 8
  for (size_t i = 0; i < 16; i++) {
    sycl::ulong v = static_cast<sycl::ulong>(i << 3);

    // preparing input to SHA2-512 hash function as words, instead of big endian
    // byte array, which is already prepared above
    //
    // so that I can test it both ways --- see below, two kernels dispatched
    *(in_1 + i) = (v + 0) << 56 | (v + 1) << 48 | (v + 2) << 40 |
                  (v + 3) << 32 | (v + 4) << 24 | (v + 5) << 16 | (v + 6) << 8 |
                  (v + 7) << 0;
  }

  // enqueue kernel execution in single work-item model
  q.single_task<class kernelTestSHA2_512_v0>([=]() {
    sycl::uchar padded[256];
    sycl::ulong parsed[32];
    sycl::ulong digest[8];

    sha2_512::pad_input_message(in_0, padded);

#pragma unroll 8
    for (size_t i = 0; i < 32; i++) {
      parsed[i] = from_be_bytes_to_u64_words(padded + (i << 3));
    }

    sha2_512::hash(parsed, digest);

    // converting each message word of digest into eight consecutive big endian
    // bytes, making total 64 -bytes SHA2-512 digest
#pragma unroll 8
    for (size_t i = 0; i < 8; i++) {
      from_words_to_be_bytes(*(digest + i), out_0 + (i << 3));
    }
  });
  q.wait();

  q.single_task<class kernelTestSHA2_512_v1>([=]() {
    sycl::ulong padded[32];
    sycl::ulong digest[8];

    sha2_512::pad_input_message(in_1, padded);
    sha2_512::hash(padded, digest);

    // converting each message word of digest into eight consecutive big endian
    // bytes, making total 64 -bytes SHA2-512 digest
#pragma unroll 8
    for (size_t i = 0; i < 8; i++) {
      from_words_to_be_bytes(*(digest + i), out_1 + (i << 3));
    }
  });
  q.wait();

  // check result !
  for (size_t i = 0; i < 64; i++) {
    assert(*(out_0 + i) == expected[i]);
    assert(*(out_1 + i) == expected[i]);
  }

  // ensure resources are deallocated
  sycl::free(in_0, q);
  sycl::free(in_1, q);
  sycl::free(out_0, q);
  sycl::free(out_1, q);
}
