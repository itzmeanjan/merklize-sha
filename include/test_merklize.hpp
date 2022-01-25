#pragma once
#include "merklize.hpp"
#include <cassert>
#include <iostream>

void
test_merklize(sycl::queue& q)
{
  // testing on binary merkle tree which has 8 leaf nodes
  constexpr size_t leaf_cnt = 1 << 3;

#if SHA == sha1
  constexpr size_t i_size = leaf_cnt * sha1::OUT_LEN_BYTES; // in bytes
  constexpr size_t o_size = leaf_cnt * sha1::OUT_LEN_BYTES; // in bytes
#elif SHA == sha2_224
  constexpr size_t i_size = leaf_cnt * sha2_224::OUT_LEN_BYTES; // in bytes
  constexpr size_t o_size = leaf_cnt * sha2_224::OUT_LEN_BYTES; // in bytes
#elif SHA == sha2_256
  constexpr size_t i_size = leaf_cnt * sha2_256::OUT_LEN_BYTES; // in bytes
  constexpr size_t o_size = leaf_cnt * sha2_256::OUT_LEN_BYTES; // in bytes
#endif

#if SHA == sha1
  // obtained using following code snippet run on python3 shell
  //
  // >>> import hashlib
  // >>> a = [0xff] * 40 # two leaf nodes
  // >>> b = list(hashlib.sha1(bytes(a)).digest()) # = [244, 67, 49, 150, 149,
  // 151, 153, 23, 160, 59, 113, 112, 73, 35, 84, 35, 135, 77, 39, 22]
  // >>> c = b * 2
  // >>> d = list(hashlib.sha1(bytes(c)).digest()) # = [7, 7, 30, 157, 84, 109,
  // 232, 147, 213, 85, 108, 21, 251, 107, 125, 35, 100, 216, 165, 28]
  // >>> e = d * 2
  // >>> f = list(hashlib.sha1(bytes(e)).digest()) # = [139, 49, 56, 44, 55, 31,
  // 24, 110, 245, 27, 105, 167, 84, 13, 218, 12, 209, 49, 184, 54]
  constexpr sycl::uchar expected[20] = { 139, 49,  56,  44,  55,  31, 24,
                                         110, 245, 27,  105, 167, 84, 13,
                                         218, 12,  209, 49,  184, 54 };
#elif SHA == sha2_224
#elif SHA == sha2_256
#endif

  // acquire resources
  sycl::uchar* in_0 = (sycl::uchar*)sycl::malloc_shared(i_size, q);
  sycl::uint* in_1 = (sycl::uint*)sycl::malloc_shared(i_size, q);
  sycl::uint* out_0 = (sycl::uint*)sycl::malloc_shared(o_size, q);
  sycl::uchar* out_1 = (sycl::uchar*)sycl::malloc_shared(o_size, q);

  // prepare input bytes
  q.memset(in_0, 0xff, i_size).wait();
  // I'm doing this intensionally just to check that
  // first digest bytes are never touched by any work-items !
  q.memset(out_0, 0, o_size).wait();

  // convert input bytes to hash words !
  //
  // instead of doing this, I could have simply set
  // all input bytes to 0xff
  //
  // q.memset(in_1, 0xff, i_size).wait();
  //
  // but I decided to do it manually, just to be sure that
  // I'm thinking correctly !
#pragma unroll 8
  for (size_t i = 0; i < (i_size >> 2); i++) {
    *(in_1 + i) = from_be_bytes_to_words(in_0 + i * 4);
  }

  // wait until completely merklized !
  merklize(
    q, in_1, i_size, leaf_cnt, out_0, o_size, leaf_cnt - 1, leaf_cnt >> 1);

  // finally convert all intermediate nodes from word representation
  // to big endian byte array form
#pragma unroll 8
  for (size_t i = 0; i < (o_size >> 2); i++) {
    const sycl::uint num = *(out_0 + i);
    from_words_to_be_bytes(num, out_1 + (i << 2));
  }
  // first 20 -bytes should never be touched !
  for (size_t i = 0; i <

#if SHA == sha1
                     sha1::OUT_LEN_BYTES
#elif SHA == sha2_224
                     sha2_224::OUT_LEN_BYTES
#elif SHA == sha2_256
                     sha2_256::OUT_LEN_BYTES
#endif

       ;

       i++) {
    assert(*(out_1 + i) == 0);
  }

  // then comes root of merkle tree !

#if SHA == sha1
  for (size_t i = sha1::OUT_LEN_BYTES, j = 0;
       i < (sha1::OUT_LEN_BYTES << 1) && j < sha1::OUT_LEN_BYTES;
       i++, j++)
#elif SHA == sha2_224
  for (size_t i = sha2_224::OUT_LEN_BYTES, j = 0;
       i < (sha2_224::OUT_LEN_BYTES << 1) && j < sha2_224::OUT_LEN_BYTES;
       i++, j++)
#elif SHA == sha2_256
  for (size_t i = sha2_256::OUT_LEN_BYTES, j = 0;
       i < (sha2_256::OUT_LEN_BYTES << 1) && j < sha2_256::OUT_LEN_BYTES;
       i++, j++)
#endif

  {
    assert(*(out_1 + i) == expected[j]);
  }

  // ensure resources are deallocated
  sycl::free(in_0, q);
  sycl::free(in_1, q);
  sycl::free(out_0, q);
  sycl::free(out_1, q);
}
