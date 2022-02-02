### Binary Merklization using SHA3-512 on Intel CPU(s)

Compiling with

```bash
SHA=sha3_512 make aot_cpu
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


Benchmarking Binary Merklization using SHA3-512

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                      1.422096 s                     6.655711 ms                     6.632538 ms
        2 ^ 21                      2.826801 s                    13.003300 ms                    13.222868 ms
        2 ^ 22                      5.655653 s                    27.421870 ms                    27.016023 ms
        2 ^ 23                     11.310605 s                    53.933905 ms                    54.153294 ms
        2 ^ 24                     23.166480 s                   107.621708 ms                   108.141864 ms
        2 ^ 25                     46.915646 s                   215.222096 ms                   223.990513 ms
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


Benchmarking Binary Merklization using SHA3-512

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                    18.212837 ms                     3.521151 ms                     1.154891 ms
        2 ^ 21                    31.598131 ms                     8.554403 ms                     4.468314 ms
        2 ^ 22                    57.098150 ms                    13.102806 ms                     4.367535 ms
        2 ^ 23                   112.831001 ms                    17.162144 ms                     8.644232 ms
        2 ^ 24                   222.342946 ms                    25.501501 ms                    17.274517 ms
        2 ^ 25                   442.875605 ms                    51.969265 ms                    33.731850 ms
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


Benchmarking Binary Merklization using SHA3-512

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                    83.168826 ms                     3.148146 ms                     1.616418 ms
        2 ^ 21                   118.585412 ms                     5.744802 ms                     3.080455 ms
        2 ^ 22                   232.349992 ms                    10.116922 ms                     6.071552 ms
        2 ^ 23                   463.172213 ms                    17.845495 ms                    12.323518 ms
        2 ^ 24                   924.780638 ms                    35.928981 ms                    26.567100 ms
        2 ^ 25                      1.853150 s                    92.435423 ms                    54.594908 ms
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


Benchmarking Binary Merklization using SHA3-512

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                    55.401972 ms                     2.028286 ms                     1.841675 ms
        2 ^ 21                   104.913798 ms                     4.298537 ms                     3.958525 ms
        2 ^ 22                   204.813210 ms                     8.176016 ms                     7.942972 ms
        2 ^ 23                   407.979027 ms                    16.235003 ms                    15.930501 ms
        2 ^ 24                   815.037735 ms                    31.475015 ms                    31.085153 ms
        2 ^ 25                      1.644981 s                    62.561881 ms                    62.212671 ms
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


Benchmarking Binary Merklization using SHA3-512

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                   203.357929 ms                     1.772444 ms                     1.711588 ms
        2 ^ 21                   405.166609 ms                     3.473879 ms                     3.392248 ms
        2 ^ 22                   809.711764 ms                     6.936534 ms                     6.848052 ms
        2 ^ 23                      1.735337 s                    13.794764 ms                    13.937174 ms
        2 ^ 24                      3.465506 s                    27.510037 ms                    27.550688 ms
        2 ^ 25                      6.932260 s                    55.162014 ms                    54.967118 ms
```
