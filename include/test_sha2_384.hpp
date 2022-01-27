#pragma once
#include "sha2_384.hpp"
#include <cassert>

void
test_sha2_384(sycl::queue& q)
{
  // obtained by executing following snippet in python3 shell
  //
  // >>> import hashlib
  // >>> list(hashlib.sha384(bytes([i for i in range(96)])).digest())
  //
  // note, same input is prepared inside 👇 for loops
  constexpr sycl::uchar expected[48] = {
    230, 6,   0,   78,  205, 198, 135, 139, 94,  193, 95,  69,
    84,  1,   124, 207, 150, 46,  146, 204, 110, 174, 190, 73,
    151, 186, 52,  236, 14,  83,  198, 125, 86,  76,  132, 97,
    192, 19,  112, 26,  64,  31,  227, 71,  236, 15,  114, 30
  };

  // acquire resources
  sycl::uchar* in_0 = static_cast<sycl::uchar*>(sycl::malloc_shared(96, q));
  sycl::ulong* in_1 = static_cast<sycl::ulong*>(sycl::malloc_shared(96, q));
  sycl::uchar* out_0 = static_cast<sycl::uchar*>(sycl::malloc_shared(48, q));
  sycl::uchar* out_1 = static_cast<sycl::uchar*>(sycl::malloc_shared(48, q));

#pragma unroll 16
  for (size_t i = 0; i < 96; i++) {
    // preparing input for testing 2-to-1 SHA2-384 hash
    *(in_0 + i) = i;
  }

#pragma unroll 12
  for (size_t i = 0; i < 12; i++) {
    sycl::ulong v = static_cast<sycl::ulong>(i << 3);

    // preparing input to SHA2-384 hash function as words, instead of big endian
    // byte array, which is already prepared above
    //
    // so that I can test it both ways --- see below, two kernels dispatched
    *(in_1 + i) = (v + 0) << 56 | (v + 1) << 48 | (v + 2) << 40 |
                  (v + 3) << 32 | (v + 4) << 24 | (v + 5) << 16 | (v + 6) << 8 |
                  (v + 7) << 0;
  }

  // enqueue kernel execution in single work-item model
  q.single_task<class kernelTestSHA2_384_v0>([=]() {
    sycl::uchar padded[128];
    sycl::ulong parsed[16];
    sycl::ulong digest[6];

    sha2_384::pad_input_message(in_0, padded);

#pragma unroll 8
    for (size_t i = 0; i < 16; i++) {
      parsed[i] = from_be_bytes_to_u64_words(padded + (i << 3));
    }

    sha2_384::hash(parsed, digest);

    // converting each message word of digest into eight consecutive big endian
    // bytes, making total 48 -bytes SHA2-384 digest
#pragma unroll 6
    for (size_t i = 0; i < 6; i++) {
      from_words_to_be_bytes(*(digest + i), out_0 + (i << 3));
    }
  });
  q.wait();

  q.single_task<class kernelTestSHA2_384_v1>([=]() {
    sycl::ulong padded[16];
    sycl::ulong digest[6];

    sha2_384::pad_input_message(in_1, padded);
    sha2_384::hash(padded, digest);

    // converting each message word of digest into eight consecutive big endian
    // bytes, making total 48 -bytes SHA2-384 digest
#pragma unroll 6
    for (size_t i = 0; i < 6; i++) {
      from_words_to_be_bytes(*(digest + i), out_1 + (i << 3));
    }
  });
  q.wait();

  // check result !
  for (size_t i = 0; i < 48; i++) {
    assert(*(out_0 + i) == expected[i]);
    assert(*(out_1 + i) == expected[i]);
  }

  // ensure resources are deallocated
  sycl::free(in_0, q);
  sycl::free(in_1, q);
  sycl::free(out_0, q);
  sycl::free(out_1, q);
}
