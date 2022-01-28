### Binary Merklization using SHA2-512/256 on Intel CPU(s)

Compiling with

```bash
SHA=sha2_512_256 make aot_cpu
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


Benchmarking Binary Merklization using SHA2-512/256

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                    32.550505 ms                     3.391010 ms                     3.527880 ms
        2 ^ 21                    64.754806 ms                     6.713047 ms                     7.122987 ms
        2 ^ 22                   129.327147 ms                    13.326640 ms                    17.007366 ms
        2 ^ 23                   259.070470 ms                    27.875930 ms                    27.398992 ms
        2 ^ 24                   516.821455 ms                    54.844493 ms                    54.471450 ms
        2 ^ 25                      1.039893 s                   108.268683 ms                   108.877984 ms
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


Benchmarking Binary Merklization using SHA2-512/256

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     1.940187 ms                     1.726368 ms                     1.729807 ms
        2 ^ 21                     2.799681 ms                     3.809619 ms                     2.703719 ms
        2 ^ 22                    37.716472 ms                     6.697562 ms                     4.615761 ms
        2 ^ 23                     3.868198 ms                    23.008360 ms                     4.461642 ms
        2 ^ 24                     6.837876 ms                    17.998318 ms                     8.785985 ms
        2 ^ 25                    13.489539 ms                    26.339575 ms                    17.360737 ms
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


Benchmarking Binary Merklization using SHA2-512/256

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     3.832856 ms                     1.338769 ms                     1.337157 ms
        2 ^ 21                     6.330743 ms                     7.831780 ms                     1.714398 ms
        2 ^ 22                     8.403662 ms                     5.851787 ms                     2.959346 ms
        2 ^ 23                    13.416846 ms                    10.474141 ms                     5.792183 ms
        2 ^ 24                    25.737139 ms                    15.827991 ms                    11.485797 ms
        2 ^ 25                    51.201072 ms                    27.656506 ms                    23.238323 ms
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


Benchmarking Binary Merklization using SHA2-512/256

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     3.588424 ms                     1.081563 ms                     1.103520 ms
        2 ^ 21                     5.187772 ms                     2.180012 ms                     1.988097 ms
        2 ^ 22                     8.943696 ms                     4.186564 ms                     3.982926 ms
        2 ^ 23                    13.902680 ms                     8.082833 ms                     7.955179 ms
        2 ^ 24                    26.567515 ms                    16.327046 ms                    15.953391 ms
        2 ^ 25                    52.883095 ms                    31.293517 ms                    31.179416 ms
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


Benchmarking Binary Merklization using SHA2-512/256

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     6.251742 ms                   894.205125 us                   880.238250 us
        2 ^ 21                    16.553353 ms                     1.782046 ms                     1.792627 ms
        2 ^ 22                    24.759339 ms                     3.494736 ms                     3.425213 ms
        2 ^ 23                    49.406884 ms                     6.927048 ms                     6.878854 ms
        2 ^ 24                    98.813316 ms                    13.797835 ms                    13.765457 ms
        2 ^ 25                   197.189056 ms                    27.542192 ms                    27.479188 ms
```
