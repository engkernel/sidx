#!/bin/bash

CURRENT_DIR=$PWD
echo $CURRENT_DIR
qemu-system-x86_64 -hda $CURRENT_DIR/bin/os.bin
