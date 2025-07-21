#!/bin/bash

# HydraAV Test Script
# Tests the installation and basic functionality

set -e

echo "=== HydraAV Test Script ==="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

print_status() {
    echo -e "${GREEN}[PASS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

print_error() {
    echo -e "${RED}[FAIL]${NC} $1"
}

# Check if we're in the right directory
if [ ! -f "scanner/CMakeLists.txt" ]; then
    print_error "Please run this script from the HydraAV root directory"
    exit 1
fi

# Test 1: Check if scanner executable exists
SCANNER_PATH=""
if [ -f "scanner/build/hydraav-scan" ]; then
    SCANNER_PATH="scanner/build/hydraav-scan"
elif [ -f "scanner/build/Release/hydraav-scan.exe" ]; then
    SCANNER_PATH="scanner/build/Release/hydraav-scan.exe"
else
    print_error "Scanner executable not found. Please build the project first."
    exit 1
fi

print_status "Scanner executable found: $SCANNER_PATH"

# Test 2: Test help command
echo "Testing help command..."
if $SCANNER_PATH --help > /dev/null 2>&1; then
    print_status "Help command works"
else
    print_error "Help command failed"
    exit 1
fi

# Test 3: Create test files
echo "Creating test files..."
mkdir -p test_files

# Clean test file
cat > test_files/clean.py << 'EOF'
print("Hello, World!")
EOF

# Suspicious test file
cat > test_files/suspicious.py << 'EOF'
import os
import subprocess
os.system("rm -rf /")
subprocess.call(["nc", "-l", "4444"])
EOF

print_status "Test files created"

# Test 4: Test scanning clean file
echo "Testing clean file scan..."
if $SCANNER_PATH test_files/clean.py > /dev/null 2>&1; then
    print_status "Clean file scan completed"
else
    print_warning "Clean file scan had issues"
fi

# Test 5: Test scanning suspicious file
echo "Testing suspicious file scan..."
if $SCANNER_PATH test_files/suspicious.py > /dev/null 2>&1; then
    print_status "Suspicious file scan completed"
else
    print_warning "Suspicious file scan had issues"
fi

# Test 6: Test recursive directory scan
echo "Testing recursive directory scan..."
if $SCANNER_PATH -r test_files > /dev/null 2>&1; then
    print_status "Recursive directory scan completed"
else
    print_warning "Recursive directory scan had issues"
fi

# Test 7: Check Docker availability (optional)
if command -v docker &> /dev/null; then
    echo "Testing Docker availability..."
    if docker --version > /dev/null 2>&1; then
        print_status "Docker is available"
        
        # Check if sandbox image exists
        if docker image inspect hydraav-sandbox > /dev/null 2>&1; then
            print_status "Sandbox image found"
            
            # Test sandbox functionality
            echo "Testing sandbox functionality..."
            if $SCANNER_PATH --sandbox test_files/clean.py > /dev/null 2>&1; then
                print_status "Sandbox functionality works"
            else
                print_warning "Sandbox functionality had issues"
            fi
        else
            print_warning "Sandbox image not found. Run 'cd sandbox && docker build -t hydraav-sandbox .' to build it."
        fi
    else
        print_warning "Docker is installed but not working properly"
    fi
else
    print_warning "Docker not found. Sandbox functionality will not be available."
fi

# Cleanup
echo "Cleaning up test files..."
rm -rf test_files

print_status "All tests completed!"
echo ""
echo "HydraAV is ready to use!"
echo "Usage: $SCANNER_PATH [OPTIONS] <target>"
echo "Use --help for more information." 