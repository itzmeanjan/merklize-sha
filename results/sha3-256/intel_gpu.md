### Binary Merklization using SHA3-256 on Intel GPU(s)

Compiling with

```bash
SHA=sha3_256 make aot_gpu
```

### On `Intel(R) Iris(R) Xe MAX Graphics [0x4905]`

```bash
running on Intel(R) Iris(R) Xe MAX Graphics [0x4905]


Benchmarking Binary Merklization using SHA3-256

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                    23.984610 ms                     3.324769 ms                     2.221226 ms
        2 ^ 21                    46.259382 ms                     6.643864 ms                     4.436172 ms
        2 ^ 22                    90.467286 ms                    13.314171 ms                     8.856666 ms
        2 ^ 23                   178.541454 ms                    26.589966 ms                    17.714424 ms
        2 ^ 24                   355.002453 ms                    53.145359 ms                    35.418396 ms
        2 ^ 25                   707.377671 ms                   106.254076 ms                    70.831839 ms
```

### On `Intel(R) UHD Graphics P630 [0x3e96]`

```bash
running on Intel(R) UHD Graphics P630 [0x3e96]


Benchmarking Binary Merklization using SHA3-256

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                   107.777160 ms                   768.600750 us                   711.330750 us
        2 ^ 21                   213.150474 ms                     1.480429 ms                     1.442208 ms
        2 ^ 22                   423.765941 ms                     2.861508 ms                     2.822228 ms
        2 ^ 23                   844.021978 ms                     5.608974 ms                     5.555252 ms
        2 ^ 24                      1.685802 s                    20.175661 ms                    11.154640 ms
        2 ^ 25                      3.369420 s                    22.098626 ms                    22.164610 ms
```
