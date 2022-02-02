### Binary Merklization using SHA3-224 on Intel CPU(s)

Compiling with

```bash
SHA=sha3_224 make aot_cpu
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


Benchmarking Binary Merklization using SHA3-224

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                   441.313146 ms                     2.828693 ms                     3.067447 ms
        2 ^ 21                   881.435162 ms                     5.918054 ms                     6.043629 ms
        2 ^ 22                      1.759670 s                    11.524675 ms                    11.689306 ms
        2 ^ 23                      3.525266 s                    24.145784 ms                    24.199104 ms
        2 ^ 24                      7.043599 s                    47.596686 ms                    47.643860 ms
        2 ^ 25                     14.165549 s                    95.153863 ms                  
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


Benchmarking Binary Merklization using SHA3-224

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                    13.079599 ms                     1.354843 ms                     1.179845 ms
        2 ^ 21                    20.786925 ms                     3.279467 ms                     1.921304 ms
        2 ^ 22                    33.636769 ms                     5.938975 ms                     3.739403 ms
        2 ^ 23                    60.527577 ms                    10.275394 ms                     3.634558 ms
        2 ^ 24                   116.507279 ms                    16.135763 ms                     7.115856 ms
        2 ^ 25                   231.727947 ms                    23.140918 ms                    13.985500 ms
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


Benchmarking Binary Merklization using SHA3-224

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                    30.821758 ms                     1.291234 ms                   710.533125 us
        2 ^ 21                    60.974097 ms                     2.444916 ms                     1.493359 ms
        2 ^ 22                   117.379986 ms                     4.960545 ms                     2.851427 ms
        2 ^ 23                   230.460750 ms                     8.972397 ms                     5.574060 ms
        2 ^ 24                   458.509651 ms                    15.732812 ms                    11.348903 ms
        2 ^ 25                   938.098973 ms                    54.020292 ms                    23.115493 ms
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


Benchmarking Binary Merklization using SHA3-224

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                    26.285759 ms                   832.090875 us                   795.686250 us
        2 ^ 21                    52.246320 ms                     1.766923 ms                     1.696765 ms
        2 ^ 22                   105.391437 ms                     3.529938 ms                     3.430882 ms
        2 ^ 23                   205.738750 ms                     7.184241 ms                     7.062021 ms
        2 ^ 24                   410.537958 ms                    14.054861 ms                    13.946278 ms
        2 ^ 25                   819.288981 ms                    27.466327 ms                    27.053277 ms
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


Benchmarking Binary Merklization using SHA3-224

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                    69.412657 ms                   795.589500 us                   727.804125 us
        2 ^ 21                   115.531715 ms                     1.561098 ms                     1.514993 ms
        2 ^ 22                   229.142733 ms                     3.169098 ms                     3.031581 ms
        2 ^ 23                   458.677792 ms                     6.097932 ms                     6.134698 ms
        2 ^ 24                   915.077926 ms                    12.123231 ms                    12.139003 ms
        2 ^ 25                      2.003310 s                    24.247329 ms                    24.197952 ms
```
