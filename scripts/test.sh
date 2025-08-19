#!/bin/bash

make clean
make
qemu-system-x86_64 -drive format=raw,file=build/os.bin
