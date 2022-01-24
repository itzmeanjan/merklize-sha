#pragma once
#include "sha1.hpp"
#include <cassert>

void
test_sha1(sycl::queue& q)
{
  // obtained by executing following snippet in python3 shell
  //
  // >>> import hashlib
  // >>> list(hashlib.sha1(bytes([i for i in range(40)])).digest())
  //
  // note, same input is prepared inside 👇 for loop
  sycl::uchar expected[20] = {
    204, 154, 217, 158, 145, 112, 66, 56,  27,  15,
    153, 88,  136, 150, 203, 242, 54, 170, 142, 211
  };

  // acquire resources
  sycl::uchar* in_0 = static_cast<sycl::uchar*>(sycl::malloc_shared(40, q));
  sycl::uint* in_1 = static_cast<sycl::uint*>(sycl::malloc_shared(40, q));
  sycl::uchar* out_0 = static_cast<sycl::uchar*>(sycl::malloc_shared(20, q));
  sycl::uchar* out_1 = static_cast<sycl::uchar*>(sycl::malloc_shared(20, q));

#pragma unroll 8
  for (size_t i = 0; i < 40; i++) {
    // preparing input for testing 2-to-1 SHA1 hash
    *(in_0 + i) = i;
  }

#pragma unroll 10
  for (size_t i = 0; i < 10; i++) {
    sycl::uint v = static_cast<sycl::uint>(i << 2);

    *(in_1 + i) = (v + 0) << 24 | (v + 1) << 16 | (v + 2) << 8 | (v + 3) << 0;
  }

  // enqueue kernel execution in single work-item model
  q.single_task<class kernelTestSHA1_v0>([=]() {
    sycl::uchar padded[64];
    sycl::uint parsed[16];
    sycl::uint digest[5];

    sha1::pad_input_message(in_0, padded);
    sha1::parse_message_words(padded, parsed);
    sha1::hash(parsed, digest);
    sha1::words_to_be_bytes(digest, out_0);
  });
  q.wait();

  q.single_task<class kernelTestSHA1_v1>([=]() {
    sycl::uint padded[16];
    sycl::uint digest[5];

    sha1::pad_input_message(in_1, padded);
    sha1::hash(padded, digest);
    sha1::words_to_be_bytes(digest, out_1);
  });
  q.wait();

  // check result !
  for (size_t i = 0; i < 20; i++) {
    assert(*(out_0 + i) == expected[i]);
    assert(*(out_1 + i) == expected[i]);
  }

  // ensure resources are deallocated
  sycl::free(in_0, q);
  sycl::free(in_1, q);
  sycl::free(out_0, q);
  sycl::free(out_1, q);
}
