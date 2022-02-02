### Binary Merklization using SHA3-384 on Intel CPU(s)

Compiling with

```bash
SHA=sha3_384 make aot_cpu
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


Benchmarking Binary Merklization using SHA3-384

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                   121.452534 ms                     5.250688 ms                     5.114491 ms
        2 ^ 21                   242.495514 ms                     9.878091 ms                    10.002006 ms
        2 ^ 22                   487.446581 ms                    20.749688 ms                    20.733140 ms
        2 ^ 23                   968.630066 ms                    41.098545 ms                    40.908267 ms
        2 ^ 24                      1.936018 s                    81.650025 ms                    81.427060 ms
        2 ^ 25                      3.877924 s                   163.935222 ms                   164.065617 ms
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


Benchmarking Binary Merklization using SHA3-384

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     5.484831 ms                     1.799175 ms                     2.268806 ms
        2 ^ 21                     6.861796 ms                     6.618026 ms                     1.753305 ms
        2 ^ 22                     9.038283 ms                    10.856676 ms                     3.195043 ms
        2 ^ 23                    15.615609 ms                    14.961957 ms                     6.015195 ms
        2 ^ 24                    30.814614 ms                    21.000240 ms                    11.727313 ms
        2 ^ 25                    61.229065 ms                    39.717597 ms                    23.346449 m
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


Benchmarking Binary Merklization using SHA3-384

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                    10.222366 ms                     2.462541 ms                     1.604796 ms
        2 ^ 21                    15.848323 ms                     4.915875 ms                     2.600809 ms
        2 ^ 22                    27.781493 ms                     8.892203 ms                     4.878789 ms
        2 ^ 23                    54.174604 ms                    13.766896 ms                     9.307206 ms
        2 ^ 24                   107.784253 ms                    36.425805 ms                    19.862340 ms
        2 ^ 25                   215.203138 ms                    68.672697 ms                    42.242766 ms
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


Benchmarking Binary Merklization using SHA3-384

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     9.466639 ms                     1.528539 ms                     1.410706 ms
        2 ^ 21                    14.855669 ms                     3.009892 ms                     2.942806 ms
        2 ^ 22                    27.278380 ms                     7.817085 ms                     5.957917 ms
        2 ^ 23                    53.826366 ms                    12.126587 ms                    11.938260 ms
        2 ^ 24                   107.146426 ms                    24.175173 ms                    23.971688 ms
        2 ^ 25                   213.836448 ms                    47.514487 ms                    46.691987 ms
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


Benchmarking Binary Merklization using SHA3-384

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                    21.584894 ms                     1.336357 ms                     1.312938 ms
        2 ^ 21                    43.433253 ms                     2.638164 ms                     2.592325 ms
        2 ^ 22                    86.769447 ms                     5.188149 ms                     5.138226 ms
        2 ^ 23                   170.747419 ms                    10.330659 ms                    10.343127 ms
        2 ^ 24                   340.255293 ms                    20.730265 ms                    20.627062 ms
        2 ^ 25                   680.419022 ms                    41.408275 ms                    41.222121 ms
```
