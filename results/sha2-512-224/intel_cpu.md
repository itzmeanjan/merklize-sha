### Binary Merklization using SHA2-512/224 on Intel CPU(s)

Compiling with

```bash
SHA=sha2_512_224 make aot_cpu
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

Benchmarking Binary Merklization using SHA2-512/224

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                    32.802526 ms                     3.049436 ms                     3.587411 ms
        2 ^ 21                    65.077378 ms                     6.105495 ms                     6.906860 ms
        2 ^ 22                   129.909475 ms                    11.763465 ms                    17.199709 ms
        2 ^ 23                   259.322664 ms                    24.619776 ms                    27.747516 ms
        2 ^ 24                   518.045701 ms                    48.316185 ms                    59.579793 ms
        2 ^ 25                      1.058124 s                    96.240897 ms                   115.631602 ms
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


Benchmarking Binary Merklization using SHA2-512/224

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     1.824125 ms                     1.359173 ms                     1.928767 ms
        2 ^ 21                     3.536511 ms                     3.326957 ms                     2.816833 ms
        2 ^ 22                     3.894740 ms                     5.996881 ms                     4.611470 ms
        2 ^ 23                     4.116974 ms                    12.254433 ms                     4.470250 ms
        2 ^ 24                     6.895942 ms                    16.815154 ms                     8.770072 ms
        2 ^ 25                    13.387531 ms                    24.421875 ms                    17.465137 ms
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


Benchmarking Binary Merklization using SHA2-512/224

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     3.801376 ms                     1.380699 ms                     1.230819 ms
        2 ^ 21                     6.034466 ms                     6.867659 ms                     1.715430 ms
        2 ^ 22                     8.480832 ms                     5.638521 ms                     3.123983 ms
        2 ^ 23                    13.301023 ms                     9.565064 ms                     6.179770 ms
        2 ^ 24                    25.738599 ms                    16.056100 ms                    12.193623 ms
        2 ^ 25                    68.062345 ms                    26.567764 ms                    25.880542 ms
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


Benchmarking Binary Merklization using SHA2-512/224

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     2.881055 ms                   874.710000 us                   974.791875 us
        2 ^ 21                     5.537288 ms                     2.067712 ms                     2.021851 ms
        2 ^ 22                     8.581146 ms                     4.243155 ms                     4.060824 ms
        2 ^ 23                    14.365893 ms                     7.135209 ms                     7.958026 ms
        2 ^ 24                    26.198792 ms                    14.044978 ms                    15.968666 ms
        2 ^ 25                    52.180593 ms                    27.378270 ms                    31.127614 ms
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


Benchmarking Binary Merklization using SHA2-512/224

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     6.126055 ms                   802.471875 us                   887.598000 us
        2 ^ 21                    13.342621 ms                     1.853781 ms                     1.756606 ms
        2 ^ 22                    24.220928 ms                     3.072485 ms                     3.456673 ms
        2 ^ 23                    48.364883 ms                     6.058199 ms                     6.872956 ms
        2 ^ 24                    96.574321 ms                    12.123167 ms                    13.770336 ms
        2 ^ 25                   193.093372 ms                    24.089391 ms                    27.506557 ms
```
