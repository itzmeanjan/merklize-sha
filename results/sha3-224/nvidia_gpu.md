### Binary Merklization using SHA3-224 on Nvidia GPU(s)

Compile with

```bash
SHA=sha3_224 make cuda
```

### On `Tesla V100-SXM2-16GB`

```bash
running on Tesla V100-SXM2-16GB


Benchmarking Binary Merklization using SHA3-224

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                   655.746375 us                     1.021944 ms                   879.684375 us
        2 ^ 21                     1.156266 ms                     2.024460 ms                     1.757996 ms
        2 ^ 22                     2.138489 ms                     4.021912 ms                     3.513062 ms
        2 ^ 23                     4.088013 ms                     8.028900 ms                     7.030884 ms
        2 ^ 24                     7.960693 ms                    16.006165 ms                    14.053711 ms
        2 ^ 25                    13.526733 ms                    31.978638 ms                    28.107422 ms
```
