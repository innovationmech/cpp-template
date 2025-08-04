# Template Customization Guide

This guide provides step-by-step instructions for customizing the cpp-template for your own projects. It covers everything from basic renaming to advanced architectural modifications.

## Table of Contents

- [Quick Start Customization](#quick-start-customization)
- [Project Renaming](#project-renaming)
- [Dependency Management](#dependency-management)
- [Adding New Components](#adding-new-components)
- [Modifying Build Configuration](#modifying-build-configuration)
- [Customizing Documentation](#customizing-documentation)
- [Advanced Customizations](#advanced-customizations)

## Quick Start Customization

### 1. Fork or Clone the Template

```bash
# Option 1: Fork on GitHub and clone your fork
git clone https://github.com/your-username/your-project-name.git
cd your-project-name

# Option 2: Clone and remove git history for a fresh start
git clone https://github.com/original/cpp-template.git your-project-name
cd your-project-name
rm -rf .git
git init
git add .
git commit -m "Initial commit from cpp-template"
```

### 2. Basic Project Setup

Run the customization script (if available) or follow manual steps:

```bash
# If customization script exists
./scripts/customize-template.sh "YourProjectName" "your_namespace"

# Or follow the manual steps below
```

## Project Renaming

### Step 1: Update Project Metadata

**File: `CMakeLists.txt`**

```cmake
# Change these lines
project(your-project-name
    VERSION 1.0.0
    DESCRIPTION "Your project description"
    LANGUAGES CXX
)

# Update the executable name
add_executable(your-project-name-app)
set_target_properties(your-project-name-app PROPERTIES OUTPUT_NAME your-project-name)
```

**File: `vcpkg.json`**

```json
{
  "name": "your-project-name",
  "version": "1.0.0",
  "description": "Your project description",
  "dependencies": [
    "gtest"
  ]
}
```

### Step 2: Update Namespace and Headers

**File: `include/your-project-name/your-project-name.h`** (rename from cpp-template.h)

```cpp
#pragma once

// Update namespace
namespace your_project_name {

namespace info {
    constexpr const char* getName() {
        return "your-project-name";
    }

    constexpr const char* getVersion() {
        return "1.0.0";
    }

    constexpr const char* getDescription() {
        return "Your project description";
    }
}

} // namespace your_project_name
```

### Step 3: Update Directory Structure

```bash
# Rename include directory
mv include/cpp-template include/your-project-name

# Update all #include statements
find . -name "*.cpp" -o -name "*.h" | xargs sed -i 's/cpp-template/your-project-name/g'
find . -name "*.cpp" -o -name "*.h" | xargs sed -i 's/cpp_template/your_project_name/g'
```

### Step 4: Update CMake Target Names

**File: `src/CMakeLists.txt`**

```cmake
# Update library names
add_library(your-project-name-impl STATIC)
add_executable(your-project-name-app)

# Update target names throughout the file
target_link_libraries(your-project-name-app
    PRIVATE
        your-project-name-impl
        # ... other dependencies
)
```

## Dependency Management

### Adding vcpkg Dependencies

1. **Update `vcpkg.json`**:

```json
{
  "name": "your-project-name",
  "version": "1.0.0",
  "dependencies": [
    "gtest",
    "nlohmann-json",
    "fmt",
    "spdlog",
    {
      "name": "boost-system",
      "version>=": "1.75.0"
    }
  ]
}
```

2. **Use in CMake**:

```cmake
# Find packages
find_package(nlohmann_json CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)

# Link to your targets
target_link_libraries(your-target
    PRIVATE
        nlohmann_json::nlohmann_json
        fmt::fmt
        spdlog::spdlog
        Boost::system
)
```

### Adding System Dependencies

**File: `cmake/FindYourDependency.cmake`**

```cmake
# Create custom find module if needed
find_path(YOURDEP_INCLUDE_DIR yourdep.h)
find_library(YOURDEP_LIBRARY yourdep)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(YourDependency
    REQUIRED_VARS YOURDEP_LIBRARY YOURDEP_INCLUDE_DIR
)

if(YourDependency_FOUND AND NOT TARGET YourDependency::YourDependency)
    add_library(YourDependency::YourDependency UNKNOWN IMPORTED)
    set_target_properties(YourDependency::YourDependency PROPERTIES
        IMPORTED_LOCATION "${YOURDEP_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${YOURDEP_INCLUDE_DIR}"
    )
endif()
```

### Adding Git Submodules

```bash
# Add submodule
git submodule add https://github.com/user/library.git third_party/submodules/library

# Update third_party/submodules/CMakeLists.txt
add_subdirectory(library)
```

## Adding New Components

### Adding a New Internal Library

1. **Create directory structure**:

```bash
mkdir -p libs/yourlib/{include/yourlib,src}
```

2. **Create `libs/yourlib/CMakeLists.txt`**:

```cmake
add_library(yourlib STATIC)

target_sources(yourlib
    PRIVATE
        src/yourlib.cpp
    PUBLIC
        include/yourlib/yourlib.h
)

target_include_directories(yourlib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(yourlib
    PUBLIC
        core  # If it depends on core
)

# Apply compiler options
if(COMMAND apply_compiler_options)
    apply_compiler_options(yourlib)
endif()

add_library(yourlib::yourlib ALIAS yourlib)
```

3. **Create header `libs/yourlib/include/yourlib/yourlib.h`**:

```cpp
#pragma once

namespace your_project_name {
namespace yourlib {

class YourClass {
public:
    YourClass();
    ~YourClass();

    void doSomething();

private:
    // Implementation details
};

} // namespace yourlib
} // namespace your_project_name
```

4. **Create implementation `libs/yourlib/src/yourlib.cpp`**:

```cpp
#include "yourlib/yourlib.h"

namespace your_project_name {
namespace yourlib {

YourClass::YourClass() = default;
YourClass::~YourClass() = default;

void YourClass::doSomething() {
    // Your implementation
}

} // namespace yourlib
} // namespace your_project_name
```

5. **Update `libs/CMakeLists.txt`**:

```cmake
add_subdirectory(core)
add_subdirectory(yourlib)  # Add this line
```

### Adding a New Application Module

1. **Create module files**:

```bash
touch src/modules/your_module.{h,cpp}
```

2. **Implement the module** following the pattern of existing modules

3. **Update `src/modules/CMakeLists.txt`**:

```cmake
add_library(your-module STATIC
    your_module.cpp
)

target_include_directories(your-module
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_SOURCE_DIR}/libs/core/include
)

target_link_libraries(your-module
    PUBLIC
        core
        yourlib  # If it depends on your new library
)
```

## Modifying Build Configuration

### Changing C++ Standard

**File: `CMakeLists.txt`**

```cmake
# Change from C++17 to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
```

### Adding Compiler-Specific Options

**File: `cmake/CompilerOptions.cmake`**

```cmake
function(apply_compiler_options target)
    target_compile_features(${target} PUBLIC cxx_std_20)

    # Add your custom compiler options
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(${target} PRIVATE
            -Wall -Wextra -Wpedantic
            -Wno-unused-parameter  # Your custom flags
        )
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(${target} PRIVATE
            -Wall -Wextra -Wpedantic
            -Wno-unused-parameter
        )
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(${target} PRIVATE
            /W4
            /wd4996  # Disable specific warnings
        )
    endif()
endfunction()
```

### Adding Custom Build Types

**File: `CMakeLists.txt`**

```cmake
# Add custom build type
set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_RELEASE} -pg")
set(CMAKE_EXE_LINKER_FLAGS_PROFILE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -pg")

# Set available build types
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo;MinSizeRel;Profile" CACHE STRING "" FORCE)
```

## Customizing Documentation

### Update README.md

Replace template-specific content with your project information:

```markdown
# Your Project Name

Brief description of your project.

## Features

- List your project's key features
- Remove template-specific features you don't use
- Add features specific to your domain

## Quick Start

Update installation and usage instructions for your project.
```

### Update Code Documentation

1. **Update Doxygen comments** in header files
2. **Add project-specific examples** in documentation
3. **Update API documentation** to reflect your interfaces

### Create Project-Specific Guides

```bash
# Create domain-specific documentation
touch docs/user-guide.md
touch docs/api-reference.md
touch docs/deployment-guide.md
```

## Advanced Customizations

### Custom CMake Modules

Create project-specific CMake utilities:

**File: `cmake/YourProjectHelpers.cmake`**

```cmake
# Custom function for your project
function(add_your_project_library name)
    cmake_parse_arguments(PARSE_ARGV 1 ARG
        "HEADER_ONLY"
        "VERSION"
        "SOURCES;HEADERS;DEPENDENCIES"
    )

    if(ARG_HEADER_ONLY)
        add_library(${name} INTERFACE)
        target_sources(${name} INTERFACE ${ARG_HEADERS})
    else()
        add_library(${name} STATIC)
        target_sources(${name} PRIVATE ${ARG_SOURCES} PUBLIC ${ARG_HEADERS})
    endif()

    # Apply your project's standard configuration
    apply_compiler_options(${name})

    if(ARG_DEPENDENCIES)
        target_link_libraries(${name} PUBLIC ${ARG_DEPENDENCIES})
    endif()
endfunction()
```

### Custom Testing Framework

If you need project-specific testing utilities:

**File: `tests/test_helpers.h`**

```cpp
#pragma once

#include <gtest/gtest.h>
#include "your-project-name/your-project-name.h"

namespace your_project_name {
namespace test {

// Custom test fixture for your project
class YourProjectTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Common setup for your tests
    }

    void TearDown() override {
        // Common cleanup
    }

    // Helper methods specific to your domain
    void setupTestData();
    void verifyResults();
};

// Custom matchers or assertions
#define EXPECT_YOUR_CONDITION(actual, expected) \
    EXPECT_TRUE(yourCustomComparison(actual, expected))

} // namespace test
} // namespace your_project_name
```

### Platform-Specific Customizations

**File: `cmake/PlatformSpecific.cmake`**

```cmake
# Platform-specific configurations for your project
if(WIN32)
    # Windows-specific settings
    add_definitions(-DYOUR_PROJECT_WINDOWS=1)
elseif(APPLE)
    # macOS-specific settings
    add_definitions(-DYOUR_PROJECT_MACOS=1)
elseif(UNIX)
    # Linux-specific settings
    add_definitions(-DYOUR_PROJECT_LINUX=1)
endif()
```

## Validation Checklist

After customization, verify everything works:

- [ ] Project builds successfully: `cmake -B build -S . && cmake --build build`
- [ ] All tests pass: `cmake --build build --target test`
- [ ] Application runs: `./build/src/your-project-name`
- [ ] Documentation is updated
- [ ] Dependencies are correctly configured
- [ ] Namespace changes are complete
- [ ] CMake targets are properly named
- [ ] Version information is updated

## Next Steps

1. **Remove unused components** from the template
2. **Add your domain-specific functionality**
3. **Set up continuous integration** for your project
4. **Configure deployment** if needed
5. **Add project-specific documentation**

## Getting Help

- Check the [DEVELOPER_GUIDE.md](../DEVELOPER_GUIDE.md) for detailed architecture information
- Look at existing examples in the `examples/` directory
- Review the CMake configuration files for patterns
- Consult the original cpp-template documentation for reference

Remember to update this customization guide as you make changes to reflect your project's specific needs!
