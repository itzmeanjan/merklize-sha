### Binary Merklization using SHA3-512 on Intel GPU(s)

Compiling with

```bash
SHA=sha3_512 make aot_gpu
```

### On `Intel(R) UHD Graphics P630 [0x3e96]`

```bash
running on Intel(R) UHD Graphics P630 [0x3e96]


Benchmarking Binary Merklization using SHA3-512

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                   289.603807 ms                     1.504084 ms                     1.416810 ms
        2 ^ 21                   576.084968 ms                     2.882175 ms                     2.812580 ms
        2 ^ 22                      1.148452 s                     5.658525 ms                     5.527675 ms
        2 ^ 23                      2.294983 s                    11.162608 ms                    11.004991 ms
        2 ^ 24                      4.582099 s                    22.103979 ms                    21.868923 ms
        2 ^ 25                      9.163329 s                    43.960825 ms                    44.113151 ms
```
