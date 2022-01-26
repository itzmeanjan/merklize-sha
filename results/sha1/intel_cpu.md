### Binary Merklization using SHA1 on Intel CPU(s)

Compiling with

```bash
SHA=sha1 make aot_cpu
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


Benchmarking Binary Merklization using SHA1

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     8.694518 ms                     1.949840 ms                     2.453224 ms
        2 ^ 21                    17.199786 ms                     4.198702 ms                     4.340316 ms
        2 ^ 22                    34.146192 ms                     8.113762 ms                     8.214299 ms
        2 ^ 23                    68.026877 ms                    17.017908 ms                    17.023445 ms
        2 ^ 24                   135.676135 ms                    33.659895 ms                    33.800263 ms
        2 ^ 25                   270.682205 ms                    67.579901 ms                    67.553929 ms
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


Benchmarking Binary Merklization using SHA1

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                   973.235250 us                     1.393460 ms                     1.373282 ms
        2 ^ 21                     1.649131 ms                     2.640978 ms                     2.139842 ms
        2 ^ 22                     2.639893 ms                     4.651534 ms                     3.276608 ms
        2 ^ 23                     2.983180 ms                     7.928330 ms                     3.842820 ms
        2 ^ 24                     3.972229 ms                    13.865614 ms                     6.017920 ms
        2 ^ 25                     5.865988 ms                    19.541160 ms                    10.827998 ms
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


Benchmarking Binary Merklization using SHA1

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     1.669263 ms                   958.988625 us                   990.652125 us
        2 ^ 21                     3.128500 ms                     2.164344 ms                     1.608114 ms
        2 ^ 22                     4.687359 ms                     3.720170 ms                     2.240008 ms
        2 ^ 23                     6.282229 ms                     7.263596 ms                    11.001944 ms
        2 ^ 24                    10.525181 ms                    16.860136 ms                     7.371937 ms
        2 ^ 25                    38.582802 ms                    19.645725 ms                    14.925842 m
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


Benchmarking Binary Merklization using SHA1

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     1.733615 ms                   814.208625 us                   860.456625 us
        2 ^ 21                     2.803117 ms                     1.773713 ms                     1.572063 ms
        2 ^ 22                     4.529533 ms                     3.151578 ms                     2.481437 ms
        2 ^ 23                     7.846518 ms                     5.298239 ms                     4.982880 ms
        2 ^ 24                    15.381177 ms                    10.111674 ms                     9.931379 ms
        2 ^ 25                    30.979469 ms                    19.558067 ms                    19.396226 ms
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


Benchmarking Binary Merklization using SHA1

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     1.838045 ms                   589.628625 us                   567.175125 us
        2 ^ 21                     3.318120 ms                     1.128462 ms                     1.144666 ms
        2 ^ 22                     6.561936 ms                     2.217953 ms                     2.187012 ms
        2 ^ 23                    13.050308 ms                     4.362224 ms                     4.311594 ms
        2 ^ 24                    26.043846 ms                     8.694190 ms                     8.636585 ms
        2 ^ 25                    51.990795 ms                    17.235775 ms                    17.202572 ms
```
