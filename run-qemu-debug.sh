#!/bin/bash
# Script to run QEMU with the Linux kernel and wait for GDB connection

echo "Starting QEMU with GDB stub on port 1234..."
echo "The kernel will be paused at startup."
echo "Connect with GDB using: target remote localhost:1234"
echo ""

# Check if Image file exists
if [ ! -f "Image" ]; then
    echo "Error: Image file not found!"
    echo "Please build the kernel first with 'make'"
    exit 1
fi

# Run QEMU with GDB stub
# -s: shorthand for -gdb tcp::1234
# -S: freeze CPU at startup (wait for GDB connection)
# -fda Image: use Image as floppy disk A
# -boot a: boot from floppy disk A
qemu-system-i386 \
    -fda Image \
    -boot a \
    -s \
    -S \
    -nographic \
    -monitor stdio

echo ""
echo "QEMU exited."

