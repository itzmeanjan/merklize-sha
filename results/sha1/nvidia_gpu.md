### Binary Merklization using SHA1 on Nvidia GPU(s)

Compiling with

```bash
SHA=sha1 make cuda
```

### On `Tesla V100-SXM2-16GB`

```bash
running on Tesla V100-SXM2-16GB


Benchmarking Binary Merklization using SHA1

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                   371.222625 us                   735.851625 us                   629.087250 us
        2 ^ 21                   710.562750 us                     1.451122 ms                     1.256813 ms
        2 ^ 22                     1.387207 ms                     2.877502 ms                     2.517654 ms
        2 ^ 23                     2.737060 ms                     5.740448 ms                     5.021942 ms
        2 ^ 24                     5.438049 ms                    11.454712 ms                    10.039673 ms
        2 ^ 25                    10.905762 ms                    22.880127 ms                    20.086304 ms
```
