#include "keccak_256.hpp"
#include <cassert>

// This example attempts to show how to use 2-to-1 KECCAK-256 hash function !
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
  // >>> import sha3
  // >>> list(sha3.keccak_256(bytes(c)).digest())
  //
  // final output digest after merging two input digests
  constexpr sycl::uchar digest_2[32] = { 134, 168, 210, 52, 189, 102, 98,  76,
                                         116, 7,   56,  37, 176, 235, 150, 128,
                                         138, 149, 200, 26, 32,  23,  79,  38,
                                         139, 137, 109, 85, 195, 183, 230, 9 };

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
  q.single_task<class kernelExampleKECCAK_256>([=]() {
    // if you want, you can replace `keccak_256::hash` with
    // `keccak_256::hash_u32`, which represents each lane of state array using
    // two 32 -bit unsigned integers ( bit interleaved form ) and uses only 32
    // -bit bitwise operations when computing keccak256 2-to-1 hash
    keccak_256::hash(in, out);
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
