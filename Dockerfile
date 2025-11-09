# Dockerfile for building Linux kernel
# This provides the vintage toolchain needed to build this old kernel

FROM ubuntu:18.04

# Prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install necessary build tools
RUN apt-get update && apt-get install -y \
    gcc \
    make \
    bin86 \
    as31 \
    build-essential \
    gdb \
    qemu-system-i386 \
    vim \
    && rm -rf /var/lib/apt/lists/*

# Create a working directory
WORKDIR /linux

# Copy the kernel source
COPY . /linux

# Set up environment
ENV PATH="/usr/bin:${PATH}"

# Default command - show build instructions
CMD ["/bin/bash", "-c", "echo 'Linux Kernel Build Environment' && \
    echo '==========================================' && \
    echo '' && \
    echo 'To build the kernel:' && \
    echo '  make clean && make' && \
    echo '' && \
    echo 'To run with QEMU (for debugging):' && \
    echo '  qemu-system-i386 -kernel tools/system -nographic -s -S' && \
    echo '' && \
    echo 'To run with full Image:' && \
    echo '  qemu-system-i386 -fda Image -boot a -s -S' && \
    echo '' && \
    echo 'GDB will be available on port 1234' && \
    echo '==========================================' && \
    /bin/bash"]

