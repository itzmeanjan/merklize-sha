### Binary Merklization using SHA2-512/256 on Intel GPU(s)

Compiling with

```bash
SHA=sha2_512_256 make aot_gpu
```

### On `Intel(R) Iris(R) Xe MAX Graphics [0x4905]`

```bash
running on Intel(R) Iris(R) Xe MAX Graphics [0x4905]


Benchmarking Binary Merklization using SHA2-512/256

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     4.310631 ms                     3.327480 ms                     2.216194 ms
        2 ^ 21                     7.954752 ms                     6.649071 ms                     4.435470 ms
        2 ^ 22                    15.138474 ms                    13.294963 ms                     8.859025 ms
        2 ^ 23                    29.383614 ms                    26.579807 ms                    17.710173 ms
        2 ^ 24                    57.712629 ms                    53.155069 ms                    35.417831 ms
        2 ^ 25                   114.374988 ms                   106.247758 ms                    70.829402 ms
```

### On `Intel(R) UHD Graphics P630 [0x3e96]`

```bash
running on Intel(R) UHD Graphics P630 [0x3e96]


Benchmarking Binary Merklization using SHA2-512/256

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                    35.286786 ms                   790.637250 us                   750.237000 us
        2 ^ 21                    69.918017 ms                     1.503462 ms                     1.450861 ms
        2 ^ 22                   139.115740 ms                     2.872900 ms                     2.822602 ms
        2 ^ 23                   277.552768 ms                     5.618623 ms                     5.612647 ms
        2 ^ 24                   554.399496 ms                    11.157254 ms                    11.115049 ms
        2 ^ 25                      1.108723 s                    22.222441 ms                    22.117363 ms
```
