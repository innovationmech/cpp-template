#!/bin/bash
# =============================================================================
# CI Stability Test Script
# This script helps test the optimized CI configuration locally
# =============================================================================

set -e

echo "🔧 CI Stability Test Script"
echo "==========================="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${GREEN}✅ $1${NC}"
}

print_warning() {
    echo -e "${YELLOW}⚠️  $1${NC}"
}

print_error() {
    echo -e "${RED}❌ $1${NC}"
}

# Check if we're in the right directory
if [ ! -f "CMakeLists.txt" ]; then
    print_error "Please run this script from the project root directory"
    exit 1
fi

print_status "Starting CI stability tests..."

# Test 1: Sequential build test
echo ""
echo "📦 Test 1: Sequential Build Test"
echo "--------------------------------"

if [ -d "build" ]; then
    print_warning "Removing existing build directory"
    rm -rf build
fi

print_status "Configuring CMake with optimized settings..."
cmake -B build -S . \
    -DCMAKE_BUILD_TYPE=Debug \
    -DBUILD_TESTING=ON \
    -DENABLE_CLANG_TIDY=OFF \
    -DENABLE_CPPCHECK=OFF \
    -G Ninja

print_status "Building with reduced parallelism (parallel=1)..."
cmake --build build --parallel 1

# Test 2: Sequential test execution
echo ""
echo "🧪 Test 2: Sequential Test Execution"
echo "-----------------------------------"

cd build

print_status "Running tests sequentially (parallel=1)..."
export CTEST_PARALLEL_LEVEL=1
export CTEST_TIMEOUT=600
export GTEST_SHUFFLE=1
export GTEST_RANDOM_SEED=12345

# Create isolated test directory
mkdir -p test_output_stability_test
TMPDIR="$(pwd)/test_output_stability_test"
export TMPDIR

print_status "Running all tests with extended timeout..."
ctest --build-config Debug \
    --parallel 1 \
    --timeout 600 \
    --output-on-failure \
    --verbose \
    --output-log test_output_stability_test/ctest.log

# Test 3: Multiple test runs for stability
echo ""
echo "🔄 Test 3: Stability Test (Multiple Runs)"
echo "----------------------------------------"

print_status "Running tests multiple times to check for flakiness..."

for i in {1..3}; do
    echo "Run $i/3:"
    if ctest --build-config Debug --parallel 1 --timeout 300 --output-on-failure >"test_run_$i.log" 2>&1; then
        print_status "Run $i: PASSED"
    else
        print_error "Run $i: FAILED"
        echo "Check test_run_$i.log for details"
    fi
done

# Test 4: Resource usage monitoring
echo ""
echo "📊 Test 4: Resource Usage Check"
echo "------------------------------"

print_status "Checking system resources during test..."
echo "Memory usage:"
free -h || echo "free command not available"

echo "CPU info:"
nproc || echo "nproc command not available"

echo "Disk usage:"
df -h . || echo "df command not available"

cd ..

# Test 5: Cleanup and summary
echo ""
echo "🧹 Test 5: Cleanup and Summary"
echo "-----------------------------"

print_status "Test logs generated:"
find build -name "*.log" -type f | head -10

print_status "Test output directories:"
find build -name "test_output_*" -type d

echo ""
print_status "CI Stability Test Completed!"
echo ""
echo "📋 Summary:"
echo "- Build parallelism: 1 (reduced from 4)"
echo "- Test parallelism: 1 (reduced from 4)"
echo "- Test timeout: 600s (increased from 300s)"
echo "- Test isolation: Enabled with separate temp directories"
echo "- Concurrency control: cancel-in-progress disabled"
echo ""
print_status "These optimizations should improve CI stability by:"
echo "  • Reducing resource contention"
echo "  • Preventing test interference"
echo "  • Avoiding premature cancellation"
echo "  • Providing better error isolation"

echo ""
print_warning "To apply these settings to your CI:"
echo "  1. The workflow files have been updated"
echo "  2. Push changes to trigger the optimized CI"
echo "  3. Monitor CI runs for improved stability"
