# Examples demonstrating dependency management

# Create dependency demo executable
add_executable(dependency_demo dependency_demo.cpp)

# Link against internal libraries
target_link_libraries(dependency_demo PRIVATE core)

# Link against manual dependencies
if(TARGET manual::example_lib)
    target_link_libraries(dependency_demo PRIVATE manual::example_lib)
    message(STATUS "Linking dependency_demo with manual::example_lib")
else()
    message(WARNING "manual::example_lib not available for dependency_demo")
endif()

# Link against Git submodule dependencies
if(TARGET submodules::json)
    target_link_libraries(dependency_demo PRIVATE submodules::json)
    target_compile_definitions(dependency_demo PRIVATE NLOHMANN_JSON_VERSION_MAJOR=3)
    message(STATUS "Linking dependency_demo with submodules::json")
elseif(TARGET nlohmann_json::nlohmann_json)
    target_link_libraries(dependency_demo PRIVATE nlohmann_json::nlohmann_json)
    target_compile_definitions(dependency_demo PRIVATE NLOHMANN_JSON_VERSION_MAJOR=3)
    message(STATUS "Linking dependency_demo with nlohmann_json::nlohmann_json")
elseif(TARGET nlohmann_json)
    target_link_libraries(dependency_demo PRIVATE nlohmann_json)
    target_compile_definitions(dependency_demo PRIVATE NLOHMANN_JSON_VERSION_MAJOR=3)
    message(STATUS "Linking dependency_demo with nlohmann_json")
else()
    message(STATUS "nlohmann/json not available for dependency_demo")
endif()

# Link against system dependencies (if available)
if(TARGET system_dependencies)
    target_link_libraries(dependency_demo PRIVATE system_dependencies)
    message(STATUS "Linking dependency_demo with system_dependencies")
endif()

# Set C++ standard
target_compile_features(dependency_demo PRIVATE cxx_std_17)

# Add compiler warnings
if(MSVC)
    target_compile_options(dependency_demo PRIVATE /W4)
else()
    target_compile_options(dependency_demo PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Set target properties
set_target_properties(dependency_demo PROPERTIES
    OUTPUT_NAME "dependency_demo"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples"
)

# Optional: Create a custom target to run the demo
add_custom_target(run_dependency_demo
    COMMAND dependency_demo
    DEPENDS dependency_demo
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/examples
    COMMENT "Running dependency management demo"
)

# Print summary of linked dependencies
get_target_property(DEMO_LINK_LIBS dependency_demo LINK_LIBRARIES)
if(DEMO_LINK_LIBS)
    message(STATUS "dependency_demo will link against: ${DEMO_LINK_LIBS}")
else()
    message(STATUS "dependency_demo has no additional link dependencies")
endif()