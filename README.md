# merklize-sha

SYCL accelerated Binary Merklization using SHA1, SHA2 & SHA3

## Motivation

After implementing BLAKE3 using SYCL, I decided to accelerate 2-to-1 hash implementation of all variants of SHA1, SHA2 & SHA3 families of cryptographic hash functions. BLAKE3 lends itself pretty well to parallelization efforts, due to its inherent data parallel friendly algorithmic construction, where each 1024 -bytes chunk can be compressed independently ( read parallelly ) and finally it's a binary merklization problem with compressed chunks as leaf nodes of binary merkle tree. But none of SHA1, SHA2 & SHA3 families of cryptographic hash functions are data parallel, requiring to process each message block ( can be 512 -bit/ 1024 -bit or padded to 1600 -bit in case of SHA3 family ) sequentially, which is why I only concentrated on accelerating Binary Merklization where SHA1/ SHA2/ SHA3 families of cryptographic ( 2-to-1 ) hash functions are used for computing all intermediate nodes of tree when N -many leaf nodes are provided, where `N = 2 ^ i | i = {1, 2, 3 ...}`. Each of these N -many leaf nodes are respective hash digests --- for example, when using SHA2-256 variant for computing all intermediate nodes of binary merkle tree, each of provided leaf node is 32 -bytes wide, representing a SHA2-256 digest. Now, N -many leaf digests are merged into N/ 2 -many digests which are intermediate nodes, living just above leaf nodes. Then in next phase, those N/ 2 -many intermediates are used for computing N/ 4 -many of intermediates which are living just above them. This process continues until root of merkle tree is computed. Notice, that in each level of tree, each consecutive pair of digests can be hashed independently --- and that's the scope of parallelism I'd like to make use of during binary merklization. In following depiction, when N ( = 4 ) nodes are provided as input, two intermediates can be computed in parallel and once they're computed root of tree can be computed as a single task.

```bash
  ((a, b), (c, d))          < --- [Level 1] [Root]
     /       \
    /         \
 (a, b)      (c, d)         < --- [Level 2] [Intermediates]
  / \        /  \
 /   \      /    \
a     b     c     d         < --- [Level 3] [Leaves]
```

I'd also like you to note that, computation of nodes of level-i of tree are data dependent on level-(i + 1).

When N is power of 2 and those many nodes are provided as input, (N - 1) -many intermediates to be computed. For that reason, size of allocated memory for output is of same size as input is. That means, very first few bytes ( = digest size of hash function in use ) of output memory allocation will be empty. To be more specific, if SHA2-224 is our choice of hash function, then first 28 -bytes of output memory allocation will not be of interest, but skipping that next 28 -bytes chunk should have root of tree, once offloaded computation finishes its execution.

```bash
input   = [a, b, c, d]
output  = [0, ((a, b), (c, d)), (a, b), (c, d)]
```

Here in this repository, I'm keeping binary merklization kernels, implemented in SYCL, while using SHA1/ SHA2/ SHA3 variants as 2-to-1 hash function, which one to use is compile-time choice using pre-processor directive.

If you happen to be interested in Binary Merklization using Rescue Prime Hash/ BLAKE3, consider seeing following links.

- [Binary Merklization using Rescue Prime Hash](https://github.com/itzmeanjan/ff-gpu)
- [Binary Merklization using BLAKE3](https://github.com/itzmeanjan/blake3)

> During SHA1, SHA2 implementations, I've followed Secure Hash Standard [specification](http://dx.doi.org/10.6028/NIST.FIPS.180-4).

> During SHA3 implementations, I've followed SHA-3 Standard [specification](http://dx.doi.org/10.6028/NIST.FIPS.202).

> Using SHA1 for binary merklization may not be a good choice these days, see [here](https://csrc.nist.gov/Projects/Hash-Functions/NIST-Policy-on-Hash-Functions). But still I'm keeping SHA1 implementation, just as a reference.

## Prerequisites

- I'm using 

```bash
$ lsb_release -d

Description:    Ubuntu 20.04.3 LTS
```

- You should have Intel's DPCPP compiler, which is an open-source llvm-based SYCL specification's implementation; see [here](https://www.intel.com/content/www/us/en/developer/tools/oneapi/base-toolkit-download.html)

```bash
$ dpcpp --version

Intel(R) oneAPI DPC++/C++ Compiler 2022.0.0 (2022.0.0.20211123)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/intel/oneapi/compiler/2022.0.2/linux/bin-llvm
```

- If you're planning to target Nvidia GPU, I suggest you compile aforementioned toolchain from source; see [here](https://intel.github.io/llvm-docs/GetStartedGuide.html#prerequisites)

```bash
$ clang++ --version

clang version 14.0.0 (https://github.com/intel/llvm c690ac8d771e8bb1a1be651872b782f4044d936c)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/ubuntu/sycl_workspace/llvm/build/bin
```

- You will also need to have `make` utility for easily running compilation flow, along with that `clang-format` for source formatting can be helpful.
- Another useful tool to have is `sycl-info`, for quickly checking available SYCL implementation related details; see [here](https://github.com/codeplaysoftware/sycl-info)

## Usage

If you happen to be interested in 2-to-1 hash implementation of

- [SHA1](https://github.com/itzmeanjan/merklize-sha/blob/fd76b7a/example/sha1.cpp)
- [SHA2-224](https://github.com/itzmeanjan/merklize-sha/blob/fd76b7a/example/sha2_224.cpp)
- [SHA2-256](https://github.com/itzmeanjan/merklize-sha/blob/fd76b7a/example/sha2_256.cpp)
- [SHA2-384](https://github.com/itzmeanjan/merklize-sha/blob/fd76b7a/example/sha2_384.cpp)
- [SHA2-512](https://github.com/itzmeanjan/merklize-sha/blob/fd76b7a/example/sha2_512.cpp)
- [SHA2-512/224](https://github.com/itzmeanjan/merklize-sha/blob/fd76b7a/example/sha2_512_224.cpp)
- [SHA2-512/256](https://github.com/itzmeanjan/merklize-sha/blob/fd76b7a/example/sha2_512_256.cpp)
- [SHA3-224](https://github.com/itzmeanjan/merklize-sha/blob/8f9b168/example/sha3_224.cpp)
- [SHA3-256](https://github.com/itzmeanjan/merklize-sha/blob/8f9b168/example/sha3_256.cpp)
- [SHA3-384](https://github.com/itzmeanjan/merklize-sha/blob/8f9b168/example/sha3_384.cpp)
- [SHA3-512](https://github.com/itzmeanjan/merklize-sha/blob/8f9b168/example/sha3_512.cpp)

where two digests of respective hash functions are input, in byte concatenated form, to `hash( ... )` function, consider taking a look at above hyperlinked examples.

> Compile above examples using `dpcpp -fsycl example/<file>.cpp -I./include`

You will probably like to see how binary merklization kernels use these 2-to-1 hash functions; see [here](https://github.com/itzmeanjan/merklize-sha/blob/4aadd99/include/merklize.hpp)

## Tests

I've accompanied each hash function implementation along with binary merklization using them, with test cases which can be executed as

```bash
bash run.sh
```

## Benchmarks

For benchmarking binary merklization, I'm taking randomly generated N -many leaf nodes as input, which are explicitly transferred to accelerator's memory; computing all (N - 1) -many intermediate nodes; finally transferring them back to host memory. This flow is executed 8 times, before taking average of kernel execution/ host <-> device data tx time, for some N.

I'm keeping binary merklization benchmark results of

- SHA1
  - [Nvidia GPU(s)](results/sha1/nvidia_gpu.md)
  - [Intel CPU(s)](results/sha1/intel_cpu.md)
  - [Intel GPU(s)](results/sha1/intel_gpu.md)
- SHA2-224
  - [Nvidia GPU(s)](results/sha2-224/nvidia_gpu.md)
  - [Intel CPU(s)](results/sha2-224/intel_cpu.md)
  - [Intel GPU(s)](results/sha2-224/intel_gpu.md)
- SHA2-256
  - [Nvidia GPU(s)](results/sha2-256/nvidia_gpu.md)
  - [Intel CPU(s)](results/sha2-256/intel_cpu.md)
  - [Intel GPU(s)](results/sha2-256/intel_gpu.md)
- SHA2-384
  - [Nvidia GPU(s)](results/sha2-384/nvidia_gpu.md)
  - [Intel CPU(s)](results/sha2-384/intel_cpu.md)
  - [Intel GPU(s)](results/sha2-384/intel_gpu.md)
- SHA2-512
  - [Nvidia GPU(s)](results/sha2-512/nvidia_gpu.md)
  - [Intel CPU(s)](results/sha2-512/intel_cpu.md)
  - [Intel GPU(s)](results/sha2-512/intel_gpu.md)
- SHA2-512/224
  - [Nvidia GPU(s)](results/sha2-512-224/nvidia_gpu.md)
  - [Intel CPU(s)](results/sha2-512-224/intel_cpu.md)
  - [Intel GPU(s)](results/sha2-512-224/intel_gpu.md)
- SHA2-512/256
  - [Nvidia GPU(s)](results/sha2-512-256/nvidia_gpu.md)
  - [Intel CPU(s)](results/sha2-512-256/intel_cpu.md)
  - [Intel GPU(s)](results/sha2-512-256/intel_gpu.md)
- SHA3-256
  - [Nvidia GPU(s)](results/sha3-256/nvidia_gpu.md)
  - [Intel CPU(s)](results/sha3-256/intel_cpu.md)
  - [Intel GPU(s)](results/sha3-256/intel_gpu.md)
- SHA3-224
  - [Nvidia GPU(s)](results/sha3-224/nvidia_gpu.md)
  - [Intel CPU(s)](results/sha3-224/intel_cpu.md)
  - [Intel GPU(s)](results/sha3-224/intel_gpu.md)
- SHA3-384
  - [Nvidia GPU(s)](results/sha3-384/nvidia_gpu.md)
  - [Intel CPU(s)](results/sha3-384/intel_cpu.md)
  - [Intel GPU(s)](results/sha3-384/intel_gpu.md)
- SHA3-512
  - [Nvidia GPU(s)](results/sha3-512/nvidia_gpu.md)
  - [Intel CPU(s)](results/sha3-512/intel_cpu.md)
  - [Intel GPU(s)](results/sha3-512/intel_gpu.md)

obtained after executing them on multiple accelerators.
