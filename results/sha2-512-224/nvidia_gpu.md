### Binary Merklization using SHA2-512/224 on Nvidia GPU(s)

Compile with

```bash
SHA=sha2_512_224 make cuda
```

### On `Tesla V100-SXM2-16GB`

```bash
running on Tesla V100-SXM2-16GB

Benchmarking Binary Merklization using SHA2-512/224

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                   907.562250 us                     1.023168 ms                     1.005495 ms
        2 ^ 21                     1.678413 ms                     2.025513 ms                     2.033500 ms
        2 ^ 22                     3.189012 ms                     4.024750 ms                     4.024841 ms
        2 ^ 23                     6.197845 ms                     8.033661 ms                     8.026794 ms
        2 ^ 24                    12.042113 ms                    15.990051 ms                    16.062378 ms
        2 ^ 25                    23.805906 ms                    31.998413 ms                    32.102051 ms
```
