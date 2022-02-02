#pragma once

#if !(defined SHA1 || defined SHA2_224 || defined SHA2_256 ||                  \
      defined SHA2_384 || defined SHA2_512 || defined SHA2_512_224 ||          \
      defined SHA2_512_256 || defined SHA3_256 || defined SHA3_224 ||          \
      defined SHA3_384 || defined SHA3_512)
#define SHA2_256
#endif

#if defined SHA1
#include "sha1.hpp"
#pragma message "Choosing to compile Merklization with SHA1 !"
#elif defined SHA2_224
#include "sha2_224.hpp"
#pragma message "Choosing to compile Merklization with SHA2-224 !"
#elif defined SHA2_256
#include "sha2_256.hpp"
#pragma message "Choosing to compile Merklization with SHA2-256 !"
#elif defined SHA2_384
#include "sha2_384.hpp"
#pragma message "Choosing to compile Merklization with SHA2-384 !"
#elif defined SHA2_512
#include "sha2_512.hpp"
#pragma message "Choosing to compile Merklization with SHA2-512 !"
#elif defined SHA2_512_224
#include "sha2_512_224.hpp"
#pragma message "Choosing to compile Merklization with SHA2-512/224 !"
#elif defined SHA2_512_256
#include "sha2_512_256.hpp"
#pragma message "Choosing to compile Merklization with SHA2-512/256 !"
#elif defined SHA3_256
#include "sha3_256.hpp"
#pragma message "Choosing to compile Merklization with SHA3-256 !"
#elif defined SHA3_224
#include "sha3_224.hpp"
#pragma message "Choosing to compile Merklization with SHA3-224 !"
#elif defined SHA3_384
#include "sha3_384.hpp"
#pragma message "Choosing to compile Merklization with SHA3-384 !"
#elif defined SHA3_512
#include "sha3_512.hpp"
#pragma message "Choosing to compile Merklization with SHA3-512 !"
#endif

// Binary merklization --- collects motivation from
// https://github.com/itzmeanjan/blake3/blob/e2a1340/include/merklize.hpp#L4-L12
//
// Choice of SHA variant as 2-to-1 hash function is compile-time decision using
// preprocessor directives, while default choice is SHA2-256
sycl::cl_ulong
merklize(sycl::queue& q,

#if defined SHA1 || defined SHA2_224 || defined SHA2_256
         const sycl::uint* __restrict leaf_nodes,
#elif defined SHA2_384 || defined SHA2_512 || defined SHA2_512_224 ||          \
  defined SHA2_512_256
         const sycl::ulong* __restrict leaf_nodes,
#elif defined SHA3_256 || defined SHA3_224 || defined SHA3_384 ||              \
  defined SHA3_512
         const sycl::uchar* __restrict leaf_nodes,
#endif

         size_t i_size, // leaf nodes size in bytes
         size_t leaf_cnt,

#if defined SHA1 || defined SHA2_224 || defined SHA2_256
         sycl::uint* const __restrict intermediates,
#elif defined SHA2_384 || defined SHA2_512 || defined SHA2_512_224 ||          \
  defined SHA2_512_256
         sycl::ulong* const __restrict intermediates,
#elif defined SHA3_256 || defined SHA3_224 || defined SHA3_384 ||              \
  defined SHA3_512
         sycl::uchar* const __restrict intermediates,
#endif

         size_t o_size, // intermediate nodes size in bytes
         size_t itmd_cnt,
         size_t wg_size)
{
  // A binary merkle tree with N -many leaf
  // nodes should have (N - 1) -many intermediates
  //
  // Note N = power of 2
  assert(leaf_cnt == itmd_cnt + 1);

#if defined SHA1
  assert(i_size == leaf_cnt * sha1::OUT_LEN_BYTES);
  assert(o_size == (itmd_cnt + 1) * sha1::OUT_LEN_BYTES);
#elif defined SHA2_224
  assert(i_size == leaf_cnt * sha2_224::OUT_LEN_BYTES);
  assert(o_size == (itmd_cnt + 1) * sha2_224::OUT_LEN_BYTES);
#elif defined SHA2_256
  assert(i_size == leaf_cnt * sha2_256::OUT_LEN_BYTES);
  assert(o_size == (itmd_cnt + 1) * sha2_256::OUT_LEN_BYTES);
#elif defined SHA2_384
  assert(i_size == leaf_cnt * sha2_384::OUT_LEN_BYTES);
  assert(o_size == (itmd_cnt + 1) * sha2_384::OUT_LEN_BYTES);
#elif defined SHA2_512
  assert(i_size == leaf_cnt * sha2_512::OUT_LEN_BYTES);
  assert(o_size == (itmd_cnt + 1) * sha2_512::OUT_LEN_BYTES);
#elif defined SHA2_512_224
  assert(i_size == leaf_cnt * sha2_512_224::OUT_LEN_BYTES);
  assert(o_size == (itmd_cnt + 1) * 32);
#elif defined SHA2_512_256
  assert(i_size == leaf_cnt * sha2_512_256::OUT_LEN_BYTES);
  assert(o_size == (itmd_cnt + 1) * sha2_512_256::OUT_LEN_BYTES);
#elif defined SHA3_256
  assert(i_size == leaf_cnt * sha3_256::OUT_LEN_BYTES);
  assert(o_size == (itmd_cnt + 1) * sha3_256::OUT_LEN_BYTES);
#elif defined SHA3_224
  assert(i_size == leaf_cnt * sha3_224::OUT_LEN_BYTES);
  assert(o_size == (itmd_cnt + 1) * sha3_224::OUT_LEN_BYTES);
#elif defined SHA3_384
  assert(i_size == leaf_cnt * sha3_384::OUT_LEN_BYTES);
  assert(o_size == (itmd_cnt + 1) * sha3_384::OUT_LEN_BYTES);
#elif defined SHA3_512
  assert(i_size == leaf_cnt * sha3_512::OUT_LEN_BYTES);
  assert(o_size == (itmd_cnt + 1) * sha3_512::OUT_LEN_BYTES);
#endif

  // both input and output allocation has same size
#if defined SHA1 || defined SHA2_224 || defined SHA2_256 ||                    \
  defined SHA2_384 || defined SHA2_512 || defined SHA2_512_256 ||              \
  defined SHA3_256 || defined SHA3_224 || defined SHA3_384 || defined SHA3_512

  assert(i_size == o_size);

#elif defined SHA2_512_224

  assert(i_size + (leaf_cnt << 2) == o_size);

#endif

  // only tree with power of 2 many leaf nodes
  // can be merklized by this implementation
  assert((leaf_cnt & (leaf_cnt - 1)) == 0);

  // At first N -many leaf nodes to be merged into
  // N/ 2 -many intermediate nodes, which are living
  // just above leaf nodes
  const size_t work_item_cnt = leaf_cnt >> 1;

  // validate whether this work group size can be used for next kernel dispatch
  assert(wg_size <= work_item_cnt);
  // ensure all dispatched work groups have equal many
  // active work-items
  assert(work_item_cnt % wg_size == 0);

#if defined SHA1 || defined SHA2_224 || defined SHA2_256
  // # -of 32 -bit unsigned integers, which can be contiguously placed
  // on output memory allocation
  //
  // note that `o_size` is in terms of bytes
  const size_t elm_cnt = o_size >> 2;
#elif defined SHA2_384 || defined SHA2_512 || defined SHA2_512_224 ||          \
  defined SHA2_512_256
  // # -of 64 -bit unsigned integers, which can be contiguously placed
  // on output memory allocation
  //
  // note that `o_size` is in terms of bytes
  const size_t elm_cnt = o_size >> 3;
#elif defined SHA3_256 || defined SHA3_224 || defined SHA3_384 ||              \
  defined SHA3_512
  // # -of 8 -bit unsigned integers ( read a byte ), which can be contiguously
  // placed on output memory allocation
  //
  // note that `o_size` is in terms of bytes
  const size_t elm_cnt = o_size;
#endif

  constexpr size_t i_offset = 0;
  const size_t o_offset = elm_cnt >> 1;

  // computes all intermediate nodes which are living just above leaf nodes of
  // binary merkle tree
  sycl::event evt_0 = q.submit([&](sycl::handler& h) {
    h.parallel_for<class kernelBinaryMerklizationPhase0>(
      sycl::nd_range<1>{ sycl::range<1>{ work_item_cnt },
                         sycl::range<1>{ wg_size } },
      [=](sycl::nd_item<1> it) {
        const size_t idx = it.get_global_linear_id();

#if defined SHA1
        const size_t in_idx = idx * (sha1::IN_LEN_BYTES >> 2);
        const size_t out_idx = idx * (sha1::OUT_LEN_BYTES >> 2);

        sycl::uint padded[16];
#elif defined SHA2_224
        const size_t in_idx = idx * (sha2_224::IN_LEN_BYTES >> 2);
        const size_t out_idx = idx * (sha2_224::OUT_LEN_BYTES >> 2);

        sycl::uint padded[32];
#elif defined SHA2_256
        const size_t in_idx = idx * (sha2_256::IN_LEN_BYTES >> 2);
        const size_t out_idx = idx * (sha2_256::OUT_LEN_BYTES >> 2);

        sycl::uint padded[32];
#elif defined SHA2_384
        const size_t in_idx = idx * (sha2_384::IN_LEN_BYTES >> 3);
        const size_t out_idx = idx * (sha2_384::OUT_LEN_BYTES >> 3);

        sycl::ulong padded[16];
#elif defined SHA2_512
        const size_t in_idx = idx * (sha2_512::IN_LEN_BYTES >> 3);
        const size_t out_idx = idx * (sha2_512::OUT_LEN_BYTES >> 3);

        sycl::ulong padded[32];
#elif defined SHA2_512_224
        const size_t in_idx = idx * (sha2_512_224::IN_LEN_BYTES >> 3);
        const size_t out_idx = idx * (32 >> 3);

        sycl::ulong padded[16];
#elif defined SHA2_512_256
        const size_t in_idx = idx * (sha2_512_256::IN_LEN_BYTES >> 3);
        const size_t out_idx = idx * (sha2_512_256::OUT_LEN_BYTES >> 3);

        sycl::ulong padded[16];
#elif defined SHA3_256
        const size_t in_idx = idx * sha3_256::IN_LEN_BYTES;
        const size_t out_idx = idx * sha3_256::OUT_LEN_BYTES;
#elif defined SHA3_224
        const size_t in_idx = idx * sha3_224::IN_LEN_BYTES;
        const size_t out_idx = idx * sha3_224::OUT_LEN_BYTES;
#elif defined SHA3_384
        const size_t in_idx = idx * sha3_384::IN_LEN_BYTES;
        const size_t out_idx = idx * sha3_384::OUT_LEN_BYTES;
#elif defined SHA3_512
        const size_t in_idx = idx * sha3_512::IN_LEN_BYTES;
        const size_t out_idx = idx * sha3_512::OUT_LEN_BYTES;
#endif

#if defined SHA1
        sha1::pad_input_message(leaf_nodes + i_offset + in_idx, padded);
        sha1::hash(padded, intermediates + o_offset + out_idx);
#elif defined SHA2_224
        sha2_224::pad_input_message(leaf_nodes + i_offset + in_idx, padded);
        sha2_224::hash(padded, intermediates + o_offset + out_idx);
#elif defined SHA2_256
        sha2_256::pad_input_message(leaf_nodes + i_offset + in_idx, padded);
        sha2_256::hash(padded, intermediates + o_offset + out_idx);
#elif defined SHA2_384
        sha2_384::pad_input_message(leaf_nodes + i_offset + in_idx, padded);
        sha2_384::hash(padded, intermediates + o_offset + out_idx);
#elif defined SHA2_512
        sha2_512::pad_input_message(leaf_nodes + i_offset + in_idx, padded);
        sha2_512::hash(padded, intermediates + o_offset + out_idx);
#elif defined SHA2_512_224
        sha2_512_224::pad_input_message(leaf_nodes + i_offset + in_idx, padded);
        sha2_512_224::hash(padded, intermediates + o_offset + out_idx);
#elif defined SHA2_512_256
        sha2_512_256::pad_input_message(leaf_nodes + i_offset + in_idx, padded);
        sha2_512_256::hash(padded, intermediates + o_offset + out_idx);
#elif defined SHA3_256
        const sycl::uchar* in = leaf_nodes + i_offset + in_idx;
        sycl::uchar* out = intermediates + o_offset + out_idx;

        sha3_256::hash(in, out);
#elif defined SHA3_224
        const sycl::uchar* in = leaf_nodes + i_offset + in_idx;
        sycl::uchar* out = intermediates + o_offset + out_idx;

        sha3_224::hash(in, out);
#elif defined SHA3_384
        const sycl::uchar* in = leaf_nodes + i_offset + in_idx;
        sycl::uchar* out = intermediates + o_offset + out_idx;

        sha3_384::hash(in, out);
#elif defined SHA3_512
        const sycl::uchar* in = leaf_nodes + i_offset + in_idx;
        sycl::uchar* out = intermediates + o_offset + out_idx;

        sha3_512::hash(in, out);
#endif
      });
  });

  // these many kernel dispatch rounds still remaining
  const size_t rounds =
    static_cast<size_t>(sycl::log2(static_cast<double>(work_item_cnt)));

  std::vector<sycl::event> evts_0;
  // reserve enough space in vector so that all events obtained as result
  // of enqueuing kernel execution commands, can be accomodated
  evts_0.reserve(rounds + 1);

  // storing event obtained as result of enqueuing first phase of kernel
  // execution command, where leaf nodes are paired to generate all intermediate
  // nodes living immediately above them
  evts_0.push_back(evt_0);

  for (size_t r = 0; r < rounds; r++) {
    // multiple rounds of kernel dispatches, where intermediate nodes are being
    // computed from already computed (in previous dispatch round) intermediate
    // nodes
    sycl::event evt_1 = q.submit([&](sycl::handler& h) {
      // note, dependency chain being built !
      //
      // each dispatch round depends on previously enqueued dispatch round
      h.depends_on(evts_0.at(r));

      const size_t work_item_cnt_ = work_item_cnt >> (r + 1);
      const size_t wg_size_ =
        wg_size <= work_item_cnt_ ? wg_size : work_item_cnt_;

      const size_t i_offset_ = o_offset >> r;
      const size_t o_offset_ = i_offset_ >> 1;

      h.parallel_for<class kernelBinaryMerklizationPhase1>(
        sycl::nd_range<1>{ sycl::range<1>{ work_item_cnt_ },
                           sycl::range<1>{ wg_size_ } },
        [=](sycl::nd_item<1> it) {
          const size_t idx = it.get_global_linear_id();

#if defined SHA1
          const size_t in_idx = idx * (sha1::IN_LEN_BYTES >> 2);
          const size_t out_idx = idx * (sha1::OUT_LEN_BYTES >> 2);

          sycl::uint padded[16];
#elif defined SHA2_224
          const size_t in_idx = idx * (sha2_224::IN_LEN_BYTES >> 2);
          const size_t out_idx = idx * (sha2_224::OUT_LEN_BYTES >> 2);

          sycl::uint padded[32];
#elif defined SHA2_256
          const size_t in_idx = idx * (sha2_256::IN_LEN_BYTES >> 2);
          const size_t out_idx = idx * (sha2_256::OUT_LEN_BYTES >> 2);

          sycl::uint padded[32];
#elif defined SHA2_384
          const size_t in_idx = idx * (sha2_384::IN_LEN_BYTES >> 3);
          const size_t out_idx = idx * (sha2_384::OUT_LEN_BYTES >> 3);

          sycl::ulong padded[16];
#elif defined SHA2_512
          const size_t in_idx = idx * (sha2_512::IN_LEN_BYTES >> 3);
          const size_t out_idx = idx * (sha2_512::OUT_LEN_BYTES >> 3);

          sycl::ulong padded[32];
#elif defined SHA2_512_224
          const size_t in_idx = idx * (64 >> 3);
          const size_t out_idx = idx * (32 >> 3);

          // in following section, I'm extracting first 28 -bytes
          // from two consecutive 32 -bytes SHA2-512/224 digests and
          // concatenating them in 64 -bit word form such that total seven 64
          // -bit words are holding total 56 -bytes (non-padded) input to 2-to-1
          // SHA2-512/224 hash function
          //
          // so from eight 64 -bit words I'll prepare seven 64 -bit words
          // which will be 2-to-1 hashed using SHA2-512/224 ( for binary
          // merklization )
          //
          // that means first digest is leaf child and second one is right child
          // of some parent node whose hash is being computed by this work-item
          sycl::ulong in_words[7];

          // pointer aliasing for ease of typing ( and understanding )
          sycl::ulong* in_ptr = intermediates + i_offset_ + in_idx;

          // first three 64 -bit words of first SHA2-512/224 digest
          // are taken as they are
          in_words[0] = *(in_ptr + 0);
          in_words[1] = *(in_ptr + 1);
          in_words[2] = *(in_ptr + 2);
          // then  MSB 32 -bits of last word of first digest
          // and MSB 32 -bits of first word of second digest are
          // concatenated into single 64 -bit word
          in_words[3] = (((*(in_ptr + 3) >> 32) & 0xfffffffful) << 32) |
                        ((*(in_ptr + 4) >> 32) & 0xfffffffful);

          // then next consecutive 192 -bits are taken such that
          // it can be stored in three consecutive 64 -bit words
          in_words[4] = ((*(in_ptr + 4) & 0xfffffffful) << 32) |
                        ((*(in_ptr + 5) >> 32) & 0xfffffffful);

          in_words[5] = ((*(in_ptr + 5) & 0xfffffffful) << 32) |
                        ((*(in_ptr + 6) >> 32) & 0xfffffffful);

          in_words[6] = ((*(in_ptr + 6) & 0xfffffffful) << 32) |
                        ((*(in_ptr + 7) >> 32) & 0xfffffffful);

          sycl::ulong padded[16];

#elif defined SHA2_512_256
          const size_t in_idx = idx * (sha2_512_256::IN_LEN_BYTES >> 3);
          const size_t out_idx = idx * (sha2_512_256::OUT_LEN_BYTES >> 3);

          sycl::ulong padded[16];
#elif defined SHA3_256
          const size_t in_idx = idx * sha3_256::IN_LEN_BYTES;
          const size_t out_idx = idx * sha3_256::OUT_LEN_BYTES;
#elif defined SHA3_224
          const size_t in_idx = idx * sha3_224::IN_LEN_BYTES;
          const size_t out_idx = idx * sha3_224::OUT_LEN_BYTES;
#elif defined SHA3_384
          const size_t in_idx = idx * sha3_384::IN_LEN_BYTES;
          const size_t out_idx = idx * sha3_384::OUT_LEN_BYTES;
#elif defined SHA3_512
          const size_t in_idx = idx * sha3_512::IN_LEN_BYTES;
          const size_t out_idx = idx * sha3_512::OUT_LEN_BYTES;
#endif

#if defined SHA1
          sha1::pad_input_message(intermediates + i_offset_ + in_idx, padded);
          sha1::hash(padded, intermediates + o_offset_ + out_idx);
#elif defined SHA2_224
          sha2_224::pad_input_message(intermediates + i_offset_ + in_idx,
                                      padded);
          sha2_224::hash(padded, intermediates + o_offset_ + out_idx);
#elif defined SHA2_256
          sha2_256::pad_input_message(intermediates + i_offset_ + in_idx,
                                      padded);
          sha2_256::hash(padded, intermediates + o_offset_ + out_idx);
#elif defined SHA2_384
          sha2_384::pad_input_message(intermediates + i_offset_ + in_idx,
                                      padded);
          sha2_384::hash(padded, intermediates + o_offset_ + out_idx);
#elif defined SHA2_512
          sha2_512::pad_input_message(intermediates + i_offset_ + in_idx,
                                      padded);
          sha2_512::hash(padded, intermediates + o_offset_ + out_idx);
#elif defined SHA2_512_224
          sha2_512_224::pad_input_message(in_words, padded);
          sha2_512_224::hash(padded, intermediates + o_offset_ + out_idx);
#elif defined SHA2_512_256
          sha2_512_256::pad_input_message(intermediates + i_offset_ + in_idx,
                                          padded);
          sha2_512_256::hash(padded, intermediates + o_offset_ + out_idx);
#elif defined SHA3_256
          const sycl::uchar* in = intermediates + i_offset_ + in_idx;
          sycl::uchar* out = intermediates + o_offset_ + out_idx;

          sha3_256::hash(in, out);
#elif defined SHA3_224
          const sycl::uchar* in = intermediates + i_offset_ + in_idx;
          sycl::uchar* out = intermediates + o_offset_ + out_idx;

          sha3_224::hash(in, out);
#elif defined SHA3_384
          const sycl::uchar* in = intermediates + i_offset_ + in_idx;
          sycl::uchar* out = intermediates + o_offset_ + out_idx;

          sha3_384::hash(in, out);
#elif defined SHA3_512
          const sycl::uchar* in = intermediates + i_offset_ + in_idx;
          sycl::uchar* out = intermediates + o_offset_ + out_idx;

          sha3_512::hash(in, out);
#endif
        });
    });
    evts_0.push_back(evt_1);
  }

  // wait for last kernel dispatch round, where root of binary merkle tree is
  // computed !
  evts_0.at(rounds).wait();

  // time execution of all enqueued kernels with nanosecond level granularity
  sycl::cl_ulong ts = 0;
  for (size_t r = 0; r < rounds + 1; r++) {
    ts += time_event(evts_0.at(r));
  }

  // return total kernel execution cost, in terms of nanosecond
  return ts;
}
