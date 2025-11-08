#!/bin/bash
# Simple test runner script for macOS

set -e

echo "=========================================="
echo "  Linux Kernel 0.96c - Unit Test Runner"
echo "=========================================="
echo ""

# Check if cmake build directory exists
if [ ! -d "cmake-build-debug" ]; then
    echo "Creating build directory..."
    cmake -S . -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug
fi

echo "Building tests..."
cmake --build cmake-build-debug --target run_tests

echo ""
echo "=========================================="
echo "  All tests completed!"
echo "=========================================="

