# =============================================================================
# Top-level CMakeLists.txt for cpp-template project
# Modern C++ Project Template with comprehensive build configuration
# 
# This file serves as the main entry point for the CMake build system and
# establishes the global configuration for the entire project. It demonstrates
# modern CMake practices and provides a foundation for C++ projects with
# multiple dependency management strategies.
# =============================================================================

# Require minimum CMake version for modern features
# Version 3.20 provides:
# - Improved C++20 support
# - Better vcpkg integration
# - Enhanced target property handling
# - Improved generator expressions
cmake_minimum_required(VERSION 3.20)

# =============================================================================
# Project Definition and Metadata
# =============================================================================

# Define project with comprehensive metadata
# This information is used by:
# - Package managers (vcpkg, Conan)
# - IDE integration
# - Documentation generation
# - Installation and packaging
project(cpp-template 
    VERSION 1.0.0 
    DESCRIPTION "Modern C++ Project Template demonstrating best practices"
    HOMEPAGE_URL "https://github.com/example/cpp-template"
    LANGUAGES CXX
)

# Display project information for build logs
message(STATUS "=== ${PROJECT_NAME} v${PROJECT_VERSION} ===")
message(STATUS "Description: ${PROJECT_DESCRIPTION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Generator: ${CMAKE_GENERATOR}")
message(STATUS "Source directory: ${CMAKE_SOURCE_DIR}")
message(STATUS "Binary directory: ${CMAKE_BINARY_DIR}")

# =============================================================================
# C++ Standard Configuration (Requirements 1.1, 1.2)
# =============================================================================

# Set C++17 as the minimum standard for modern features:
# - Structured bindings
# - std::optional, std::variant
# - Filesystem library
# - Parallel algorithms
# - Improved constexpr support
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # Fail if C++17 is not available
set(CMAKE_CXX_EXTENSIONS OFF)        # Use standard C++, not compiler extensions

message(STATUS "C++ Standard: C++${CMAKE_CXX_STANDARD}")

# =============================================================================
# Build Configuration (Requirement 4.1)
# =============================================================================

# Set default build type if not specified
# Release is chosen as default for better performance in production
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build" FORCE)
    # Provide available options for CMake GUI and ccmake
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS 
        "Debug"           # No optimization, debug symbols, assertions enabled
        "Release"         # Full optimization, no debug symbols
        "MinSizeRel"      # Optimize for size
        "RelWithDebInfo"  # Optimization with debug symbols
    )
endif()

# Display build configuration information
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Debug build: Optimizations disabled, debug symbols enabled")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Release build: Full optimizations enabled, debug symbols disabled")
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    message(STATUS "Release with debug info: Optimizations and debug symbols enabled")
elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    message(STATUS "Minimum size release: Size optimizations enabled")
endif()

# =============================================================================
# Global Compiler and Linker Settings
# =============================================================================

# Enable position independent code for all targets
# This is required for:
# - Shared libraries on most platforms
# - Static libraries that may be linked into shared libraries
# - Better security (ASLR support)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Set symbol visibility to hidden by default
# This provides:
# - Better performance (fewer symbol relocations)
# - Cleaner API surface (only explicitly exported symbols are visible)
# - Reduced binary size
# - Better optimization opportunities
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

# Enable interprocedural optimization (Link Time Optimization) for Release builds
# This can significantly improve performance but increases build time
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    include(CheckIPOSupported)
    check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error)
    if(ipo_supported)
        message(STATUS "IPO/LTO enabled for Release build")
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    else()
        message(STATUS "IPO/LTO not supported: ${ipo_error}")
    endif()
endif()

# =============================================================================
# CMake Module Path Configuration (Requirement 4.1)
# =============================================================================

# Add custom CMake modules directory to module path
# This allows us to use custom find modules and utility functions
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
message(STATUS "CMake module path: ${CMAKE_MODULE_PATH}")

# =============================================================================
# Platform Detection and Configuration
# =============================================================================

# Detect and configure platform-specific settings
if(WIN32)
    message(STATUS "Platform: Windows")
    # Windows-specific configuration
    add_compile_definitions(
        WIN32_LEAN_AND_MEAN    # Exclude rarely-used Windows headers
        NOMINMAX               # Prevent Windows.h from defining min/max macros
        _CRT_SECURE_NO_WARNINGS # Disable MSVC security warnings for standard functions
    )
elseif(APPLE)
    message(STATUS "Platform: macOS")
    # macOS-specific configuration
    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
        message(STATUS "Architecture: Apple Silicon (ARM64)")
    else()
        message(STATUS "Architecture: Intel x64")
    endif()
elseif(UNIX)
    message(STATUS "Platform: Linux/Unix")
    # Linux-specific configuration
    # Enable GNU extensions for better performance and functionality
    add_compile_definitions(_GNU_SOURCE)
endif()

# =============================================================================
# vcpkg Integration (Requirement 3.3)
# =============================================================================

# Detect and configure vcpkg integration
# vcpkg toolchain should be set via CMAKE_TOOLCHAIN_FILE
# Example usage:
#   cmake -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
if(DEFINED CMAKE_TOOLCHAIN_FILE AND CMAKE_TOOLCHAIN_FILE MATCHES "vcpkg")
    message(STATUS "vcpkg integration detected")
    message(STATUS "Toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
    
    # Set vcpkg target triplet if not already specified
    # The triplet determines the target architecture and library linkage
    if(NOT DEFINED VCPKG_TARGET_TRIPLET)
        if(WIN32)
            if(CMAKE_SIZEOF_VOID_P EQUAL 8)
                set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "vcpkg target triplet")
            else()
                set(VCPKG_TARGET_TRIPLET "x86-windows" CACHE STRING "vcpkg target triplet")
            endif()
        elseif(APPLE)
            if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
                set(VCPKG_TARGET_TRIPLET "arm64-osx" CACHE STRING "vcpkg target triplet")
            else()
                set(VCPKG_TARGET_TRIPLET "x64-osx" CACHE STRING "vcpkg target triplet")
            endif()
        else()
            if(CMAKE_SIZEOF_VOID_P EQUAL 8)
                set(VCPKG_TARGET_TRIPLET "x64-linux" CACHE STRING "vcpkg target triplet")
            else()
                set(VCPKG_TARGET_TRIPLET "x86-linux" CACHE STRING "vcpkg target triplet")
            endif()
        endif()
    endif()
    
    message(STATUS "vcpkg target triplet: ${VCPKG_TARGET_TRIPLET}")
    
    # Enable vcpkg manifest mode features if specified
    if(DEFINED VCPKG_MANIFEST_FEATURES)
        message(STATUS "vcpkg manifest features: ${VCPKG_MANIFEST_FEATURES}")
    endif()
else()
    message(STATUS "vcpkg not detected - using alternative dependency resolution")
    message(STATUS "To use vcpkg, configure with:")
    message(STATUS "  cmake -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake")
endif()

# =============================================================================
# Custom CMake Modules
# =============================================================================

# Include custom compiler options module
# This provides standardized compiler warnings and optimization settings
include(CompilerOptions)
message(STATUS "Loaded CompilerOptions module")

# Include vcpkg helper utilities
# This provides functions for vcpkg integration and fallback dependency resolution
include(VcpkgHelpers)
message(STATUS "Loaded VcpkgHelpers module")

# =============================================================================
# Development Tools Configuration
# =============================================================================

# Export compile commands for IDE integration and static analysis tools
# This generates compile_commands.json used by:
# - Language servers (clangd, ccls)
# - Static analysis tools (clang-tidy, cppcheck)
# - IDEs (Visual Studio Code, CLion)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
message(STATUS "Compile commands export: ${CMAKE_EXPORT_COMPILE_COMMANDS}")

# Configure output directories for better organization
# This keeps build artifacts organized and makes cleanup easier
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)  # Executables
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)  # Shared libraries
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)  # Static libraries

message(STATUS "Output directories:")
message(STATUS "  Executables: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "  Libraries: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

# =============================================================================
# Testing Configuration
# =============================================================================

# Enable CTest for running tests
# This provides:
# - Test discovery and execution
# - Parallel test execution
# - Test result reporting
# - Integration with CI/CD systems
enable_testing()
message(STATUS "Testing enabled with CTest")

# Set test timeout (in seconds) to prevent hanging tests
set(CTEST_TEST_TIMEOUT 300)

# Configure test output
set(CTEST_OUTPUT_ON_FAILURE ON)  # Show test output when tests fail

# =============================================================================
# Git Integration
# =============================================================================

# Find Git for submodule management
find_package(Git QUIET)
if(GIT_FOUND)
    message(STATUS "Git found: ${GIT_EXECUTABLE}")
    
    # Get current Git commit hash for version information
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    
    if(GIT_COMMIT_HASH)
        message(STATUS "Git commit: ${GIT_COMMIT_HASH}")
        add_compile_definitions(GIT_COMMIT_HASH="${GIT_COMMIT_HASH}")
    endif()
    
    # Check if there are uncommitted changes
    execute_process(
        COMMAND ${GIT_EXECUTABLE} diff-index --quiet HEAD --
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE GIT_DIRTY
        ERROR_QUIET
    )
    
    if(GIT_DIRTY)
        message(STATUS "Git status: Working directory has uncommitted changes")
        add_compile_definitions(GIT_DIRTY=1)
    else()
        message(STATUS "Git status: Working directory is clean")
    endif()
else()
    message(STATUS "Git not found - submodule management not available")
endif()

# =============================================================================
# Subdirectory Inclusion (Requirement 4.3)
# =============================================================================

# Include subdirectories in dependency order
# The order matters because later subdirectories may depend on earlier ones

message(STATUS "=== Configuring Subdirectories ===")

# 1. Internal libraries (no external dependencies)
message(STATUS "Configuring internal libraries...")
add_subdirectory(libs)

# 2. Third-party dependencies (may be used by src and tests)
message(STATUS "Configuring third-party dependencies...")
add_subdirectory(third_party)

# 3. Application source code (depends on libs and third_party)
message(STATUS "Configuring application source...")
add_subdirectory(src)

# 4. Tests (depends on all previous components)
message(STATUS "Configuring tests...")
add_subdirectory(tests)

message(STATUS "=== Configuration Complete ===")

# =============================================================================
# Build Summary and Information
# =============================================================================

# Display final build configuration summary
message(STATUS "")
message(STATUS "=== Build Configuration Summary ===")
message(STATUS "Project: ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Platform: ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_PROCESSOR}")

# Display enabled features
if(DEFINED CMAKE_TOOLCHAIN_FILE AND CMAKE_TOOLCHAIN_FILE MATCHES "vcpkg")
    message(STATUS "Package manager: vcpkg")
else()
    message(STATUS "Package manager: system/manual")
endif()

if(CMAKE_EXPORT_COMPILE_COMMANDS)
    message(STATUS "IDE support: compile_commands.json enabled")
endif()

if(BUILD_TESTING)
    message(STATUS "Testing: enabled")
else()
    message(STATUS "Testing: disabled")
endif()

message(STATUS "=====================================")
message(STATUS "")