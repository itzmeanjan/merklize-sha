### Binary Merklization using SHA3-384 on Nvidia GPU(s)

Compile with

```bash
SHA=sha3_384 make cuda
```

### On `Tesla V100-SXM2-16GB`

```bash
running on Tesla V100-SXM2-16GB


Benchmarking Binary Merklization using SHA3-384

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                   999.549375 us                     1.739604 ms                     1.508103 ms
        2 ^ 21                     1.821351 ms                     3.455978 ms                     3.011856 ms
        2 ^ 22                     3.385437 ms                     6.848968 ms                     6.024078 ms
        2 ^ 23                     6.186218 ms                    13.745361 ms                    12.038086 ms
        2 ^ 24                    10.959961 ms                    27.432861 ms                    24.085327 ms
        2 ^ 25                    21.593261 ms                    54.867188 ms                    48.198486 ms
```
