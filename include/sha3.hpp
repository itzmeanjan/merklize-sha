#pragma once
#include "utils.hpp"
#include <CL/sycl.hpp>

// keccak-p[b, n_r] step mapping
//
// Input is 5 x 5 x 64 state array and output is in-place modified state array
//
// See specification of `θ` step mapping function in section 3.2.1
// of http://dx.doi.org/10.6028/NIST.FIPS.202
inline void
θ(std::bitset<64> state[5][5])
{
  std::bitset<64> c[5];
  std::bitset<64> d[5];

  // see step 1 of algorithm 1
#pragma unroll 5
  for (size_t x = 0; x < 5; x++) {
    c[x] = state[x][0] ^ state[x][1] ^ state[x][2] ^ state[x][3] ^ state[x][4];
  }

  // see step 2 of algorithm 1
  for (size_t x = 0; x < 5; x++) {
    // just to ensure that 0 - 1 does't overflow !
    // and instead behaves in modular fashion
    //
    // try executing `assert(-1 % 5 == 4)` in Python3 shell
    const size_t x_ = x == 0 ? 4 : x - 1;

    d[x] = c[x_] ^ rotl<1>(c[(x + 1) % 5]);
  }

  // see step 3 of algorithm 1
#pragma unroll 5
  for (size_t x = 0; x < 5; x++) {
#pragma unroll 5
    for (size_t y = 0; y < 5; y++) {
      state[x][y] ^= d[x];
    }
  }
}

// keccak-p[b, n_r] step mapping
//
// Input is 5 x 5 x 64 state array and output is in-place modified state array
//
// See specification of `ρ` step mapping function in section 3.2.2
// of http://dx.doi.org/10.6028/NIST.FIPS.202
inline void
ρ(std::bitset<64> state_in[5][5])
{
  // see table 2 below algorithm 2, for rotation factors
  // ( = % lane size, which is 64 ) of each lanes
  state_in[1][0] = rotl<1>(state_in[1][0]);
  state_in[2][0] = rotl<62>(state_in[2][0]);
  state_in[3][0] = rotl<28>(state_in[3][0]);
  state_in[4][0] = rotl<27>(state_in[4][0]);

  state_in[0][1] = rotl<36>(state_in[0][1]);
  state_in[1][1] = rotl<44>(state_in[1][1]);
  state_in[2][1] = rotl<6>(state_in[2][1]);
  state_in[3][1] = rotl<55>(state_in[3][1]);
  state_in[4][1] = rotl<20>(state_in[4][1]);

  state_in[0][2] = rotl<3>(state_in[0][2]);
  state_in[1][2] = rotl<10>(state_in[1][2]);
  state_in[2][2] = rotl<43>(state_in[2][2]);
  state_in[3][2] = rotl<25>(state_in[3][2]);
  state_in[4][2] = rotl<39>(state_in[4][2]);

  state_in[0][3] = rotl<41>(state_in[0][3]);
  state_in[1][3] = rotl<45>(state_in[1][3]);
  state_in[2][3] = rotl<15>(state_in[2][3]);
  state_in[3][3] = rotl<21>(state_in[3][3]);
  state_in[4][3] = rotl<8>(state_in[4][3]);

  state_in[0][4] = rotl<18>(state_in[0][4]);
  state_in[1][4] = rotl<2>(state_in[1][4]);
  state_in[2][4] = rotl<61>(state_in[2][4]);
  state_in[3][4] = rotl<56>(state_in[3][4]);
  state_in[4][4] = rotl<14>(state_in[4][4]);
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
#pragma unroll 5
  for (size_t x = 0; x < 5; x++) {
#pragma unroll 5
    for (size_t y = 0; y < 5; y++) {
      state_out[x][y] = state_in[(x + 3 * y) % 5][x];
    }
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
#pragma unroll 5
  for (size_t y = 0; y < 5; y++) {
#pragma unroll 5
    for (size_t x = 0; x < 5; x++) {
      const size_t x_0 = (x + 1) % 5;
      const size_t x_1 = (x + 2) % 5;
      const std::bitset<64> rhs = ~state_in[x_0][y] & state_in[x_1][y];

      state_out[x][y] = state_in[x][y] ^ rhs;
    }
  }
}

// See algorithm 5 in section 3.2.5 of http://dx.doi.org/10.6028/NIST.FIPS.202
// which helps in computing round constant; lane (0, 0) to be multiplied with
// computed Round Constant
bool
rc(size_t t)
{
  // step 1 of algorithm 5
  if (t % 255 == 0) {
    return 1;
  }

  // step 2 of algorithm 5
  //
  // note, step 3.a of algorithm 5 is also being
  // executed in this statement ( for first iteration, with i = 1 ) !
  std::bitset<9> r{ 0b010000000 };

  // step 3 of algorithm 5
  for (size_t i = 1; i <= t % 255; i++) {
    // note, bit position indexing is opposite
    // because as per SHA3 specification all bit strings
    // are indexed left to right ascending, while here in std::bitset<_>
    // abstraction indexes right to left ascending
    r[8] = r[8] ^ r[0];
    r[4] = r[4] ^ r[0];
    r[3] = r[3] ^ r[0];
    r[2] = r[2] ^ r[0];

    // step 3.f of algorithm 5
    //
    // note, this statement also executes step 3.a for upcoming iterations (
    // when i > 1 )
    r >>= 1;
  }

  // because indexing is done in opposite direction, I'm required
  // to read `[]` bit position from `0b0[_] _ _ _ | _ _ _ _` bit array
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
    RC[(1 << j) - 1] = rc(j + 7 * round_index);
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
  ρ(state);
  π(state, tmp);
  χ(tmp, state);
  ι(state, round_index);
}

// Routine for converting bit string to 5 x 5 x 64 state array
// using technique defined in section 3.1.2 of
// http://dx.doi.org/10.6028/NIST.FIPS.202
void
to_state_array(std::bitset<1600>& s, std::bitset<64> state[5][5])
{
  for (size_t y = 0; y < 5; y++) {
    for (size_t x = 0; x < 5; x++) {
      for (size_t z = 0; z < 64; z++) {
        state[x][y][z] = s[1599 - (64 * (5 * y + x) + z)];
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
        s[s_idx--] = state[x][y][z];
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
