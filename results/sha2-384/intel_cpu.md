### Binary Merklization using SHA2-384 on Intel CPU(s)

Compiling with

```bash
SHA=sha2_384 make aot_cpu
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


Benchmarking Binary Merklization using SHA2-384

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                    34.178252 ms                     5.218289 ms                     5.176465 ms
        2 ^ 21                    67.694605 ms                     9.876354 ms                     9.956149 ms
        2 ^ 22                   135.818527 ms                    20.974393 ms                    20.623947 ms
        2 ^ 23                   269.923540 ms                    41.135928 ms                    41.253850 ms
        2 ^ 24                   539.628880 ms                    81.100490 ms                    81.234604 ms
        2 ^ 25                      1.076815 s                   162.093054 ms                   170.061430 ms
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


Benchmarking Binary Merklization using SHA2-384

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     2.041907 ms                     2.473095 ms                     2.159533 ms
        2 ^ 21                     2.939270 ms                     5.150314 ms                     3.615391 ms
        2 ^ 22                     3.365169 ms                     8.887749 ms                     3.851239 ms
        2 ^ 23                     4.071329 ms                    14.324398 ms                     6.099382 ms
        2 ^ 24                     7.771932 ms                    20.828297 ms                    11.840717 ms
        2 ^ 25                    15.053307 ms                    32.482493 ms                    23.908711 ms
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


Benchmarking Binary Merklization using SHA2-384

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     4.064986 ms                     2.310313 ms                     1.988848 ms
        2 ^ 21                     5.825557 ms                     5.045159 ms                     2.566375 ms
        2 ^ 22                     8.369808 ms                     7.572489 ms                     4.657053 ms
        2 ^ 23                    14.607859 ms                    13.775337 ms                     9.238311 ms
        2 ^ 24                    29.402728 ms                    22.677070 ms                    18.767093 ms
        2 ^ 25                    58.258111 ms                    50.695223 ms                    37.394962 ms
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


Benchmarking Binary Merklization using SHA2-384

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     3.254170 ms                     1.595552 ms                     1.494076 ms
        2 ^ 21                     6.101297 ms                     3.465971 ms                     3.164907 ms
        2 ^ 22                     9.464844 ms                     6.263030 ms                     6.078313 ms
        2 ^ 23                    17.985553 ms                    12.084928 ms                    11.970836 ms
        2 ^ 24                    35.754916 ms                    24.186201 ms                    24.326572 ms
        2 ^ 25                    71.200293 ms                    47.340005 ms                    46.669111 ms
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


Benchmarking Binary Merklization using SHA2-384

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     9.374931 ms                     1.534824 ms                     1.399091 ms
        2 ^ 21                    12.748752 ms                     2.719041 ms                     2.688041 ms
        2 ^ 22                    25.307536 ms                     5.312269 ms                     5.241426 ms
        2 ^ 23                    50.210153 ms                    11.913176 ms                    10.421805 ms
        2 ^ 24                   100.164710 ms                    20.851078 ms                    20.712672 ms
        2 ^ 25                   201.872463 ms                    41.370812 ms                    41.488984 m
```
