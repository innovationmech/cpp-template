# System Package Manager Dependencies

This document describes how to manage dependencies using system package managers like Homebrew on macOS, apt on Ubuntu, or other platform-specific package managers.

## macOS with Homebrew

### Installation

First, ensure Homebrew is installed:
```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

### Common Development Dependencies

```bash
# Essential build tools
brew install cmake
brew install ninja
brew install pkg-config

# Common C++ libraries
brew install boost
brew install openssl
brew install zlib
brew install curl
brew install sqlite

# Development tools
brew install llvm
brew install clang-format
brew install cppcheck
```

### CMake Integration

When using system-installed libraries, CMake can find them using `find_package()`:

```cmake
# Find system-installed libraries
find_package(PkgConfig REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(CURL REQUIRED)

# Link against system libraries
target_link_libraries(your_target
    PRIVATE
        OpenSSL::SSL
        OpenSSL::Crypto
        ZLIB::ZLIB
        CURL::libcurl
)
```

## Ubuntu/Debian with apt

### Installation

```bash
# Update package list
sudo apt update

# Essential build tools
sudo apt install build-essential
sudo apt install cmake
sudo apt install ninja-build
sudo apt install pkg-config

# Common C++ libraries
sudo apt install libboost-all-dev
sudo apt install libssl-dev
sudo apt install zlib1g-dev
sudo apt install libcurl4-openssl-dev
sudo apt install libsqlite3-dev

# Development tools
sudo apt install clang-format
sudo apt install cppcheck
sudo apt install valgrind
```

## CentOS/RHEL with yum/dnf

### Installation

```bash
# For CentOS 8+ / RHEL 8+ / Fedora (use dnf)
sudo dnf groupinstall "Development Tools"
sudo dnf install cmake
sudo dnf install ninja-build
sudo dnf install pkg-config

# For older versions (use yum)
sudo yum groupinstall "Development Tools"
sudo yum install cmake3
sudo yum install pkg-config

# Common libraries
sudo dnf install boost-devel
sudo dnf install openssl-devel
sudo dnf install zlib-devel
sudo dnf install libcurl-devel
```

## Windows with vcpkg (Recommended)

For Windows, we recommend using vcpkg instead of system package managers:

```cmd
# Install vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat

# Install dependencies
.\vcpkg install boost:x64-windows
.\vcpkg install openssl:x64-windows
.\vcpkg install curl:x64-windows
```

## CMake Configuration for System Dependencies

Create a CMake module to handle system dependencies:

```cmake
# cmake/FindSystemDependencies.cmake

# Function to find and configure system dependencies
function(find_system_dependencies)
    # Try to find common system libraries
    find_package(PkgConfig QUIET)
    
    # OpenSSL
    find_package(OpenSSL QUIET)
    if(OpenSSL_FOUND)
        message(STATUS "Found system OpenSSL: ${OPENSSL_VERSION}")
    endif()
    
    # ZLIB
    find_package(ZLIB QUIET)
    if(ZLIB_FOUND)
        message(STATUS "Found system ZLIB: ${ZLIB_VERSION_STRING}")
    endif()
    
    # CURL
    find_package(CURL QUIET)
    if(CURL_FOUND)
        message(STATUS "Found system CURL: ${CURL_VERSION_STRING}")
    endif()
    
    # Boost (optional)
    find_package(Boost QUIET COMPONENTS system filesystem thread)
    if(Boost_FOUND)
        message(STATUS "Found system Boost: ${Boost_VERSION}")
    endif()
endfunction()
```

## Troubleshooting

### Library Not Found

If CMake cannot find a system library:

1. **Check installation**: Verify the library is installed
   ```bash
   # macOS
   brew list | grep library-name
   
   # Ubuntu
   dpkg -l | grep library-name
   ```

2. **Check development headers**: Ensure development packages are installed
   ```bash
   # Ubuntu example
   sudo apt install libssl-dev  # not just libssl
   ```

3. **Set CMAKE_PREFIX_PATH**: Help CMake find libraries
   ```bash
   cmake -DCMAKE_PREFIX_PATH=/usr/local ..
   ```

4. **Use pkg-config**: For libraries that support it
   ```cmake
   find_package(PkgConfig REQUIRED)
   pkg_check_modules(LIBRARY_NAME REQUIRED library-name)
   ```

### Version Conflicts

When system libraries conflict with project requirements:

1. **Use specific versions**: Specify minimum required versions
   ```cmake
   find_package(OpenSSL 1.1.0 REQUIRED)
   ```

2. **Consider vcpkg**: For precise version control
3. **Use Docker**: For consistent environments

### Platform Differences

Handle platform-specific differences:

```cmake
if(APPLE)
    # macOS-specific configuration
    set(CMAKE_PREFIX_PATH "/usr/local" ${CMAKE_PREFIX_PATH})
elseif(UNIX)
    # Linux-specific configuration
    set(CMAKE_PREFIX_PATH "/usr" ${CMAKE_PREFIX_PATH})
endif()
```

## Best Practices

1. **Document system requirements**: List required system packages in README
2. **Provide installation scripts**: Create setup scripts for different platforms
3. **Use version constraints**: Specify minimum required versions
4. **Fallback to vcpkg**: Provide vcpkg as alternative for missing system libraries
5. **Test on CI**: Verify builds work with system packages on different platforms

## Integration with Other Dependency Methods

System dependencies can be combined with other methods:

```cmake
# Try system first, fallback to vcpkg
find_package(OpenSSL QUIET)
if(NOT OpenSSL_FOUND)
    find_package(OpenSSL REQUIRED CONFIG)
endif()
```