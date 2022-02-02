### Binary Merklization using SHA3-256 on Intel CPU(s)

Compiling with

```bash
SHA=sha3_256 make aot_cpu
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


Benchmarking Binary Merklization using SHA3-256

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                   460.527613 ms                     3.378705 ms                     3.513406 ms
        2 ^ 21                   884.408269 ms                     6.608696 ms                     6.680540 ms
        2 ^ 22                      1.750542 s                    13.186952 ms                    13.351823 ms
        2 ^ 23                      3.498270 s                    27.775797 ms                    27.589655 ms
        2 ^ 24                      7.032438 s                    54.475082 ms                    54.506723 ms
        2 ^ 25                     14.041351 s                   107.947459 ms                   109.120553 ms
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


Benchmarking Binary Merklization using SHA3-256

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                    24.213955 ms                     2.099417 ms                     1.147591 ms
        2 ^ 21                    62.796379 ms                     3.296168 ms                     2.220697 ms
        2 ^ 22                    33.891272 ms                     6.672769 ms                     4.235582 ms
        2 ^ 23                    59.978552 ms                    12.807243 ms                     4.105317 ms
        2 ^ 24                   117.749699 ms                    17.167419 ms                     7.970099 ms
        2 ^ 25                   234.548106 ms                    25.114616 ms                    15.699562 ms
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


Benchmarking Binary Merklization using SHA3-256

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                    33.152645 ms                     1.682082 ms                   796.108500 us
        2 ^ 21                    65.376513 ms                     2.994367 ms                     1.589848 ms
        2 ^ 22                   117.765811 ms                     5.919532 ms                    11.558829 ms
        2 ^ 23                   231.303439 ms                     9.850140 ms                    15.033037 ms
        2 ^ 24                   461.649286 ms                    25.151985 ms                    12.094869 ms
        2 ^ 25                   922.339764 ms                    34.289637 ms                    26.320650 ms
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


Benchmarking Binary Merklization using SHA3-256

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                    26.406774 ms                     1.258974 ms                   935.372625 us
        2 ^ 21                    52.468677 ms                     2.075004 ms                     2.027945 ms
        2 ^ 22                   104.464652 ms                     4.209483 ms                     4.136641 ms
        2 ^ 23                   205.639685 ms                     8.374299 ms                     8.174240 ms
        2 ^ 24                   409.958382 ms                    16.108984 ms                    15.956122 ms
        2 ^ 25                   818.950744 ms                    32.119388 ms                    31.103533 ms
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


Benchmarking Binary Merklization using SHA3-256

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                    62.844377 ms                   912.211875 us                   841.460250 us
        2 ^ 21                   117.112114 ms                     1.771213 ms                     1.703229 ms
        2 ^ 22                   231.993665 ms                     3.471314 ms                     3.412241 ms
        2 ^ 23                   463.345803 ms                     6.896977 ms                     8.071636 ms
        2 ^ 24                   925.487535 ms                    14.186806 ms                    13.770779 ms
        2 ^ 25                      2.019036 s                    27.535975 ms                    27.752319 ms
```
