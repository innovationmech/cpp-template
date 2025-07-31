# C++ Project Template

A comprehensive C++ project template demonstrating modern C++ development practices with CMake build system. This template serves as a starting point for C++ projects, incorporating best practices for project structure, dependency management, and build configuration.

## Features

- **Modern C++ Standards**: Uses C++17 as the minimum standard with explicit version specification
- **Modular Architecture**: Clear separation of concerns with hierarchical project structure
- **Multiple Dependency Management**: Supports vcpkg, Git submodules, manual dependencies, and system packages
- **Comprehensive Testing**: Integrated Google Test framework with unit and integration tests
- **Custom CMake Modules**: Standardized compiler options and custom find modules
- **Cross-platform Support**: Compatible with major compilers (GCC, Clang, MSVC)

## Project Structure

```
cpp-template/
├── CMakeLists.txt              # Top-level build configuration
├── README.md                   # Project documentation
├── .gitmodules                 # Git submodules configuration
├── vcpkg.json                  # vcpkg manifest file
├── src/                        # Application source code
│   ├── CMakeLists.txt
│   ├── main.cpp
│   └── modules/                # Application modules
│       └── example/
├── include/                    # Public header files
│   └── cpp-template/
├── libs/                       # Internal project libraries
│   ├── CMakeLists.txt
│   └── core/                   # Core library
│       ├── include/
│       └── src/
├── tests/                      # Unit tests
│   ├── CMakeLists.txt
│   └── unit/
├── third_party/                # External dependencies
│   ├── CMakeLists.txt
│   ├── manual/                 # Manually managed dependencies
│   └── submodules/             # Git submodule dependencies
└── cmake/                      # Custom CMake modules
    ├── FindCustomLib.cmake
    └── CompilerOptions.cmake
```

## Quick Start

### Prerequisites

- CMake 3.20 or higher
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- Git

### Building the Project

1. **Clone the repository**:
   ```bash
   git clone <repository-url>
   cd cpp-template
   ```

2. **Create build directory**:
   ```bash
   mkdir build
   cd build
   ```

3. **Configure with CMake**:
   ```bash
   cmake ..
   ```

4. **Build the project**:
   ```bash
   cmake --build .
   ```

5. **Run the application**:
   ```bash
   ./cpp-template
   ```

6. **Run tests** (if Google Test is available):
   ```bash
   ctest
   ```

## Dependency Management

This template supports multiple dependency management approaches:

### 1. vcpkg (Recommended)

1. **Install vcpkg**:
   ```bash
   git clone https://github.com/Microsoft/vcpkg.git
   cd vcpkg
   ./bootstrap-vcpkg.sh  # or .\bootstrap-vcpkg.bat on Windows
   ```

2. **Configure CMake with vcpkg**:
   ```bash
   cmake -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake ..
   ```

### 2. System Package Manager (macOS)

**Install dependencies with Homebrew**:
```bash
brew install gtest
```

### 3. Git Submodules

**Add a dependency as submodule**:
```bash
git submodule add <repository-url> third_party/submodules/<library-name>
git submodule update --init --recursive
```

### 4. Manual Dependencies

Place header-only libraries or pre-built libraries in `third_party/manual/` and configure them in the corresponding CMakeLists.txt.

## Development

### Adding New Libraries

1. Create a new directory under `libs/`
2. Add CMakeLists.txt with library configuration
3. Update `libs/CMakeLists.txt` to include the new subdirectory
4. Add public headers to `include/cpp-template/`

### Adding New Modules

1. Create a new directory under `src/modules/`
2. Implement module functionality
3. Update `src/modules/CMakeLists.txt` to include the new module
4. Add corresponding tests in `tests/unit/`

### Testing

The project uses Google Test for unit testing:

- Unit tests are located in `tests/unit/`
- Each library/module should have corresponding tests
- Run tests with `ctest` or individual test executables

## Customization

### Compiler Options

Modify `cmake/CompilerOptions.cmake` to adjust:
- Warning levels
- Optimization flags
- Debug information
- Platform-specific options

### Project Configuration

Update the top-level `CMakeLists.txt` to change:
- Project name and version
- C++ standard requirement
- Global compile definitions

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests for new functionality
5. Ensure all tests pass
6. Submit a pull request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- Modern CMake practices
- C++ Core Guidelines
- vcpkg package manager
- Google Test framework