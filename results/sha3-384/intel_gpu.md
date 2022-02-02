### Binary Merklization using SHA3-384 on Intel GPU(s)

Compiling with

```bash
SHA=sha3_384 make aot_gpu
```

### On `Intel(R) Iris(R) Xe MAX Graphics [0x4905]`

```bash
running on Intel(R) Iris(R) Xe MAX Graphics [0x4905]


Benchmarking Binary Merklization using SHA3-384

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                    26.168610 ms                     4.986228 ms                     3.323404 ms
        2 ^ 21                    50.483082 ms                     9.982226 ms                     6.647920 ms
        2 ^ 22                    98.915973 ms                    19.956007 ms                    13.281021 ms
        2 ^ 23                   195.430248 ms                    39.864279 ms                    26.563836 ms
        2 ^ 24                   388.282635 ms                    79.700069 ms                    53.124162 ms
        2 ^ 25                   773.846658 ms                   159.363769 ms                   106.251229 ms
```

### On `Intel(R) UHD Graphics P630 [0x3e96]`

```bash
running on Intel(R) UHD Graphics P630 [0x3e96]


Benchmarking Binary Merklization using SHA3-384

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                   113.338949 ms                     1.135689 ms                     1.077983 ms
        2 ^ 21                   224.178310 ms                     4.001492 ms                     2.097452 ms
        2 ^ 22                   445.379515 ms                     4.231070 ms                     4.163218 ms
        2 ^ 23                   888.437042 ms                     8.346355 ms                     8.339944 ms
        2 ^ 24                      1.773451 s                    16.782476 ms                    16.473902 ms
        2 ^ 25                      3.544434 s                    32.968222 ms                    33.054750 ms
```
