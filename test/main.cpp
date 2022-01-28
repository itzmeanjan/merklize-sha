#include "test_merklize.hpp"
#include "test_sha1.hpp"
#include "test_sha2_224.hpp"
#include "test_sha2_256.hpp"
#include "test_sha2_384.hpp"
#include "test_sha2_512.hpp"
#include "test_sha2_512_224.hpp"
#include <iostream>

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

  test_sha1(q);
  std::cout << "passed SHA1     test !" << std::endl;

  test_sha2_224(q);
  std::cout << "passed SHA2-224 test !" << std::endl;

  test_sha2_256(q);
  std::cout << "passed SHA2-256 test !" << std::endl;

  test_sha2_384(q);
  std::cout << "passed SHA2-384 test !" << std::endl;

  test_sha2_512(q);
  std::cout << "passed SHA2-512 test !" << std::endl;

  test_sha2_512_224(q);
  std::cout << "passed SHA2-512/224 test !" << std::endl;

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
#endif

  return EXIT_SUCCESS;
}
