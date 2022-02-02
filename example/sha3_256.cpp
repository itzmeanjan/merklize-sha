#include "sha3_256.hpp"
#include <cassert>

// This example attempts to show how to use 2-to-1 SHA3-256 hash function !
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
  // >>> import hashlib
  // >>> list(hashlib.sha3_256(bytes(c)).digest())
  //
  // final output digest after merging two input digests
  constexpr sycl::uchar digest_2[32] = {
    121, 136, 237, 222, 17, 197, 60,  82,  161, 87, 52,  66,  251, 235, 8,  125,
    1,   95,  88,  134, 1,  235, 132, 182, 114, 55, 207, 202, 17,  104, 74, 95
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
  q.single_task<class kernelExampleSHA3_256>(
    [=]() { sha3_256::hash(in, out); });
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
