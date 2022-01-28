### Binary Merklization using SHA2-512 on Intel GPU(s)

Compiling with

```bash
SHA=sha2_512 make aot_gpu
```

### On `Intel(R) UHD Graphics P630 [0x3e96]`

```bash
running on Intel(R) UHD Graphics P630 [0x3e96]


Benchmarking Binary Merklization using SHA2-512

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                    72.199791 ms                     1.497735 ms                     1.446130 ms
        2 ^ 21                   142.566693 ms                     2.902282 ms                     2.860325 ms
        2 ^ 22                   283.811694 ms                     5.658338 ms                     5.638231 ms
        2 ^ 23                   566.726801 ms                    11.190371 ms                    11.236063 ms
        2 ^ 24                      1.130808 s                    22.284130 ms                    22.242423 ms
        2 ^ 25                      2.260371 s                    44.421164 ms                    44.365699 ms
```
