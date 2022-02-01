### Binary Merklization using SHA3-256 on Nvidia GPU(s)

Compile with

```bash
SHA=sha3_256 make cuda
```

### On `Tesla V100-SXM2-16GB`

```bash
running on Tesla V100-SXM2-16GB


Benchmarking Binary Merklization using SHA3-256

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                   752.334750 us                     1.167084 ms                     1.005913 ms
        2 ^ 21                     1.347657 ms                     2.312119 ms                     2.012466 ms
        2 ^ 22                     2.526214 ms                     4.599151 ms                     4.014862 ms
        2 ^ 23                     4.852661 ms                     9.173950 ms                     8.025696 ms
        2 ^ 24                     8.640747 ms                    18.299194 ms                    16.064941 ms
        2 ^ 25                    16.185790 ms                    36.593994 ms                    32.170166 ms
```
