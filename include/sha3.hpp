#pragma once
#include "utils.hpp"
#include <CL/sycl.hpp>

// Leftwards circular rotation offset of 24 lanes of state array ( except
// lane(0, 0), which is not touched ), as provided in table 2 below algorithm 2
// in section 3.2.2 of http://dx.doi.org/10.6028/NIST.FIPS.202
//
// Note, following offsets are obtained by performing % 64 ( = lane size )
// on offsets provided in above mentioned link
constexpr size_t ROT[24] = { 1,  62, 28, 27, 36, 44, 6, 55, 20, 3,  10, 43,
                             25, 39, 41, 45, 15, 21, 8, 18, 2,  61, 56, 14 };

// keccak-p[b, n_r] step mapping
//
// Input is 5 x 5 x 64 state array and output is in-place modified state array
//
// See specification of `θ` step mapping function in section 3.2.1
// of http://dx.doi.org/10.6028/NIST.FIPS.202
static inline void
θ(sycl::ulong* const state)
{
  sycl::ulong c[5];
  sycl::ulong d[5];

  // see step 1 of algorithm 1
#pragma unroll 5
  for (size_t x = 0; x < 5; x++) {
    const sycl::ulong tmp0 = state[x] ^ state[x + 5];
    const sycl::ulong tmp1 = state[x + 10] ^ state[x + 15];
    const sycl::ulong tmp2 = tmp0 ^ tmp1 ^ state[x + 20];

    c[x] = tmp2;
  }

// see step 2 of algorithm 1
#pragma unroll 5
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
// See specification of `θ` step mapping function in section 3.2.1
// of http://dx.doi.org/10.6028/NIST.FIPS.202
//
// Note, word size is 32 -bit, so expects keccak-p[1600, 24] state array/ round
// constants in bit interleaved form; see section 2.1 of
// https://keccak.team/files/Keccak-implementation-3.2.pdf
static inline void
θ(uint32_t* const state)
{
  uint32_t c[10];
  uint32_t d[10];

  // see step 1 of algorithm 1
#pragma unroll 5
  for (size_t x = 0; x < 10; x++) {
    const uint32_t t0 = state[x] ^ state[x + 10];
    const uint32_t t1 = state[x + 20] ^ state[x + 30];
    const uint32_t t2 = t0 ^ t1 ^ state[x + 40];

    c[x] = t2;
  }

  // see step 2 of algorithm 1
  for (size_t x = 0; x < 5; x++) {
    const size_t p_idx = ((x + 4) % 5) << 1;
    const size_t n_idx = ((x + 1) % 5) << 1;
    const size_t c_idx = x << 1;

    // read section 2.1 of
    // https://keccak.team/files/Keccak-implementation-3.2.pdf
    d[c_idx + 0] = c[p_idx + 0] ^ rotl(c[n_idx + 1], 1);
    d[c_idx + 1] = c[p_idx + 1] ^ c[n_idx + 0];
  }

  // see step 3 of algorithm 1
#pragma unroll 5
  for (size_t x = 0; x < 50; x++) {
    state[x] ^= d[x % 10];
  }
}

// keccak-p[b, n_r] step mapping
//
// Input is 5 x 5 x 64 state array and output is in-place modified state array
//
// See specification of `ρ` step mapping function in section 3.2.2
// of http://dx.doi.org/10.6028/NIST.FIPS.202
static inline void
ρ(sycl::ulong* const state)
{
#pragma unroll 8
  for (size_t i = 1; i < 25; i++) {
    state[i] = rotl(state[i], ROT[i - 1]);
  }
}

// keccak-p[b, n_r] step mapping
//
// Input is 5 x 5 x 64 state array and output is in-place modified state array
//
// See specification of `ρ` step mapping function in section 3.2.2
// of http://dx.doi.org/10.6028/NIST.FIPS.202
//
// Note, word size is 32 -bit, so expects keccak-p[1600, 24] state array/ round
// constants in bit interleaved form; see section 2.1 of
// https://keccak.team/files/Keccak-implementation-3.2.pdf
static inline void
ρ(uint32_t* const state)
{
#pragma unroll 8
  for (size_t i = 1; i < 25; i++) {
    const size_t c_idx = i << 1;
    const size_t offset = ROT[i - 1];

    // read section 2.1 of
    // https://keccak.team/files/Keccak-implementation-3.2.pdf
    //
    if ((offset & 0b1) == 0) { // even
      state[c_idx + 0] = rotl(state[c_idx + 0], offset >> 1);
      state[c_idx + 1] = rotl(state[c_idx + 1], offset >> 1);
    } else { // odd
      const uint32_t even = rotl(state[c_idx + 1], (offset >> 1) + 1);
      const uint32_t odd = rotl(state[c_idx + 0], offset >> 1);

      state[c_idx + 0] = even;
      state[c_idx + 1] = odd;
    }
  }
}

// keccak-p[b, n_r] step mapping
//
// Input is 5 x 5 x 64 state array and output is modified state array
//
// See specification of `π` step mapping function in section 3.2.3
// of http://dx.doi.org/10.6028/NIST.FIPS.202
static inline void
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
// See specification of `π` step mapping function in section 3.2.3
// of http://dx.doi.org/10.6028/NIST.FIPS.202
//
// Note, word size is 32 -bit, so expects keccak-p[1600, 24] state array/ round
// constants in bit interleaved form; see section 2.1 of
// https://keccak.team/files/Keccak-implementation-3.2.pdf
static inline void
π(uint32_t* const __restrict state)
{
  uint32_t tmp[50];

  // step 1 of algorithm 3
#pragma unroll 5
  for (size_t y = 0; y < 5; y++) {
#pragma unroll 5
    for (size_t x = 0; x < 5; x++) {
      const size_t idx = (y * 5 + x) << 1;

      tmp[idx + 0] = state[idx + 0];
      tmp[idx + 1] = state[idx + 1];
    }
  }

#pragma unroll 5
  for (size_t y = 0; y < 5; y++) {
#pragma unroll 5
    for (size_t x = 0; x < 5; x++) {
      const size_t to_idx = (y * 5 + x) << 1;
      const size_t frm_idx = (5 * x + (x + 3 * y) % 5) << 1;

      state[to_idx + 0] = tmp[frm_idx + 0];
      state[to_idx + 1] = tmp[frm_idx + 1];
    }
  }
}

// keccak-p[b, n_r] step mapping
//
// Input is 5 x 5 x 64 state array and output is modified state array
//
// See specification of `χ` step mapping function in section 3.2.4
// of http://dx.doi.org/10.6028/NIST.FIPS.202
static inline void
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

// keccak-p[b, n_r] step mapping
//
// Input is 5 x 5 x 64 state array and output is modified state array
//
// See specification of `χ` step mapping function in section 3.2.4
// of http://dx.doi.org/10.6028/NIST.FIPS.202
//
// Note, word size is 32 -bit, so expects keccak-p[1600, 24] state array/ round
// constants in bit interleaved form; see section 2.1 of
// https://keccak.team/files/Keccak-implementation-3.2.pdf
static inline void
χ(uint32_t* const __restrict state)
{
  // step 1 of algorithm 4
  uint32_t c[10];

#pragma unroll 5
  for (size_t y = 0; y < 5; y++) {
#pragma unroll 5
    for (size_t x = 0; x < 5; x++) {
      const size_t x_0 = (y * 5 + (x + 1) % 5) << 1;
      const size_t x_1 = (y * 5 + (x + 2) % 5) << 1;

      const uint32_t rhs_0 = ~state[x_0 + 0] & state[x_1 + 0];
      const uint32_t rhs_1 = ~state[x_0 + 1] & state[x_1 + 1];

      c[(x << 1) + 0] = rhs_0;
      c[(x << 1) + 1] = rhs_1;
    }

#pragma unroll 5
    for (size_t x = 0; x < 5; x++) {
      const size_t idx = (y * 5 + x) << 1;

      state[idx + 0] ^= c[(x << 1) + 0];
      state[idx + 1] ^= c[(x << 1) + 1];
    }
  }
}

// keccak-p[b, n_r] step mapping
//
// Input is 5 x 5 x 64 state array, along with round index ∈ [0, 24)
// Output is modified state array
//
// See specification of `ι` step mapping function in section 3.2.5
// of http://dx.doi.org/10.6028/NIST.FIPS.202
template<sycl::ulong rc>
static inline void
ι(sycl::ulong* const state)
{
  // step 4 of algorithm 6
  state[0] ^= rc;
}

// keccak-p[b, n_r] step mapping
//
// Input is 5 x 5 x 64 state array, along with round index ∈ [0, 24)
// Output is modified state array
//
// See specification of `ι` step mapping function in section 3.2.5
// of http://dx.doi.org/10.6028/NIST.FIPS.202
//
// Note, word size is 32 -bit, so expects keccak-p[1600, 24] state array/ round
// constants in bit interleaved form; see section 2.1 of
// https://keccak.team/files/Keccak-implementation-3.2.pdf
template<const uint32_t rc_even, const uint32_t rc_odd>
static inline void
ι(uint32_t* const state)
{
  // step 4 of algorithm 6
  state[0] ^= rc_even;
  state[1] ^= rc_odd;
}

// keccak-p[b, n_r] round function, which applies all five
// step mapping functions in order, updating state array
//
// See section 3.3 of http://dx.doi.org/10.6028/NIST.FIPS.202
template<sycl::ulong rc>
static inline void
rnd(sycl::ulong* const state)
{
  sycl::ulong tmp[25];

  θ(state);
  ρ(state);
  π(state, tmp);
  χ(tmp, state);
  ι<rc>(state);
}

// keccak-p[b, n_r] round function, which applies all five
// step mapping functions in order, updating state array
//
// See section 3.3 of http://dx.doi.org/10.6028/NIST.FIPS.202
//
// Note, word size is 32 -bit, so expects keccak-p[1600, 24] state array/ round
// constants in bit interleaved form; see section 2.1 of
// https://keccak.team/files/Keccak-implementation-3.2.pdf
template<const uint32_t rc_even, const uint32_t rc_odd>
static inline void
rnd(uint32_t* const state)
{
  θ(state);
  ρ(state);
  π(state);
  χ(state);
  ι<rc_even, rc_odd>(state);
}

// keccak-p[b, n_r] permutation, applying n_r ( = 24 ) rounds
// on state bit array of dimension 5 x 5 x 64, using algorithm 7
// defined in section 3.3 of http://dx.doi.org/10.6028/NIST.FIPS.202
static inline void
keccak_p(sycl::ulong* const state)
{
  // step 2 of algorithm 7
  //
  // all 24 keccak-p permutation rounds sequentially applied
  rnd<1ull>(state);
  rnd<32898ull>(state);
  rnd<9223372036854808714ull>(state);
  rnd<9223372039002292224ull>(state);
  rnd<32907ull>(state);
  rnd<2147483649ull>(state);
  rnd<9223372039002292353ull>(state);
  rnd<9223372036854808585ull>(state);
  rnd<138ull>(state);
  rnd<136ull>(state);
  rnd<2147516425ull>(state);
  rnd<2147483658ull>(state);
  rnd<2147516555ull>(state);
  rnd<9223372036854775947ull>(state);
  rnd<9223372036854808713ull>(state);
  rnd<9223372036854808579ull>(state);
  rnd<9223372036854808578ull>(state);
  rnd<9223372036854775936ull>(state);
  rnd<32778ull>(state);
  rnd<9223372039002259466ull>(state);
  rnd<9223372039002292353ull>(state);
  rnd<9223372036854808704ull>(state);
  rnd<2147483649ull>(state);
  rnd<9223372039002292232ull>(state);
}

// keccak-p[b, n_r] permutation, applying n_r ( = 24 ) rounds
// on state bit array of dimension 5 x 5 x 64, using algorithm 7
// defined in section 3.3 of http://dx.doi.org/10.6028/NIST.FIPS.202
//
// Note, this function works with 32 -bit word size, while function written just
// above it works with 64 -bit word size.
//
// Bit interleaving is the technique used for working with 32 -bit unsigned
// integers; see section 2.1 of
// https://keccak.team/files/Keccak-implementation-3.2.pdf for more info
//
// You may also want to see
// https://github.com/XKCP/XKCP/blob/f07a7f6/lib/low/KeccakP-1600/ref-32bits/KeccakP-1600-reference32BI.c#L336-L521
// as an alternative implementation with 32 -bit word size, which worked as an
// inspiration to me
static inline void
keccak_p(uint32_t* const state)
{
  // step 2 of algorithm 7
  //
  // all 24 keccak-p permutation rounds sequentially applied
  //
  // for bit interleaved round constants see
  // https://github.com/XKCP/XKCP/blob/f07a7f6/lib/low/KeccakP-1600/ref-32bits/KeccakP-1600-reference32BI.c#L153-L179
  rnd<1u, 0u>(state);
  rnd<0u, 137u>(state);
  rnd<0u, 2147483787u>(state);
  rnd<0u, 2147516544u>(state);
  rnd<1u, 139u>(state);
  rnd<1u, 32768u>(state);
  rnd<1u, 2147516552u>(state);
  rnd<1u, 2147483778u>(state);
  rnd<0u, 11u>(state);
  rnd<0u, 10u>(state);
  rnd<1u, 32898u>(state);
  rnd<0u, 32771u>(state);
  rnd<1u, 32907u>(state);
  rnd<1u, 2147483659u>(state);
  rnd<1u, 2147483786u>(state);
  rnd<1u, 2147483777u>(state);
  rnd<0u, 2147483777u>(state);
  rnd<0u, 2147483656u>(state);
  rnd<0u, 131u>(state);
  rnd<0u, 2147516419u>(state);
  rnd<1u, 2147516552u>(state);
  rnd<0u, 2147483784u>(state);
  rnd<1u, 32768u>(state);
  rnd<0u, 2147516546u>(state);
}
