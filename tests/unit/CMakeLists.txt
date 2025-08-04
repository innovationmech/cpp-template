# Unit tests CMakeLists.txt Configuration for unit tests

# Basic test to verify Google Test integration
add_cpp_template_test(
    basic SOURCES basic_test.cpp LIBRARIES # No additional libraries needed for basic test
)

# Core library unit tests
add_cpp_template_test(core SOURCES core_test.cpp LIBRARIES core)

# Utils library unit tests
add_cpp_template_test(utils SOURCES utils_test.cpp LIBRARIES core)

# Integration tests for application modules
add_cpp_template_test(
    integration
    SOURCES
    integration_test.cpp
    LIBRARIES
    core
    config-manager
    data-processor)
