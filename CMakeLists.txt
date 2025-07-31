# Top-level CMakeLists.txt for cpp-template project
# Modern C++ Project Template with comprehensive build configuration

cmake_minimum_required(VERSION 3.20)

# Project metadata and configuration
project(cpp-template 
    VERSION 1.0.0 
    DESCRIPTION "Modern C++ Project Template"
    LANGUAGES CXX
)

# Set C++ standard (Requirement 1.1, 1.2)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Global compiler options and build type configurations (Requirement 4.1)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Enable position independent code
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Set default visibility to hidden
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

# Add custom CMake modules (Requirement 4.1)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# vcpkg integration (Requirement 3.3)
# Note: vcpkg toolchain should be set via CMAKE_TOOLCHAIN_FILE
# Example: cmake -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
if(DEFINED CMAKE_TOOLCHAIN_FILE AND CMAKE_TOOLCHAIN_FILE MATCHES "vcpkg")
    message(STATUS "Using vcpkg toolchain: ${CMAKE_TOOLCHAIN_FILE}")
    # Set vcpkg target triplet if not already set
    if(NOT DEFINED VCPKG_TARGET_TRIPLET)
        if(WIN32)
            set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "vcpkg target triplet")
        elseif(APPLE)
            set(VCPKG_TARGET_TRIPLET "x64-osx" CACHE STRING "vcpkg target triplet")
        else()
            set(VCPKG_TARGET_TRIPLET "x64-linux" CACHE STRING "vcpkg target triplet")
        endif()
    endif()
    message(STATUS "vcpkg target triplet: ${VCPKG_TARGET_TRIPLET}")
endif()

# Include custom compiler options
include(CompilerOptions)

# Include vcpkg helpers
include(VcpkgHelpers)

# Export compile commands for IDEs
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable testing
enable_testing()

# Include subdirectories (Requirement 4.3)
add_subdirectory(libs)
add_subdirectory(src)
add_subdirectory(third_party)
add_subdirectory(tests)