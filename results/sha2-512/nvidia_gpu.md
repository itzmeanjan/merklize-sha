### Binary Merklization using SHA2-512 on Nvidia GPU(s)

Compile with

```bash
SHA=sha2_512 make cuda
```

### On `Tesla V100-SXM2-16GB`

```bash
running on Tesla V100-SXM2-16GB


Benchmarking Binary Merklization using SHA2-512

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     1.969058 ms                     2.309687 ms                     2.009434 ms
        2 ^ 21                     3.785888 ms                     4.592331 ms                     4.015182 ms
        2 ^ 22                     7.367341 ms                     9.173218 ms                     8.045655 ms
        2 ^ 23                    14.480712 ms                    18.306702 ms                    16.052490 ms
        2 ^ 24                    28.763672 ms                    36.593994 ms                    32.110107 ms
        2 ^ 25                    57.205079 ms                    73.100098 ms                    64.215820 ms
```
