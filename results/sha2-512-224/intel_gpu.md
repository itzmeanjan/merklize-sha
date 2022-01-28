### Binary Merklization using SHA2-512/224 on Intel GPU(s)

Compiling with

```bash
SHA=sha2_512_224 make aot_gpu
```

### On `Intel(R) Iris(R) Xe MAX Graphics [0x4905]`

```bash
running on Intel(R) Iris(R) Xe MAX Graphics [0x4905]


Benchmarking Binary Merklization using SHA2-512/224

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     4.382508 ms                     2.910297 ms                     2.218359 ms
        2 ^ 21                     8.159814 ms                     5.820106 ms                     4.433520 ms
        2 ^ 22                    15.642432 ms                    11.638536 ms                     8.862282 ms
        2 ^ 23                    30.472143 ms                    23.258059 ms                    17.715048 ms
        2 ^ 24                    60.050250 ms                    46.502079 ms                    35.419254 ms
        2 ^ 25                   119.221323 ms                    92.983878 ms                    70.835680 ms
```

### On `Intel(R) UHD Graphics P630 [0x3e96]`

```bash
running on Intel(R) UHD Graphics P630 [0x3e96]


Benchmarking Binary Merklization using SHA2-512/224

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                    35.615092 ms                     1.168868 ms                   734.923500 us
        2 ^ 21                    70.520722 ms                     2.289493 ms                     1.440216 ms
        2 ^ 22                   140.673172 ms                     4.557322 ms                     2.847004 ms
        2 ^ 23                   280.236676 ms                     4.940222 ms                     5.570815 ms
        2 ^ 24                   559.565748 ms                     9.766091 ms                    11.085356 ms
        2 ^ 25                      1.118228 s                    19.446589 ms                    22.242983 ms
```
