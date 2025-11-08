#!/bin/bash
# Script to run QEMU with the kernel ELF binary (tools/system) for debugging

echo "Starting QEMU with kernel ELF binary and GDB stub on port 1234..."
echo "The kernel will be paused at startup."
echo "Connect with GDB using: target remote localhost:1234"
echo ""

# Check if tools/system file exists
if [ ! -f "tools/system" ]; then
    echo "Error: tools/system file not found!"
    echo "Please build the kernel first with 'make'"
    exit 1
fi

# Run QEMU with GDB stub using the ELF kernel directly
# This is better for debugging as it includes symbol information
# -s: shorthand for -gdb tcp::1234
# -S: freeze CPU at startup (wait for GDB connection)
# -kernel: load the kernel ELF file directly
qemu-system-i386 \
    -kernel tools/system \
    -s \
    -S \
    -nographic \
    -monitor stdio

echo ""
echo "QEMU exited."

