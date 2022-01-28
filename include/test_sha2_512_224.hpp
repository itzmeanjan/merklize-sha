#pragma once
#include "sha2_512_224.hpp"
#include <cassert>

void
test_sha2_512_224(sycl::queue& q)
{
  // obtained by executing following snippet in bash/ python3 shell
  // $ python3 -m pip install --user pycryptodome
  //
  // >>> from Crypto.Hash import SHA512
  // >>> h = SHA512(truncate='224')
  // >>> h.update(bytes([i for i in range(56)]))
  // >>> list(h.digest())
  //
  // note, same input is prepared inside 👇 for loops
  constexpr sycl::uchar expected[28] = { 223, 27,  183, 2,   129, 60,  73,
                                         17,  1,   66,  188, 133, 128, 115,
                                         230, 24,  171, 34,  175, 82,  250,
                                         248, 213, 240, 64,  61,  63,  240 };

  // acquire resources
  sycl::uchar* in_0 = static_cast<sycl::uchar*>(sycl::malloc_shared(56, q));
  sycl::ulong* in_1 = static_cast<sycl::ulong*>(sycl::malloc_shared(56, q));
  sycl::uchar* out_0 = static_cast<sycl::uchar*>(sycl::malloc_shared(28, q));
  sycl::uchar* out_1 = static_cast<sycl::uchar*>(sycl::malloc_shared(28, q));

#pragma unroll 8
  for (size_t i = 0; i < 56; i++) {
    // preparing input for testing 2-to-1 SHA2-512/224 hash
    *(in_0 + i) = i;
  }

#pragma unroll 7
  for (size_t i = 0; i < 7; i++) {
    sycl::ulong v = static_cast<sycl::ulong>(i << 3);

    // preparing input to SHA2-512/224 hash function as words, instead of big
    // endian byte array, which is already prepared above
    //
    // so that I can test it both ways --- see below, two kernels dispatched
    *(in_1 + i) = (v + 0) << 56 | (v + 1) << 48 | (v + 2) << 40 |
                  (v + 3) << 32 | (v + 4) << 24 | (v + 5) << 16 | (v + 6) << 8 |
                  (v + 7) << 0;
  }

  // enqueue kernel execution in single work-item model
  q.single_task<class kernelTestSHA2_512_224_v0>([=]() {
    sycl::uchar padded[128];
    sycl::ulong parsed[16];
    sycl::ulong digest[4];

    sha2_512_224::pad_input_message(in_0, padded);

#pragma unroll 8
    for (size_t i = 0; i < 16; i++) {
      parsed[i] = from_be_bytes_to_u64_words(padded + (i << 3));
    }

    sha2_512_224::hash(parsed, digest);

    // converting first three message word of digest into eight consecutive big
    // endian bytes, making total first 24 -bytes SHA2-512/224 digest
#pragma unroll 3
    for (size_t i = 0; i < 3; i++) {
      from_words_to_be_bytes(*(digest + i), out_0 + (i << 3));
    }

    // finally taking MSB 32 -bits of last message word of digest and converting
    // to 4 consecutive big endian bytes
    from_words_to_be_bytes(
      static_cast<sycl::uint>((digest[3] >> 32) & 0xffffffff), out_0 + 24);
  });
  q.wait();

  q.single_task<class kernelTestSHA2_512_224_v1>([=]() {
    sycl::ulong padded[16];
    sycl::ulong digest[4];

    sha2_512_224::pad_input_message(in_1, padded);
    sha2_512_224::hash(padded, digest);

    // converting first three message word of digest into eight consecutive big
    // endian bytes, making total first 24 -bytes SHA2-512/224 digest
#pragma unroll 3
    for (size_t i = 0; i < 3; i++) {
      from_words_to_be_bytes(*(digest + i), out_1 + (i << 3));
    }

    // finally taking MSB 32 -bits of last message word of digest and converting
    // to 4 consecutive big endian bytes
    from_words_to_be_bytes(
      static_cast<sycl::uint>((digest[3] >> 32) & 0xffffffff), out_1 + 24);
  });
  q.wait();

  // check result !
  for (size_t i = 0; i < 28; i++) {
    assert(*(out_0 + i) == expected[i]);
    assert(*(out_1 + i) == expected[i]);
  }

  // ensure resources are deallocated
  sycl::free(in_0, q);
  sycl::free(in_1, q);
  sycl::free(out_0, q);
  sycl::free(out_1, q);
}
