#include "test_bit_interleaving.hpp"
#include "test_merklize.hpp"
#include <iostream>

#if defined SHA1
#include "test_sha1.hpp"
#elif defined SHA2_224
#include "test_sha2_224.hpp"
#elif defined SHA2_256
#include "test_sha2_256.hpp"
#elif defined SHA2_384
#include "test_sha2_384.hpp"
#elif defined SHA2_512
#include "test_sha2_512.hpp"
#elif defined SHA2_512_224
#include "test_sha2_512_224.hpp"
#elif defined SHA2_512_256
#include "test_sha2_512_256.hpp"
#elif defined SHA3_256
#include "test_sha3_256.hpp"
#elif defined SHA3_224
#include "test_sha3_224.hpp"
#elif defined SHA3_384
#include "test_sha3_384.hpp"
#elif defined SHA3_512
#include "test_sha3_512.hpp"
#elif defined KECCAK_256_U64 || defined KECCAK_256_U32
#include "test_keccak_256.hpp"
#endif

int
main(int argc, char** argv)
{
  sycl::default_selector s{};
  sycl::device d{ s };
  sycl::context c{ d };
  // this is required for finding execution time of kernels on accelerator !
  sycl::queue q{ c, d, sycl::property::queue::enable_profiling{} };

  std::cout << "running on " << d.get_info<sycl::info::device::name>()
            << std::endl
            << std::endl;

  test_bit_interleaving<1ul << 20>();
  std::cout << "passed bit interleaving test !" << std::endl;

#if defined SHA1

  test_sha1(q);
  std::cout << "passed SHA1     test !" << std::endl;

#elif defined SHA2_224

  test_sha2_224(q);
  std::cout << "passed SHA2-224 test !" << std::endl;

#elif defined SHA2_256

  test_sha2_256(q);
  std::cout << "passed SHA2-256 test !" << std::endl;

#elif defined SHA2_384

  test_sha2_384(q);
  std::cout << "passed SHA2-384 test !" << std::endl;

#elif defined SHA2_512

  test_sha2_512(q);
  std::cout << "passed SHA2-512 test !" << std::endl;

#elif defined SHA2_512_224

  test_sha2_512_224(q);
  std::cout << "passed SHA2-512/224 test !" << std::endl;

#elif defined SHA2_512_256

  test_sha2_512_256(q);
  std::cout << "passed SHA2-512/256 test !" << std::endl;

#elif defined SHA3_256

  test_sha3_256(q);
  std::cout << "passed SHA3-256 test !" << std::endl;

#elif defined SHA3_224

  test_sha3_224(q);
  std::cout << "passed SHA3-224 test !" << std::endl;

#elif defined SHA3_384

  test_sha3_384(q);
  std::cout << "passed SHA3-384 test !" << std::endl;

#elif defined SHA3_512

  test_sha3_512(q);
  std::cout << "passed SHA3-512 test !" << std::endl;

#elif defined KECCAK_256_U64 || defined KECCAK_256_U32

  test_keccak_256(q);
  std::cout << "passed Keccak-256 test !" << std::endl;

#endif

  test_merklize(q);

#if defined SHA1
  std::cout << "passed binary merklization ( using SHA1 ) test !" << std::endl;
#elif defined SHA2_224
  std::cout << "passed binary merklization ( using SHA2-224 ) test !"
            << std::endl;
#elif defined SHA2_256
  std::cout << "passed binary merklization ( using SHA2-256 ) test !"
            << std::endl;
#elif defined SHA2_384
  std::cout << "passed binary merklization ( using SHA2-384 ) test !"
            << std::endl;
#elif defined SHA2_512
  std::cout << "passed binary merklization ( using SHA2-512 ) test !"
            << std::endl;
#elif defined SHA2_512_224
  std::cout << "passed binary merklization ( using SHA2-512/224 ) test !"
            << std::endl;
#elif defined SHA2_512_256
  std::cout << "passed binary merklization ( using SHA2-512/256 ) test !"
            << std::endl;
#elif defined SHA3_256
  std::cout << "passed binary merklization ( using SHA3-256 ) test !"
            << std::endl;
#elif defined SHA3_224
  std::cout << "passed binary merklization ( using SHA3-224 ) test !"
            << std::endl;
#elif defined SHA3_384
  std::cout << "passed binary merklization ( using SHA3-384 ) test !"
            << std::endl;
#elif defined SHA3_512
  std::cout << "passed binary merklization ( using SHA3-512 ) test !"
            << std::endl;
#elif defined KECCAK_256_U64
  std::cout
    << "passed binary merklization ( using KECCAK-256, 64 -bit word ) test !"
    << std::endl;
#elif defined KECCAK_256_U32
  std::cout
    << "passed binary merklization ( using KECCAK-256, 32 -bit word ) test !"
    << std::endl;
#endif

  return EXIT_SUCCESS;
}
