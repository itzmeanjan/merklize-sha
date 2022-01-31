#pragma once
#include <CL/sycl.hpp>
#include <bitset>

// keccak-p[b, n_r] step mapping
//
// Input is 5 x 5 x 64 state array and output is modified state array
//
// See specification of `θ` step mapping function in section 3.2.1
// of http://dx.doi.org/10.6028/NIST.FIPS.202
inline void
θ(std::bitset<64>** const state)
{
  std::bitset<64> c[5];

  // see step 1 of algorithm 1 in http://dx.doi.org/10.6028/NIST.FIPS.202
#pragma unroll 5
  for (size_t x = 0; x < 5; x++) {
    c[x] = state[x][0] ^ state[x][1] ^ state[x][2] ^ state[x][3] ^ state[x][4];
  }

  std::bitset<64> d[5];

  // see step 2 of algorithm 1 in http://dx.doi.org/10.6028/NIST.FIPS.202
  for (size_t x = 0; x < 5; x++) {
#pragma unroll 64
    for (size_t z = 0; z < 64; z++) {
      d[x][63 - z] = c[(x - 1) % 5][63 - z] ^ c[(x + 1) % 5][63 - (z - 1) % 64];
    }
  }

  // see step 3 of algorithm 1 in http://dx.doi.org/10.6028/NIST.FIPS.202
#pragma unroll 5
  for (size_t y = 0; y < 5; y++) {
#pragma unroll 5
    for (size_t x = 0; x < 5; x++) {
      state[x][y] ^= d[x];
    }
  }
}
