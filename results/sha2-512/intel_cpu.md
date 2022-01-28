### Binary Merklization using SHA2-512 on Intel CPU(s)

Compiling with

```bash
SHA=sha2_512 make aot_cpu
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


Benchmarking Binary Merklization using SHA2-512

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                    67.144504 ms                     6.714332 ms                     6.728427 ms
        2 ^ 21                   134.738256 ms                    13.286070 ms                    13.617427 ms
        2 ^ 22                   267.064921 ms                    27.441172 ms                    27.270495 ms
        2 ^ 23                   533.060150 ms                    54.278747 ms                    54.534778 ms
        2 ^ 24                      1.069998 s                   108.441495 ms                   110.707765 ms
        2 ^ 25                      2.130898 s                   245.418493 ms                   222.972335 ms
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


Benchmarking Binary Merklization using SHA2-512

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     2.995395 ms                     3.744705 ms                     2.715633 ms
        2 ^ 21                     3.742123 ms                     6.554994 ms                     3.069671 ms
        2 ^ 22                     4.505393 ms                    11.223970 ms                     4.752674 ms
        2 ^ 23                     7.122330 ms                    16.716258 ms                     8.287495 ms
        2 ^ 24                    13.824964 ms                    25.307137 ms                    16.361083 ms
        2 ^ 25                    27.244398 ms                    40.844700 ms                    32.690295 ms
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


Benchmarking Binary Merklization using SHA2-512

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     7.299252 ms                     6.475906 ms                     1.740733 ms
        2 ^ 21                     9.060641 ms                     5.778054 ms                     3.121193 ms
        2 ^ 22                    15.531022 ms                    10.750715 ms                     6.077307 ms
        2 ^ 23                    30.065469 ms                    16.544751 ms                    11.839282 ms
        2 ^ 24                    67.052003 ms                    28.448928 ms                    37.608156 ms
        2 ^ 25                   119.537976 ms                    51.117129 ms                    47.492178 ms
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


Benchmarking Binary Merklization using SHA2-512

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     7.154580 ms                     2.081263 ms                     1.983975 ms
        2 ^ 21                     9.599392 ms                     4.089008 ms                     3.959287 ms
        2 ^ 22                    16.340005 ms                     8.065150 ms                     7.945405 ms
        2 ^ 23                    30.580216 ms                    16.228159 ms                    15.960153 ms
        2 ^ 24                    61.029389 ms                    31.553414 ms                    31.107359 ms
        2 ^ 25                   125.061850 ms                    62.904980 ms                    62.254563 ms
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


Benchmarking Binary Merklization using SHA2-512

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                    14.571336 ms                     1.851269 ms                     1.854122 ms
        2 ^ 21                    25.145573 ms                     3.493801 ms                     3.463226 ms
        2 ^ 22                    50.266274 ms                     6.918788 ms                     6.874712 ms
        2 ^ 23                   100.707850 ms                    13.786173 ms                    13.763155 ms
        2 ^ 24                   201.647746 ms                    27.555017 ms                    27.498056 ms
        2 ^ 25                   404.422047 ms                    55.035208 ms                    54.967606 ms
```
