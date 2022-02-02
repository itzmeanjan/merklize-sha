### Binary Merklization using SHA3-224 on Intel GPU(s)

Compiling with

```bash
SHA=sha3_224 make aot_gpu
```

### On `Intel(R) Iris(R) Xe MAX Graphics [0x4905]`

```bash
running on Intel(R) Iris(R) Xe MAX Graphics [0x4905]


Benchmarking Binary Merklization using SHA3-224

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                    25.223484 ms                     2.909517 ms                     1.943604 ms
        2 ^ 21                    48.836151 ms                     5.816285 ms                     3.879427 ms
        2 ^ 22                    95.447547 ms                    11.645907 ms                     7.751796 ms
        2 ^ 23                   187.926882 ms                    23.257689 ms                    15.500472 ms
        2 ^ 24                   373.580415 ms                    46.510990 ms                    30.994879 ms
        2 ^ 25                   744.879213 ms                    92.975356 ms                    61.980399 ms
```

### On `Intel(R) UHD Graphics P630 [0x3e96]`

```bash
running on Intel(R) UHD Graphics P630 [0x3e96]


Benchmarking Binary Merklization using SHA3-224

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                   106.771200 ms                     1.167934 ms                   652.068750 us
        2 ^ 21                   211.127162 ms                     1.298971 ms                     1.234853 ms
        2 ^ 22                   419.490985 ms                     2.499773 ms                     2.436278 ms
        2 ^ 23                   836.673428 ms                     4.903121 ms                     4.823504 ms
        2 ^ 24                      1.669896 s                     9.690955 ms                     9.642774 ms
        2 ^ 25                      3.337676 s                    19.390999 ms                    19.220123 ms
```
