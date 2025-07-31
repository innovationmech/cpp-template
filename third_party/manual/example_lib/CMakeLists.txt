# Example Manual Dependency Library
# This demonstrates how to integrate manually managed third-party libraries

# Create a static library for the manual dependency
add_library(example_lib STATIC
    src/math_utils.cpp
)

# Set target properties
target_include_directories(example_lib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Set C++ standard for this library
target_compile_features(example_lib PUBLIC cxx_std_17)

# Create an alias for consistent naming with other dependencies
add_library(manual::example_lib ALIAS example_lib)

# Optional: Add compiler-specific options
if(MSVC)
    target_compile_options(example_lib PRIVATE /W4)
else()
    target_compile_options(example_lib PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Optional: Set library properties
set_target_properties(example_lib PROPERTIES
    OUTPUT_NAME "example_lib"
    VERSION 1.0.0
    SOVERSION 1
)

# Export the target for use by other parts of the project
if(NOT TARGET manual_dependencies)
    add_library(manual_dependencies INTERFACE)
endif()

target_link_libraries(manual_dependencies INTERFACE example_lib)