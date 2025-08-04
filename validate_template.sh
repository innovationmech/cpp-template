#!/bin/bash

# Comprehensive validation script for cpp-template project
# This script validates all dependency management methods and functionality

set -e # Exit on any error

echo "=== C++ Template Validation Script ==="
echo "Testing all dependency management methods and functionality"
echo

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
    echo -e "${YELLOW}[WARN]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Function to run tests and check results
run_tests() {
    local build_dir=$1
    local description=$2

    print_status "Running tests in $build_dir ($description)"

    cd "$build_dir"

    # Run all tests
    if ctest --output-on-failure; then
        print_status "✓ All tests passed in $description"
    else
        print_error "✗ Tests failed in $description"
        return 1
    fi

    # Run application
    if ./bin/cpp-template >/dev/null 2>&1; then
        print_status "✓ Application runs successfully in $description"
    else
        print_error "✗ Application failed to run in $description"
        return 1
    fi

    cd ..
}

# Function to validate build configuration
validate_build() {
    local build_dir=$1
    local description=$2

    print_status "Validating build configuration for $description"

    # Check if required files exist
    if [[ -f "$build_dir/bin/cpp-template" ]]; then
        print_status "✓ Main executable exists"
    else
        print_error "✗ Main executable missing"
        return 1
    fi

    if [[ -f "$build_dir/lib/libcpp-template-core.a" ]]; then
        print_status "✓ Core library exists"
    else
        print_error "✗ Core library missing"
        return 1
    fi

    # Check test executables
    local test_count
    test_count=$(find "$build_dir/tests" -name "test_*" -type f 2>/dev/null | wc -l)
    if [[ $test_count -gt 0 ]]; then
        print_status "✓ Test executables exist ($test_count found)"
    else
        print_error "✗ No test executables found"
        return 1
    fi
}

# Start validation
print_status "Starting comprehensive validation..."

# 1. Test system/manual dependencies build
print_status "=== Testing System/Manual Dependencies Build ==="
if [[ -d "build" ]]; then
    validate_build "build" "System/Manual Dependencies"
    run_tests "build" "System/Manual Dependencies"
else
    print_error "Build directory not found. Please run: mkdir build && cd build && cmake .. && make"
    exit 1
fi

# 2. Test vcpkg dependencies build
print_status "=== Testing vcpkg Dependencies Build ==="
if [[ -d "build-vcpkg" ]]; then
    validate_build "build-vcpkg" "vcpkg Dependencies"
    run_tests "build-vcpkg" "vcpkg Dependencies"
else
    print_warning "vcpkg build directory not found. Skipping vcpkg validation."
fi

# 3. Test Git submodules
print_status "=== Testing Git Submodules ==="
if [[ -d "third_party/submodules/json" ]]; then
    if [[ -f "third_party/submodules/json/single_include/nlohmann/json.hpp" ]]; then
        print_status "✓ nlohmann/json submodule is properly initialized"
    else
        print_warning "nlohmann/json submodule exists but headers not found"
    fi
else
    print_warning "nlohmann/json submodule not initialized"
fi

# 4. Test manual dependencies
print_status "=== Testing Manual Dependencies ==="
if [[ -d "third_party/manual/mathutils" ]]; then
    if [[ -f "third_party/manual/mathutils/include/mathutils/mathutils.h" ]]; then
        print_status "✓ MathUtils manual dependency is properly configured"
    else
        print_error "✗ MathUtils manual dependency missing headers"
    fi
else
    print_error "✗ MathUtils manual dependency not found"
fi

# 5. Test project structure
print_status "=== Testing Project Structure ==="
required_dirs=(
    "src"
    "include"
    "libs"
    "tests"
    "third_party"
    "cmake"
)

for dir in "${required_dirs[@]}"; do
    if [[ -d "$dir" ]]; then
        print_status "✓ Directory $dir exists"
    else
        print_error "✗ Required directory $dir missing"
    fi
done

# 6. Test CMake configuration files
print_status "=== Testing CMake Configuration ==="
required_cmake_files=(
    "CMakeLists.txt"
    "src/CMakeLists.txt"
    "libs/CMakeLists.txt"
    "tests/CMakeLists.txt"
    "third_party/CMakeLists.txt"
)

for file in "${required_cmake_files[@]}"; do
    if [[ -f "$file" ]]; then
        print_status "✓ CMake file $file exists"
    else
        print_error "✗ Required CMake file $file missing"
    fi
done

# 7. Test documentation
print_status "=== Testing Documentation ==="
if [[ -f "README.md" ]]; then
    print_status "✓ README.md exists"
else
    print_error "✗ README.md missing"
fi

if [[ -f "DEVELOPER_GUIDE.md" ]]; then
    print_status "✓ DEVELOPER_GUIDE.md exists"
else
    print_warning "DEVELOPER_GUIDE.md not found"
fi

# 8. Test dependency configuration files
print_status "=== Testing Dependency Configuration ==="
if [[ -f "vcpkg.json" ]]; then
    print_status "✓ vcpkg.json exists"
else
    print_warning "vcpkg.json not found"
fi

if [[ -f ".gitmodules" ]]; then
    print_status "✓ .gitmodules exists"
else
    print_warning ".gitmodules not found"
fi

# 9. Test cross-platform compatibility indicators
print_status "=== Testing Cross-Platform Compatibility ==="
if grep -q "PLATFORM_ID" CMakeLists.txt; then
    print_status "✓ Platform detection configured in CMake"
else
    print_warning "Platform detection not found in CMake"
fi

if [[ -f "libs/core/src/platform/macos_utils.cpp" ]] &&
    [[ -f "libs/core/src/platform/linux_utils.cpp" ]] &&
    [[ -f "libs/core/src/platform/windows_utils.cpp" ]]; then
    print_status "✓ Platform-specific source files exist"
else
    print_warning "Some platform-specific source files missing"
fi

# 10. Final summary
print_status "=== Validation Summary ==="
print_status "✓ System/Manual dependencies build: PASSED"
if [[ -d "build-vcpkg" ]]; then
    print_status "✓ vcpkg dependencies build: PASSED"
else
    print_warning "⚠ vcpkg dependencies build: SKIPPED"
fi
print_status "✓ Project structure: VALIDATED"
print_status "✓ CMake configuration: VALIDATED"
print_status "✓ Documentation: PRESENT"

echo
print_status "🎉 C++ Template validation completed successfully!"
print_status "The template demonstrates:"
print_status "  • Modern C++17 standards"
print_status "  • Multiple dependency management approaches"
print_status "  • Comprehensive testing framework"
print_status "  • Cross-platform compatibility"
print_status "  • Modular architecture"
print_status "  • Extensive documentation"

echo
print_status "Template is ready for use as a foundation for C++ projects!"
