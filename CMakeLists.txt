cmake_minimum_required(VERSION 3.10)
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
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-main -Wno-implicit-function-declaration")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-int-conversion -Wno-incompatible-library-redeclaration")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-incompatible-pointer-types -Wno-deprecated-declarations")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-builtin")

# For Apple Silicon Macs, don't force -m32 (incompatible)
if(NOT APPLE OR CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
endif()

set(CMAKE_C_FLAGS_DEBUG "-g -ggdb -O0")

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
target_compile_definitions(build_tool PRIVATE -UKERNEL_MATH_EMULATION -U__KERNEL__)
set_target_properties(build_tool PROPERTIES 
    COMPILE_FLAGS "-m32"
    LINK_FLAGS "-m32"
)

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

# Test for lib/ functions (string, malloc, etc) - uses system headers only  
# This test avoids ALL kernel headers to work on ARM64
add_executable(test_string_functions
    tests/lib/test_string.c
)
target_include_directories(test_string_functions PRIVATE ${TEST_INCLUDES})
# Explicitly exclude kernel include directories for this test
set_target_properties(test_string_functions PROPERTIES 
    INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/tests;${CMAKE_SOURCE_DIR}/tests/mocks"
)
add_test(NAME StringFunctions COMMAND test_string_functions)

# Test for vsprintf - needs kernel include, x86 only due to inline assembly
if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|i386|i686")
    add_executable(test_vsprintf
        tests/kernel/test_vsprintf.c
        kernel/vsprintf.c
    )
    target_include_directories(test_vsprintf PRIVATE 
        ${TEST_INCLUDES}
        ${CMAKE_SOURCE_DIR}/include
    )
    target_compile_definitions(test_vsprintf PRIVATE __KERNEL__=)
    add_test(NAME VSprintf COMMAND test_vsprintf)
    set(VSPRINTF_TEST test_vsprintf)
else()
    message(STATUS "Skipping test_vsprintf on ${CMAKE_SYSTEM_PROCESSOR} (requires x86 inline assembly)")
    set(VSPRINTF_TEST "")
endif()

# Test for ctype functions - x86 only due to inline assembly in dependencies
if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|i386|i686")
    add_executable(test_ctype
        tests/lib/test_ctype.c
        lib/ctype.c
    )
    target_include_directories(test_ctype PRIVATE 
        ${TEST_INCLUDES}
        ${CMAKE_SOURCE_DIR}/include
    )
    target_compile_definitions(test_ctype PRIVATE __KERNEL__=)
    add_test(NAME CType COMMAND test_ctype)
    set(CTYPE_TEST test_ctype)
else()
    message(STATUS "Skipping test_ctype on ${CMAKE_SYSTEM_PROCESSOR} (requires x86 inline assembly)")
    set(CTYPE_TEST "")
endif()

# Test for string functions from lib (disabled on ARM64 due to inline assembly)
if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|i386|i686")
    add_executable(test_lib_string
        tests/lib/test_lib_string.c
        lib/string.c
    )
    target_include_directories(test_lib_string PRIVATE ${TEST_INCLUDES})
    target_compile_definitions(test_lib_string PRIVATE __KERNEL__=)
    add_test(NAME LibString COMMAND test_lib_string)
    set(LIB_STRING_TEST test_lib_string)
else()
    message(STATUS "Skipping test_lib_string on ${CMAKE_SYSTEM_PROCESSOR} (requires x86 inline assembly)")
    set(LIB_STRING_TEST "")
endif()

# Test runner target
add_custom_target(
    run_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure --verbose
    DEPENDS test_string_functions ${VSPRINTF_TEST} ${CTYPE_TEST} ${LIB_STRING_TEST}
    COMMENT "Running all unit tests"
    USES_TERMINAL
)

# Documentation
message(STATUS "==================================================")
message(STATUS "Linux Kernel 0.96c - CLion Configuration")
message(STATUS "==================================================")
message(STATUS "This CMake configuration is for IDE navigation only.")
message(STATUS "To actually build the kernel, use:")
message(STATUS "  1. Docker: docker-compose up --build")
message(STATUS "  2. Make: make (requires Linux + as86/ld86)")
message(STATUS "  3. CMake: make kernel_build")
message(STATUS "")
message(STATUS "To run unit tests:")
message(STATUS "  1. Build: cmake --build . --target run_tests")
message(STATUS "  2. Or in CLion: Run -> Run 'run_tests'")
message(STATUS "==================================================")
