#include "sha2_512.hpp"
#include <cassert>

// This example attempts to show how to use 2-to-1 SHA2-512 hash function !
int
main(int argc, char** argv)
{
  // $ python3
  // >>> a = [0xff] * 64
  //
  // first input digest
  constexpr sycl::uchar digest_0[64] = {
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255
  };

  // >>> b = [0x0f] * 64
  //
  // second input digest
  constexpr sycl::uchar digest_1[64] = {
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15
  };

  // >>> c = a + b
  // >>> import hashlib
  // >>> list(hashlib.sha512(bytes(c)).digest())
  //
  // final output digest after merging two input digests
  constexpr sycl::uchar digest_2[64] = {
    22,  120, 32,  19,  115, 1,   226, 191, 63,  114, 128, 219, 4,
    213, 145, 198, 52,  45,  102, 194, 8,   17,  107, 55,  114, 214,
    20,  23,  159, 17,  38,  46,  185, 203, 74,  117, 23,  163, 40,
    28,  148, 79,  50,  110, 156, 157, 196, 114, 96,  110, 82,  90,
    13,  55,  157, 186, 151, 174, 78,  105, 157, 251, 155, 160
  };

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

  q.single_task<class kernelExampleSHA2_512>([=]() {
    sycl::uchar padded[256];
    sycl::ulong parsed[32];
    sycl::ulong digest[8];

    // pad input so that it's two full message blocks
    sha2_512::pad_input_message(in, padded);

    // parse message blocks into words
#pragma unroll 8
    for (size_t i = 0; i < 32; i++) {
      parsed[i] = from_be_bytes_to_u64_words(padded + (i << 3));
    }

    // now compute SHA2-512 on input words
    sha2_512::hash(parsed, digest);

    // convert SHA2-512 digest words to big endian bytes
#pragma unroll 8
    for (size_t i = 0; i < 8; i++) {
      from_words_to_be_bytes(digest[i], out + (i << 3));
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
