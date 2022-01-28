### Binary Merklization using SHA2-512/256 on Nvidia GPU(s)

Compile with

```bash
SHA=sha2_512_256 make cuda
```

### On `Tesla V100-SXM2-16GB`

```bash
running on Tesla V100-SXM2-16GB


Benchmarking Binary Merklization using SHA2-512/256

      leaf count                  execution time                host-to-device tx time          device-to-host tx time
        2 ^ 20                   904.547250 us                     1.169220 ms                     1.006079 ms
        2 ^ 21                     1.673768 ms                     2.311089 ms                     2.008942 ms
        2 ^ 22                     3.180954 ms                     4.595947 ms                     4.018524 ms
        2 ^ 23                     6.150878 ms                     9.173584 ms                     8.041260 ms
        2 ^ 24                    11.967042 ms                    18.297363 ms                    16.067871 ms
        2 ^ 25                    23.630860 ms                    36.579346 ms                    32.120361 ms
```
