### Binary Merklization using KECCAK-256 on Intel CPU(s)

Compiling with

```bash
SHA=keccak_256 make aot_cpu
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


Benchmarking Binary Merklization using KECCAK-256

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                   466.478477 ms                     3.288778 ms                     3.442020 ms
        2 ^ 21                   898.963977 ms                     6.508914 ms                     6.558546 ms
        2 ^ 22                      1.797621 s                    13.061319 ms                    13.172746 ms
        2 ^ 23                      3.591501 s                    27.324937 ms                    27.123078 ms
        2 ^ 24                      7.186666 s                    54.148528 ms                    54.237210 ms
        2 ^ 25                     14.404052 s                   123.865217 ms                   108.246855 ms
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


Benchmarking Binary Merklization using KECCAK-256

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                    13.362355 ms                     1.821476 ms                     1.326708 ms
        2 ^ 21                    20.922397 ms                     3.589614 ms                     2.430955 ms
        2 ^ 22                    33.674320 ms                     6.493885 ms                     4.294246 ms
        2 ^ 23                   106.859444 ms                    11.947260 ms                     8.593155 ms
        2 ^ 24                   117.165222 ms                    23.851139 ms                     8.417020 ms
        2 ^ 25                   233.647003 ms                    25.051263 ms                    16.673447 ms
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


Benchmarking Binary Merklization using KECCAK-256

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                    34.571529 ms                     1.809763 ms                   897.616875 us
        2 ^ 21                    61.404680 ms                     3.326612 ms                     1.588368 ms
        2 ^ 22                   117.968746 ms                     5.674248 ms                     7.157974 ms
        2 ^ 23                   231.852088 ms                     9.238144 ms                    13.273680 ms
        2 ^ 24                   462.241001 ms                    20.315251 ms                    12.602417 ms
        2 ^ 25                   924.972606 ms                    31.446401 ms                    24.707977 ms
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


Benchmarking Binary Merklization using KECCAK-256

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                    73.894415 ms                   932.138625 us                   850.445250 us
        2 ^ 21                   109.423621 ms                     1.782943 ms                     1.715456 ms
        2 ^ 22                   218.244072 ms                     3.493360 ms                     3.446031 ms
        2 ^ 23                   436.918616 ms                     6.905427 ms                     6.842661 ms
        2 ^ 24                   883.594877 ms                    13.812258 ms                    13.749230 ms
        2 ^ 25                      1.930962 s                    27.554382 ms                    27.591307 ms
```
