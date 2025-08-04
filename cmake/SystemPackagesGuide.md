# System Package Manager Dependencies Guide

This document provides comprehensive instructions for using system package managers with the cpp-template project, with a focus on macOS Homebrew.

## Overview

System package managers provide pre-built libraries and tools that are installed system-wide. This approach is useful for:

- Common system libraries (OpenSSL, zlib, etc.)
- Development tools (CMake, Git, compilers)
- Large libraries that are commonly used across projects
- Libraries that require system integration
- Dependencies that other system software relies on

## Advantages and Disadvantages

### Advantages

- **Fast installation**: Pre-built binaries install quickly
- **System integration**: Proper integration with system services
- **Automatic updates**: Package manager handles updates and security patches
- **Dependency resolution**: Automatic handling of transitive dependencies
- **Disk space efficiency**: Shared libraries reduce disk usage
- **Maintenance**: Package maintainers handle build configuration

### Disadvantages

- **Version constraints**: Limited to available package versions
- **System dependency**: Requires specific package manager on target system
- **Potential conflicts**: Different projects may need different versions
- **Platform specific**: Different commands and packages across platforms
- **Root access**: May require administrator privileges
- **Build environment**: Builds depend on system configuration

## Platform Support

### macOS - Homebrew

Homebrew is the recommended package manager for macOS development.

**Installation:**

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

**Common Development Dependencies:**

```bash
# Essential build tools
brew install cmake
brew install git
brew install ninja

# Compilers and toolchains
brew install llvm
brew install gcc

# Common C++ libraries
brew install boost
brew install openssl@3
brew install zlib
brew install curl
brew install sqlite
brew install postgresql
brew install mysql

# Development utilities
brew install pkg-config
brew install autoconf
brew install automake
brew install libtool

# Optional: vcpkg for additional package management
brew install vcpkg
```

### Linux - APT (Ubuntu/Debian)

**Common Development Dependencies:**

```bash
# Essential build tools
sudo apt update
sudo apt install build-essential cmake git ninja-build

# Common C++ libraries
sudo apt install libboost-all-dev
sudo apt install libssl-dev
sudo apt install zlib1g-dev
sudo apt install libcurl4-openssl-dev
sudo apt install libsqlite3-dev
sudo apt install libpq-dev
sudo apt install libmysqlclient-dev

# Development utilities
sudo apt install pkg-config
sudo apt install autoconf
sudo apt install automake
sudo apt install libtool-bin
```

### Linux - YUM/DNF (RHEL/Fedora/CentOS)

**Common Development Dependencies:**

```bash
# Essential build tools
sudo dnf groupinstall "Development Tools"
sudo dnf install cmake git ninja-build

# Common C++ libraries
sudo dnf install boost-devel
sudo dnf install openssl-devel
sudo dnf install zlib-devel
sudo dnf install libcurl-devel
sudo dnf install sqlite-devel
sudo dnf install postgresql-devel
sudo dnf install mysql-devel

# Development utilities
sudo dnf install pkgconfig
sudo dnf install autoconf
sudo dnf install automake
sudo dnf install libtool
```

### Windows - Chocolatey

**Installation:**

```powershell
Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
```

**Common Development Dependencies:**

```powershell
# Essential build tools
choco install cmake
choco install git
choco install ninja

# Compilers
choco install visualstudio2022buildtools
choco install llvm

# Development utilities
choco install vcpkg
```

## CMake Integration

### Finding System Packages

The project uses CMake's `find_package()` mechanism to locate system-installed libraries:

```cmake
# Standard CMake package finding
find_package(Boost REQUIRED COMPONENTS system filesystem thread)
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(CURL REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(PostgreSQL REQUIRED)

# Link against found packages
target_link_libraries(your_target
    PRIVATE
        Boost::system
        Boost::filesystem
        Boost::thread
        OpenSSL::SSL
        OpenSSL::Crypto
        ZLIB::ZLIB
        CURL::libcurl
        SQLite::SQLite3
        PostgreSQL::PostgreSQL
)
```

### Custom Find Modules

For packages without built-in CMake support, create custom find modules in `cmake/`:

```cmake
# cmake/FindCustomLib.cmake
find_path(CUSTOMLIB_INCLUDE_DIR
    NAMES customlib/customlib.h
    PATHS
        /usr/local/include
        /usr/include
        /opt/homebrew/include  # macOS Homebrew ARM64
        /opt/local/include     # macOS MacPorts
    PATH_SUFFIXES customlib
)

find_library(CUSTOMLIB_LIBRARY
    NAMES customlib libcustomlib
    PATHS
        /usr/local/lib
        /usr/lib
        /opt/homebrew/lib      # macOS Homebrew ARM64
        /opt/local/lib         # macOS MacPorts
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CustomLib
    REQUIRED_VARS CUSTOMLIB_LIBRARY CUSTOMLIB_INCLUDE_DIR
)

if(CustomLib_FOUND AND NOT TARGET CustomLib::CustomLib)
    add_library(CustomLib::CustomLib UNKNOWN IMPORTED)
    set_target_properties(CustomLib::CustomLib PROPERTIES
        IMPORTED_LOCATION "${CUSTOMLIB_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${CUSTOMLIB_INCLUDE_DIR}"
    )
endif()
```

### Platform-Specific Configuration

Handle platform differences in CMake:

```cmake
# Platform-specific package finding
if(APPLE)
    # macOS-specific configuration
    if(EXISTS "/opt/homebrew")
        # Apple Silicon Homebrew
        list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")
    elseif(EXISTS "/usr/local")
        # Intel Homebrew
        list(APPEND CMAKE_PREFIX_PATH "/usr/local")
    endif()

    # macOS-specific packages
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(EXAMPLE libexample)

elseif(UNIX AND NOT APPLE)
    # Linux-specific configuration
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

elseif(WIN32)
    # Windows-specific configuration
    find_package(unofficial-sqlite3 CONFIG REQUIRED)
endif()
```

## Project Configuration

### Environment Setup Script

Create a setup script for easy environment configuration:

**setup-dev-env.sh (macOS/Linux):**

```bash
#!/bin/bash
# Development environment setup script

set -e

echo "Setting up development environment..."

# Detect platform
if [[ "$OSTYPE" == "darwin"* ]]; then
    # macOS
    if ! command -v brew &> /dev/null; then
        echo "Installing Homebrew..."
        /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    fi

    echo "Installing macOS dependencies..."
    brew install cmake git ninja boost openssl@3 zlib curl sqlite postgresql

elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    # Linux
    if command -v apt &> /dev/null; then
        # Ubuntu/Debian
        echo "Installing Ubuntu/Debian dependencies..."
        sudo apt update
        sudo apt install -y build-essential cmake git ninja-build \
            libboost-all-dev libssl-dev zlib1g-dev libcurl4-openssl-dev \
            libsqlite3-dev libpq-dev
    elif command -v dnf &> /dev/null; then
        # Fedora/RHEL
        echo "Installing Fedora/RHEL dependencies..."
        sudo dnf install -y cmake git ninja-build boost-devel openssl-devel \
            zlib-devel libcurl-devel sqlite-devel postgresql-devel
    else
        echo "Unsupported Linux distribution"
        exit 1
    fi
else
    echo "Unsupported operating system: $OSTYPE"
    exit 1
fi

echo "Development environment setup complete!"
echo "You can now build the project with:"
echo "  cmake -B build -S ."
echo "  cmake --build build"
```

**setup-dev-env.ps1 (Windows):**

```powershell
# Development environment setup script for Windows

Write-Host "Setting up development environment..."

# Check if Chocolatey is installed
if (!(Get-Command choco -ErrorAction SilentlyContinue)) {
    Write-Host "Installing Chocolatey..."
    Set-ExecutionPolicy Bypass -Scope Process -Force
    [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072
    iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
}

Write-Host "Installing Windows dependencies..."
choco install -y cmake git ninja visualstudio2022buildtools vcpkg

Write-Host "Development environment setup complete!"
Write-Host "You can now build the project with:"
Write-Host "  cmake -B build -S ."
Write-Host "  cmake --build build"
```

### CMake Toolchain Configuration

Update the main CMakeLists.txt to better handle system packages:

```cmake
# Enhanced system package configuration
if(APPLE)
    # Configure Homebrew paths
    if(CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "arm64")
        # Apple Silicon
        set(HOMEBREW_PREFIX "/opt/homebrew")
    else()
        # Intel Mac
        set(HOMEBREW_PREFIX "/usr/local")
    endif()

    if(EXISTS "${HOMEBREW_PREFIX}")
        list(APPEND CMAKE_PREFIX_PATH "${HOMEBREW_PREFIX}")
        set(ENV{PKG_CONFIG_PATH} "${HOMEBREW_PREFIX}/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")
    endif()
endif()

# Enable pkg-config support
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    message(STATUS "pkg-config found: ${PKG_CONFIG_EXECUTABLE}")
else()
    message(STATUS "pkg-config not found - some packages may not be discoverable")
endif()
```

## Common System Dependencies

### Essential Build Tools

| Package | macOS (brew) | Ubuntu (apt) | Fedora (dnf) | Windows (choco) |
|---------|--------------|--------------|--------------|-----------------|
| CMake | `cmake` | `cmake` | `cmake` | `cmake` |
| Git | `git` | `git` | `git` | `git` |
| Ninja | `ninja` | `ninja-build` | `ninja-build` | `ninja` |
| pkg-config | `pkg-config` | `pkg-config` | `pkgconfig` | - |

### C++ Libraries

| Library | macOS (brew) | Ubuntu (apt) | Fedora (dnf) | CMake Target |
|---------|--------------|--------------|--------------|--------------|
| Boost | `boost` | `libboost-all-dev` | `boost-devel` | `Boost::*` |
| OpenSSL | `openssl@3` | `libssl-dev` | `openssl-devel` | `OpenSSL::SSL` |
| zlib | `zlib` | `zlib1g-dev` | `zlib-devel` | `ZLIB::ZLIB` |
| curl | `curl` | `libcurl4-openssl-dev` | `libcurl-devel` | `CURL::libcurl` |
| SQLite | `sqlite` | `libsqlite3-dev` | `sqlite-devel` | `SQLite::SQLite3` |
| PostgreSQL | `postgresql` | `libpq-dev` | `postgresql-devel` | `PostgreSQL::PostgreSQL` |

### Usage Examples

**Boost Libraries:**

```cmake
find_package(Boost REQUIRED COMPONENTS system filesystem thread regex)
target_link_libraries(your_target PRIVATE
    Boost::system
    Boost::filesystem
    Boost::thread
    Boost::regex
)
```

**OpenSSL:**

```cmake
find_package(OpenSSL REQUIRED)
target_link_libraries(your_target PRIVATE OpenSSL::SSL OpenSSL::Crypto)
```

**SQLite:**

```cmake
find_package(SQLite3 REQUIRED)
target_link_libraries(your_target PRIVATE SQLite::SQLite3)
```

## Troubleshooting

### Common Issues

1. **Package not found**

   ```bash
   # Check if package is installed
   brew list | grep package_name  # macOS
   dpkg -l | grep package_name    # Ubuntu
   rpm -qa | grep package_name    # Fedora
   ```

2. **Wrong package version**

   ```bash
   # Check installed version
   brew info package_name         # macOS
   apt show package_name          # Ubuntu
   dnf info package_name          # Fedora
   ```

3. **CMake can't find package**

   ```cmake
   # Add debug information
   set(CMAKE_FIND_DEBUG_MODE TRUE)
   find_package(PackageName REQUIRED)
   set(CMAKE_FIND_DEBUG_MODE FALSE)
   ```

4. **pkg-config issues**

   ```bash
   # Check pkg-config path
   echo $PKG_CONFIG_PATH

   # List available packages
   pkg-config --list-all

   # Check specific package
   pkg-config --exists package_name && echo "Found" || echo "Not found"
   ```

### Platform-Specific Issues

**macOS:**

- **Xcode Command Line Tools**: `xcode-select --install`
- **Homebrew permissions**: `sudo chown -R $(whoami) /opt/homebrew`
- **Apple Silicon vs Intel**: Check architecture with `uname -m`

**Linux:**

- **Missing development packages**: Install `-dev` or `-devel` variants
- **Package manager updates**: Run `sudo apt update` or `sudo dnf update`
- **Permission issues**: Use `sudo` for system package installation

**Windows:**

- **Visual Studio Build Tools**: Required for most C++ packages
- **PowerShell execution policy**: `Set-ExecutionPolicy RemoteSigned`
- **Path issues**: Restart terminal after package installation

## Best Practices

### Version Management

1. **Document requirements**: List minimum required versions
2. **Test compatibility**: Verify builds work with available versions
3. **Provide alternatives**: Offer multiple installation methods
4. **Version constraints**: Use CMake version requirements when needed

### Cross-Platform Support

1. **Conditional configuration**: Use platform-specific CMake logic
2. **Consistent naming**: Use CMake targets for consistent interface
3. **Fallback options**: Provide alternatives when packages aren't available
4. **Documentation**: Document platform-specific requirements

### Integration Strategy

1. **Prefer system packages**: For common, stable libraries
2. **Combine approaches**: Use system packages with other methods
3. **Optional dependencies**: Make system packages optional when possible
4. **Clear error messages**: Provide helpful installation instructions

## Example: Complete Integration

Here's a complete example showing how to integrate a system package:

**CMakeLists.txt:**

```cmake
# Find Boost with fallback options
find_package(Boost 1.70 QUIET COMPONENTS system filesystem)

if(Boost_FOUND)
    message(STATUS "Found Boost ${Boost_VERSION} via system packages")
    add_library(third_party::boost INTERFACE IMPORTED)
    target_link_libraries(third_party::boost INTERFACE
        Boost::system
        Boost::filesystem
    )
else()
    message(STATUS "Boost not found via system packages")
    message(STATUS "Install with:")
    if(APPLE)
        message(STATUS "  brew install boost")
    elseif(UNIX)
        message(STATUS "  sudo apt install libboost-all-dev  # Ubuntu")
        message(STATUS "  sudo dnf install boost-devel       # Fedora")
    elseif(WIN32)
        message(STATUS "  choco install boost-msvc-14.3")
    endif()

    # Try alternative methods (vcpkg, manual, etc.)
    # ... fallback logic here ...
endif()
```

**Usage:**

```cpp
#include <boost/system/error_code.hpp>
#include <boost/filesystem.hpp>

int main() {
    boost::system::error_code ec;
    boost::filesystem::path p("/tmp");

    if (boost::filesystem::exists(p, ec)) {
        std::cout << "Path exists" << std::endl;
    }

    return 0;
}
```

This approach provides a robust foundation for system package integration while maintaining flexibility for different development environments.
