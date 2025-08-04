# CMake Modules Documentation

This directory contains custom CMake modules that provide standardized build configuration and dependency management for the cpp-template project.

## Available Modules

### CompilerOptions.CMake

Provides standardized compiler settings and options for consistent builds across different compilers and platforms.

#### Functions

- `set_project_compiler_options(target_name)`: Applies standard compiler warnings and optimization settings to a target

#### Usage

```cmake
# Include the module
include(CompilerOptions)

# Apply compiler options to a target
add_executable(my_app main.cpp)
set_project_compiler_options(my_app)
```

#### Features

- Cross-platform compiler detection (GCC, Clang, MSVC)
- Comprehensive warning flags for better code quality
- Debug and Release configuration optimization
- Platform-specific definitions and settings

### FindCustomLib.CMake

Example find module for manual dependency management. Demonstrates how to create custom find modules for libraries that don't provide CMake configuration files.

#### Usage

```cmake
# Find the library
find_package(CustomLib REQUIRED)

# Link against the imported target
target_link_libraries(my_target PRIVATE CustomLib::CustomLib)
```

#### Customization

To adapt this module for your own library:

1. Replace `CUSTOMLIB` with your library's prefix
2. Update library names in `CUSTOMLIB_NAMES`
3. Update header names in `CUSTOMLIB_HEADER_NAMES`
4. Modify search paths as needed
5. Update version detection logic if applicable

#### Environment Variables

- `CUSTOMLIB_ROOT`: Set to specify custom installation directory

### VcpkgHelpers.CMake

Provides utilities for vcpkg integration and dependency management with fallback options.

#### Functions

- `check_vcpkg_integration()`: Detects if vcpkg toolchain is being used
- `find_package_with_fallback(PackageName VCPKG_NAME vcpkg_name SYSTEM_NAME system_name)`: Finds packages with multiple naming conventions
- `setup_vcpkg_manifest()`: Configures vcpkg manifest mode
- `print_vcpkg_info()`: Displays vcpkg configuration information

#### Usage

```cmake
# Include the module (automatically included in main CMakeLists.txt)
include(VcpkgHelpers)

# Find a package with fallback names
find_package_with_fallback(MyLib VCPKG_NAME mylib SYSTEM_NAME my-lib)
```

## Dependency Management Strategies

### 1. vcpkg Integration

vcpkg is integrated through the CMake toolchain file. To use vcpkg:

```bash
# Configure with vcpkg toolchain
cmake -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake ..

# Or set environment variable
export CMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake ..
```

#### vcpkg.JSON Manifest

The project uses vcpkg manifest mode with dependencies defined in `vcpkg.json`:

```json
{
  "name": "cpp-template",
  "version": "1.0.0",
  "dependencies": [
    "gtest"
  ]
}
```

### 2. System Package Manager

For system-level dependencies (e.g., via brew on macOS):

```cmake
# Find system-installed packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(MYLIB REQUIRED mylib)

# Or use find_package with system paths
find_package(MyLib REQUIRED)
```

### 3. Git Submodules

For source-based dependencies in `third_party/submodules/`:

```cmake
# Add submodule directory
add_subdirectory(third_party/submodules/mylib)

# Link against submodule target
target_link_libraries(my_target PRIVATE mylib::mylib)
```

### 4. Manual Dependencies

For manually managed dependencies in `third_party/manual/`:

```cmake
# Use custom find module
find_package(CustomLib REQUIRED)
target_link_libraries(my_target PRIVATE CustomLib::CustomLib)
```

## Creating Custom CMake Modules

### Best Practices

1. **Naming Convention**: Use PascalCase for module names (e.g., `FindMyLib.cmake`)
2. **Function Naming**: Use lowercase with underscores (e.g., `set_compiler_options`)
3. **Variable Naming**: Use UPPERCASE with underscores for cache variables
4. **Documentation**: Include comprehensive comments and usage examples

### Module Template

```cmake
# MyModule.cmake
# Brief description of the module's purpose

# Include guard (optional but recommended)
if(MYMODULE_INCLUDED)
    return()
endif()
set(MYMODULE_INCLUDED TRUE)

# Module variables
set(MYMODULE_VERSION "1.0.0")

# Function definition
function(my_module_function target_name)
    # Function implementation
    message(STATUS "Configuring ${target_name} with MyModule")
endfunction()

# Module initialization (if needed)
message(STATUS "MyModule loaded (version ${MYMODULE_VERSION})")
```

### Find Module Template

```cmake
# FindMyLib.cmake
# Find module for MyLib library

# Search for library
find_library(MYLIB_LIBRARY
    NAMES mylib
    PATHS /usr/local/lib /usr/lib
    DOC "MyLib library"
)

# Search for headers
find_path(MYLIB_INCLUDE_DIR
    NAMES mylib.h
    PATHS /usr/local/include /usr/include
    DOC "MyLib include directory"
)

# Handle standard arguments
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MyLib
    FOUND_VAR MYLIB_FOUND
    REQUIRED_VARS MYLIB_LIBRARY MYLIB_INCLUDE_DIR
)

# Create imported target
if(MYLIB_FOUND AND NOT TARGET MyLib::MyLib)
    add_library(MyLib::MyLib UNKNOWN IMPORTED)
    set_target_properties(MyLib::MyLib PROPERTIES
        IMPORTED_LOCATION "${MYLIB_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${MYLIB_INCLUDE_DIR}"
    )
endif()
```

## Extension Patterns

### Adding New Compiler Support

To add support for a new compiler in `CompilerOptions.cmake`:

```cmake
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    # Intel compiler-specific options
    add_compile_options(-diag-enable=all)
```

### Adding New Dependency Types

To support a new dependency management approach:

1. Create a new helper module (e.g., `ConanHelpers.cmake`)
2. Add detection logic similar to `VcpkgHelpers.cmake`
3. Include the module in the main `CMakeLists.txt`
4. Update documentation

### Custom Build Configurations

To add custom build types:

```cmake
# In CompilerOptions.cmake or main CMakeLists.txt
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Profile" CACHE STRING "Available build types" FORCE)

# Add Profile-specific options
target_compile_options(${target_name} PRIVATE
    $<$<CONFIG:Profile>:
        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-O2 -g -pg>
    >
)
```

## Troubleshooting

### Common Issues

1. **Module Not Found**: Ensure the module is in the `cmake/` directory and `CMAKE_MODULE_PATH` includes it
2. **vcpkg Not Detected**: Verify `CMAKE_TOOLCHAIN_FILE` points to vcpkg's CMake file
3. **Find Module Fails**: Check library and header search paths, set custom root variables if needed

### Debug Information

Enable verbose output to debug module issues:

```bash
cmake --debug-output ..
# or
cmake -DCMAKE_VERBOSE_MAKEFILE=ON ..
```

### Environment Variables

Useful environment variables for debugging:

- `CMAKE_PREFIX_PATH`: Additional search paths for find_package
- `CMAKE_MODULE_PATH`: Additional paths for CMake modules
- `VCPKG_ROOT`: vcpkg installation directory
- `PKG_CONFIG_PATH`: pkg-config search paths
