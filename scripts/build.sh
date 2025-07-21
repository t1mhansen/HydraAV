#!/bin/bash

# HydraAV Build Script
# Builds the static scanner and optionally the sandbox

set -e

echo "=== HydraAV Build Script ==="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if we're in the right directory
if [ ! -f "scanner/CMakeLists.txt" ]; then
    print_error "Please run this script from the HydraAV root directory"
    exit 1
fi

# Build static scanner
print_status "Building static scanner..."
cd scanner

# Create build directory
mkdir -p build
cd build

# Configure with CMake
print_status "Configuring with CMake..."
cmake ..

# Build
print_status "Building scanner..."
make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

# Check if build was successful
if [ -f "hydraav-scan" ] || [ -f "hydraav-scan.exe" ]; then
    print_status "Static scanner built successfully!"
else
    print_error "Failed to build static scanner"
    exit 1
fi

cd ../..

# Check if Docker is available for sandbox build
if command -v docker &> /dev/null; then
    print_status "Docker found. Building sandbox..."
    
    # Check if user wants to build sandbox
    if [ "$1" = "--sandbox" ] || [ "$1" = "-s" ]; then
        cd sandbox
        print_status "Building Docker sandbox image..."
        docker build -t hydraav-sandbox .
        
        if [ $? -eq 0 ]; then
            print_status "Sandbox built successfully!"
        else
            print_error "Failed to build sandbox"
            exit 1
        fi
        
        cd ..
    else
        print_warning "Docker available but sandbox not requested. Use --sandbox to build it."
    fi
else
    print_warning "Docker not found. Sandbox functionality will not be available."
    print_warning "Install Docker to enable dynamic analysis features."
fi

print_status "Build completed successfully!"
print_status "You can now use: ./scanner/build/hydraav-scan <target>" 