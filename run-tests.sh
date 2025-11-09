#!/usr/bin/env bash
# Improved test runner for macOS/Linux
set -euo pipefail

echo "=========================================="
echo "  Linux Kernel - Unit Test Runner"
echo "=========================================="
echo ""

BUILD_DIR="cmake-build-debug"

# Ensure required tools exist
require_cmd() {
    if ! command -v "$1" >/dev/null 2>&1; then
        echo "Error: '$1' not found in PATH. Please install it and try again."
        exit 1
    fi
}

require_cmd cmake

# Detect a usable C compiler (allow CC env override)
DETECTED_CC=""
if [ -n "${CC:-}" ]; then
    DETECTED_CC="$(command -v "$CC" 2>/dev/null || echo "$CC")"
fi

if [ -z "$DETECTED_CC" ]; then
    if command -v clang >/dev/null 2>&1; then
        DETECTED_CC="$(command -v clang)"
    elif command -v gcc >/dev/null 2>&1; then
        DETECTED_CC="$(command -v gcc)"
    elif command -v cc >/dev/null 2>&1; then
        DETECTED_CC="$(command -v cc)"
    fi
fi

if [ -z "$DETECTED_CC" ]; then
    echo "Error: No C compiler found. On macOS install the Xcode command line tools (xcode-select --install) or 'brew install gcc'."
    exit 1
fi

echo "Using C compiler: $DETECTED_CC"

# Configure the project (always run configure to pick up changes)
mkdir -p "$BUILD_DIR"

echo "Configuring project..."
cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER="$DETECTED_CC"

# Determine parallel jobs
if command -v sysctl >/dev/null 2>&1; then
    JOBS=$(sysctl -n hw.ncpu 2>/dev/null || echo 1)
else
    JOBS=1
fi

echo "Building tests... (jobs=$JOBS)"
# Try to build the run_tests target; fall back to default if not present
if cmake --build "$BUILD_DIR" --target run_tests -- -j"$JOBS"; then
    echo "Build finished."
else
    echo "Warning: 'run_tests' target not available or build failed; attempting full build."
    cmake --build "$BUILD_DIR" -- -j"$JOBS"
fi

echo ""
# Run tests: prefer ctest (gives nicer output), otherwise run the test binary
if command -v ctest >/dev/null 2>&1; then
    echo "Running tests with ctest..."
    (cd "$BUILD_DIR" && ctest --output-on-failure -C Debug) || {
        echo "ctest reported failures. Exit status: $?"
        exit 1
    }
else
    TEST_BIN="$BUILD_DIR/run_tests"
    if [ -x "$TEST_BIN" ]; then
        echo "Running test binary..."
        "$TEST_BIN"
    else
        echo "No test runner found. Built files are in: $BUILD_DIR"
        exit 1
    fi
fi

echo ""
echo "=========================================="
echo "  All tests completed!"
echo "=========================================="

