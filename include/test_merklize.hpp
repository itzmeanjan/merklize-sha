#pragma once
#include "merklize.hpp"
#include <cassert>
#include <iostream>

void
test_merklize(sycl::queue& q)
{
  // testing on binary merkle tree which has 8 leaf nodes
  constexpr size_t leaf_cnt = 1 << 3;

#if defined SHA1
  constexpr size_t i_size = leaf_cnt * sha1::OUT_LEN_BYTES; // in bytes
  constexpr size_t o_size = leaf_cnt * sha1::OUT_LEN_BYTES; // in bytes
#elif defined SHA2_224
  constexpr size_t i_size = leaf_cnt * sha2_224::OUT_LEN_BYTES; // in bytes
  constexpr size_t o_size = leaf_cnt * sha2_224::OUT_LEN_BYTES; // in bytes
#elif defined SHA2_256
  constexpr size_t i_size = leaf_cnt * sha2_256::OUT_LEN_BYTES; // in bytes
  constexpr size_t o_size = leaf_cnt * sha2_256::OUT_LEN_BYTES; // in bytes
#elif defined SHA2_384
  constexpr size_t i_size = leaf_cnt * sha2_384::OUT_LEN_BYTES; // in bytes
  constexpr size_t o_size = leaf_cnt * sha2_384::OUT_LEN_BYTES; // in bytes
#elif defined SHA2_512
  constexpr size_t i_size = leaf_cnt * sha2_512::OUT_LEN_BYTES; // in bytes
  constexpr size_t o_size = leaf_cnt * sha2_512::OUT_LEN_BYTES; // in bytes
#elif defined SHA2_512_224
  constexpr size_t i_size = leaf_cnt * sha2_512_224::OUT_LEN_BYTES; // in bytes
  constexpr size_t o_size =
    leaf_cnt * 32; // in bytes
                   // note digest is actually 28 -bytes, so drop last 4 !
#elif defined SHA2_512_256
  constexpr size_t i_size = leaf_cnt * sha2_512_256::OUT_LEN_BYTES; // in bytes
  constexpr size_t o_size = leaf_cnt * sha2_512_256::OUT_LEN_BYTES; // in bytes
#elif defined SHA3_256
  constexpr size_t i_size = leaf_cnt * sha3_256::OUT_LEN_BYTES; // in bytes
  constexpr size_t o_size = leaf_cnt * sha3_256::OUT_LEN_BYTES; // in bytes
#endif

  // obtained using following code snippet run on python3 shell
  //
  // >>> import hashlib
#if defined SHA1
  //
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
#elif defined SHA2_224
  //
  // >>> a = [0xff] * 56
  // >>> b = list(hashlib.sha224(bytes(a)).digest()); b
  // [140, 250, 128, 28, 254, 116, 112, 113, 88, 113, 102, 5, 189, 54, 5, 27,
  // 74, 136, 109, 48, 20, 8, 50, 168, 140, 123, 210, 114]

  // >>> c = b * 2
  // >>> d = list(hashlib.sha224(bytes(c)).digest()); d
  // [71, 212, 232, 90, 92, 160, 135, 245, 176, 115, 198, 156, 203, 178, 147,
  // 104, 12, 141, 40, 52, 153, 47, 215, 175, 88, 78, 74, 219]

  // >>> e = d * 2
  // >>> f = list(hashlib.sha224(bytes(e)).digest())

  // >>> f
  // [68, 112, 247, 219, 202, 225, 184, 209, 196, 9, 206, 28, 243, 98, 103, 193,
  // 123, 100, 218, 42, 254, 195, 132, 224, 199, 116, 140, 223]

  constexpr sycl::uchar expected[28] = { 68,  112, 247, 219, 202, 225, 184,
                                         209, 196, 9,   206, 28,  243, 98,
                                         103, 193, 123, 100, 218, 42,  254,
                                         195, 132, 224, 199, 116, 140, 223 };
#elif defined SHA2_256
  //
  // >>> a = [0xff] * 64
  // >>> b = list(hashlib.sha256(bytes(a)).digest()); b
  // [134, 103, 231, 24, 41, 78, 158, 13, 241, 211, 6, 0, 186, 62, 235, 32, 31,
  // 118, 74, 173, 45, 173, 114, 116, 134, 67, 228, 162, 133, 225, 209, 247]

  // >>> c = b * 2
  // >>> d = list(hashlib.sha256(bytes(c)).digest()); d
  // [55, 93, 108, 123, 40, 10, 30, 48, 249, 104, 219, 29, 148, 141, 160, 249,
  // 119, 191, 145, 57, 176, 213, 81, 103, 97, 172, 135, 71, 0, 32, 138, 186]

  // >>> e = d * 2
  // >>> f = list(hashlib.sha256(bytes(e)).digest())

  // >>> f
  // [190, 27, 112, 21, 237, 80, 215, 73, 10, 81, 241, 177, 29, 255, 128, 74,
  // 68, 64, 119, 92, 200, 8, 185, 207, 210, 97, 87, 128, 92, 31, 142, 134]
  constexpr sycl::uchar expected[32] = {
    190, 27, 112, 21, 237, 80, 215, 73,  10,  81, 241, 177, 29, 255, 128, 74,
    68,  64, 119, 92, 200, 8,  185, 207, 210, 97, 87,  128, 92, 31,  142, 134
  };
#elif defined SHA2_384
  //
  // >>> a = [0xff] * 96
  // >>> b = list(hashlib.sha384(bytes(a)).digest()); b
  // [120, 195, 4, 101, 32, 184, 165, 150, 9, 221, 16, 126, 43, 186, 64, 107,
  // 143, 124, 119, 179, 53, 135, 31, 39, 146, 115, 75, 158, 151, 254, 247, 182,
  // 91, 31, 17, 212, 123, 219, 246, 75, 217, 24, 111, 77, 215, 195, 125, 165]

  // >>> c = b * 2
  // >>> d = list(hashlib.sha384(bytes(c)).digest()); d
  // [227, 29, 252, 255, 250, 146, 71, 38, 152, 231, 169, 100, 72, 182, 172, 85,
  // 39, 82, 76, 213, 182, 23, 141, 45, 195, 141, 134, 156, 50, 73, 29, 223,
  // 251, 156, 145, 97, 16, 6, 12, 104, 80, 1, 254, 85, 175, 233, 154, 150]

  // >>> e = d * 2
  // >>> f = list(hashlib.sha384(bytes(e)).digest())

  // >>> f
  // [239, 157, 55, 183, 110, 217, 152, 174, 198, 161, 104, 34, 255, 210, 42,
  // 127, 109, 225, 231, 137, 155, 208, 1, 12, 92, 229, 164, 16, 115, 202, 32,
  // 70, 178, 181, 244, 155, 15, 182, 228, 7, 163, 103, 145, 117, 126, 76, 22,
  // 60]
  constexpr sycl::uchar expected[48] = {
    239, 157, 55,  183, 110, 217, 152, 174, 198, 161, 104, 34,
    255, 210, 42,  127, 109, 225, 231, 137, 155, 208, 1,   12,
    92,  229, 164, 16,  115, 202, 32,  70,  178, 181, 244, 155,
    15,  182, 228, 7,   163, 103, 145, 117, 126, 76,  22,  60
  };
#elif defined SHA2_512
  //
  // >>> a = [0xff] * 128
  // >>> b = list(hashlib.sha512(bytes(a)).digest()); b
  // [90, 202, 240, 111, 93, 209, 209, 7, 184, 27, 155, 117, 22, 212, 84, 227,
  // 4, 207, 86, 153, 208, 31, 254, 102, 160, 26, 213, 84, 207, 176, 219, 137,
  // 107, 188, 22, 224, 139, 212, 251, 202, 179, 99, 100, 144, 158, 223, 80,
  // 236, 182, 200, 4, 39, 34, 164, 197, 148, 86, 217, 4, 130, 68, 205, 87, 240]

  // >>> c = b * 2
  // >>> d = list(hashlib.sha512(bytes(c)).digest()); d
  // [29, 250, 176, 241, 104, 30, 96, 125, 162, 189, 87, 132, 239, 233, 197, 38,
  // 115, 203, 5, 77, 121, 19, 221, 76, 158, 205, 2, 246, 119, 235, 142, 168,
  // 208, 86, 184, 121, 23, 124, 5, 35, 213, 226, 12, 28, 89, 184, 202, 238, 78,
  // 226, 3, 191, 191, 67, 130, 141, 106, 49, 60, 195, 37, 126, 191, 246]

  // >>> e = d * 2
  // >>> f = list(hashlib.sha512(bytes(e)).digest())

  // >>> f
  // [16, 89, 255, 34, 217, 58, 55, 214, 124, 223, 84, 72, 189, 98, 82, 87, 164,
  // 252, 176, 254, 76, 1, 212, 167, 85, 125, 123, 2, 88, 197, 250, 70, 142, 62,
  // 29, 73, 251, 23, 13, 164, 62, 38, 67, 243, 171, 8, 222, 186, 25, 108, 214,
  // 177, 241, 243, 178, 130, 121, 21, 200, 224, 122, 187, 59, 187]
  constexpr sycl::uchar expected[64] = {
    16,  89,  255, 34,  217, 58,  55,  214, 124, 223, 84,  72,  189,
    98,  82,  87,  164, 252, 176, 254, 76,  1,   212, 167, 85,  125,
    123, 2,   88,  197, 250, 70,  142, 62,  29,  73,  251, 23,  13,
    164, 62,  38,  67,  243, 171, 8,   222, 186, 25,  108, 214, 177,
    241, 243, 178, 130, 121, 21,  200, 224, 122, 187, 59,  187
  };
#elif defined SHA2_512_224
  //
  // >>> a = [0xff] * 56
  // >>> b = list(SHA512.new(data= bytes([0xff] * 56),truncate='224').digest());
  // b [48, 203, 99, 172, 231, 234, 247, 242, 145, 165, 10, 53, 219, 85, 130,
  // 55, 155, 52, 43, 55, 172, 78, 125, 185, 119, 230, 148, 129]

  // >>> c = b * 2
  // >>> d = list(SHA512.new(data= bytes(c),truncate='224').digest()); d
  // [35, 211, 149, 84, 66, 218, 192, 196, 121, 52, 94, 75, 251, 40, 83, 102,
  // 182, 23, 45, 239, 44, 2, 97, 100, 31, 26, 4, 142]

  // >>> e = d * 2
  // >>> f =  list(SHA512.new(data= bytes(e),truncate='224').digest())

  // >>> f
  // [45, 213, 101, 185, 49, 91, 242, 198, 250, 179, 90, 147, 49, 158, 113, 189,
  // 131, 120, 67, 135, 193, 39, 110, 71, 26, 195, 63, 193]
  constexpr sycl::uchar expected[28] = { 45,  213, 101, 185, 49,  91,  242,
                                         198, 250, 179, 90,  147, 49,  158,
                                         113, 189, 131, 120, 67,  135, 193,
                                         39,  110, 71,  26,  195, 63,  193 };
#elif defined SHA2_512_256
  //
  // >>> a = [0xff] * 64
  // >>> b = list(SHA512.new(data= bytes(a),truncate='256').digest()); b
  // [254, 218, 174, 155, 245, 143, 133, 143, 128, 130, 195, 85, 44, 169, 71,
  // 77, 8, 123, 94, 131, 30, 38, 179, 26, 164, 7, 159, 115, 132, 111, 54, 93]

  // >>> c = b * 2
  // >>> d = list(SHA512.new(data= bytes(c),truncate='256').digest()); d
  // [111, 152, 206, 204, 224, 191, 32, 143, 125, 172, 90, 72, 37, 40, 72, 147,
  // 253, 199, 207, 161, 98, 76, 13, 24, 105, 250, 17, 79, 29, 58, 7, 136]

  // >>> e = d * 2
  // >>> f =  list(SHA512.new(data= bytes(e),truncate='256').digest())

  // >>> f
  // [129, 151, 248, 46, 143, 39, 163, 78, 234, 177, 146, 147, 233, 80, 172,
  // 144, 1, 184, 229, 187, 174, 201, 189, 160, 169, 168, 64, 21, 112, 149, 72,
  // 139]
  constexpr sycl::uchar expected[32] = {
    129, 151, 248, 46,  143, 39, 163, 78,  234, 177, 146,
    147, 233, 80,  172, 144, 1,  184, 229, 187, 174, 201,
    189, 160, 169, 168, 64,  21, 112, 149, 72,  139
  };
#elif defined SHA3_256
  //
  // >>> a = [0xff] * 64
  // >>> b = list(hashlib.sha3_256(bytes(a)).digest()); b
  // [127, 216, 219, 145, 139, 238, 83, 121, 178, 47, 88, 60, 230, 71, 159, 120,
  // 77, 35, 40, 22, 190, 170, 86, 66, 36, 58, 115, 74, 129, 101, 161, 90]

  // >>> c = b * 2
  // >>> d = list(hashlib.sha3_256(bytes(c)).digest()); d
  // [105, 66, 252, 242, 214, 146, 9, 148, 126, 206, 138, 110, 64, 115, 31, 49,
  // 54, 217, 247, 151, 154, 223, 58, 84, 111, 217, 196, 181, 72, 62, 22, 52]

  // >>> e = d * 2
  // >>> f =  list(hashlib.sha3_256(bytes(e)).digest())

  // >>> f
  // [159, 200, 74, 194, 101, 231, 247, 10, 65, 194, 250, 128, 32, 140, 171, 51,
  // 143, 128, 183, 61, 78, 102, 179, 87, 41, 4, 59, 151, 162, 190, 109, 76]
  constexpr sycl::uchar expected[32] = {
    159, 200, 74,  194, 101, 231, 247, 10, 65, 194, 250, 128, 32,  140, 171, 51,
    143, 128, 183, 61,  78,  102, 179, 87, 41, 4,   59,  151, 162, 190, 109, 76
  };
#endif

#if defined SHA1 || defined SHA2_224 || defined SHA2_256
  // acquire resources
  sycl::uchar* in_0 = (sycl::uchar*)sycl::malloc_shared(i_size, q);
  sycl::uint* in_1 = (sycl::uint*)sycl::malloc_shared(i_size, q);
  sycl::uint* out_0 = (sycl::uint*)sycl::malloc_shared(o_size, q);
  sycl::uchar* out_1 = (sycl::uchar*)sycl::malloc_shared(o_size, q);
#elif defined SHA2_384 || defined SHA2_512 || defined SHA2_512_224 ||          \
  defined SHA2_512_256
  // acquire resources
  sycl::uchar* in_0 = (sycl::uchar*)sycl::malloc_shared(i_size, q);
  sycl::ulong* in_1 = (sycl::ulong*)sycl::malloc_shared(i_size, q);
  sycl::ulong* out_0 = (sycl::ulong*)sycl::malloc_shared(o_size, q);
  sycl::uchar* out_1 = (sycl::uchar*)sycl::malloc_shared(o_size, q);
#elif defined SHA3_256
  // acquire resources
  sycl::uchar* in = (sycl::uchar*)sycl::malloc_shared(i_size, q);
  sycl::uchar* out = (sycl::uchar*)sycl::malloc_shared(o_size, q);
#endif

#if defined SHA3_256

  // prepare input bytes
  q.memset(in, 0xff, i_size).wait();
  // I'm doing this intensionally just to check that
  // first digest bytes are never touched by any work-items !
  q.memset(out, 0, o_size).wait();

#else

  // prepare input bytes
  q.memset(in_0, 0xff, i_size).wait();
  // I'm doing this intensionally just to check that
  // first digest bytes are never touched by any work-items !
  q.memset(out_0, 0, o_size).wait();

#endif

  // convert input bytes to hash words !
  //
  // instead of doing this, I could have simply set
  // all input bytes to 0xff
  //
  // q.memset(in_1, 0xff, i_size).wait();
  //
  // but I decided to do it manually, just to be sure that
  // I'm thinking correctly !
#if defined SHA1 || defined SHA2_224 || defined SHA2_256

#pragma unroll 8
  for (size_t i = 0; i < (i_size >> 2); i++) {
    *(in_1 + i) = from_be_bytes_to_u32_words(in_0 + (i << 2));
  }

#elif defined SHA2_384 || defined SHA2_512 || defined SHA2_512_256

#pragma unroll 8
  for (size_t i = 0; i < (i_size >> 3); i++) {
    *(in_1 + i) = from_be_bytes_to_u64_words(in_0 + (i << 3));
  }

#elif defined SHA2_512_224

#pragma unroll 7
  for (size_t i = 0; i < (i_size >> 3); i++) {
    *(in_1 + i) = from_be_bytes_to_u64_words(in_0 + (i << 3));
  }

#endif

#if defined SHA3_256

  // wait until completely merklized !
  merklize(q, in, i_size, leaf_cnt, out, o_size, leaf_cnt - 1, leaf_cnt >> 1);

#else

  // wait until completely merklized !
  merklize(
    q, in_1, i_size, leaf_cnt, out_0, o_size, leaf_cnt - 1, leaf_cnt >> 1);

#endif

  // finally convert all intermediate nodes from word representation
  // to big endian byte array form
#if defined SHA1 || defined SHA2_224 || defined SHA2_256

#pragma unroll 8
  for (size_t i = 0; i < (o_size >> 2); i++) {
    const sycl::uint num = *(out_0 + i);
    from_words_to_be_bytes(num, out_1 + (i << 2));
  }

#elif defined SHA2_384 || defined SHA2_512 || defined SHA2_512_224 ||          \
  defined SHA2_512_256

#pragma unroll 8
  for (size_t i = 0; i < (o_size >> 3); i++) {
    const sycl::ulong num = *(out_0 + i);
    from_words_to_be_bytes(num, out_1 + (i << 3));
  }

#endif

  // first digest should never be touched !
  for (size_t i = 0; i <

#if defined SHA1
                     sha1::OUT_LEN_BYTES
#elif defined SHA2_224
                     sha2_224::OUT_LEN_BYTES
#elif defined SHA2_256
                     sha2_256::OUT_LEN_BYTES
#elif defined SHA2_384
                     sha2_384::OUT_LEN_BYTES
#elif defined SHA2_512
                     sha2_512::OUT_LEN_BYTES
#elif defined SHA2_512_224
                     sha2_512_224::OUT_LEN_BYTES
#elif defined SHA2_512_256
                     sha2_512_256::OUT_LEN_BYTES
#elif defined SHA3_256
                     sha3_256::OUT_LEN_BYTES
#endif

       ;

       i++) {
#if defined SHA3_256

    assert(*(out + i) == 0);

#else

    assert(*(out_1 + i) == 0);

#endif
  }

  // then comes root of merkle tree !

#if defined SHA1
  for (size_t i = sha1::OUT_LEN_BYTES, j = 0;
       i < (sha1::OUT_LEN_BYTES << 1) && j < sha1::OUT_LEN_BYTES;
       i++, j++)
#elif defined SHA2_224
  for (size_t i = sha2_224::OUT_LEN_BYTES, j = 0;
       i < (sha2_224::OUT_LEN_BYTES << 1) && j < sha2_224::OUT_LEN_BYTES;
       i++, j++)
#elif defined SHA2_256
  for (size_t i = sha2_256::OUT_LEN_BYTES, j = 0;
       i < (sha2_256::OUT_LEN_BYTES << 1) && j < sha2_256::OUT_LEN_BYTES;
       i++, j++)
#elif defined SHA2_384
  for (size_t i = sha2_384::OUT_LEN_BYTES, j = 0;
       i < (sha2_384::OUT_LEN_BYTES << 1) && j < sha2_384::OUT_LEN_BYTES;
       i++, j++)
#elif defined SHA2_512
  for (size_t i = sha2_512::OUT_LEN_BYTES, j = 0;
       i < (sha2_512::OUT_LEN_BYTES << 1) && j < sha2_512::OUT_LEN_BYTES;
       i++, j++)
#elif defined SHA2_512_224
  for (size_t i = sha2_512_224::OUT_LEN_BYTES + 4, j = 0;
       i < ((sha2_512_224::OUT_LEN_BYTES << 1) + 4) &&
       j < sha2_512_224::OUT_LEN_BYTES;
       i++, j++)
#elif defined SHA2_512_256
  for (size_t i = sha2_512_256::OUT_LEN_BYTES, j = 0;
       i < (sha2_512_256::OUT_LEN_BYTES << 1) &&
       j < sha2_512_256::OUT_LEN_BYTES;
       i++, j++)
#elif defined SHA3_256
  for (size_t i = sha3_256::OUT_LEN_BYTES, j = 0;
       i < (sha3_256::OUT_LEN_BYTES << 1) && j < sha3_256::OUT_LEN_BYTES;
       i++, j++)
#endif

  {
#if defined SHA3_256

    assert(*(out + i) == expected[j]);

#else

    assert(*(out_1 + i) == expected[j]);

#endif
  }

  // ensure resources are deallocated
#if defined SHA3_256

  sycl::free(in, q);
  sycl::free(out, q);

#else

  sycl::free(in_0, q);
  sycl::free(in_1, q);
  sycl::free(out_0, q);
  sycl::free(out_1, q);

#endif
}
