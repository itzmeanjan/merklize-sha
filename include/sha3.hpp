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
θ(sycl::ulong* const state)
{
  sycl::ulong c[5];
  sycl::ulong d[5];

  // see step 1 of algorithm 1
#pragma unroll 5
  for (size_t x = 0; x < 5; x++) {
    const sycl::ulong tmp0 = state[x] ^ state[x + 5];
    const sycl::ulong tmp1 = state[x + 10] ^ state[x + 15];
    const sycl::ulong tmp3 = tmp0 ^ tmp1 ^ state[x + 20];

    c[x] = tmp3;
  }

  // see step 2 of algorithm 1
  for (size_t x = 0; x < 5; x++) {
    // just to ensure that 0 - 1 does't overflow !
    // and instead behaves in modular fashion
    //
    // try executing `assert(-1 % 5 == 4)` in Python3 shell
    const size_t x_ = x == 0 ? 4 : x - 1;

    d[x] = c[x_] ^ rotl(c[(x + 1) % 5], 1);
  }

  // see step 3 of algorithm 1
#pragma unroll 5
  for (size_t x = 0; x < 25; x++) {
    state[x] ^= d[x % 5];
  }
}

// keccak-p[b, n_r] step mapping
//
// Input is 5 x 5 x 64 state array and output is in-place modified state array
//
// See specification of `ρ` step mapping function in section 3.2.2
// of http://dx.doi.org/10.6028/NIST.FIPS.202
inline void
ρ(sycl::ulong* const state)
{
  // see table 2 below algorithm 2, for rotation factors
  // ( = % lane size, which is 64 ) of each lanes
  state[1] = rotl(state[1], 1);
  state[2] = rotl(state[2], 62);
  state[3] = rotl(state[3], 28);
  state[4] = rotl(state[4], 27);

  state[5] = rotl(state[5], 36);
  state[6] = rotl(state[6], 44);
  state[7] = rotl(state[7], 6);
  state[8] = rotl(state[8], 55);
  state[9] = rotl(state[9], 20);

  state[10] = rotl(state[10], 3);
  state[11] = rotl(state[11], 10);
  state[12] = rotl(state[12], 43);
  state[13] = rotl(state[13], 25);
  state[14] = rotl(state[14], 39);

  state[15] = rotl(state[15], 41);
  state[16] = rotl(state[16], 45);
  state[17] = rotl(state[17], 15);
  state[18] = rotl(state[18], 21);
  state[19] = rotl(state[19], 8);

  state[20] = rotl(state[20], 18);
  state[21] = rotl(state[21], 2);
  state[22] = rotl(state[22], 61);
  state[23] = rotl(state[23], 56);
  state[24] = rotl(state[24], 14);
}

// keccak-p[b, n_r] step mapping
//
// Input is 5 x 5 x 64 state array and output is modified state array
//
// See specification of `π` step mapping function in section 3.2.3
// of http://dx.doi.org/10.6028/NIST.FIPS.202
inline void
π(const sycl::ulong* __restrict state_in,
  sycl::ulong* const __restrict state_out)
{
  // step 1 of algorithm 3
#pragma unroll 5
  for (size_t y = 0; y < 5; y++) {
#pragma unroll 5
    for (size_t x = 0; x < 5; x++) {
      state_out[y * 5 + x] = state_in[5 * x + (x + 3 * y) % 5];
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
χ(const sycl::ulong* __restrict state_in,
  sycl::ulong* const __restrict state_out)
{
  // step 1 of algorithm 4
#pragma unroll 5
  for (size_t y = 0; y < 5; y++) {
#pragma unroll 5
    for (size_t x = 0; x < 5; x++) {
      const size_t x_0 = (x + 1) % 5;
      const size_t x_1 = (x + 2) % 5;
      const sycl::ulong rhs = ~state_in[y * 5 + x_0] & state_in[y * 5 + x_1];

      state_out[y * 5 + x] = state_in[y * 5 + x] ^ rhs;
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
ι(sycl::ulong* const state, size_t round_index)
{
  // step 2 of algorithm 6
  std::bitset<64> RC;
  RC.reset();

  // step 3 of algorithm 6
  for (size_t j = 0; j < 7; j++) {
    RC[(1 << j) - 1] = rc(j + 7 * round_index);
  }

  // step 4 of algorithm 6
  state[0] ^= RC.to_ulong();
}

// keccak-p[b, n_r] round function, which applies all five
// step mapping functions in order, updating state array
//
// See section 3.3 of http://dx.doi.org/10.6028/NIST.FIPS.202
void
rnd(sycl::ulong* const state, size_t round_index)
{
  sycl::ulong tmp[25];

  θ(state);
  ρ(state);
  π(state, tmp);
  χ(tmp, state);
  ι(state, round_index);
}

// keccak-p[b, n_r] permutation, applying n_r ( = 24 ) rounds
// on state bit array of dimension 5 x 5 x 64, using algorithm 7
// defined in section 3.3 of http://dx.doi.org/10.6028/NIST.FIPS.202
inline void
keccak_p(sycl::ulong* const state)
{
  // step 2 of algorithm 7
  for (size_t i = 0; i < 24; i++) {
    rnd(state, i);
  }
}
