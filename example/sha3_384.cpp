#include "sha3_384.hpp"
#include <cassert>

// This example attempts to show how to use 2-to-1 SHA3-384 hash function !
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

  // >>> b = [0x0f] * 48
  //
  // second input digest
  constexpr sycl::uchar digest_1[48] = { 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
                                         15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
                                         15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
                                         15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
                                         15, 15, 15, 15, 15, 15, 15, 15 };

  // >>> c = a + b
  // >>> import hashlib
  // >>> list(hashlib.sha3_384(bytes(c)).digest())
  //
  // final output digest after merging two input digests
  constexpr sycl::uchar digest_2[48] = {
    25,  254, 93,  230, 2,  191, 78,  51,  238, 228, 239, 160,
    231, 101, 38,  216, 38, 8,   135, 59,  34,  169, 154, 20,
    221, 245, 50,  59,  27, 9,   21,  234, 249, 223, 45,  73,
    214, 0,   146, 51,  25, 83,  0,   0,   111, 210, 47,  206
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

  // compute 2-to-1 hash
  q.single_task<class kernelExampleSHA3_384>(
    [=]() { sha3_384::hash(in, out); });
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
