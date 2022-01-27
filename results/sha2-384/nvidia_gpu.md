### Binary Merklization using SHA2-384 on Nvidia GPU(s)

Compile with

```bash
SHA=sha2_384 make cuda
```

### On `Tesla V100-SXM2-16GB`

```bash
running on Tesla V100-SXM2-16GB


Benchmarking Binary Merklization using SHA2-384

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                   982.126125 us                     1.744032 ms                     1.507438 ms
        2 ^ 21                     1.833298 ms                     3.457054 ms                     3.012680 ms
        2 ^ 22                     3.523499 ms                     6.881653 ms                     6.020691 ms
        2 ^ 23                     6.842102 ms                    13.744263 ms                    12.048889 ms
        2 ^ 24                    13.480591 ms                    27.436157 ms                    24.112793 ms
        2 ^ 25                    26.730469 ms                    54.882568 ms                    48.191162 ms
```
