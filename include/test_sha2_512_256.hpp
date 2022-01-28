#pragma once
#include "sha2_512_256.hpp"
#include <cassert>

void
test_sha2_512_256(sycl::queue& q)
{
  // obtained by executing following snippet in bash/ python3 shell
  // $ python3 -m pip install --user pycryptodome
  //
  // >>> from Crypto.Hash import SHA512
  // >>> h = SHA512.new(truncate='256')
  // >>> h.update(bytes([i for i in range(64)]))
  // >>> list(h.digest())
  //
  // note, same input is prepared inside 👇 for loops
  constexpr sycl::uchar expected[32] = {
    201, 228, 131, 185, 98,  37, 21, 232, 50, 89,  225,
    224, 117, 116, 107, 112, 20, 44, 177, 33, 120, 99,
    251, 140, 133, 250, 227, 50, 86, 244, 24, 138
  };

  // acquire resources
  sycl::uchar* in_0 = static_cast<sycl::uchar*>(
    sycl::malloc_shared(sha2_512_256::IN_LEN_BYTES, q));
  sycl::ulong* in_1 = static_cast<sycl::ulong*>(
    sycl::malloc_shared(sha2_512_256::IN_LEN_BYTES, q));
  sycl::uchar* out_0 = static_cast<sycl::uchar*>(
    sycl::malloc_shared(sha2_512_256::OUT_LEN_BYTES, q));
  sycl::uchar* out_1 = static_cast<sycl::uchar*>(
    sycl::malloc_shared(sha2_512_256::OUT_LEN_BYTES, q));

#pragma unroll 32
  for (size_t i = 0; i < 64; i++) {
    // preparing input for testing 2-to-1 SHA2-512/256 hash
    *(in_0 + i) = i;
  }

#pragma unroll 8
  for (size_t i = 0; i < 8; i++) {
    sycl::ulong v = static_cast<sycl::ulong>(i << 3);

    // preparing input to SHA2-512/256 hash function as words, instead of big
    // endian byte array, which is already prepared above
    //
    // so that I can test it both ways --- see below, two kernels dispatched
    *(in_1 + i) = (v + 0) << 56 | (v + 1) << 48 | (v + 2) << 40 |
                  (v + 3) << 32 | (v + 4) << 24 | (v + 5) << 16 | (v + 6) << 8 |
                  (v + 7) << 0;
  }

  // enqueue kernel execution in single work-item model
  q.single_task<class kernelTestSHA2_512_256_v0>([=]() {
    sycl::uchar padded[128];
    sycl::ulong parsed[16];
    sycl::ulong digest[4];

    sha2_512_256::pad_input_message(in_0, padded);

#pragma unroll 8
    for (size_t i = 0; i < 16; i++) {
      parsed[i] = from_be_bytes_to_u64_words(padded + (i << 3));
    }

    sha2_512_256::hash(parsed, digest);

#pragma unroll 4
    for (size_t i = 0; i < 4; i++) {
      from_words_to_be_bytes(digest[i], out_0 + (i << 3));
    }
  });
  q.wait();

  q.single_task<class kernelTestSHA2_512_256_v1>([=]() {
    sycl::ulong padded[16];
    sycl::ulong digest[4];

    sha2_512_256::pad_input_message(in_1, padded);
    sha2_512_256::hash(padded, digest);

#pragma unroll 4
    for (size_t i = 0; i < 4; i++) {
      from_words_to_be_bytes(digest[i], out_1 + (i << 3));
    }
  });
  q.wait();

  // check result !
  for (size_t i = 0; i < 32; i++) {
    assert(*(out_0 + i) == expected[i]);
    assert(*(out_1 + i) == expected[i]);
  }

  // ensure resources are deallocated
  sycl::free(in_0, q);
  sycl::free(in_1, q);
  sycl::free(out_0, q);
  sycl::free(out_1, q);
}
