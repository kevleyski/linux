# CLion-specific CMake configuration
# This file tells CLion to use the system compiler instead of cached paths

# Force use of system compiler
if(EXISTS "/usr/bin/clang")
    set(CMAKE_C_COMPILER "/usr/bin/clang" CACHE FILEPATH "C compiler" FORCE)
    set(CMAKE_CXX_COMPILER "/usr/bin/clang++" CACHE FILEPATH "C++ compiler" FORCE)
    message(STATUS "Using system clang: /usr/bin/clang")
endif()

# Force Debug mode with full symbols and no optimizations
set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
message(STATUS "Build type forced to Debug (-g -ggdb -O0)")

# Don't pass -m32 on ARM64 for the main library target
if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64")
    message(STATUS "Detected ARM64 - skipping -m32 flag")
endif()

