# ============================================
# ORIGINAL KERNEL CODE TESTS (Working Tests Only)
# Each test links with actual 1991-1993 Linux kernel source
# ============================================

# Test: Error number handling - lib/errno.c (ORIGINAL KERNEL CODE)
# This is one of the few kernel files that can compile standalone
add_executable(test_kernel_errno
    tests/lib/test_kernel_errno.c
    lib/errno.c
)
set_target_properties(test_kernel_errno PROPERTIES
    INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/tests;${CMAKE_SOURCE_DIR}/tests/mocks"
)
add_test(NAME test_kernel_errno COMMAND test_kernel_errno)
list(APPEND KERNEL_CODE_TESTS test_kernel_errno)

# Note: test_kernel_mktime is defined separately in CMakeLists.txt
# Note: Most other kernel .c files require kernel headers and cannot
#       be compiled standalone without the full kernel build environment
