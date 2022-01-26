### Binary Merklization using SHA2-256 on Nvidia GPU(s)

Compile with

```bash
SHA=sha2_256 make cuda
```

### On `Tesla V100-SXM2-16GB`

```bash
running on Tesla V100-SXM2-16GB


Benchmarking Binary Merklization using SHA2-256

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                   719.621625 us                     1.169785 ms                     1.005707 ms
        2 ^ 21                     1.340929 ms                     2.312210 ms                     2.009216 ms
        2 ^ 22                     2.567003 ms                     4.590637 ms                     4.015045 ms
        2 ^ 23                     5.018921 ms                     9.169464 ms                     8.026062 ms
        2 ^ 24                     9.837526 ms                    18.313293 ms                    16.052857 ms
        2 ^ 25                    19.494141 ms                    36.597290 ms                    32.106445 ms
```
