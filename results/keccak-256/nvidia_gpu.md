### Binary Merklization using KECCAK-256 on Nvidia GPU(s)

Compile with

```bash
SHA=keccak_256 make cuda
```

### On `Tesla V100-SXM2-16GB`

```bash
running on Tesla V100-SXM2-16GB


Benchmarking Binary Merklization using KECCAK-256

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                   751.924875 us                     1.167792 ms                     1.005363 ms
        2 ^ 21                     1.344910 ms                     2.304931 ms                     2.016678 ms
        2 ^ 22                     2.517974 ms                     4.593017 ms                     4.025208 ms
        2 ^ 23                     4.864380 ms                     9.128906 ms                     8.053345 ms
        2 ^ 24                     8.179686 ms                    18.250488 ms                    16.049194 ms
        2 ^ 25                    16.144776 ms                    36.534668 ms                    32.099121 ms
```
