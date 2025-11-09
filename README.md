# The (original) Linux Kernel

Linux kernel version 0.96c (circa 1991-1993) configured for modern development and debugging in JetBrains CLion on macOS.

Originally written by **Linus Torvalds** and contributors in the early 1990s. The original code is distributed under the GNU General Public License v2. This repository contains the original source code with modern build and debugging tools added for educational and development purposes. No modifications have been made to the original kernel source files.

## ⚠️ Debug Build Configuration

**This project is configured for optimal debugging:**
- `-g -ggdb` : Full debug symbols
- `-O0` : No optimizations (accurate stepping)
- `-fno-omit-frame-pointer` : Complete stack traces

See [DEBUG_BUILD.md](DEBUG_BUILD.md) for complete details.

```
├── boot/           # Boot sector and setup code
├── fs/             # File system code
│   ├── ext/        # Extended filesystem
│   └── minix/      # Minix filesystem
├── include/        # Header files
│   ├── asm/        # Assembly headers
│   ├── linux/      # Linux kernel headers
│   └── sys/        # System headers
├── init/           # Initialization code (main.c)
├── kernel/         # Core kernel code
│   ├── blk_drv/    # Block device drivers
│   ├── chr_drv/    # Character device drivers
│   └── math/       # Math emulation
├── lib/            # Kernel library functions
├── mm/             # Memory management
├── net/            # Networking code
├── tools/          # Build tools
├── CMakeLists.txt  # CLion project file
├── Makefile        # Traditional kernel build
├── Dockerfile      # Docker build environment
└── BUILD.md        # This file
```

## Prerequisites

1. **Docker Desktop** (for building the kernel on macOS)
   ```bash
   brew install --cask docker
   ```

2. **QEMU** (for running the kernel)
   ```bash
   brew install qemu
   ```

3. **GDB** (for debugging, preferably with i386 support)
   ```bash
   brew install gdb
   ```

4. **JetBrains CLion** (your IDE)
    - Download from: https://www.jetbrains.com/clion/

## Quick Start

### 1. Build the Kernel Using Docker

```bash
# Start the Docker build environment
docker-compose up -d

# Enter the container
docker exec -it linux_kernel bash

# Inside the container, build the kernel
make clean
make

# Exit the container
exit
```

This will produce:
- `Image` - The bootable floppy disk image
- `tools/system` - The kernel ELF binary with debug symbols
- `System.map` - Symbol map for debugging

### 2. Run QEMU with GDB Stub

In your terminal:

```bash
# Run QEMU with the full bootable Image
./run-qemu-debug.sh

# OR run with just the kernel (better for debugging)
./run-qemu-kernel.sh
```

```bash
# Run all tests
./run-tests.sh

# Or manually
cmake -S . -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug
cd cmake-build-debug
make run_tests
```

## Running Tests

### Kernel Code Tests (2 TESTS LINK ORIGINAL KERNEL CODE! 🔬)

These tests compile and link **actual 1991-1993 Linux kernel source files** for step debugging:

1. ✅ **test_kernel_mktime** - Links `kernel/mktime.c` 
   - Debug Linus's original time conversion algorithm!
   - Set breakpoints in `kernel/mktime.c`, step through code
   - See 1992 leap year calculations in action
   
2. ✅ **test_kernel_errno** - Links `lib/errno.c`
   - Debug the kernel's global errno variable
   - Simple example of linking kernel code

```bash
# Build and run
cd cmake-build-debug
cmake ..
cmake --build . --target test_kernel_mktime
./test_kernel_mktime

# Debug in CLion:
# 1. Select "test_kernel_mktime" from run configurations
# 2. Open kernel/mktime.c (ORIGINAL 1992 code!)
# 3. Set breakpoint in kernel_mktime() function
# 4. Click Debug and step through the algorithm!
```

**Why only 2 tests?** Most kernel `.c` files have deep dependencies on kernel headers and internal structures. Only a few simple files like `mktime.c` and `errno.c` can be compiled standalone without the full kernel build environment.

**See [HONEST_RESULTS.md](HONEST_RESULTS.md) for complete details on what works and why.**



The kernel will pause at startup, waiting for GDB to connect.

### 3. Connect GDB

In another terminal:

```bash
gdb -x gdbinit.kernel
```

Or manually:

```bash
gdb tools/system
(gdb) target remote localhost:1234
(gdb) break main
(gdb) continue
```

## CLion Setup

### 1. Open the Project

1. Launch CLion
2. Open the `/Users/klambert/workspace/github_kevleyski/linux` directory
3. CLion will automatically detect the `CMakeLists.txt` and configure the project

### 2. Configure CLion for Code Navigation

The `CMakeLists.txt` is configured for IDE navigation and analysis only. It:
- Includes all kernel source files
- Sets up proper include paths
- Defines necessary macros
- Creates a static library target for analysis

**Important**: The CMake build in CLion is for code navigation only. Use the Makefile (via Docker) for actual kernel compilation.

### 3. Set Up Remote GDB Debugging in CLion

1. Go to **Run** → **Edit Configurations...**
2. Click the **+** button and select **GDB Remote Debug**
3. Configure the settings:
    - **Name**: `Linux Kernel Debug`
    - **GDB**: `/usr/local/bin/gdb` (or your GDB path)
    - **'target remote' args**: `localhost:1234`
    - **Symbol file**: `/Users/klambert/workspace/github_kevleyski/linux/tools/system`
    - **Path mappings**: Add mapping if needed (usually not required for this setup)
4. Click **OK**

### 4. Start Debugging

1. Start QEMU with GDB stub (using `./run-qemu-kernel.sh`)
2. In CLion, select your **Linux Kernel Debug** configuration
3. Click the **Debug** button (or press Ctrl+D / Cmd+D)
4. CLion will connect to QEMU
5. Set breakpoints in the code
6. Use the debugger controls to step through the code

## Building the Kernel

### Method 1: Docker (Recommended for macOS)

This is the recommended method as it provides all necessary vintage tools (as86, ld86, etc.):

```bash
# Build and start the container
docker-compose up -d

# Enter the container
docker exec -it linux_kernel bash

# Clean previous builds
make clean

# Build the kernel
make

# The build process will:
# 1. Generate version files
# 2. Compile all subdirectories (kernel, mm, fs, lib, net)
# 3. Assemble boot sector and setup
# 4. Link everything into tools/system
# 5. Create the bootable Image

# Exit when done
exit
```

### Method 2: Native Linux Build

If you have access to a Linux system with the proper tools:

```bash
# Install required packages (Ubuntu/Debian)
sudo apt-get install gcc make bin86 build-essential

# Build
make clean
make
```

### Method 3: CLion Custom Target

You can also trigger the Makefile build from CLion:

1. Go to **Run** → **Edit Configurations...**
2. Add a new **Makefile** configuration
3. Set **Makefile**: `Makefile`
4. Set **Target**: `all`
5. Run this configuration to build

However, this will only work inside the Docker container or on a Linux system with proper tools.

## Debugging with GDB

### Understanding the Boot Process

The Linux kernel boots in several stages:

1. **BIOS loads bootsect** (`boot/bootsect.S`)
2. **bootsect loads setup** (`boot/setup.S`)
3. **setup configures the system and loads the kernel**
4. **Kernel starts at head.s** (`boot/head.s`)
5. **Control transfers to main()** (`init/main.c`)

### Useful GDB Commands

```gdb
# Set breakpoints
break main                    # Break at main()
break start_kernel            # Break at kernel start
break *0x100000              # Break at specific address

# Examine code
disassemble main             # Disassemble function
x/10i $pc                    # Show 10 instructions at PC
info registers               # Show all registers
info frame                   # Show stack frame

# Step through code
si                           # Step one instruction
ni                           # Next instruction (skip calls)
s                            # Step into (C level)
n                            # Next (C level)
c                            # Continue execution

# Examine memory
x/10x $esp                   # Show 10 hex words at stack pointer
x/s 0x12345                  # Show string at address
print variable_name          # Print variable value

# Watchpoints
watch some_variable          # Break when variable changes
rwatch address               # Break on read
awatch address               # Break on access
```

### Debugging Kernel Initialization

```gdb
# Connect to QEMU
target remote localhost:1234

# Set breakpoint at kernel entry
break main

# Continue to main
continue

# Step through initialization
next
next
print variable_name
```

### Debugging Specific Subsystems

```gdb
# Memory management
break mem_init
break kmalloc

# File system
break mount_root
break sys_open

# Process management
break fork
break schedule

# Interrupt handling
break trap_init
break do_IRQ
```

## Running tests (unit tests)

Prerequisites:

- CMake (>= 3.10)
- A C compiler available on PATH (clang/gcc). You can also set CC to a specific compiler binary.
- ctest (optional, but recommended — typically bundled with CMake)

Quick commands:

```bash
# Automatic: configure, build and run tests (recommended)
./run-tests.sh

# Manual steps:
cmake -S . -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build cmake-build-debug --target run_tests -- -j$(sysctl -n hw.ncpu || echo 1)
cd cmake-build-debug && ctest --output-on-failure -C Debug
```

Notes:
- On macOS you may need to install the Xcode command line tools: `xcode-select --install` or install gcc/clang via Homebrew.
- If the repository was configured on another machine, the CMake cache may reference a compiler path that doesn't exist locally; `./run-tests.sh` tries to detect a local compiler and pass it to CMake automatically.
- For kernel compilation (not just tests) prefer the Docker-based Makefile workflow described earlier.

## Troubleshooting

### Issue: "Cannot find as86 or ld86"

**Solution**: Use Docker. These are vintage 16-bit assemblers not readily available on modern systems.

```bash
docker-compose up -d
docker exec -it linux_kernel bash
make
```

### Issue: "GDB cannot connect to localhost:1234"

**Solution**: Make sure QEMU is running with `-s -S` flags:

```bash
# Check if QEMU is running
ps aux | grep qemu

# If not, start it
./run-qemu-kernel.sh
```

### Issue: "GDB shows no symbols"

**Solution**: Ensure you're loading the `tools/system` file:

```bash
gdb tools/system
(gdb) file tools/system
(gdb) target remote localhost:1234
```

### Issue: "Kernel builds but won't boot in QEMU"

**Solution**:
1. Check that the Image file was created: `ls -lh Image`
2. Try booting with: `qemu-system-i386 -fda Image -boot a`
3. Check for build errors: `make clean && make 2>&1 | tee build.log`

### Issue: "CMake errors in CLion"

**Solution**: The CMake configuration is for IDE navigation only. Some errors are expected:

1. Ignore assembler file errors (`.S`, `.s` files)
2. Ignore some inline assembly errors
3. Focus on using the Makefile for actual builds

### Issue: "Port 1234 already in use"

**Solution**: Kill any existing QEMU/GDB processes:

```bash
# Find and kill QEMU
pkill qemu

# Find and kill any process on port 1234
lsof -ti:1234 | xargs kill -9
```

### Issue: "Docker container won't start"

**Solution**:

```bash
# Stop and remove existing container
docker-compose down

# Rebuild
docker-compose build --no-cache

# Start fresh
docker-compose up -d
```
