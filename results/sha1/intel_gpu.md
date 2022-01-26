### Binary Merklization using SHA1 on Intel GPU(s)

Compiling with

```bash
SHA=sha1 make aot_gpu
```

### On `Intel(R) Iris(R) Xe MAX Graphics [0x4905]`

```bash
running on Intel(R) Iris(R) Xe MAX Graphics [0x4905]


Benchmarking Binary Merklization using SHA1

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     1.540110 ms                     2.075697 ms                     1.384617 ms
        2 ^ 21                     2.780271 ms                     4.154280 ms                     2.767148 ms
        2 ^ 22                     5.217264 ms                     8.316750 ms                     5.537688 ms
        2 ^ 23                    10.041954 ms                    16.615872 ms                    11.068980 ms
        2 ^ 24                    19.637670 ms                    33.221877 ms                    22.139305 ms
        2 ^ 25                    38.768184 ms                    66.419808 ms                    44.274379 ms
```

### On `Intel(R) UHD Graphics P630 [0x3e96]`

```bash
running on Intel(R) UHD Graphics P630 [0x3e96]


Benchmarking Binary Merklization using SHA1

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                     5.891589 ms                   515.056500 us                   458.471250 us
        2 ^ 21                    11.574329 ms                     1.674400 ms                   932.629500 us
        2 ^ 22                    22.752375 ms                     3.321722 ms                     1.759496 ms
        2 ^ 23                    45.262099 ms                     3.524844 ms                     3.527396 ms
        2 ^ 24                    89.886199 ms                     7.033752 ms                     6.912925 ms
        2 ^ 25                   179.697573 ms                    13.852368 ms                    13.747352 ms
```
