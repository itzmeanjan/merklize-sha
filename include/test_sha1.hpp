#pragma once
#include "sha1.hpp"
#include <cassert>

void
test_sha1(sycl::queue& q)
{
  sycl::uchar expected[20] = {
    204, 154, 217, 158, 145, 112, 66, 56,  27,  15,
    153, 88,  136, 150, 203, 242, 54, 170, 142, 211
  };

  sycl::uchar* in = static_cast<sycl::uchar*>(sycl::malloc_shared(40, q));
  sycl::uchar* padded = static_cast<sycl::uchar*>(sycl::malloc_shared(64, q));
  sycl::uint* parsed = static_cast<sycl::uint*>(sycl::malloc_shared(64, q));
  sycl::uint* digest = static_cast<sycl::uint*>(sycl::malloc_shared(20, q));
  sycl::uchar* out = static_cast<sycl::uchar*>(sycl::malloc_shared(20, q));

#pragma unroll 8
  for (size_t i = 0; i < 40; i++) {
    *(in + i) = i;
  }

  q.single_task([=]() {
    sha1::pad_input_message(in, padded);
    sha1::parse_message_words(padded, parsed);
    sha1::hash(parsed, digest);
    sha1::words_to_be_bytes(digest, out);
  });
  q.wait();

  for (size_t i = 0; i < 20; i++) {
    assert(*(out + i) == expected[i]);
  }

  sycl::free(in, q);
  sycl::free(padded, q);
  sycl::free(parsed, q);
  sycl::free(digest, q);
  sycl::free(out, q);
}
