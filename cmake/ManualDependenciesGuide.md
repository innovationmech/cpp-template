# Manual Dependencies Guide

This document provides comprehensive instructions for managing dependencies manually in the cpp-template project.

## Overview

Manual dependency management involves directly including external libraries in your project repository. This approach is useful for:

- Libraries not available in package managers
- Custom or modified versions of libraries
- Legacy libraries without modern build systems
- Small, specialized libraries
- Dependencies that require specific configuration

## Advantages and Disadvantages

### Advantages

- **Full control**: Complete control over library configuration and build process
- **No external dependencies**: No need for package managers or internet access
- **Custom modifications**: Easy to modify library code for specific needs
- **Reproducible builds**: Exact library version is always available
- **Simple distribution**: Everything needed is in the repository

### Disadvantages

- **Repository size**: Increases repository size significantly
- **Update overhead**: Manual process to update dependencies
- **Maintenance burden**: Responsibility for security updates and bug fixes
- **Build complexity**: May require custom CMake configuration
- **License compliance**: Need to track and comply with multiple licenses

## Directory Structure

Manual dependencies are organized in `third_party/manual/`:

```
third_party/manual/
├── CMakeLists.txt              # Manual dependencies configuration
├── mathutils/                  # Example header-only library
│   ├── include/mathutils/
│   │   └── mathutils.h
│   └── README.md
├── utillib/                    # Example compiled library
│   ├── include/
│   ├── src/
│   └── README.md
├── prebuilt/                   # Example pre-built library
│   ├── include/
│   ├── lib/
│   └── README.md
└── customlib/                  # Example library with CMake
    ├── CMakeLists.txt
    ├── include/
    └── src/
```

## Types of Manual Dependencies

### 1. Header-Only Libraries

For libraries that consist only of header files:

**Structure:**

```
library_name/
├── include/
│   └── library_name/
│       ├── header1.h
│       └── header2.h
└── README.md
```

**CMake Integration:**

```cmake
create_header_only_target(manual_library_name
    "${CMAKE_CURRENT_SOURCE_DIR}/library_name/include"
)
add_library(third_party::library_name ALIAS manual_library_name)
```

**Usage:**

```cpp
#include <library_name/header1.h>
```

### 2. Compiled Libraries

For libraries that need to be compiled from source:

**Structure:**

```
library_name/
├── include/
│   └── library_name/
│       └── library.h
├── src/
│   ├── library.cpp
│   └── internal.cpp
└── README.md
```

**CMake Integration:**

```cmake
create_compiled_target(manual_library_name
    "${CMAKE_CURRENT_SOURCE_DIR}/library_name/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/library_name/include"
)
add_library(third_party::library_name ALIAS manual_library_name)
```

### 3. Pre-built Libraries

For libraries distributed as pre-compiled binaries:

**Structure:**

```
library_name/
├── include/
│   └── library_name/
│       └── library.h
├── lib/
│   ├── liblibrary.a      # Static library (Unix)
│   ├── library.lib       # Static library (Windows)
│   └── library.dll       # Dynamic library (Windows)
└── README.md
```

**CMake Integration:**

```cmake
add_library(manual_library_name UNKNOWN IMPORTED)
target_include_directories(manual_library_name INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/library_name/include"
)
set_target_properties(manual_library_name PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/library_name/lib/liblibrary.a"
)
add_library(third_party::library_name ALIAS manual_library_name)
```

### 4. Libraries with CMake Support

For libraries that include their own CMakeLists.txt:

**Structure:**

```
library_name/
├── CMakeLists.txt
├── include/
├── src/
└── README.md
```

**CMake Integration:**

```cmake
# Set options to disable unwanted features
set(LIBRARY_BUILD_TESTS OFF CACHE BOOL "Disable library tests")
set(LIBRARY_BUILD_EXAMPLES OFF CACHE BOOL "Disable library examples")

add_subdirectory(library_name)
add_library(third_party::library_name ALIAS library_name)
```

## Adding New Manual Dependencies

### Step 1: Obtain the Library

1. **Download from official source**: Get the library from its official repository or website
2. **Choose appropriate version**: Select a stable release version
3. **Verify integrity**: Check checksums or signatures if available
4. **Review license**: Ensure license compatibility with your project

### Step 2: Prepare the Directory Structure

```bash
# Create directory for the new dependency
mkdir -p third_party/manual/library_name

# Copy library files
cp -r /path/to/library/* third_party/manual/library_name/

# Organize files according to expected structure
mkdir -p third_party/manual/library_name/{include,src,lib}
```

### Step 3: Update CMake Configuration

Add configuration for the new library in `third_party/manual/CMakeLists.txt`:

```cmake
# Example: New Library Integration
check_manual_dependency("NewLibrary" "newlibrary" NEWLIBRARY_AVAILABLE)
if(NEWLIBRARY_AVAILABLE)
    # Configure based on library type
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/newlibrary/CMakeLists.txt")
        # Library with CMake support
        add_subdirectory(newlibrary)
        add_library(third_party::newlibrary ALIAS newlibrary)
    elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/newlibrary/src")
        # Compiled library
        create_compiled_target(manual_newlibrary
            "${CMAKE_CURRENT_SOURCE_DIR}/newlibrary/src"
            "${CMAKE_CURRENT_SOURCE_DIR}/newlibrary/include"
        )
        add_library(third_party::newlibrary ALIAS manual_newlibrary)
    else()
        # Header-only library
        create_header_only_target(manual_newlibrary
            "${CMAKE_CURRENT_SOURCE_DIR}/newlibrary/include"
        )
        add_library(third_party::newlibrary ALIAS manual_newlibrary)
    endif()
endif()
```

### Step 4: Test Integration

```bash
# Reconfigure CMake
cmake -B build -S .

# Build to test integration
cmake --build build

# Run tests if available
cmake --build build --target test
```

### Step 5: Document the Dependency

Create a README.md file in the dependency directory:

```markdown
# Library Name

Brief description of the library and its purpose.

## Version
- Version: X.Y.Z
- Source: https://github.com/author/library
- License: MIT/Apache/GPL/etc.

## Usage
```cpp
#include <library/header.h>
// Usage example
```

## CMake Integration

```cmake
target_link_libraries(your_target PRIVATE third_party::library_name)
```

## Notes

- Any special configuration notes
- Known issues or limitations
- Custom modifications made

```

## Best Practices

### Version Management

1. **Document versions**: Always document which version you're using
2. **Use stable releases**: Avoid development branches for production
3. **Track changes**: Keep notes about any modifications made
4. **Regular updates**: Periodically check for security updates

### Repository Organization

1. **Consistent structure**: Follow the established directory patterns
2. **Clean integration**: Remove unnecessary files (tests, examples, docs)
3. **License tracking**: Include license files and track obligations
4. **Size optimization**: Remove unused components to minimize repository size

### Build Configuration

1. **Disable unnecessary features**: Turn off tests, examples, and unused features
2. **Consistent compiler settings**: Ensure compatibility with project settings
3. **Error handling**: Provide clear error messages for missing dependencies
4. **Optional dependencies**: Make dependencies optional when possible

### Documentation

1. **Installation instructions**: Document how to obtain and install the library
2. **Usage examples**: Provide clear usage examples
3. **Integration notes**: Document any special CMake configuration
4. **Troubleshooting**: Include common issues and solutions

## Troubleshooting

### Common Issues

1. **Library not found**
   - Verify directory structure matches expected layout
   - Check that CMake configuration is correct
   - Ensure all required files are present

2. **Compilation errors**
   - Check compiler compatibility
   - Verify C++ standard requirements
   - Look for missing dependencies

3. **Linking errors**
   - Ensure library files are in expected locations
   - Check that library was built for correct architecture
   - Verify all required libraries are linked

4. **Header not found**
   - Check include directory structure
   - Verify header file names and paths
   - Ensure include directories are properly configured

### Debug Information

Enable verbose CMake output:

```bash
cmake -B build -S . -DCMAKE_VERBOSE_MAKEFILE=ON
```

Check available targets:

```bash
cmake --build build --target help
```

List manual dependency information:

```bash
cmake --build build --target manual-deps-info
```

## Example: Adding a New Library

Here's a complete example of adding a new manual dependency:

### 1. Download and Prepare

```bash
# Download library
wget https://github.com/example/mylib/archive/v1.0.0.tar.gz
tar -xzf v1.0.0.tar.gz

# Create directory and copy files
mkdir -p third_party/manual/mylib
cp -r mylib-1.0.0/include third_party/manual/mylib/
cp -r mylib-1.0.0/src third_party/manual/mylib/
cp mylib-1.0.0/LICENSE third_party/manual/mylib/
```

### 2. Update CMake Configuration

Add to `third_party/manual/CMakeLists.txt`:

```cmake
check_manual_dependency("MyLib" "mylib" MYLIB_AVAILABLE)
if(MYLIB_AVAILABLE)
    create_compiled_target(manual_mylib
        "${CMAKE_CURRENT_SOURCE_DIR}/mylib/src"
        "${CMAKE_CURRENT_SOURCE_DIR}/mylib/include"
    )
    target_compile_features(manual_mylib PUBLIC cxx_std_17)
    add_library(third_party::mylib ALIAS manual_mylib)
endif()
```

### 3. Use in Your Code

```cpp
#include <mylib/mylib.h>

int main() {
    mylib::function();
    return 0;
}
```

### 4. Link in CMakeLists.txt

```cmake
target_link_libraries(your_target PRIVATE third_party::mylib)
```

## Integration with Other Dependency Methods

Manual dependencies can work alongside other dependency management approaches:

- **Fallback option**: Use manual dependencies when package managers fail
- **Custom versions**: Use manual dependencies for modified or custom versions
- **Development dependencies**: Use manual dependencies for libraries under development
- **Platform-specific**: Use manual dependencies for platform-specific libraries

The project's CMake configuration handles these scenarios automatically and provides appropriate fallbacks.

## Maintenance

### Regular Tasks

1. **Security updates**: Monitor for security vulnerabilities
2. **Version updates**: Periodically update to newer stable versions
3. **License compliance**: Ensure continued license compliance
4. **Cleanup**: Remove unused dependencies to reduce repository size

### Update Process

1. **Backup current version**: Create a backup of the current dependency
2. **Download new version**: Obtain the new version from official sources
3. **Test integration**: Verify that the new version works with your code
4. **Update documentation**: Update version information and notes
5. **Commit changes**: Commit the update with clear commit message

For more complex dependency management scenarios, consider using dedicated tools like Conan, vcpkg, or Git submodules.
