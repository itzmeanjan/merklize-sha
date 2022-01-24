#include "test_sha1.hpp"
#include <iostream>

int
main(int argc, char** argv)
{
  sycl::default_selector s{};
  sycl::device d{ s };
  sycl::context c{ d };
  sycl::queue q{ c, d };

  std::cout << "running on " << d.get_info<sycl::info::device::name>()
            << std::endl
            << std::endl;

  test_sha1(q);
  std::cout << "passed SHA1 test !" << std::endl;

  return EXIT_SUCCESS;
}
