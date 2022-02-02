#include "sha3_512.hpp"
#include <cassert>

// This example attempts to show how to use 2-to-1 SHA3-512 hash function !
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
  // >>> list(hashlib.sha3_512(bytes(c)).digest())
  //
  // final output digest after merging two input digests
  constexpr sycl::uchar digest_2[64] = {
    73,  228, 11,  92,  59,  196, 139, 212, 163, 66,  229, 66,  106,
    155, 168, 55,  241, 215, 241, 253, 75,  61,  91,  215, 172, 186,
    250, 212, 10,  12,  61,  253, 80,  236, 57,  238, 27,  53,  53,
    20,  81,  55,  63,  196, 104, 93,  94,  74,  19,  36,  181, 15,
    41,  21,  198, 35,  60,  3,   65,  232, 15,  78,  220, 61
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
  q.single_task<class kernelExampleSHA3_512>(
    [=]() { sha3_512::hash(in, out); });
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
