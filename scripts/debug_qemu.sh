#!/bin/bash
set -e

# Rebuild everything (optional)
scripts/build.sh

qemu-system-i386 -kernel bin/os.bin -d cpu,int -no-reboot -no-shutdown


