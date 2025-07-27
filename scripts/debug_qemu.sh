#!/bin/bash
set -e

# Rebuild everything (optional)
scripts/build.sh

# Start QEMU with GDB stub
qemu-system-i386 -fda bin/os.bin -s -S &
QEMU_PID=$!

# Wait briefly to make sure QEMU is ready
sleep 1

# Run GDB with predefined commands
gdb -x .gdbinit

# Kill QEMU after debugging (optional)
kill $QEMU_PID

