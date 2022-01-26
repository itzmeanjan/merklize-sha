#pragma once
#include "merklize.hpp"
#include <cassert>
#include <random>

// Benchmarks binary merklization implementation --- collects motivation from
// https://github.com/itzmeanjan/blake3/blob/e2a1340/include/bench_merklize.hpp#L6-L10
//
// Which SHA variant of 2-to-1 hash function to be used in a compile-time
// decision using preprocessor directives
//
// If none chosen, SHA2-256 is chosen by default !
void
benchmark_merklize(sycl::queue& q,
                   size_t leaf_cnt,
                   size_t wg_size,
                   sycl::cl_ulong* const ts)
{
  // this implementation is only helpful when
  // relatively large number of leaf nodes are
  // required to be merklized
  assert(leaf_cnt >= (1 << 20));

#if defined SHA1
  const size_t i_size = leaf_cnt * sha1::OUT_LEN_BYTES; // in bytes
  const size_t o_size = leaf_cnt * sha1::OUT_LEN_BYTES; // in bytes
#elif defined SHA2_224
  const size_t i_size = leaf_cnt * sha2_224::OUT_LEN_BYTES; // in bytes
  const size_t o_size = leaf_cnt * sha2_224::OUT_LEN_BYTES; // in bytes
#elif defined SHA2_256
  const size_t i_size = leaf_cnt * sha2_256::OUT_LEN_BYTES; // in bytes
  const size_t o_size = leaf_cnt * sha2_256::OUT_LEN_BYTES; // in bytes
#endif

  // allocate resources
  sycl::uint* i_h = static_cast<sycl::uint*>(sycl::malloc_host(i_size, q));
  sycl::uint* o_h = static_cast<sycl::uint*>(sycl::malloc_host(o_size, q));
  sycl::uint* i_d = static_cast<sycl::uint*>(sycl::malloc_device(i_size, q));
  sycl::uint* o_d = static_cast<sycl::uint*>(sycl::malloc_device(o_size, q));

  // Set all intermediate nodes to zero bytes,
  //
  // I'll make use of this fact later to assert that first digest bytes (
  // different for each SHA variant ) will never be touched by any work-item
  q.memset(o_d, 0, o_size).wait();

  {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint8_t> dis(0, 255);

    memset(i_h, dis(gen), i_size); // prepare (random) input bytes
  }

  sycl::cl_ulong ts_0, ts_1, ts_2;

  // copy input from host to device
  sycl::event evt_0 = q.memcpy(i_d, i_h, i_size);
  evt_0.wait();
  // time host to device tx command
  ts_0 = time_event(evt_0);

  // merklization, get sum of all dispatched kernel execution time
  ts_1 = merklize(q, i_d, i_size, leaf_cnt, o_d, o_size, leaf_cnt - 1, wg_size);

  // copy output from device to host
  sycl::event evt_1 = q.memcpy(o_h, o_d, o_size);
  evt_1.wait();
  // time device to host data tx command
  ts_2 = time_event(evt_1);

  // ensuring that first digest bytes ( different for each SHA variant ) are
  // never touched by any work-items
  for (size_t i = 0; i < (

#if defined SHA1
                           sha1::OUT_LEN_BYTES
#elif defined SHA2_224
                           sha2_224::OUT_LEN_BYTES
#elif defined SHA2_256
                           sha2_256::OUT_LEN_BYTES
#endif

                           >> 2);

       i++) {
    assert(*(o_h + i) == 0);
  }

  // ensure all acquired resources are deallocated too !
  sycl::free(i_h, q);
  sycl::free(o_h, q);
  sycl::free(i_d, q);
  sycl::free(o_d, q);

  // all time in nanosecond level granularity
  *(ts + 0) = ts_0; // host to device data transfer time
  *(ts + 1) = ts_1; // total kernel execution cost
  *(ts + 2) = ts_2; // device to host data transfer time
}
