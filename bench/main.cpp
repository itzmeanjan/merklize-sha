#include "bench_merklize.hpp"
#include <iomanip>
#include <iostream>

// Taken from
// https://github.com/itzmeanjan/blake3/blob/8b07337774df0544d411d04caf6ccd23c571b4f4/bench/main.cpp#L158-L165
std::string
to_readable_timespan(double ts);

// Compute average execution time of kernel
//
// taken from
// https://github.com/itzmeanjan/blake3/blob/8b07337774df0544d411d04caf6ccd23c571b4f4/bench/main.cpp#L111-L156
void
take_avg(sycl::queue& q,
         size_t leaf_cnt,
         size_t wg_size,
         size_t itr_cnt,
         double* const ts);

// This function implementation is adapted from
// https://github.com/itzmeanjan/blake3/blob/8b07337774df0544d411d04caf6ccd23c571b4f4/bench/main.cpp#L24-L26
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

  const size_t wg_size = 1 << 5;
  const size_t itr_cnt = 1 << 3;

  double* ts = (double*)std::malloc(sizeof(double) * 3);

#if defined SHA1
  std::cout << "\nBenchmarking Binary Merklization using SHA1" << std::endl
            << std::endl;
#elif defined SHA2_224
  std::cout << "\nBenchmarking Binary Merklization using SHA2-224" << std::endl
            << std::endl;
#elif defined SHA2_256
  std::cout << "\nBenchmarking Binary Merklization using SHA2-256" << std::endl
            << std::endl;
#elif defined SHA2_384
  std::cout << "\nBenchmarking Binary Merklization using SHA2-384" << std::endl
            << std::endl;
#elif defined SHA2_512
  std::cout << "\nBenchmarking Binary Merklization using SHA2-512" << std::endl
            << std::endl;
#endif

  std::cout << std::setw(16) << std::right << "leaf count"
            << "\t\t" << std::setw(16) << std::right << "execution time"
            << "\t\t" << std::setw(16) << std::right << "host-to-device tx time"
            << "\t\t" << std::setw(16) << std::right << "device-to-host tx time"
            << std::endl;

  for (size_t i = 20; i <= 25; i++) {
    const size_t leaf_cnt = 1 << i;

    take_avg(q, leaf_cnt, wg_size, itr_cnt, ts);

    std::cout << std::setw(12) << std::right << "2 ^ " << i << "\t\t"
              << std::setw(22) << std::right << to_readable_timespan(*(ts + 1))
              << "\t\t" << std::setw(22) << std::right
              << to_readable_timespan(*(ts + 0)) << "\t\t" << std::setw(22)
              << std::right << to_readable_timespan(*(ts + 2)) << std::endl;
  }

  std::free(ts);

  return EXIT_SUCCESS;
}

void
take_avg(sycl::queue& q,
         size_t leaf_cnt,
         size_t wg_size,
         size_t itr_cnt,
         double* const ts)
{
  size_t req_size = sizeof(sycl::cl_ulong) * 3;

  sycl::cl_ulong* ts_acc = (sycl::cl_ulong*)std::malloc(req_size);
  sycl::cl_ulong* ts_cur = (sycl::cl_ulong*)std::malloc(req_size);

  // so that accumulation begins with empty slate !
  memset(ts_acc, 0, req_size);

  for (size_t i = 0; i < itr_cnt; i++) {
    benchmark_merklize(q, leaf_cnt, wg_size, ts_cur);

#pragma unroll 3
    for (size_t j = 0; j < 3; j++) {
      *(ts_acc + i) += *(ts_cur + i);
    }
  }

#pragma unroll 3
  for (size_t i = 0; i < 3; i++) {
    *(ts + i) = (double)*(ts_acc + i) / (double)itr_cnt;
  }

  std::free(ts_acc);
  std::free(ts_cur);
}

std::string
to_readable_timespan(double ts)
{
  return ts >= 1e9 ? std::to_string(ts * 1e-9) + " s"
                   : ts >= 1e6 ? std::to_string(ts * 1e-6) + " ms"
                               : ts >= 1e3 ? std::to_string(ts * 1e-3) + " us"
                                           : std::to_string(ts) + " ns";
}
