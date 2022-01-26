### Binary Merklization using SHA2-256 on Intel GPU(s)

Compiling with

```bash
SHA=sha2_256 make aot_gpu
```

### On `Intel(R) Iris(R) Xe MAX Graphics [0x4905]`

```bash
running on Intel(R) Iris(R) Xe MAX Graphics [0x4905]


Benchmarking Binary Merklization using SHA2-256

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     4.661943 ms                     3.329839 ms                     2.217833 ms
        2 ^ 21                     9.002214 ms                     6.644352 ms                     4.428859 ms
        2 ^ 22                    17.619459 ms                    13.286383 ms                     8.860488 ms
        2 ^ 23                    34.812453 ms                    26.583785 ms                    17.713059 ms
        2 ^ 24                    69.134013 ms                    53.142063 ms                    35.421653 ms
        2 ^ 25                   137.558889 ms                   106.253335 ms                    70.831332 ms
```

### On `Intel(R) UHD Graphics P630 [0x3e96]`

```bash
running on Intel(R) UHD Graphics P630 [0x3e96]


Benchmarking Binary Merklization using SHA2-256

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                    18.288241 ms                     1.332399 ms                   814.728000 us
        2 ^ 21                    36.194453 ms                     1.485098 ms                     1.440838 ms
        2 ^ 22                    72.090231 ms                     2.873336 ms                     2.841028 ms
        2 ^ 23                   144.270973 ms                     5.664750 ms                     5.593536 ms
        2 ^ 24                   287.842319 ms                    11.170638 ms                    11.055911 ms
        2 ^ 25                   574.907261 ms                    22.107091 ms                    22.232961 ms
```
