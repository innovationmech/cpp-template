# ============================================================================= Source Directory
# CMakeLists.txt Build configuration for application source code and main executable
#
# This file defines the main application components: 1. Implementation library (cpp-template-impl) -
# Contains core application logic 2. Main executable (cpp-template-app) - Application entry point 3.
# Application modules - Modular components for specific functionality
#
# The structure demonstrates: - Separation of interface and implementation - Modular architecture
# with clear dependencies - Proper linking and include directory management - Consistent compiler
# and build settings
# =============================================================================

message(STATUS "Configuring application source code...")

# ============================================================================= Implementation
# Library Target
# =============================================================================

# Create a static library for the public interface implementation This library contains the actual
# implementation of the public API defined in the include/ directory. Using a separate
# implementation library provides: - Better build times (implementation changes don't require
# relinking executables) - Easier testing (tests can link against the implementation directly) -
# Cleaner separation between interface and implementation - Reusability across multiple executables
# if needed
add_library(
    cpp-template-impl STATIC core_impl.cpp # Implementation of core functionality
                             utils_impl.cpp # Implementation of utility functions
)

# Set descriptive target properties for better IDE integration
set_target_properties(
    cpp-template-impl
    PROPERTIES # Enforce C++17 standard for this target
               CXX_STANDARD 17
               CXX_STANDARD_REQUIRED ON
               CXX_EXTENSIONS OFF
               # Set output name and version information
               OUTPUT_NAME "cpp-template-impl"
               VERSION ${PROJECT_VERSION}
               # Add target description for documentation
               DESCRIPTION "Implementation library for cpp-template public API")

# ============================================================================= Implementation
# Library Dependencies
# =============================================================================

# Link to the internal core library PUBLIC linkage means that targets linking to cpp-template-impl
# will also automatically link to the core library and inherit its interface
target_link_libraries(
    cpp-template-impl PUBLIC core # Internal core library providing fundamental functionality
)

# ============================================================================= Implementation
# Library Include Directories
# =============================================================================

# Configure include directories for the implementation library
target_include_directories(
    cpp-template-impl
    # PUBLIC includes are available to both this target and consumers
    PUBLIC # Public API headers that define the interface Use generator expressions for proper
           # build/install interface
           $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>
    # PRIVATE includes are only available to this target during compilation
    PRIVATE # Internal library headers for implementation details
            ${CMAKE_SOURCE_DIR}/libs/core/include
            # Current source directory for internal headers
            ${CMAKE_CURRENT_SOURCE_DIR})

# ============================================================================= Compiler Options for
# Implementation Library
# =============================================================================

# Apply standardized compiler options from the custom CompilerOptions module This ensures consistent
# warning levels, optimization settings, and platform-specific configurations across all targets
if(COMMAND apply_compiler_options)
    apply_compiler_options(cpp-template-impl)
    message(STATUS "Applied compiler options to cpp-template-impl")
else()
    # Fallback compiler options if the custom module is not available
    target_compile_options(
        cpp-template-impl
        PRIVATE # GCC and Clang warning flags
                $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
                -Wall
                -Wextra
                -Wpedantic
                -Wshadow
                -Wconversion
                -Wsign-conversion
                >
                # MSVC warning flags
                $<$<CXX_COMPILER_ID:MSVC>:
                /W4
                /permissive-
                >)
    message(STATUS "Applied fallback compiler options to cpp-template-impl")
endif()

# ============================================================================= Application Modules
# =============================================================================

# Include the modules subdirectory which contains modular application components Modules are built
# as separate libraries to promote: - Modular design and clear separation of concerns - Independent
# testing of individual components - Easier maintenance and code organization - Potential reuse in
# other projects
add_subdirectory(modules)
message(STATUS "Included application modules")

# ============================================================================= Main Executable
# Target
# =============================================================================

# Create the main application executable This is the primary entry point for the application and
# demonstrates how to use the implementation library and application modules
add_executable(cpp-template-app main.cpp # Application entry point with main() function
)

# Set target properties for the executable
set_target_properties(
    cpp-template-app
    PROPERTIES # Enforce C++17 standard consistency
               CXX_STANDARD 17
               CXX_STANDARD_REQUIRED ON
               CXX_EXTENSIONS OFF
               # Set executable name and version
               OUTPUT_NAME "cpp-template" # Final executable name
               VERSION ${PROJECT_VERSION}
               # Add target description
               DESCRIPTION "Main executable for cpp-template application"
               # Set runtime output directory (defined in top-level CMakeLists.txt)
               RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# ============================================================================= Main Executable
# Dependencies
# =============================================================================

# Link the executable to required libraries PRIVATE linkage is used because the executable is the
# final consumer and doesn't need to expose these dependencies to other targets
target_link_libraries(
    cpp-template-app
    PRIVATE # Implementation library containing core application logic
            cpp-template-impl
            # Application modules providing specific functionality
            data-processor # Data processing and transformation
            config-manager # Configuration management and parsing
)

# ============================================================================= Main Executable
# Compiler Options
# =============================================================================

# Apply the same compiler options to the executable for consistency
if(COMMAND apply_compiler_options)
    apply_compiler_options(cpp-template-app)
    message(STATUS "Applied compiler options to cpp-template-app")
else()
    # Fallback compiler options
    target_compile_options(
        cpp-template-app
        PRIVATE $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
                -Wall
                -Wextra
                -Wpedantic
                >
                $<$<CXX_COMPILER_ID:MSVC>:
                /W4
                >)
    message(STATUS "Applied fallback compiler options to cpp-template-app")
endif()

# ============================================================================= Optional Third-Party
# Dependencies
# =============================================================================

# Link against available third-party libraries if they were found This demonstrates how to
# conditionally use dependencies based on availability

# Example: Link against fmt library if available (from vcpkg or system)
if(TARGET third_party::fmt)
    target_link_libraries(cpp-template-impl PRIVATE third_party::fmt)
    target_compile_definitions(cpp-template-impl PRIVATE HAVE_FMT=1)
    message(STATUS "Linked cpp-template-impl with fmt library")
endif()

# Example: Link against spdlog if available
if(TARGET third_party::spdlog)
    target_link_libraries(cpp-template-impl PRIVATE third_party::spdlog)
    target_compile_definitions(cpp-template-impl PRIVATE HAVE_SPDLOG=1)
    message(STATUS "Linked cpp-template-impl with spdlog library")
endif()

# Example: Link against JSON library if available
if(TARGET third_party::json)
    target_link_libraries(cpp-template-impl PRIVATE third_party::json)
    target_compile_definitions(cpp-template-impl PRIVATE HAVE_JSON=1)
    message(STATUS "Linked cpp-template-impl with nlohmann/json library")
endif()

# ============================================================================= Installation
# Configuration (Optional)
# =============================================================================

# Configure installation rules for the executable and libraries This allows the project to be
# installed system-wide or packaged

# Install the main executable
install(TARGETS cpp-template-app RUNTIME DESTINATION bin COMPONENT Runtime)

# Install the implementation library (if needed for development)
install(
    TARGETS cpp-template-impl
    EXPORT ${PROJECT_NAME}Targets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib COMPONENT Development)

# Install public headers
install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/include/
    DESTINATION include
    COMPONENT Development
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.hpp")

# ============================================================================= Build Summary
# =============================================================================

message(STATUS "Application source configuration complete:")
message(STATUS "  - Implementation library: cpp-template-impl")
message(STATUS "  - Main executable: cpp-template-app")
message(STATUS "  - Application modules: included from modules/")
message(STATUS "  - Third-party dependencies: conditionally linked")
