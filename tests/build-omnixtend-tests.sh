#!/bin/bash

# OmniXtend Memory Test Build Script
# This script builds the OmniXtend memory test programs

set -e

echo "=== Building OmniXtend Memory Tests ==="

# Check if we're in the tests directory
if [ ! -f "CMakeLists.txt" ]; then
    echo "Error: Please run this script from the tests directory"
    exit 1
fi

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    echo "Creating build directory..."
    mkdir -p build
fi

# Configure with CMake
echo "Configuring with CMake..."
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Build the OmniXtend tests
echo "Building OmniXtend tests..."
make omnixtend-simple-test
make omnixtend-getput-test
make omnixtend-simple-write
make omnixtend-simple-read

# Generate disassembly files
echo "Generating disassembly files..."
make omnixtend-simple-test-dump
make omnixtend-getput-test-dump
make omnixtend-simple-write-dump
make omnixtend-simple-read-dump

cd ..

# Copy binaries to generators/testchipip/uart_tsi directory
echo "Copying binaries to generators/testchipip/uart_tsi..."
TARGET_DIR="../generators/testchipip/uart_tsi"
if [ ! -d "$TARGET_DIR" ]; then
    echo "Creating target directory: $TARGET_DIR"
    mkdir -p "$TARGET_DIR"
fi

cp build/omnixtend-simple-test.riscv "$TARGET_DIR/"
cp build/omnixtend-getput-test.riscv "$TARGET_DIR/"
cp build/omnixtend-simple-write.riscv "$TARGET_DIR/"
cp build/omnixtend-simple-read.riscv "$TARGET_DIR/"

echo "=== Build completed successfully ==="
echo "Executables created:"
echo "  - build/omnixtend-simple-test.riscv"
echo "  - build/omnixtend-getput-test.riscv"
echo "  - build/omnixtend-simple-write.riscv"
echo "  - build/omnixtend-simple-read.riscv"
echo ""
echo "Binaries copied to: $TARGET_DIR/"
echo ""
echo "To run the tests (recommended order):"
echo "  1. ./build/omnixtend-simple-test.riscv (basic test)"
echo "  2. ./build/omnixtend-simple-write.riscv (simple write test)"
echo "  3. ./build/omnixtend-simple-read.riscv (simple read test)"
echo "  4. ./build/omnixtend-getput-test.riscv (basic GET/PUT test)" 