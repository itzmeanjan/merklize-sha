#include "sha2_384.hpp"
#include <cassert>

// This example attempts to show how to use 2-to-1 SHA2-384 hash function !
int
main(int argc, char** argv)
{
  // $ python3
  // >>> a = [0xff] * 48
  //
  // first input digest
  constexpr sycl::uchar digest_0[48] = {
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255
  };

  // >>> b = [0xff] * 48
  //
  // second input digest
  constexpr sycl::uchar digest_1[48] = {
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255
  };

  // >>> c = a + b
  // >>> import hashlib
  // >>> list(hashlib.sha384(bytes(c)).digest())
  //
  // final output digest after merging two input digests
  constexpr sycl::uchar digest_2[48] = {
    120, 195, 4,   101, 32,  184, 165, 150, 9,   221, 16,  126,
    43,  186, 64,  107, 143, 124, 119, 179, 53,  135, 31,  39,
    146, 115, 75,  158, 151, 254, 247, 182, 91,  31,  17,  212,
    123, 219, 246, 75,  217, 24,  111, 77,  215, 195, 125, 165
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

  q.single_task<class kernelExampleSHA2_384>([=]() {
    sycl::uchar padded[128];
    sycl::ulong parsed[16];
    sycl::ulong digest[6];

    // pad input so that it's two full message blocks
    sha2_384::pad_input_message(in, padded);

    // parse message blocks into words
#pragma unroll 8
    for (size_t i = 0; i < 16; i++) {
      parsed[i] = from_be_bytes_to_u64_words(padded + (i << 3));
    }

    // now compute SHA2-384 on input words
    sha2_384::hash(parsed, digest);

    // convert SHA2-384 digest words to big endian bytes
#pragma unroll 6
    for (size_t i = 0; i < 6; i++) {
      from_words_to_be_bytes(*(digest + i), out + (i << 3));
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
