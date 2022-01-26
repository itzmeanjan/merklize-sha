### Binary Merklization using SHA2-256 on Intel CPU(s)

Compiling with

```bash
SHA=sha2_256 make aot_cpu
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


Benchmarking Binary Merklization using SHA2-256

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                    31.548119 ms                     3.278957 ms                     3.405930 ms
        2 ^ 21                    74.383953 ms                     6.448621 ms                     6.583408 ms
        2 ^ 22                   125.460220 ms                    12.921625 ms                    13.118602 ms
        2 ^ 23                   251.510298 ms                    27.017091 ms                    26.885037 ms
        2 ^ 24                   517.607107 ms                    54.093915 ms                    53.885593 ms
        2 ^ 25                      1.003261 s                   108.179536 ms                   107.411605 ms
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


Benchmarking Binary Merklization using SHA2-256

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     1.932850 ms                     1.724421 ms                     1.783388 ms
        2 ^ 21                     2.873556 ms                     3.809529 ms                     2.683071 ms
        2 ^ 22                     3.807557 ms                     7.039419 ms                     4.697493 ms
        2 ^ 23                     3.833782 ms                    12.987659 ms                     4.747487 ms
        2 ^ 24                     7.031215 ms                    17.484357 ms                     8.856618 ms
        2 ^ 25                    13.411188 ms                    26.905162 ms                    17.626729 ms
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


Benchmarking Binary Merklization using SHA2-256

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     4.279716 ms                     1.507160 ms                     1.260632 ms
        2 ^ 21                     6.346261 ms                     3.351768 ms                     1.676602 ms
        2 ^ 22                     9.212702 ms                     5.802055 ms                     2.976046 ms
        2 ^ 23                    14.617657 ms                     9.280188 ms                     5.821066 ms
        2 ^ 24                    46.915648 ms                    15.775025 ms                    11.660212 ms
        2 ^ 25                    55.808860 ms                    27.940748 ms                    23.630878 ms
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


Benchmarking Binary Merklization using SHA2-256

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     3.345548 ms                     1.012641 ms                     1.034661 ms
        2 ^ 21                     6.304066 ms                     2.055579 ms                     1.951553 ms
        2 ^ 22                     9.097706 ms                     4.116220 ms                     3.959602 ms
        2 ^ 23                    14.797761 ms                     8.225529 ms                     7.940669 ms
        2 ^ 24                    28.128955 ms                    16.154102 ms                    16.046252 ms
        2 ^ 25                    56.248897 ms                    31.414169 ms                    31.100541 ms
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


Benchmarking Binary Merklization using SHA2-256

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     5.964265 ms                   916.032750 us                   880.749375 us
        2 ^ 21                    11.833022 ms                     1.835574 ms                     1.758230 ms
        2 ^ 22                    23.575996 ms                     3.509018 ms                     3.454184 ms
        2 ^ 23                    46.975276 ms                     6.929970 ms                     6.879241 ms
        2 ^ 24                    93.821823 ms                    13.806551 ms                    13.762214 ms
        2 ^ 25                   187.466457 ms                    38.789720 ms                    27.488194 ms
```
