#include "sha2_224.hpp"
#include <cassert>

// This example attempts to show how to use 2-to-1 SHA2-224 hash function !
int
main(int argc, char** argv)
{
  // $ python3
  // >>> a = [0xff] * 28
  //
  // first input digest
  constexpr sycl::uchar digest_0[28] = { 255, 255, 255, 255, 255, 255, 255,
                                         255, 255, 255, 255, 255, 255, 255,
                                         255, 255, 255, 255, 255, 255, 255,
                                         255, 255, 255, 255, 255, 255, 255 };
  // >>> b = [0x0f] * 28
  //
  // second input digest
  constexpr sycl::uchar digest_1[28] = { 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
                                         15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
                                         15, 15, 15, 15, 15, 15, 15, 15 };

  // >>> c = a + b
  // >>> import hashlib
  // >>> list(hashlib.sha224(bytes(c)).digest())
  //
  // final output digest after merging two input digests
  constexpr sycl::uchar digest_2[28] = { 168, 239, 85,  136, 55,  216, 197,
                                         106, 126, 224, 146, 191, 38,  143,
                                         213, 130, 52,  170, 14,  66,  157,
                                         155, 179, 118, 194, 193, 205, 83 };

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

  q.single_task<class kernelExampleSHA2_224>([=]() {
    sycl::uchar padded[128];
    sycl::uint parsed[32];
    sycl::uint digest[7];

    // pad input so that it's two full message blocks
    sha2_224::pad_input_message(in, padded);

    // parse message blocks into words
#pragma unroll 16
    for (size_t i = 0; i < 32; i++) {
      parsed[i] = from_be_bytes_to_words(padded + i * 4);
    }

    // now compute SHA2-224 on input words
    sha2_224::hash(parsed, digest);

    // convert SHA2-224 digest words to big endian bytes
#pragma unroll 7
    for (size_t i = 0; i < 7; i++) {
      from_words_to_be_bytes(*(digest + i), out + i * 4);
    }
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
