# ==============================================================================
# cpp-template Project - Main CMake Configuration
# ==============================================================================
#
# @file CMakeLists.txt
# @brief Main CMake configuration file for the cpp-template project
#
# This file defines the primary build configuration for the cpp-template project,
# including compiler settings, subdirectory management, and build options.
# It serves as the entry point for the entire build system.
#
# Key Features:
# - Modern CMake practices (3.20+)
# - C++17 standard compliance
# - Modular subdirectory organization
# - Flexible build configuration
# - Cross-platform compatibility
# - Testing framework integration
#
# Build Requirements:
# - CMake 3.20 or higher
# - C++17 compatible compiler
# - Optional: Google Test for testing
# - Optional: vcpkg for dependency management
#
# Usage:
#   mkdir build && cd build
#   cmake ..
#   cmake --build .
#
# @author cpp-template project
# @version 1.0.0
# @since 1.0.0
# ==============================================================================

# Minimum CMake version requirement
# Using 3.20 for modern CMake features and better C++17 support
cmake_minimum_required(VERSION 3.20)

# Project declaration with version and language specification
# Version follows semantic versioning (MAJOR.MINOR.PATCH)
project(cpp-template 
    VERSION 1.0.0 
    LANGUAGES CXX
    DESCRIPTION "A modern C++ project template with modular architecture"
    HOMEPAGE_URL "https://github.com/your-username/cpp-template"
)

# ==============================================================================
# C++ Standard Configuration
# ==============================================================================

# Set C++17 as the standard for all targets
# C++17 provides modern features while maintaining broad compatibility
set(CMAKE_CXX_STANDARD 17)

# Require C++17 compliance - fail if compiler doesn't support it
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Disable compiler-specific extensions for better portability
# This ensures code works across different compilers (GCC, Clang, MSVC)
set(CMAKE_CXX_EXTENSIONS OFF)

# ==============================================================================
# CMake Module Configuration
# ==============================================================================

# Add custom CMake modules directory to module path
# This allows including custom .cmake files for extended functionality
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Include compiler-specific options and optimizations
# CompilerOptions.cmake contains platform and compiler specific settings
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/cmake/CompilerOptions.cmake)
    include(CompilerOptions)
    message(STATUS "Loaded custom compiler options")
else()
    message(STATUS "No custom compiler options found - using defaults")
endif()

# ==============================================================================
# Subdirectory Management
# ==============================================================================
# Add subdirectories conditionally based on the presence of CMakeLists.txt
# This approach provides flexibility for partial builds and optional components

# External libraries and dependencies
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/libs/CMakeLists.txt)
    add_subdirectory(libs)
    message(STATUS "Added libs subdirectory")
endif()

# Main source code - core functionality
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/CMakeLists.txt)
    add_subdirectory(src)
    message(STATUS "Added src subdirectory")
else()
    message(WARNING "No src/CMakeLists.txt found - main source code missing")
endif()

# Third-party dependencies (if not using package managers)
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/third_party/CMakeLists.txt)
    add_subdirectory(third_party)
    message(STATUS "Added third_party subdirectory")
endif()

# Test suite - unit and integration tests
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt)
    add_subdirectory(tests)
    message(STATUS "Added tests subdirectory")
else()
    message(STATUS "No tests found - testing disabled")
endif()

# Example applications and demonstrations
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/examples/CMakeLists.txt)
    add_subdirectory(examples)
    message(STATUS "Added examples subdirectory")
endif()

# ==============================================================================
# Testing Configuration
# ==============================================================================

# Enable CTest integration for the entire project
# This allows running tests with 'ctest' command and integrates with IDEs
enable_testing()

# Print build configuration summary
message(STATUS "")
message(STATUS "=== cpp-template Build Configuration ===")
message(STATUS "Project: ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Source Directory: ${CMAKE_SOURCE_DIR}")
message(STATUS "Binary Directory: ${CMAKE_BINARY_DIR}")
message(STATUS "Testing: Enabled")
message(STATUS "========================================")
message(STATUS "")