#pragma once
#include "utils.hpp"
#include <cassert>
#include <random>

constexpr bool
gt_n(const size_t rounds, const size_t n)
{
  return rounds > n;
}

// Generate random 64 -bit unsigned integer, which is transformed to bit
// interleaved form ( even & odd parts ) & again converted back to standard
// representation by merging bit interleaved even, odd parts --- assert equality
// of final standard representation and original u64 random number generated
template<const size_t rounds>
void
test_bit_interleaving() requires(gt_n(rounds, 0))
{
  std::random_device rd;
  std::mt19937 gen(rd());
  // [0, (1 << 64) - 1]
  std::uniform_int_distribution<uint64_t> dis(0, 18446744073709551615ull);

#pragma unroll 16
  for (size_t i = 0; i < rounds; i++) {
    const uint64_t word = dis(gen);
    uint32_t even, odd;

    to_bit_interleaved(word, &even, &odd);
    const uint64_t word_ = from_bit_interleaved(even, odd);

    assert(word == word_);
  }
}
