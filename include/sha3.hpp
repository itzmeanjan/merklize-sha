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
θ(std::bitset<64> state[5][5])
{
  std::bitset<64> c[5];

  // see step 1 of algorithm 1
#pragma unroll 5
  for (size_t x = 0; x < 5; x++) {
    c[x] = state[x][0] ^ state[x][1] ^ state[x][2] ^ state[x][3] ^ state[x][4];
  }

  std::bitset<64> d[5];

  // see step 2 of algorithm 1
  for (size_t x = 0; x < 5; x++) {
#pragma unroll 64
    for (size_t z = 0; z < 64; z++) {
      d[x][63 - z] = c[(x - 1) % 5][63 - z] ^ c[(x + 1) % 5][63 - (z - 1) % 64];
    }
  }

  // see step 3 of algorithm 1
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
ρ(std::bitset<64> state_in[5][5], std::bitset<64> state_out[5][5])
{
  // step 1 of algorithm 2
  state_out[0][0] = state_in[0][0];

  // step 2 of algorithm 2
  size_t x = 1;
  size_t y = 0;

  // step 3 of algorithm 2
  for (size_t t = 0; t < 24; t++) {
    // step 3a of algorithm 2
    for (size_t z = 0; z < 64; z++) {
      size_t _z = 63 - (z - (((t + 1) * (t + 2)) / 2)) % 64;

      state_out[x][y][z] = state_in[x][y][_z];
    }

    // step 3b of algorithm 2
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
π(std::bitset<64> state_in[5][5], std::bitset<64> state_out[5][5])
{
  // step 1 of algorithm 3
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
χ(std::bitset<64> state_in[5][5], std::bitset<64> state_out[5][5])
{
  // step 1 of algorithm 4
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

// See algorithm 5 in section 3.2.5 of http://dx.doi.org/10.6028/NIST.FIPS.202
inline bool
rc(size_t t)
{
  if (t % 255 == 0) {
    return 1;
  }

  std::bitset<9> r{ 0b010000000 };
  for (size_t i = 1; i <= t % 255; i++) {
    r[8] = r[8] ^ r[0];
    r[4] = r[4] ^ r[0];
    r[3] = r[3] ^ r[0];
    r[2] = r[2] ^ r[0];

    r[0] = 0;
  }

  return r[7];
}

// keccak-p[b, n_r] step mapping
//
// Input is 5 x 5 x 64 state array, along with round index ∈ [0, 24)
// Output is modified state array
//
// See specification of `ι` step mapping function in section 3.2.5
// of http://dx.doi.org/10.6028/NIST.FIPS.202
inline void
ι(std::bitset<64> state[5][5], size_t round_index)
{
  // step 2 of algorithm 6
  std::bitset<64> RC;
  RC.reset();

  // step 3 of algorithm 6
  for (size_t j = 0; j < 7; j++) {
    RC[63 - ((1 << j) - 1)] = rc(j + 7 * round_index);
  }

  // step 4 of algorithm 6
  state[0][0] ^= RC;
}

// keccak-p[b, n_r] round function, which applies all five
// step mapping functions in order, updating state array
//
// See section 3.3 of http://dx.doi.org/10.6028/NIST.FIPS.202
void
rnd(std::bitset<64> state[5][5], size_t round_index)
{
  std::bitset<64> tmp[5][5];

  θ(state);
  ρ(state, tmp);
  π(tmp, state);
  χ(state, tmp);
  ι(tmp, round_index);

#pragma unroll 5
  for (size_t x = 0; x < 5; x++) {
#pragma unroll 5
    for (size_t y = 0; y < 5; y++) {
      state[x][y] = tmp[x][y];
    }
  }
}

// Routine for converting bit string to 5 x 5 x 64 state array
// using technique defined in section 3.1.2 of
// http://dx.doi.org/10.6028/NIST.FIPS.202
void
to_state_array(std::bitset<1600>& s, std::bitset<64> state[5][5])
{
  for (size_t x = 0; x < 5; x++) {
    for (size_t y = 0; y < 5; y++) {
      for (size_t z = 0; z < 64; z++) {
        state[x][y][63 - z] = s[1599 - (64 * (5 * y + x) + z)];
      }
    }
  }
}

// Routine for converting 5 x 5 x 64 state array to bit string
// using method defined in section 3.1.3 of
// http://dx.doi.org/10.6028/NIST.FIPS.202
void
to_bit_string(std::bitset<64> state[5][5], std::bitset<1600>& s)
{
  size_t s_idx = 1599;

  for (size_t y = 0; y < 5; y++) {
    for (size_t x = 0; x < 5; x++) {
      for (size_t z = 0; z < 64; z++) {
        s[s_idx--] = state[x][y][63 - z];
      }
    }
  }
}

// keccak-p[b, n_r] permutation, applying n_r ( = 24 ) rounds
// on state bit array ( = s ), using algorithm 7 defined in
// section 3.3 of http://dx.doi.org/10.6028/NIST.FIPS.202
void
keccak_p(std::bitset<1600>& s)
{
  // step 1 of algorithm 7
  std::bitset<64> state[5][5];
  to_state_array(s, state);

  // step 2 of algorithm 7
  for (size_t i = 0; i < 24; i++) {
    rnd(state, i);
  }

  // step 3 of algorithm 7
  to_bit_string(state, s);
}
