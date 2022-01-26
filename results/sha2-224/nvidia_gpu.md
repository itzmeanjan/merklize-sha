### Binary Merklization using SHA2-224 on Nvidia GPU(s)

Compiling with

```bash
SHA=sha2_224 make cuda
```

### On `Tesla V100-SXM2-16GB`

```bash
running on Tesla V100-SXM2-16GB


Benchmarking Binary Merklization using SHA2-224

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                   704.910000 us                     1.027631 ms                   881.401125 us
        2 ^ 21                     1.331406 ms                     2.025925 ms                     1.758224 ms
        2 ^ 22                     2.569016 ms                     4.024841 ms                     3.518417 ms
        2 ^ 23                     5.039886 ms                     8.027435 ms                     7.029602 ms
        2 ^ 24                     9.962402 ms                    16.032166 ms                    14.043823 ms
        2 ^ 25                    19.845703 ms                    32.005005 ms                    28.119873 ms
```
