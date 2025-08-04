# Examples CMakeLists.txt This file builds all the example programs that demonstrate how to use the
# cpp-template library

cmake_minimum_required(VERSION 3.20)

# Function to create example executables with consistent configuration
function(add_example_executable name)
    cmake_parse_arguments(PARSE_ARGV 1 ARG "" "" "SOURCES;LIBRARIES;DEFINITIONS")

    # Create the executable
    add_executable(${name} ${ARG_SOURCES})

    # Set output directory
    set_target_properties(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples)

    # Link required libraries
    target_link_libraries(${name} PRIVATE cpp-template-impl ${ARG_LIBRARIES})

    # Add include directories
    target_include_directories(${name} PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src)

    # Add compile definitions if provided
    if(ARG_DEFINITIONS)
        target_compile_definitions(${name} PRIVATE ${ARG_DEFINITIONS})
    endif()

    # Apply compiler options
    if(COMMAND apply_compiler_options)
        apply_compiler_options(${name})
    endif()

    # Add to examples target group
    set_target_properties(${name} PROPERTIES FOLDER "Examples")
endfunction()

# Basic Examples
add_example_executable(basic_core_usage SOURCES basic_core_usage.cpp)

add_example_executable(string_utilities_demo SOURCES string_utilities_demo.cpp)

# Advanced Examples
add_example_executable(modular_architecture_demo SOURCES modular_architecture_demo.cpp LIBRARIES
                       config-manager data-processor)

add_example_executable(config_management_demo SOURCES config_management_demo.cpp LIBRARIES
                       config-manager)

add_example_executable(performance_benchmark SOURCES performance_benchmark.cpp)

# Integration Examples (only if third-party libraries are available)
if(TARGET nlohmann_json::nlohmann_json)
    add_example_executable(
        third_party_integration
        SOURCES
        third_party_integration.cpp
        LIBRARIES
        nlohmann_json::nlohmann_json
        DEFINITIONS
        HAS_NLOHMANN_JSON=1)
endif()

# Validation Example
add_example_executable(validation_demo SOURCES validation_demo.cpp)

# Custom target to build all examples
add_custom_target(
    examples
    DEPENDS basic_core_usage
            string_utilities_demo
            modular_architecture_demo
            config_management_demo
            performance_benchmark
            validation_demo
            $<$<TARGET_EXISTS:third_party_integration>:third_party_integration>)

# Add examples to the main build if requested
if(BUILD_EXAMPLES)
    message(STATUS "Building examples")
else()
    message(
        STATUS
            "Examples available but not built by default. Use 'cmake --build build --target examples' to build them."
    )
endif()

# Install examples (optional)
if(INSTALL_EXAMPLES)
    install(
        TARGETS basic_core_usage string_utilities_demo modular_architecture_demo
                config_management_demo performance_benchmark validation_demo
        DESTINATION bin/examples
        COMPONENT examples)

    install(
        FILES README.md customization_guide.md
        DESTINATION share/doc/cpp-template/examples
        COMPONENT examples)
endif()
