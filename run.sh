#!/bin/bash

# easy to use script for executing all possible test cases !

make clean

SHA=sha1         make; make clean
SHA=sha2_224     make; make clean
SHA=sha2_256     make; make clean
SHA=sha2_384     make; make clean
SHA=sha2_512     make; make clean
SHA=sha2_512_224 make; make clean
SHA=sha2_512_256 make; make clean
SHA=sha3_256 make; make clean
