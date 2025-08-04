# ============================================================================= Core Library
# CMakeLists.txt Internal project library providing fundamental functionality
#
# This library serves as the foundation for the cpp-template project and demonstrates best practices
# for creating reusable C++ libraries: - Modern CMake target-based configuration - Proper include
# directory management with generator expressions - Clear separation of public interface and private
# implementation - Export configuration for use by other targets and external projects -
# Comprehensive source file organization
#
# The core library provides: - Basic utility functions and data structures - Common algorithms and
# helper functions - Platform abstraction layers - Fundamental types and constants used throughout
# the project
# =============================================================================

message(STATUS "Configuring core library...")

# ============================================================================= Core Library Target
# Definition
# =============================================================================

# Create a static library target for the core functionality Static linking is chosen for: - Better
# performance (no runtime linking overhead) - Simpler deployment (no separate library files to
# distribute) - Better optimization opportunities (whole program optimization) - Easier debugging
# (all code is in the main executable)
add_library(core STATIC)

# Alternative: Create a shared library (uncomment if needed) add_library(core SHARED) This would be
# useful for: - Reduced memory usage when multiple executables use the library - Easier updates
# (library can be updated without recompiling executables) - Plugin architectures

# ============================================================================= Target Properties
# Configuration
# =============================================================================

# Set comprehensive target properties for the core library
set_target_properties(
    core
    PROPERTIES # C++ Standard Configuration
               # Enforce C++17 for modern language features and standard library
               CXX_STANDARD 17
               CXX_STANDARD_REQUIRED ON # Fail build if C++17 is not available
               CXX_EXTENSIONS OFF # Use standard C++, not compiler-specific extensions
               # Library Naming and Versioning
               OUTPUT_NAME "cpp-template-core" # Final library name
               VERSION ${PROJECT_VERSION} # Library version for shared libraries
               SOVERSION ${PROJECT_VERSION_MAJOR} # ABI version for shared libraries
               # Export Configuration
               # This name is used when the library is exported for external use
               EXPORT_NAME Core
               # Position Independent Code
               # Required for shared libraries and recommended for static libraries
               # that might be linked into shared libraries
               POSITION_INDEPENDENT_CODE ON
               # Symbol Visibility
               # Control which symbols are exported from the library
               CXX_VISIBILITY_PRESET hidden
               VISIBILITY_INLINES_HIDDEN ON
               # Target Description
               DESCRIPTION "Core functionality library for cpp-template project")

# ============================================================================= Source Files
# Configuration
# =============================================================================

# Add source files to the target using modern CMake target_sources This approach is preferred over
# listing sources in add_library because: - It's more flexible and allows conditional source
# addition - It clearly separates public interface from private implementation - It supports
# generator expressions for platform-specific sources
target_sources(
    core
    # PRIVATE sources are implementation files that are compiled into the library but are not part
    # of the public interface
    PRIVATE src/core.cpp # Core functionality implementation
            src/utils.cpp # Utility functions implementation
            # Platform-specific sources can be added conditionally
            $<$<PLATFORM_ID:Windows>:src/platform/windows_utils.cpp>
            $<$<PLATFORM_ID:Linux>:src/platform/linux_utils.cpp>
            $<$<PLATFORM_ID:Darwin>:src/platform/macos_utils.cpp>)

# ============================================================================= Include Directory
# Configuration
# =============================================================================

# Configure include directories using generator expressions for proper handling in both build and
# install contexts
target_include_directories(
    core
    # PUBLIC include directories are available to both this target and any targets that link against
    # it
    PUBLIC # BUILD_INTERFACE: Used when building the project This allows targets in the same project
           # to find headers
           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
           # INSTALL_INTERFACE: Used when the library is installed and used by external projects
           # This specifies where headers will be located after installation
           $<INSTALL_INTERFACE:include>
    # PRIVATE include directories are only available to this target during compilation
    PRIVATE # Source directory for internal implementation headers
            ${CMAKE_CURRENT_SOURCE_DIR}/src
            # Include any internal utility headers
            ${CMAKE_CURRENT_SOURCE_DIR}/src/internal)

# ============================================================================= Compiler Options and
# Definitions
# =============================================================================

# Apply standardized compiler options from the custom CompilerOptions module This ensures consistent
# warning levels, optimization settings, and platform-specific configurations
if(COMMAND apply_compiler_options)
    apply_compiler_options(core)
    message(STATUS "Applied custom compiler options to core library")
else()
    # Fallback compiler options if the custom module is not available
    message(STATUS "Custom compiler options not available, using fallback")

    target_compile_options(
        core
        PRIVATE # GCC and Clang compiler options
                $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
                -Wall # Enable most warning messages
                -Wextra # Enable extra warning messages
                -Wpedantic # Enable pedantic warnings for strict standard compliance
                -Wshadow # Warn about variable shadowing
                -Wconversion # Warn about type conversions that may lose data
                -Wsign-conversion # Warn about sign conversions
                -Wunused # Warn about unused variables and functions
                -Wformat=2 # Enable format string security warnings
                >
                # MSVC compiler options
                $<$<CXX_COMPILER_ID:MSVC>:
                /W4 # Enable high warning level
                /permissive- # Disable non-conforming code
                /w14640 # Enable warning for thread-unsafe static member initialization
                >)
endif()

# Add compile definitions for the core library
target_compile_definitions(
    core
    # PUBLIC definitions are available to both this target and consumers
    PUBLIC # Version information
           CORE_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
           CORE_VERSION_MINOR=${PROJECT_VERSION_MINOR}
           CORE_VERSION_PATCH=${PROJECT_VERSION_PATCH}
           # Feature flags
           $<$<CONFIG:Debug>:CORE_DEBUG_MODE=1>
           $<$<CONFIG:Release>:CORE_RELEASE_MODE=1>
    # PRIVATE definitions are only available to this target
    PRIVATE # Internal build configuration
            CORE_BUILDING_LIBRARY=1
            # Platform-specific definitions
            $<$<PLATFORM_ID:Windows>:CORE_PLATFORM_WINDOWS=1>
            $<$<PLATFORM_ID:Linux>:CORE_PLATFORM_LINUX=1>
            $<$<PLATFORM_ID:Darwin>:CORE_PLATFORM_MACOS=1>)

# ============================================================================= Dependencies
# =============================================================================

# Link against system libraries that the core library depends on
target_link_libraries(
    core
    # PUBLIC dependencies are propagated to consumers of this library
    PUBLIC # Standard threading library (if needed)
           $<$<BOOL:${CMAKE_USE_PTHREADS_INIT}>:Threads::Threads>
    # PRIVATE dependencies are only used by this library internally
    PRIVATE # Platform-specific system libraries
            $<$<PLATFORM_ID:Windows>:ws2_32> # Windows sockets
            $<$<PLATFORM_ID:Linux>:dl> # Dynamic loading
)

# Link against available third-party libraries if they were found This demonstrates conditional
# dependency usage

# Example: Use fmt library for better string formatting if available
if(TARGET third_party::fmt)
    target_link_libraries(core PRIVATE third_party::fmt)
    target_compile_definitions(core PRIVATE CORE_HAS_FMT=1)
    message(STATUS "Core library will use fmt for string formatting")
endif()

# Example: Use spdlog for logging if available
if(TARGET third_party::spdlog)
    target_link_libraries(core PRIVATE third_party::spdlog)
    target_compile_definitions(core PRIVATE CORE_HAS_SPDLOG=1)
    message(STATUS "Core library will use spdlog for logging")
endif()

# ============================================================================= Testing Support
# =============================================================================

# Enable testing for this library if BUILD_TESTING is enabled
if(BUILD_TESTING)
    # Add a custom property to track that this library supports testing
    set_target_properties(core PROPERTIES HAS_TESTS TRUE TEST_DIRECTORY
                                                         "${CMAKE_CURRENT_SOURCE_DIR}/tests")

    message(STATUS "Core library testing support enabled")
endif()

# ============================================================================= Installation
# Configuration
# =============================================================================

# Configure installation rules for the core library This allows the library to be installed and used
# by external projects

# Install the library target
install(
    TARGETS core
    # Export set for find_package support
    EXPORT ${PROJECT_NAME}Targets
    # Installation destinations
    ARCHIVE DESTINATION lib # Static libraries
    LIBRARY DESTINATION lib # Shared libraries
    RUNTIME DESTINATION bin # DLLs on Windows
            # Component for selective installation
            COMPONENT Development # Include directories for installed target
    INCLUDES
    DESTINATION include)

# Install public headers
install(
    DIRECTORY include/
    DESTINATION include
    COMPONENT Development
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.hpp"
    PATTERN "*.hxx")

# ============================================================================= Export Configuration
# =============================================================================

# Create an alias target for consistent naming within the project This allows other targets to use
# core::core instead of just core
add_library(core::core ALIAS core)

# Export the target for use by external projects This is used by the top-level CMakeLists.txt to
# create a package config file
set_target_properties(core PROPERTIES EXPORT_NAME Core INTERFACE_COMPILE_FEATURES cxx_std_17)

# ============================================================================= Build Summary
# =============================================================================

# Display configuration summary for the core library
message(STATUS "Core library configuration complete:")
message(STATUS "  - Library type: STATIC")
message(STATUS "  - C++ standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "  - Output name: cpp-template-core")
message(STATUS "  - Version: ${PROJECT_VERSION}")
message(STATUS "  - Export name: Core")

# List source files for verification
get_target_property(CORE_SOURCES core SOURCES)
message(STATUS "  - Source files: ${CORE_SOURCES}")

# List dependencies
get_target_property(CORE_LINK_LIBRARIES core LINK_LIBRARIES)
if(CORE_LINK_LIBRARIES)
    message(STATUS "  - Dependencies: ${CORE_LINK_LIBRARIES}")
else()
    message(STATUS "  - Dependencies: none")
endif()
