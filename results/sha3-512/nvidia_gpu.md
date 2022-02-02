### Binary Merklization using SHA3-512 on Nvidia GPU(s)

Compile with

```bash
SHA=sha3_512 make cuda
```

### On `Tesla V100-SXM2-16GB`

```bash
running on Tesla V100-SXM2-16GB


Benchmarking Binary Merklization using SHA3-512

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     1.764427 ms                     2.308462 ms                     2.009170 ms
        2 ^ 21                     3.197203 ms                     4.596451 ms                     4.014771 ms
        2 ^ 22                     5.991760 ms                     9.152618 ms                     8.026062 ms
        2 ^ 23                     9.980712 ms                    18.291870 ms                    16.050659 ms
        2 ^ 24                    19.426758 ms                    36.573853 ms                    32.111572 ms
        2 ^ 25                    38.345214 ms                    73.152832 ms                    64.349121 ms
```
