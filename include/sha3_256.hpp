#pragma once
#include "sha3.hpp"

namespace sha3_256 {

// Given two ( byte concatenated ) SHA3-256 digests ( i.e. 64 -bytes ), prepares
// bit string of length 1600, which can be passed to keccak_p[b, n_r]
// permutation function, in later phase
void
prepare_bit_string(sycl::uchar* const in, std::bitset<1600>& s)
{
  s.reset();

  size_t s_idx = 1599;
  for (size_t i = 0; i < 64; i++) {
    s[s_idx--] = bit_at<0>(*(in + i));
    s[s_idx--] = bit_at<1>(*(in + i));
    s[s_idx--] = bit_at<2>(*(in + i));
    s[s_idx--] = bit_at<3>(*(in + i));
    s[s_idx--] = bit_at<4>(*(in + i));
    s[s_idx--] = bit_at<5>(*(in + i));
    s[s_idx--] = bit_at<6>(*(in + i));
    s[s_idx--] = bit_at<7>(*(in + i));
  }

  s[s_idx--] = 0;
  s[s_idx--] = 1;

  s[s_idx--] = 1;
  for (size_t i = 0; i < pad(1088, 514); i++) {
    s[s_idx--] = 0;
  }
  s[s_idx--] = 1;
}

}
