# Developer Guide

This guide provides comprehensive information for developers who want to extend, modify, or contribute to the cpp-template project. It covers the project architecture, coding standards, build system details, and best practices.

## Table of Contents

- [Project Architecture](#project-architecture)
- [Build System Overview](#build-system-overview)
- [Coding Standards](#coding-standards)
- [Adding New Components](#adding-new-components)
- [Dependency Management](#dependency-management)
- [Testing Guidelines](#testing-guidelines)
- [CMake Best Practices](#cmake-best-practices)
- [Contributing Guidelines](#contributing-guidelines)

## Project Architecture

### Directory Structure Philosophy

The project follows a hierarchical structure that promotes:

- **Separation of concerns**: Clear boundaries between different types of code
- **Modularity**: Components can be developed and tested independently
- **Reusability**: Libraries can be used across multiple applications
- **Maintainability**: Easy to locate and modify specific functionality

```
cpp-template/
├── include/           # Public API headers (what users see)
├── src/              # Application implementation
│   └── modules/      # Modular application components
├── libs/             # Internal project libraries
│   └── core/         # Fundamental functionality
├── tests/            # All test code
├── third_party/      # External dependencies
├── cmake/            # Build system modules
└── scripts/          # Development and build scripts
```

### Component Relationships

```mermaid
graph TD
    A[Application] --> B[Implementation Library]
    B --> C[Core Library]
    B --> D[Application Modules]
    D --> C
    E[Tests] --> B
    E --> C
    E --> D
    F[Third Party] --> C
    F --> B
    F --> D
```

### Design Principles

1. **Interface Segregation**: Public headers in `include/` define clean APIs
2. **Dependency Inversion**: High-level modules don't depend on low-level modules
3. **Single Responsibility**: Each library/module has a focused purpose
4. **Open/Closed Principle**: Easy to extend without modifying existing code

## Build System Overview

### CMake Architecture

The build system uses modern CMake (3.20+) with target-based configuration:

- **Top-level CMakeLists.txt**: Global configuration and project setup
- **Component CMakeLists.txt**: Local configuration for each component
- **Custom modules**: Reusable CMake functions and utilities

### Key CMake Concepts Used

1. **Target-based approach**: All configuration attached to specific targets
2. **Generator expressions**: Platform and configuration-specific settings
3. **Interface libraries**: Header-only and dependency management
4. **Export/install**: Support for external project usage

### Build Configuration Flow

1. **Project setup**: Version, standards, global settings
2. **Dependency resolution**: Find or download required libraries
3. **Target creation**: Libraries and executables
4. **Property configuration**: Compile options, definitions, includes
5. **Linking**: Establish target dependencies
6. **Testing setup**: Test discovery and execution
7. **Installation**: Package for distribution

## Coding Standards

### C++ Standards

- **Language version**: C++17 minimum, C++20 preferred for new code
- **Standard library**: Prefer standard library over third-party when possible
- **Modern features**: Use RAII, smart pointers, range-based loops, auto

### Naming Conventions

```cpp
// Namespaces: lowercase with underscores
namespace cpp_template {
namespace core {

// Classes: PascalCase
class DataProcessor {
public:
    // Public methods: camelCase
    void processData();

    // Public members: camelCase (avoid public data)
    int getCount() const;

private:
    // Private members: camelCase with trailing underscore
    std::string data_;
    int count_;
};

// Functions: camelCase
void initializeSystem();

// Constants: UPPER_CASE
const int MAX_BUFFER_SIZE = 1024;

// Enums: PascalCase with PascalCase values
enum class ProcessingMode {
    Fast,
    Accurate,
    Balanced
};

} // namespace core
} // namespace cpp_template
```

### File Organization

```cpp
// header_file.h
#pragma once

#include <system_headers>
#include <third_party/headers>
#include "project/headers.h"

namespace cpp_template {

class ExampleClass {
public:
    // Public interface first
    ExampleClass();
    ~ExampleClass();

    // Copy/move operations
    ExampleClass(const ExampleClass&) = delete;
    ExampleClass& operator=(const ExampleClass&) = delete;
    ExampleClass(ExampleClass&&) = default;
    ExampleClass& operator=(ExampleClass&&) = default;

    // Main functionality
    void doSomething();

private:
    // Private implementation
    void helperFunction();

    // Data members last
    std::unique_ptr<Implementation> impl_;
};

} // namespace cpp_template
```

### Documentation Standards

```cpp
/**
 * @brief Brief description of the class or function
 *
 * Detailed description explaining the purpose, behavior, and usage.
 * Include examples when helpful.
 *
 * @param parameter_name Description of the parameter
 * @return Description of the return value
 * @throws ExceptionType When this exception is thrown
 *
 * @example
 * ```cpp
 * ExampleClass obj;
 * obj.doSomething();
 * ```
 */
void doSomething(int parameter_name);
```

## Adding New Components

### Adding a New Internal Library

1. **Create directory structure**:

   ```bash
   mkdir -p libs/newlib/{include/newlib,src}
   ```

2. **Create CMakeLists.txt**:

   ```cmake
   # libs/newlib/CMakeLists.txt
   add_library(newlib STATIC)

   target_sources(newlib
       PRIVATE
           src/newlib.cpp
       PUBLIC
           include/newlib/newlib.h
   )

   target_include_directories(newlib
       PUBLIC
           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
           $<INSTALL_INTERFACE:include>
       PRIVATE
           ${CMAKE_CURRENT_SOURCE_DIR}/src
   )

   target_link_libraries(newlib
       PUBLIC
           core  # If it depends on core
   )

   # Apply compiler options
   if(COMMAND apply_compiler_options)
       apply_compiler_options(newlib)
   endif()

   # Create alias
   add_library(newlib::newlib ALIAS newlib)
   ```

3. **Update parent CMakeLists.txt**:

   ```cmake
   # libs/CMakeLists.txt
   add_subdirectory(core)
   add_subdirectory(newlib)  # Add this line
   ```

4. **Create public header**:

   ```cpp
   // libs/newlib/include/newlib/newlib.h
   #pragma once

   namespace cpp_template {
   namespace newlib {

   class NewLibrary {
   public:
       NewLibrary();
       ~NewLibrary();

       void doSomething();
   };

   } // namespace newlib
   } // namespace cpp_template
   ```

5. **Create implementation**:

   ```cpp
   // libs/newlib/src/newlib.cpp
   #include "newlib/newlib.h"

   namespace cpp_template {
   namespace newlib {

   NewLibrary::NewLibrary() = default;
   NewLibrary::~NewLibrary() = default;

   void NewLibrary::doSomething() {
       // Implementation
   }

   } // namespace newlib
   } // namespace cpp_template
   ```

### Adding a New Application Module

1. **Create module files**:

   ```bash
   touch src/modules/new_module.{h,cpp}
   ```

2. **Update modules CMakeLists.txt**:

   ```cmake
   # src/modules/CMakeLists.txt
   add_library(new-module STATIC
       new_module.cpp
   )

   target_include_directories(new-module
       PUBLIC
           ${CMAKE_CURRENT_SOURCE_DIR}
           ${CMAKE_SOURCE_DIR}/include
       PRIVATE
           ${CMAKE_SOURCE_DIR}/libs/core/include
   )

   target_link_libraries(new-module
       PUBLIC
           core
   )
   ```

3. **Link in main application**:

   ```cmake
   # src/CMakeLists.txt
   target_link_libraries(cpp-template-app
       PRIVATE
           cpp-template-impl
           data-processor
           config-manager
           new-module  # Add this line
   )
   ```

### Adding Tests for New Components

1. **Create test file**:

   ```cpp
   // tests/unit/newlib_test.cpp
   #include <gtest/gtest.h>
   #include "newlib/newlib.h"

   namespace cpp_template {
   namespace newlib {
   namespace test {

   class NewLibraryTest : public ::testing::Test {
   protected:
       void SetUp() override {
           library_ = std::make_unique<NewLibrary>();
       }

       void TearDown() override {
           library_.reset();
       }

       std::unique_ptr<NewLibrary> library_;
   };

   TEST_F(NewLibraryTest, BasicFunctionality) {
       ASSERT_NO_THROW(library_->doSomething());
   }

   } // namespace test
   } // namespace newlib
   } // namespace cpp_template
   ```

2. **Add test to CMakeLists.txt**:

   ```cmake
   # tests/unit/CMakeLists.txt
   add_cpp_template_test(newlib
       SOURCES newlib_test.cpp
       LIBRARIES newlib
   )
   ```

## Dependency Management

### Adding vcpkg Dependencies

1. **Update vcpkg.JSON**:

   ```json
   {
     "dependencies": [
       "existing-dep",
       "new-dependency"
     ]
   }
   ```

2. **Use in CMake**:

   ```cmake
   find_package(NewDependency CONFIG REQUIRED)
   target_link_libraries(your_target PRIVATE NewDependency::NewDependency)
   ```

### Adding System Dependencies

1. **Create find module** (if needed):

   ```cmake
   # cmake/FindNewDep.cmake
   find_path(NEWDEP_INCLUDE_DIR newdep.h)
   find_library(NEWDEP_LIBRARY newdep)

   include(FindPackageHandleStandardArgs)
   find_package_handle_standard_args(NewDep
       REQUIRED_VARS NEWDEP_LIBRARY NEWDEP_INCLUDE_DIR
   )

   if(NewDep_FOUND AND NOT TARGET NewDep::NewDep)
       add_library(NewDep::NewDep UNKNOWN IMPORTED)
       set_target_properties(NewDep::NewDep PROPERTIES
           IMPORTED_LOCATION "${NEWDEP_LIBRARY}"
           INTERFACE_INCLUDE_DIRECTORIES "${NEWDEP_INCLUDE_DIR}"
       )
   endif()
   ```

2. **Use in project**:

   ```cmake
   find_package(NewDep REQUIRED)
   target_link_libraries(your_target PRIVATE NewDep::NewDep)
   ```

### Adding Manual Dependencies

1. **Create directory structure**:

   ```bash
   mkdir -p third_party/manual/newdep/{include,src,lib}
   ```

2. **Add to manual CMakeLists.txt**:

   ```cmake
   # third_party/manual/CMakeLists.txt
   check_manual_dependency("NewDep" "newdep" NEWDEP_AVAILABLE)
   if(NEWDEP_AVAILABLE)
       create_header_only_target(manual_newdep
           "${CMAKE_CURRENT_SOURCE_DIR}/newdep/include"
       )
       add_library(third_party::newdep ALIAS manual_newdep)
   endif()
   ```

## Testing Guidelines

### Test Categories

1. **Unit Tests**: Test individual classes/functions in isolation
2. **Integration Tests**: Test component interactions
3. **Performance Tests**: Benchmark critical functionality
4. **System Tests**: End-to-end application testing

### Test Structure

```cpp
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "component_under_test.h"

namespace cpp_template {
namespace component {
namespace test {

// Test fixture for shared setup/teardown
class ComponentTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Common setup
    }

    void TearDown() override {
        // Common cleanup
    }

    // Shared test data
    ComponentUnderTest component_;
};

// Basic functionality test
TEST_F(ComponentTest, BasicOperation) {
    // Arrange
    auto input = createTestInput();

    // Act
    auto result = component_.process(input);

    // Assert
    EXPECT_EQ(expected_result, result);
}

// Error handling test
TEST_F(ComponentTest, HandlesInvalidInput) {
    auto invalid_input = createInvalidInput();

    EXPECT_THROW(component_.process(invalid_input), std::invalid_argument);
}

// Parameterized test for multiple inputs
class ComponentParameterizedTest : public ComponentTest,
                                  public ::testing::WithParamInterface<TestData> {
};

TEST_P(ComponentParameterizedTest, ProcessesVariousInputs) {
    auto test_data = GetParam();
    auto result = component_.process(test_data.input);
    EXPECT_EQ(test_data.expected_output, result);
}

INSTANTIATE_TEST_SUITE_P(
    VariousInputs,
    ComponentParameterizedTest,
    ::testing::Values(
        TestData{input1, output1},
        TestData{input2, output2}
    )
);

} // namespace test
} // namespace component
} // namespace cpp_template
```

### Mock Objects

```cpp
// Mock interface for testing
class MockDependency : public IDependency {
public:
    MOCK_METHOD(int, getValue, (), (override));
    MOCK_METHOD(void, setValue, (int value), (override));
    MOCK_METHOD(bool, isValid, (), (const, override));
};

// Using mocks in tests
TEST_F(ComponentTest, UsesMockDependency) {
    auto mock_dep = std::make_shared<MockDependency>();

    EXPECT_CALL(*mock_dep, getValue())
        .WillOnce(::testing::Return(42));

    component_.setDependency(mock_dep);
    auto result = component_.calculate();

    EXPECT_EQ(84, result);  // Assuming calculate() doubles the value
}
```

## CMake Best Practices

### Target-Based Configuration

```cmake
# Good: Target-based approach
add_library(mylib STATIC src/mylib.cpp)
target_include_directories(mylib PUBLIC include)
target_compile_features(mylib PUBLIC cxx_std_17)
target_link_libraries(mylib PUBLIC dependency::dependency)

# Avoid: Global variables
# set(CMAKE_CXX_STANDARD 17)  # Affects all targets
# include_directories(include)  # Global include
```

### Generator Expressions

```cmake
# Platform-specific configuration
target_compile_definitions(mylib PRIVATE
    $<$<PLATFORM_ID:Windows>:PLATFORM_WINDOWS=1>
    $<$<PLATFORM_ID:Linux>:PLATFORM_LINUX=1>
    $<$<PLATFORM_ID:Darwin>:PLATFORM_MACOS=1>
)

# Configuration-specific options
target_compile_options(mylib PRIVATE
    $<$<CONFIG:Debug>:-g -O0>
    $<$<CONFIG:Release>:-O3 -DNDEBUG>
)

# Compiler-specific flags
target_compile_options(mylib PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)
```

### Custom Functions

```cmake
# Reusable function for consistent target configuration
function(configure_cpp_target TARGET_NAME)
    target_compile_features(${TARGET_NAME} PUBLIC cxx_std_17)

    if(COMMAND apply_compiler_options)
        apply_compiler_options(${TARGET_NAME})
    endif()

    set_target_properties(${TARGET_NAME} PROPERTIES
        CXX_EXTENSIONS OFF
        POSITION_INDEPENDENT_CODE ON
    )
endfunction()

# Usage
add_library(mylib STATIC src/mylib.cpp)
configure_cpp_target(mylib)
```

## Contributing Guidelines

### Development Workflow

1. **Fork the repository**
2. **Create a feature branch**: `git checkout -b feature/new-feature`
3. **Make changes** following coding standards
4. **Add tests** for new functionality
5. **Update documentation** as needed
6. **Ensure all tests pass**: `cmake --build build --target test`
7. **Submit a pull request**

### Code Review Checklist

- [ ] Code follows project coding standards
- [ ] All new functionality has tests
- [ ] Tests pass on all supported platforms
- [ ] Documentation is updated
- [ ] CMake configuration is correct
- [ ] No unnecessary dependencies added
- [ ] Performance impact considered
- [ ] Error handling is appropriate

### Commit Message Format

```
type(scope): brief description

Detailed explanation of the change, including:
- What was changed and why
- Any breaking changes
- References to issues or tickets

Closes #123
```

Types: `feat`, `fix`, `docs`, `style`, `refactor`, `test`, `chore`

### Release Process

1. **Update version numbers** in CMakeLists.txt and vcpkg.JSON
2. **Update CHANGELOG.md** with new features and fixes
3. **Create release branch**: `git checkout -b release/v1.2.0`
4. **Final testing** on all supported platforms
5. **Merge to main** and tag release
6. **Create GitHub release** with release notes

This developer guide should be updated as the project evolves and new patterns emerge. For questions or clarifications, please open an issue or start a discussion.
