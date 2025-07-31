# cpp-template

A comprehensive C++ project template demonstrating modern C++ development practices with CMake build system.

## Project Structure

```
cpp-template/
├── CMakeLists.txt              # Top-level build configuration
├── README.md                   # Project documentation
├── src/                        # Application source code
│   ├── CMakeLists.txt
│   ├── main.cpp
│   └── modules/                # Application modules
├── include/                    # Public header files
│   └── cpp-template/
├── libs/                       # Internal project libraries
│   ├── CMakeLists.txt
│   └── core/                   # Example internal library
├── tests/                      # Unit tests
│   ├── CMakeLists.txt
│   └── unit/
├── third_party/                # External dependencies
│   ├── CMakeLists.txt
│   ├── manual/                 # Manually managed dependencies
│   └── submodules/             # Git submodule dependencies
└── cmake/                      # Custom CMake modules
    └── CompilerOptions.cmake
```

## Requirements

- CMake 3.20 or higher
- C++17 compatible compiler
- Git (for submodule management)

## Building

```bash
mkdir build
cd build
cmake ..
make
```

## Features

- Modern C++ standards (C++17+)
- Modular architecture
- Multiple dependency management approaches
- Comprehensive testing framework
- Cross-platform compatibility

This is a template project - customize it for your specific needs.