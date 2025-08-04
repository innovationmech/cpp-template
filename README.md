# cpp-template

A comprehensive C++ project template demonstrating modern C++ development practices with CMake build system. This template provides a solid foundation for C++ projects with multiple dependency management strategies, comprehensive testing, and cross-platform compatibility.

## Table of Contents

- [Features](#features)
- [Project Structure](#project-structure)
- [Prerequisites](#prerequisites)
- [Quick Start](#quick-start)
- [Dependency Management](#dependency-management)
- [Building the Project](#building-the-project)
- [Testing](#testing)
- [Development Environment Setup](#development-environment-setup)
- [Customization](#customization)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)

## Features

- **Modern C++ Standards**: C++17+ with explicit version specification
- **Modular Architecture**: Clear separation of concerns with hierarchical structure
- **Multiple Dependency Management**: Support for vcpkg, system packages, Git submodules, and manual dependencies
- **Comprehensive Testing**: Google Test integration with unit and integration tests
- **Cross-Platform**: Works on macOS, Linux, and Windows
- **CMake Best Practices**: Hierarchical build system with custom modules
- **Development Tools**: Code formatting, static analysis, and IDE integration

## Project Structure

```
cpp-template/
├── CMakeLists.txt              # Top-level build configuration
├── CMakePresets.json           # CMake presets for different configurations
├── README.md                   # This documentation
├── vcpkg.json                  # vcpkg manifest file
├── .gitmodules                 # Git submodules configuration
├── scripts/
│   └── setup-dev-env.sh        # Development environment setup
├── src/                        # Application source code
│   ├── CMakeLists.txt
│   ├── main.cpp                # Application entry point
│   └── modules/                # Application modules
│       ├── config_manager.cpp
│       └── data_processor.cpp
├── include/                    # Public header files
│   └── cpp-template/
│       ├── cpp-template.h      # Main public header
│       ├── core.h              # Core functionality
│       └── utils.h             # Utility functions
├── libs/                       # Internal project libraries
│   ├── CMakeLists.txt
│   └── core/                   # Example internal library
│       ├── CMakeLists.txt
│       ├── include/core/
│       └── src/
├── tests/                      # Unit and integration tests
│   ├── CMakeLists.txt
│   └── unit/
│       ├── basic_test.cpp
│       ├── core_test.cpp
│       ├── utils_test.cpp
│       └── integration_test.cpp
├── third_party/                # External dependencies
│   ├── CMakeLists.txt
│   ├── manual/                 # Manually managed dependencies
│   │   └── mathutils/          # Example manual dependency
│   └── submodules/             # Git submodule dependencies
└── cmake/                      # Custom CMake modules
    ├── CompilerOptions.cmake   # Compiler configuration
    ├── VcpkgHelpers.cmake      # vcpkg integration utilities
    ├── FindCustomLib.cmake     # Example find module
    ├── VcpkgGuide.md          # vcpkg usage documentation
    ├── GitSubmodulesGuide.md  # Git submodules documentation
    ├── ManualDependenciesGuide.md # Manual dependencies documentation
    └── SystemPackagesGuide.md # System packages documentation
```

## Prerequisites

### Essential Requirements

- **CMake 3.20 or higher**: Build system generator
- **C++17 compatible compiler**: GCC 7+, Clang 5+, MSVC 2017+, or Apple Clang 10+
- **Git**: Version control and submodule management

### Platform-Specific Requirements

#### macOS

```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install Homebrew (recommended)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install essential tools
brew install cmake git ninja
```

#### Ubuntu/Debian

```bash
# Update package list
sudo apt update

# Install essential tools
sudo apt install build-essential cmake git ninja-build pkg-config
```

#### Fedora/RHEL/CentOS

```bash
# Install development tools
sudo dnf groupinstall "Development Tools"
sudo dnf install cmake git ninja-build pkgconfig
```

#### Windows

```powershell
# Install Chocolatey (run as Administrator)
Set-ExecutionPolicy Bypass -Scope Process -Force
[System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072
iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))

# Install essential tools
choco install cmake git ninja visualstudio2022buildtools
```

## Quick Start

### 1. Clone the Repository

```bash
# Clone with submodules
git clone --recurse-submodules https://github.com/your-username/cpp-template.git
cd cpp-template

# Or clone and initialize submodules separately
git clone https://github.com/your-username/cpp-template.git
cd cpp-template
git submodule update --init --recursive
```

### 2. Set Up Development Environment

```bash
# Run the setup script (macOS/Linux)
./scripts/setup-dev-env.sh

# Or manually install dependencies (see Development Environment Setup section)
```

### 3. Build the Project

```bash
# Basic build
cmake -B build -S .
cmake --build build

# Or use presets (if available)
cmake --preset default
cmake --build build
```

### 4. Run Tests

```bash
# Run all tests
cmake --build build --target test

# Or use ctest directly
cd build
ctest --output-on-failure
```

### 5. Run the Application

```bash
# Run the example application
./build/src/cpp-template
```

## Dependency Management

This project supports four different dependency management approaches. You can use any combination of these methods based on your needs.

### 1. vcpkg (Recommended)

vcpkg is a C++ package manager that provides pre-built libraries for multiple platforms.

#### Installation

```bash
# Clone vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg

# Bootstrap vcpkg
./bootstrap-vcpkg.sh  # Linux/macOS
# or
.\bootstrap-vcpkg.bat  # Windows
```

#### Usage

```bash
# Configure with vcpkg toolchain
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake

# Build the project
cmake --build build
```

#### Adding Dependencies

Edit `vcpkg.json` to add new dependencies:

```json
{
  "dependencies": [
    "existing-package",
    "new-package",
    {
      "name": "versioned-package",
      "version>=": "1.0.0"
    }
  ]
}
```

For detailed vcpkg usage, see [CMake/VcpkgGuide.md](cmake/VcpkgGuide.md).

### 2. System Package Managers

Use system-level package managers for common libraries.

#### macOS (Homebrew)

```bash
# Install common C++ libraries
brew install boost openssl zlib curl sqlite postgresql

# Configure and build
cmake -B build -S .
cmake --build build
```

#### Ubuntu/Debian

```bash
# Install development libraries
sudo apt install libboost-all-dev libssl-dev zlib1g-dev libcurl4-openssl-dev libsqlite3-dev libpq-dev

# Configure and build
cmake -B build -S .
cmake --build build
```

For detailed system package usage, see [CMake/SystemPackagesGuide.md](cmake/SystemPackagesGuide.md).

### 3. Git Submodules

Use Git submodules for source-based dependencies.

```bash
# Add a new submodule
git submodule add https://github.com/nlohmann/json.git third_party/submodules/json

# Update all submodules
git submodule update --remote --merge

# Build with submodules
cmake -B build -S .
cmake --build build
```

For detailed submodule usage, see [CMake/GitSubmodulesGuide.md](cmake/GitSubmodulesGuide.md).

### 4. Manual Dependencies

Place libraries directly in the `third_party/manual/` directory.

```bash
# Add a manual dependency
mkdir -p third_party/manual/mylib
# Copy library files to third_party/manual/mylib/

# Build with manual dependencies
cmake -B build -S .
cmake --build build
```

For detailed manual dependency management, see [CMake/ManualDependenciesGuide.md](cmake/ManualDependenciesGuide.md).

## Building the Project

### Build Configurations

```bash
# Debug build (default)
cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# Release build
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Release with debug info
cmake -B build -S . -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build
```

### Build Options

```bash
# Enable verbose output
cmake --build build --verbose

# Build specific target
cmake --build build --target cpp-template

# Parallel build
cmake --build build --parallel 4
```

### CMake Presets (Optional)

If `CMakePresets.json` is configured:

```bash
# List available presets
cmake --list-presets

# Use a specific preset
cmake --preset vcpkg-release
cmake --build build
```

## Testing

### Running Tests

```bash
# Run all tests
cmake --build build --target test

# Run tests with verbose output
cd build
ctest --output-on-failure --verbose

# Run specific test
ctest -R "core_test"

# Run tests in parallel
ctest --parallel 4
```

### Test Categories

- **Unit Tests**: Test individual components in isolation
- **Integration Tests**: Test component interactions
- **System Tests**: Test complete application functionality

### Adding New Tests

1. Create test file in `tests/unit/`
2. Add test to `tests/unit/CMakeLists.txt`
3. Follow Google Test conventions

Example:

```cpp
#include <gtest/gtest.h>
#include "your_component.h"

TEST(YourComponentTest, BasicFunctionality) {
    // Test implementation
    EXPECT_EQ(expected, actual);
}
```

## Development Environment Setup

### Automated Setup

Use the provided setup script:

```bash
# macOS/Linux
./scripts/setup-dev-env.sh

# Windows (PowerShell as Administrator)
.\scripts\setup-dev-env.ps1
```

### Manual Setup

#### macOS

```bash
# Install Homebrew
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install development tools
brew install cmake git ninja boost openssl@3 zlib curl sqlite postgresql

# Install vcpkg (optional)
git clone https://github.com/Microsoft/vcpkg.git ~/vcpkg
~/vcpkg/bootstrap-vcpkg.sh
```

#### Linux (Ubuntu/Debian)

```bash
# Update package list
sudo apt update

# Install build tools
sudo apt install build-essential cmake git ninja-build pkg-config

# Install common libraries
sudo apt install libboost-all-dev libssl-dev zlib1g-dev libcurl4-openssl-dev libsqlite3-dev libpq-dev

# Install vcpkg (optional)
git clone https://github.com/Microsoft/vcpkg.git ~/vcpkg
~/vcpkg/bootstrap-vcpkg.sh
```

#### Windows

```powershell
# Install Chocolatey (as Administrator)
Set-ExecutionPolicy Bypass -Scope Process -Force
[System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072
iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))

# Install development tools
choco install cmake git ninja visualstudio2022buildtools

# Install vcpkg
git clone https://github.com/Microsoft/vcpkg.git C:\vcpkg
C:\vcpkg\bootstrap-vcpkg.bat
```

### IDE Configuration

#### Visual Studio Code

Install recommended extensions:

- C/C++ Extension Pack
- CMake Tools
- GitLens

#### CLion

CLion should automatically detect the CMake configuration.

#### Visual Studio

Open the folder in Visual Studio 2019+ with CMake support.

## Customization

### Adapting the Template

1. **Rename the project**: Update `CMakeLists.txt` and `vcpkg.json`
2. **Modify dependencies**: Edit dependency configuration files
3. **Update structure**: Reorganize directories as needed
4. **Customize build**: Modify CMake configuration

### Adding New Components

#### Internal Library

```bash
# Create new library directory
mkdir -p libs/newlib/{include/newlib,src}

# Add CMakeLists.txt
# Update libs/CMakeLists.txt to include new library
```

#### Application Module

```bash
# Create new module
mkdir -p src/modules/newmodule

# Add source files and update src/modules/CMakeLists.txt
```

### Configuration Options

Common customization points:

- **C++ Standard**: Change in main `CMakeLists.txt`
- **Compiler Options**: Modify `cmake/CompilerOptions.cmake`
- **Dependencies**: Update respective configuration files
- **Testing**: Modify test configuration in `tests/CMakeLists.txt`

## Troubleshooting

### Common Build Issues

#### CMake Configuration Fails

```bash
# Clear CMake cache
rm -rf build/
cmake -B build -S .
```

#### Dependency Not Found

```bash
# Check if dependency is installed
# For vcpkg:
vcpkg list

# For system packages (macOS):
brew list | grep package-name

# For system packages (Ubuntu):
dpkg -l | grep package-name
```

#### Compiler Errors

```bash
# Check compiler version
gcc --version
clang --version

# Ensure C++17 support
cmake -B build -S . -DCMAKE_CXX_STANDARD=17
```

### Platform-Specific Issues

#### macOS

- **Xcode Command Line Tools**: Run `xcode-select --install`
- **Homebrew Permissions**: `sudo chown -R $(whoami) /opt/homebrew`
- **Apple Silicon**: Ensure ARM64 compatibility

#### Linux

- **Missing Development Headers**: Install `-dev` packages
- **Permission Issues**: Use `sudo` for system package installation
- **Compiler Version**: Ensure GCC 7+ or Clang 5+

#### Windows

- **Visual Studio Build Tools**: Required for most C++ packages
- **PowerShell Execution Policy**: `Set-ExecutionPolicy RemoteSigned`
- **Path Issues**: Restart terminal after installations

### Debug Information

Enable verbose output for debugging:

```bash
# CMake verbose output
cmake -B build -S . --debug-output

# Build verbose output
cmake --build build --verbose

# Test verbose output
cd build && ctest --verbose
```

### Getting Help

1. **Check Documentation**: Review guides in `cmake/` directory
2. **Search Issues**: Look for similar problems in project issues
3. **Enable Debug Output**: Use verbose flags for detailed information
4. **Check Dependencies**: Verify all required dependencies are installed

## Contributing

### Development Workflow

1. Fork the repository
2. Create a feature branch
3. Make changes and add tests
4. Ensure all tests pass
5. Submit a pull request

### Code Standards

- Follow C++17 best practices
- Use consistent naming conventions
- Add comprehensive tests for new features
- Document public APIs
- Follow existing code style

### Testing Requirements

- All new code must have tests
- Tests must pass on all supported platforms
- Integration tests for new features
- Performance tests for critical paths

---

This template provides a solid foundation for C++ projects. Customize it according to your specific needs and requirements. For detailed information about specific aspects, refer to the documentation files in the `cmake/` directory.
