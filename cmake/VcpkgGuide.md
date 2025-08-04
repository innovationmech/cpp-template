# vcpkg Package Management Guide

This document provides comprehensive instructions for using vcpkg with the cpp-template project.

## Overview

vcpkg is a C++ package manager that simplifies the process of acquiring and building open source libraries on Windows, Linux, and macOS. This project uses vcpkg in manifest mode, which allows dependencies to be declared in a `vcpkg.json` file.

## Installation

### Prerequisites

- Git
- CMake 3.20 or higher
- A C++ compiler (Visual Studio, GCC, or Clang)

### Installing vcpkg

1. **Clone vcpkg repository:**

   ```bash
   git clone https://github.com/Microsoft/vcpkg.git
   cd vcpkg
   ```

2. **Bootstrap vcpkg:**

   **On Windows:**

   ```cmd
   .\bootstrap-vcpkg.bat
   ```

   **On Linux/macOS:**

   ```bash
   ./bootstrap-vcpkg.sh
   ```

3. **Add vcpkg to your PATH (optional but recommended):**

   **On Windows:**

   ```cmd
   set PATH=%PATH%;C:\path\to\vcpkg
   ```

   **On Linux/macOS:**

   ```bash
   export PATH=$PATH:/path/to/vcpkg
   ```

## Project Configuration

### Manifest Mode

This project uses vcpkg's manifest mode, which means dependencies are declared in the `vcpkg.json` file at the project root. This approach provides several benefits:

- **Reproducible builds**: Dependencies are versioned and locked
- **Project isolation**: Each project manages its own dependencies
- **Simplified CI/CD**: No need to pre-install packages

### vcpkg.JSON Structure

The `vcpkg.json` file contains:

```json
{
  "name": "cpp-template",
  "version": "1.0.0",
  "description": "Modern C++ Project Template",
  "dependencies": [
    "fmt",
    "spdlog",
    {
      "name": "nlohmann-json",
      "version>=": "3.11.0"
    }
  ],
  "features": {
    "networking": {
      "description": "Enable networking capabilities",
      "dependencies": ["curl", "openssl"]
    }
  }
}
```

### Dependency Types

1. **Simple dependencies**: Just specify the package name

   ```json
   "fmt"
   ```

2. **Versioned dependencies**: Specify minimum version requirements

   ```json
   {
     "name": "nlohmann-json",
     "version>=": "3.11.0"
   }
   ```

3. **Platform-specific dependencies**: Only install on certain platforms

   ```json
   {
     "name": "cpprestsdk",
     "platform": "!uwp"
   }
   ```

4. **Feature dependencies**: Optional features that can be enabled

   ```json
   {
     "name": "gtest",
     "features": ["gmock"]
   }
   ```

## Building with vcpkg

### Method 1: Using CMake Toolchain File

```bash
# Configure with vcpkg toolchain
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake

# Build the project
cmake --build build
```

### Method 2: Using vcpkg integrate

```bash
# Install vcpkg integration (one-time setup)
vcpkg integrate install

# Configure and build normally
cmake -B build -S .
cmake --build build
```

### Method 3: Using Preset (Recommended)

Create a `CMakePresets.json` file:

```json
{
  "version": 3,
  "configurePresets": [
    {
      "name": "vcpkg",
      "displayName": "vcpkg Configuration",
      "description": "Configure with vcpkg toolchain",
      "toolchainFile": "$env{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Release"
      }
    }
  ]
}
```

Then build with:

```bash
cmake --preset vcpkg
cmake --build build
```

## Managing Dependencies

### Adding New Dependencies

1. **Find the package**: Search for available packages

   ```bash
   vcpkg search <package-name>
   ```

2. **Add to vcpkg.JSON**: Update the dependencies array

   ```json
   "dependencies": [
     "existing-package",
     "new-package"
   ]
   ```

3. **Reconfigure**: CMake will automatically install the new dependency

   ```bash
   cmake --build build --target clean
   cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
   ```

### Using Features

Enable optional features when configuring:

```bash
cmake -B build -S . \
  -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake \
  -DVCPKG_MANIFEST_FEATURES="networking;database"
```

### Version Constraints

Supported version constraints:

- `"version>=": "1.0.0"` - Minimum version
- `"version": "1.0.0"` - Exact version
- `"version-string": "master"` - String version (for git refs)

## CMake Integration

### Finding Packages

Use standard CMake `find_package()` commands:

```cmake
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

target_link_libraries(your_target
  PRIVATE
    fmt::fmt
    spdlog::spdlog
    nlohmann_json::nlohmann_json
)
```

### Helper Functions

The project provides helper functions in `VcpkgHelpers.cmake`:

```cmake
# Check if vcpkg is being used
check_vcpkg_integration()

# Find package with fallback options
find_package_with_fallback(MyPackage
  VCPKG_NAME my-package
  SYSTEM_NAME mypackage
)
```

## Troubleshooting

### Common Issues

1. **vcpkg not found**
   - Ensure vcpkg is in your PATH or specify full path to toolchain file
   - Verify vcpkg is properly bootstrapped

2. **Package not found**
   - Check if package name is correct: `vcpkg search <name>`
   - Verify the package supports your target triplet

3. **Version conflicts**
   - Use `overrides` in vcpkg.JSON to force specific versions
   - Check dependency tree: `vcpkg depend-info <package>`

4. **Build failures**
   - Clean build directory and reconfigure
   - Check compiler compatibility
   - Verify all dependencies are available for your platform

### Debug Information

Enable verbose output for debugging:

```bash
cmake -B build -S . \
  -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake \
  -DVCPKG_VERBOSE=ON
```

### Platform-Specific Notes

**Windows:**

- Use Visual Studio Developer Command Prompt
- Default triplet: `x64-windows`

**macOS:**

- Install Xcode command line tools
- Default triplet: `x64-osx`

**Linux:**

- Install build essentials
- Default triplet: `x64-linux`

## Best Practices

1. **Pin versions**: Use version constraints for reproducible builds
2. **Use features**: Organize optional dependencies with features
3. **Document requirements**: Keep this guide updated with project needs
4. **Test regularly**: Verify builds work on clean environments
5. **Cache vcpkg**: Use binary caching for faster CI builds

## Example Usage

Here's a complete example of adding and using a new dependency:

1. **Add to vcpkg.JSON:**

   ```json
   "dependencies": [
     "boost-system"
   ]
   ```

2. **Use in CMakeLists.txt:**

   ```cmake
   find_package(Boost REQUIRED COMPONENTS system)
   target_link_libraries(your_target PRIVATE Boost::system)
   ```

3. **Use in code:**

   ```cpp
   #include <boost/system/error_code.hpp>

   void example() {
       boost::system::error_code ec;
       // Use boost functionality
   }
   ```

For more information, visit the [vcpkg documentation](https://vcpkg.io/en/docs/).
