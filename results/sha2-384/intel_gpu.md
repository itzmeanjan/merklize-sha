### Binary Merklization using SHA2-384 on Intel GPU(s)

Compiling with

```bash
SHA=sha2_384 make aot_gpu
```

### On `Intel(R) Iris(R) Xe MAX Graphics [0x4905]`

```bash
running on Intel(R) Iris(R) Xe MAX Graphics [0x4905]


Benchmarking Binary Merklization using SHA2-384

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     4.427943 ms                     4.985896 ms                     3.320733 ms
        2 ^ 21                     8.178846 ms                     9.974016 ms                     6.650143 ms
        2 ^ 22                    15.558582 ms                    19.935025 ms                    13.285408 ms
        2 ^ 23                    30.297657 ms                    39.871942 ms                    26.567385 ms
        2 ^ 24                    59.669610 ms                    79.701707 ms                    53.124454 ms
        2 ^ 25                   118.088451 ms                   159.366987 ms                   106.247778 ms
```

### On `Intel(R) UHD Graphics P630 [0x3e96]`

```bash
running on Intel(R) UHD Graphics P630 [0x3e96]


Benchmarking Binary Merklization using SHA2-384

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                    35.474719 ms                     1.999283 ms                     2.027918 ms
        2 ^ 21                    70.131535 ms                     3.979767 ms                     3.991096 ms
        2 ^ 22                   139.684518 ms                     7.969120 ms                     7.980388 ms
        2 ^ 23                   278.812085 ms                    15.334540 ms                    15.672994 ms
        2 ^ 24                   555.954314 ms                    29.387789 ms                    29.410822 ms
        2 ^ 25                      1.112163 s                    48.478432 ms                    47.542255 ms
```
