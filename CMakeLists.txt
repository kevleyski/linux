cmake_minimum_required(VERSION 3.10)

# Include CLion-specific overrides if present
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.clion.cmake")
    include("${CMAKE_CURRENT_SOURCE_DIR}/.clion.cmake")
endif()

project(linux_kernel C)

# This CMakeLists.txt is designed for CLion IDE navigation and analysis only.
# The actual kernel build should use the traditional Makefile.

set(CMAKE_C_STANDARD 90)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable testing
enable_testing()

# Include directories - using local include path
# Use SYSTEM so these come AFTER system includes
include_directories(SYSTEM
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/linux
    ${CMAKE_SOURCE_DIR}/include/asm
    ${CMAKE_SOURCE_DIR}/include/sys
)

# Compiler flags to simulate the kernel build environment
add_compile_definitions(
    __KERNEL__
    KERNEL_MATH_EMULATION
    KBD_FINNISH
    KBDFLAGS=0
)

# Compiler flags that match the original Makefile (relaxed for IDE analysis)
# Force debug mode with no optimizations for all configurations
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-main -Wno-implicit-function-declaration")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-int-conversion -Wno-incompatible-library-redeclaration")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-incompatible-pointer-types -Wno-deprecated-declarations")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-builtin -fno-omit-frame-pointer")

# Full debug symbols, no optimizations - for ALL build types
set(CMAKE_C_FLAGS_DEBUG "-g -ggdb -O0")
set(CMAKE_C_FLAGS_RELEASE "-g -ggdb -O0")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-g -ggdb -O0")
set(CMAKE_C_FLAGS_MINSIZEREL "-g -ggdb -O0")

# Force debug mode
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
endif()

# Source files organized by directory

# init/
set(INIT_SOURCES
    init/main.c
)

# kernel/
set(KERNEL_SOURCES
    kernel/sched.c
    kernel/sys.c
    kernel/traps.c
    kernel/fork.c
    kernel/exit.c
    kernel/signal.c
    kernel/panic.c
    kernel/printk.c
    kernel/vsprintf.c
    kernel/mktime.c
    kernel/ptrace.c
    kernel/ioport.c
    kernel/itimer.c
)

# kernel/chr_drv/
set(KERNEL_CHR_DRV_SOURCES
    kernel/chr_drv/console.c
    kernel/chr_drv/keyboard.c
    kernel/chr_drv/lp.c
    kernel/chr_drv/mem.c
    kernel/chr_drv/pty.c
    kernel/chr_drv/serial.c
    kernel/chr_drv/tty_io.c
    kernel/chr_drv/tty_ioctl.c
    kernel/chr_drv/vt.c
)

# kernel/blk_drv/
set(KERNEL_BLK_DRV_SOURCES
    kernel/blk_drv/ll_rw_blk.c
    kernel/blk_drv/floppy.c
    kernel/blk_drv/hd.c
    kernel/blk_drv/ramdisk.c
)

# kernel/blk_drv/scsi/
set(KERNEL_SCSI_SOURCES
    kernel/blk_drv/scsi/hosts.c
    kernel/blk_drv/scsi/scsi.c
    kernel/blk_drv/scsi/scsi_ioctl.c
    kernel/blk_drv/scsi/sd.c
    kernel/blk_drv/scsi/sd_ioctl.c
    kernel/blk_drv/scsi/st.c
    kernel/blk_drv/scsi/st_ioctl.c
    kernel/blk_drv/scsi/aha1542.c
    kernel/blk_drv/scsi/seagate.c
    kernel/blk_drv/scsi/ultrastor.c
)

# kernel/math/
set(KERNEL_MATH_SOURCES
    kernel/math/add.c
    kernel/math/compare.c
    kernel/math/convert.c
    kernel/math/div.c
    kernel/math/ea.c
    kernel/math/emulate.c
    kernel/math/error.c
    kernel/math/get_put.c
    kernel/math/mul.c
    kernel/math/sqrt.c
)

# mm/
set(MM_SOURCES
    mm/memory.c
    mm/swap.c
    mm/mmap.c
)

# fs/
set(FS_SOURCES
    fs/open.c
    fs/read_write.c
    fs/inode.c
    fs/file_table.c
    fs/buffer.c
    fs/super.c
    fs/block_dev.c
    fs/stat.c
    fs/exec.c
    fs/pipe.c
    fs/namei.c
    fs/fcntl.c
    fs/ioctl.c
    fs/select.c
    fs/fifo.c
)

# fs/minix/
set(FS_MINIX_SOURCES
    fs/minix/bitmap.c
    fs/minix/blkdev.c
    fs/minix/chrdev.c
    fs/minix/dir.c
    fs/minix/fifo.c
    fs/minix/file.c
    fs/minix/inode.c
    fs/minix/namei.c
    fs/minix/symlink.c
    fs/minix/truncate.c
)

# fs/ext/
set(FS_EXT_SOURCES
    fs/ext/bitmap.c
    fs/ext/blkdev.c
    fs/ext/chrdev.c
    fs/ext/dir.c
    fs/ext/fifo.c
    fs/ext/file.c
    fs/ext/freelists.c
    fs/ext/inode.c
    fs/ext/namei.c
    fs/ext/symlink.c
    fs/ext/truncate.c
)

# lib/
set(LIB_SOURCES
    lib/ctype.c
    lib/_exit.c
    lib/open.c
    lib/close.c
    lib/errno.c
    lib/write.c
    lib/dup.c
    lib/setsid.c
    lib/execve.c
    lib/wait.c
    lib/string.c
    lib/malloc.c
    lib/itimer.c
)

# net/
set(NET_SOURCES
    net/socket.c
    net/unix.c
)

# tools/
set(TOOLS_SOURCES
    tools/build.c
)

# Combine all sources
set(ALL_SOURCES
    ${INIT_SOURCES}
    ${KERNEL_SOURCES}
    ${KERNEL_CHR_DRV_SOURCES}
    ${KERNEL_BLK_DRV_SOURCES}
    ${KERNEL_SCSI_SOURCES}
    ${KERNEL_MATH_SOURCES}
    ${MM_SOURCES}
    ${FS_SOURCES}
    ${FS_MINIX_SOURCES}
    ${FS_EXT_SOURCES}
    ${LIB_SOURCES}
    ${NET_SOURCES}
)

# Create a library target for IDE navigation
# We use a library instead of executable because this code cannot run natively on the host
add_library(linux_kernel STATIC ${ALL_SOURCES})

# Add a separate target for tools
add_executable(build_tool ${TOOLS_SOURCES})
# Don't define __KERNEL__ for build tools (they're not kernel code)
target_compile_definitions(build_tool PRIVATE KERNEL_MATH_EMULATION=0)
if(NOT APPLE OR CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
    set_target_properties(build_tool PROPERTIES 
        COMPILE_FLAGS "-m32"
        LINK_FLAGS "-m32"
    )
endif()

# Custom target to run the actual Makefile build
add_custom_target(
    kernel_build
    COMMAND $(MAKE) -C ${CMAKE_SOURCE_DIR}
    COMMENT "Building kernel using traditional Makefile"
    USES_TERMINAL
)

#
# Unit Tests Configuration
#

# Test include directories (NO kernel includes to avoid conflicts)
set(TEST_INCLUDES
    ${CMAKE_SOURCE_DIR}/tests
    ${CMAKE_SOURCE_DIR}/tests/mocks
)

# Test configuration removed - only kernel code tests remain
# See test_kernel_mktime and test_kernel_errno above for working tests

# NEW: Tests that link with ACTUAL kernel code for debugging
# These compile and link original Linux kernel source files!

# Include automatically generated kernel tests (50+ tests!)
include("${CMAKE_SOURCE_DIR}/kernel_tests.cmake")

# Manually configured tests that ACTUALLY WORK with original kernel code:

# Test kernel mktime - links with kernel/mktime.c (original kernel code)
add_executable(test_kernel_mktime 
    tests/kernel/test_mktime.c
    kernel/mktime.c
)
set_target_properties(test_kernel_mktime PROPERTIES 
    INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/tests;${CMAKE_SOURCE_DIR}/tests/mocks"
)
add_test(NAME KernelMkTime COMMAND test_kernel_mktime)
list(APPEND KERNEL_CODE_TESTS test_kernel_mktime)

# Note: lib/malloc.c requires linux/kernel.h and can't compile standalone
# Note: lib/string.c has x86 inline assembly and can't compile on ARM64
# Note: kernel/vsprintf.c requires kernel headers
# Note: Most kernel .c files have deep header dependencies

# Kernel subsystem tests - all use system headers only, work on ARM64
# These tests explicitly exclude kernel headers to avoid conflicts on ARM64
# NOTE: Most tests were removed because they either:
#   - Don't link actual kernel code (just test constants/macros)
#   - Require complex kernel header dependencies
#   - Have x86 inline assembly that doesn't work on ARM64

# All utility tests have been removed. Only 2 tests remain that link ORIGINAL
# kernel .c files: test_kernel_mktime and test_kernel_errno

# Collect all test targets
set(ALL_TEST_TARGETS ${KERNEL_CODE_TESTS})


# Test runner target
add_custom_target(
    run_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure --verbose
    DEPENDS ${ALL_TEST_TARGETS}
    COMMENT "Running all unit tests"
    USES_TERMINAL
)

# Documentation
message(STATUS "==================================================")
message(STATUS "Linux Kernel 0.96c - Educational Testing Framework")
message(STATUS "==================================================")
message(STATUS "This CMake configuration provides:")
message(STATUS "  1. IDE navigation (CLion, VS Code, etc)")
message(STATUS "  2. Step debugging of ORIGINAL kernel source code")
message(STATUS "")
message(STATUS "To build the actual kernel:")
message(STATUS "  - Docker: docker-compose up --build")
message(STATUS "  - Make: make (requires Linux + as86/ld86)")
message(STATUS "")
message(STATUS "UNIT TESTS:")
message(STATUS "  Tests linking ORIGINAL KERNEL CODE:")
list(LENGTH KERNEL_CODE_TESTS NUM_KERNEL_TESTS)
message(STATUS "  📦 ${NUM_KERNEL_TESTS} tests available")
message(STATUS "")
foreach(test ${KERNEL_CODE_TESTS})
    message(STATUS "    🔬 ${test}")
endforeach()
message(STATUS "")
message(STATUS "  To run all tests:")
message(STATUS "    cmake --build . --target run_tests")
message(STATUS "    OR in CLion: Run -> Run 'run_tests'")
message(STATUS "")
message(STATUS "  To step-debug kernel code:")
message(STATUS "    1. Open test file (e.g., tests/kernel/test_mktime.c)")
message(STATUS "    2. Set breakpoints in kernel/*.c or lib/*.c")
message(STATUS "    3. CLion: Right-click test -> Debug")
message(STATUS "    4. Step through ORIGINAL 1991-1993 kernel code!")
message(STATUS "==================================================")
