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

// keccak-p[b, n_r] step mapping
//
// Input is 5 x 5 x 64 state array and output is modified state array
//
// See specification of `ρ` step mapping function in section 3.2.2
// of http://dx.doi.org/10.6028/NIST.FIPS.202
inline void
ρ(const std::bitset<64>** state_in, std::bitset<64>** const state_out)
{
  // step 1 of algorithm 2 in http://dx.doi.org/10.6028/NIST.FIPS.202
  state_out[0][0] = state_in[0][0];

  // step 2 of algorithm 2 in http://dx.doi.org/10.6028/NIST.FIPS.202
  size_t x = 1;
  size_t y = 0;

  // step 3 of algorithm 2 in http://dx.doi.org/10.6028/NIST.FIPS.202
  for (size_t t = 0; t < 24; t++) {
    // step 3a of algorithm 2 in http://dx.doi.org/10.6028/NIST.FIPS.202
    for (size_t z = 0; z < 64; z++) {
      size_t _z = 63 - (z - (((t + 1) * (t + 2)) / 2)) % 64;

      state_out[x][y][z] = state_in[x][y][_z];
    }

    // step 3b of algorithm 2 in http://dx.doi.org/10.6028/NIST.FIPS.202
    const size_t tmp = x;
    x = y;
    y = (2 * tmp + 3 * y) % 5;
  }
}

// keccak-p[b, n_r] step mapping
//
// Input is 5 x 5 x 64 state array and output is modified state array
//
// See specification of `π` step mapping function in section 3.2.3
// of http://dx.doi.org/10.6028/NIST.FIPS.202
inline void
π(const std::bitset<64>** state_in, std::bitset<64>** const state_out)
{
  // step 1 of algorithm 3 in http://dx.doi.org/10.6028/NIST.FIPS.202
  for (size_t z = 0; z < 64; z++) {
    // a single slice
#pragma unroll 5
    for (size_t x = 0; x < 5; x++) {
#pragma unroll 5
      for (size_t y = 0; y < 5; y++) {
        state_out[x][y][63 - z] = state_in[(x + 3 * y) % 5][x][63 - z];
      }
    }
    // a single slice
  }
}

// keccak-p[b, n_r] step mapping
//
// Input is 5 x 5 x 64 state array and output is modified state array
//
// See specification of `χ` step mapping function in section 3.2.4
// of http://dx.doi.org/10.6028/NIST.FIPS.202
inline void
χ(const std::bitset<64>** state_in, std::bitset<64>** const state_out)
{
  // step 1 of algorithm 4 in http://dx.doi.org/10.6028/NIST.FIPS.202
  for (size_t x = 0; x < 5; x++) {
    for (size_t y = 0; y < 5; y++) {
      for (size_t z = 0; z < 64; z++) {
        bool v0 = state_in[(x + 1) % 5][y][63 - z] ^ 1;
        bool v1 = state_in[(x + 2) % 5][y][63 - z];
        bool v2 = v0 & v1;

        state_out[x][y][63 - z] = state_in[x][y][63 - z] ^ v2;
      }
    }
  }
}
