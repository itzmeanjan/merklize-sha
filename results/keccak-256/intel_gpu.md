### Binary Merklization using KECCAK-256 on Intel GPU(s)

Compiling with

```bash
SHA=keccak_256 make aot_gpu
```

### On `Intel(R) UHD Graphics P630 [0x3e96]`

```bash
running on Intel(R) UHD Graphics P630 [0x3e96]


Benchmarking Binary Merklization using KECCAK-256

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                   108.488926 ms                     1.332275 ms                   745.381500 us
        2 ^ 21                   212.384799 ms                     1.497735 ms                     1.454533 ms
        2 ^ 22                   422.459127 ms                     5.289694 ms                     2.832562 ms
        2 ^ 23                   841.035348 ms                     5.684048 ms                     5.597084 ms
        2 ^ 24                      1.679276 s                    11.176738 ms                    11.080438 ms
        2 ^ 25                      3.355854 s                    22.150604 ms                    22.356589 ms
```
