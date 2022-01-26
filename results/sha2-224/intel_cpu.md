### Binary Merklization using SHA2-224 on Intel CPU(s)

Compiling with

```bash
SHA=sha2_224 make aot_cpu
```

### On `Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz`

```bash
$ lscpu | grep -i cpu\(s\)

CPU(s):                          4
On-line CPU(s) list:             0-3
NUMA node0 CPU(s):               0-3
```

```bash
running on Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz


Benchmarking Binary Merklization using SHA2-224

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                    32.951925 ms                     2.759520 ms                     2.814926 ms
        2 ^ 21                    64.516266 ms                     5.788733 ms                     5.968292 ms
        2 ^ 22                   128.407755 ms                    11.307367 ms                    11.437529 ms
        2 ^ 23                   256.624889 ms                    23.686885 ms                    23.621088 ms
        2 ^ 24                   513.835194 ms                    47.147656 ms                    46.956709 ms
        2 ^ 25                      1.028178 s                    93.797258 ms                    93.685513 ms
```

### On `Intel(R) Xeon(R) Platinum 8358 CPU @ 2.60GHz`

```bash
$ lscpu | grep -i cpu\(s\)

CPU(s):                          128
On-line CPU(s) list:             0-127
NUMA node0 CPU(s):               0-31,64-95
NUMA node1 CPU(s):               32-63,96-127
```

```bash
running on Intel(R) Xeon(R) Platinum 8358 CPU @ 2.60GHz


Benchmarking Binary Merklization using SHA2-224

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     1.965417 ms                     1.544897 ms                     1.355293 ms
        2 ^ 21                     3.172233 ms                     3.031929 ms                     3.537930 ms
        2 ^ 22                     4.156599 ms                     5.563559 ms                     3.975025 ms
        2 ^ 23                     5.361394 ms                     9.679101 ms                     3.940944 ms
        2 ^ 24                     6.778410 ms                    16.225821 ms                     6.837070 ms
        2 ^ 25                    13.008825 ms                    22.569791 ms                    13.593592 ms
```

### On `Intel(R) Xeon(R) Gold 6128 CPU @ 3.40GHz`

```bash
$ lscpu | grep -i cpu\(s\)

CPU(s):                          24
On-line CPU(s) list:             0-23
NUMA node0 CPU(s):               0-5,12-17
NUMA node1 CPU(s):               6-11,18-23
```

```bash
running on Intel(R) Xeon(R) Gold 6128 CPU @ 3.40GHz


Benchmarking Binary Merklization using SHA2-224

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     4.385677 ms                     1.306831 ms                     1.156641 ms
        2 ^ 21                     6.390041 ms                     2.952304 ms                     1.459918 ms
        2 ^ 22                     9.426814 ms                     5.543553 ms                     2.713314 ms
        2 ^ 23                    14.413681 ms                     8.516656 ms                     5.229903 ms
        2 ^ 24                    27.101954 ms                    14.686123 ms                    20.741114 ms
        2 ^ 25                    53.921158 ms                    29.347154 ms                    20.799277 ms
```

### On `Intel(R) Core(TM) i9-10920X CPU @ 3.50GHz`

```bash
$ lscpu | grep -i cpu\(s\)

CPU(s):                          24
On-line CPU(s) list:             0-23
NUMA node0 CPU(s):               0-23
```

```bash
running on Intel(R) Core(TM) i9-10920X CPU @ 3.50GHz


Benchmarking Binary Merklization using SHA2-224

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     3.185755 ms                   826.565625 us                   879.207750 us
        2 ^ 21                     5.637165 ms                     2.280328 ms                     1.707118 ms
        2 ^ 22                     8.636681 ms                     3.531792 ms                     3.427982 ms
        2 ^ 23                    14.285976 ms                     7.091349 ms                     7.063932 ms
        2 ^ 24                    26.902393 ms                    14.187718 ms                    13.970556 ms
        2 ^ 25                    53.573530 ms                    27.341058 ms                    27.141584 ms
```

### On `Intel(R) Xeon(R) E-2176G CPU @ 3.70GHz`

```bash
$ lscpu | grep -i cpu\(s\)

CPU(s):                          12
On-line CPU(s) list:             0-11
NUMA node0 CPU(s):               0-11
```

```bash
running on Intel(R) Xeon(R) E-2176G CPU @ 3.70GHz


Benchmarking Binary Merklization using SHA2-224

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     6.343735 ms                   793.691250 us                   758.298750 us
        2 ^ 21                    13.125360 ms                     1.584928 ms                     1.535259 ms
        2 ^ 22                    23.618047 ms                     3.097280 ms                     3.064426 ms
        2 ^ 23                    47.116797 ms                     6.069967 ms                     6.034023 ms
        2 ^ 24                    94.138271 ms                    12.137357 ms                    12.170783 ms
        2 ^ 25                   188.112406 ms                    24.107783 ms                    24.067320 ms
```
