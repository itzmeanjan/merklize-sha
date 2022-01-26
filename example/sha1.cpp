#include "sha1.hpp"
#include <cassert>

// This example attempts to show how to use 2-to-1 SHA1 hash function !
int
main(int argc, char** argv)
{
  // a = [0xff] * 20
  //
  // first input digest
  constexpr sycl::uchar digest_0[20] = { 255, 255, 255, 255, 255, 255, 255,
                                         255, 255, 255, 255, 255, 255, 255,
                                         255, 255, 255, 255, 255, 255 };
  // b = [0x0f] * 20
  //
  // second input digest
  constexpr sycl::uchar digest_1[20] = {
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15
  };

  // >>> c = a + b
  // >>> import hashlib
  // >>> list(hashlib.sha1(bytes(c)).digest())
  //
  // final output digest after merging two input digests
  constexpr sycl::uchar digest_2[20] = { 75,  214, 120, 118, 80,  218, 70,
                                         167, 98,  76,  218, 115, 156, 23,
                                         96,  238, 203, 169, 232, 21 };

  sycl::default_selector s{};
  sycl::device d{ s };
  sycl::context c{ d };
  sycl::queue q{ c, d };

  // so that input digests can be transferred from host to device ( by runtime )
  sycl::uchar* in = static_cast<sycl::uchar*>(
    sycl::malloc_shared(sizeof(digest_0) + sizeof(digest_1), q));

  // so that output digest can be transferred from device to host ( by runtime )
  sycl::uchar* out =
    static_cast<sycl::uchar*>(sycl::malloc_shared(sizeof(digest_2), q));

  // copy both input digests to device memory
  q.memcpy(in + 0, digest_0, sizeof(digest_0)).wait();
  q.memcpy(in + sizeof(digest_0), digest_1, sizeof(digest_1)).wait();

  q.single_task<class kernelExampleSHA1>([=]() {
    sycl::uchar padded[64];
    sycl::uint parsed[16];
    sycl::uint digest[5];

    // pad input so that it's a full message block
    sha1::pad_input_message(in, padded);
    // parse message block into words
    sha1::parse_message_words(padded, parsed);
    // now compute SHA1 on input words
    sha1::hash(parsed, digest);
    // convert SHA1 digest words to big endian bytes
    sha1::words_to_be_bytes(digest, out);
  });
  q.wait();

  // finally assert !
  for (size_t i = 0; i < sizeof(digest_2); i++) {
    assert(*(out + i) == digest_2[i]);
  }

  // deallocate resources !
  sycl::free(in, q);
  sycl::free(out, q);

  return EXIT_SUCCESS;
}
