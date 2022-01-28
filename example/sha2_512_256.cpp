#include "sha2_512_256.hpp"
#include <cassert>

// This example attempts to show how to use 2-to-1 SHA2-512/256 hash function !
int
main(int argc, char** argv)
{
  // $ python3
  // >>> a = [0xff] * 32
  //
  // first input digest
  constexpr sycl::uchar digest_0[32] = {
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255
  };

  // >>> b = [0x0f] * 32
  //
  // second input digest
  constexpr sycl::uchar digest_1[32] = { 15, 15, 15, 15, 15, 15, 15, 15,
                                         15, 15, 15, 15, 15, 15, 15, 15,
                                         15, 15, 15, 15, 15, 15, 15, 15,
                                         15, 15, 15, 15, 15, 15, 15, 15 };

  // >>> c = a + b
  // >>> from Crypto.Hash import SHA512
  // >>> list(SHA512.new(data=bytes(c), truncate='256').digest())
  //
  // final output digest after merging two input digests
  constexpr sycl::uchar digest_2[32] = {
    181, 113, 77,  90,  168, 239, 241, 120, 194, 91, 93,  15, 166, 83,  98,  67,
    151, 51,  125, 123, 23,  103, 109, 36,  151, 26, 167, 0,  143, 191, 194, 221
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

  q.single_task<class kernelExampleSHA2_512_256>([=]() {
    sycl::uchar padded[128];
    sycl::ulong parsed[16];
    sycl::ulong digest[4];

    // pad input so that it's two full message blocks
    sha2_512_256::pad_input_message(in, padded);

    // parse message blocks into words
#pragma unroll 8
    for (size_t i = 0; i < 16; i++) {
      parsed[i] = from_be_bytes_to_u64_words(padded + (i << 3));
    }

    // now compute SHA2-512/256 on input words
    sha2_512_256::hash(parsed, digest);

    // convert SHA2-512/256 digest words to big endian bytes
#pragma unroll 4
    for (size_t i = 0; i < 4; i++) {
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
