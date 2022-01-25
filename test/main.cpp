#include "test_merklize.hpp"
#include "test_sha1.hpp"
#include "test_sha2_224.hpp"
#include "test_sha2_256.hpp"
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
  std::cout << "passed SHA1 test !" << std::endl;
  test_merklize(q);
  std::cout << "passed binary merklization ( using SHA1 ) test !" << std::endl;

  test_sha2_256(q);
  std::cout << "passed SHA2-256 test !" << std::endl;

  test_sha2_224(q);
  std::cout << "passed SHA2-224 test !" << std::endl;

  return EXIT_SUCCESS;
}
