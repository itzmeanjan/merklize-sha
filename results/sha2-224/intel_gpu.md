### Binary Merklization using SHAS-224 on Intel GPU(s)

Compiling with

```bash
SHA=sha2_224 make aot_gpu
```

### On `Intel(R) Iris(R) Xe MAX Graphics [0x4905]`

```bash
running on Intel(R) Iris(R) Xe MAX Graphics [0x4905]


Benchmarking Binary Merklization using SHA2-224

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     3.299556 ms                     2.910804 ms                     1.937344 ms
        2 ^ 21                     6.122727 ms                     5.826639 ms                     3.878219 ms
        2 ^ 22                    11.706552 ms                    11.633836 ms                     7.752127 ms
        2 ^ 23                    22.779120 ms                    23.257279 ms                    15.497059 ms
        2 ^ 24                    44.795868 ms                    46.504594 ms                    30.991331 ms
        2 ^ 25                    88.647936 ms                    92.975571 ms                    61.978897 ms
```

### On `Intel(R) UHD Graphics P630 [0x3e96]`

```bash
running on Intel(R) UHD Graphics P630 [0x3e96]


Benchmarking Binary Merklization using SHA2-224

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                    17.515718 ms                     1.176525 ms                   646.715250 us
        2 ^ 21                    34.412236 ms                     2.338297 ms                     1.263488 ms
        2 ^ 22                    68.043794 ms                     2.483962 ms                     2.461925 ms
        2 ^ 23                   135.763826 ms                     4.867016 ms                     4.868448 ms
        2 ^ 24                   270.338491 ms                     9.695064 ms                     9.765406 ms
        2 ^ 25                   539.590906 ms                    19.611053 ms                    19.346926 ms
```
