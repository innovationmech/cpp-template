# Tests CMakeLists.txt
# Google Test configuration and CTest integration

# Find Google Test package (supports both vcpkg and system installations)
find_package(GTest REQUIRED)

# Handle different Google Test configurations
if(TARGET GTest::gtest)
    # Modern CMake targets (vcpkg or newer system installations)
    set(GTEST_LIBRARIES GTest::gtest GTest::gtest_main)
    set(GMOCK_LIBRARIES GTest::gmock GTest::gmock_main)
else()
    # Legacy variables (older system installations)
    set(GTEST_LIBRARIES ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})
    set(GMOCK_LIBRARIES ${GMOCK_LIBRARIES} ${GMOCK_MAIN_LIBRARIES})
endif()

# Include Google Test's CMake utilities
include(GoogleTest)

# Set up test executable naming convention
set(TEST_TARGET_PREFIX "test_")

# Create a function to add tests with consistent configuration
function(add_cpp_template_test TEST_NAME)
    # Parse arguments
    cmake_parse_arguments(TEST "" "" "SOURCES;LIBRARIES" ${ARGN})
    
    # Create test executable
    set(TEST_TARGET ${TEST_TARGET_PREFIX}${TEST_NAME})
    add_executable(${TEST_TARGET} ${TEST_SOURCES})
    
    # Link against Google Test and specified libraries
    target_link_libraries(${TEST_TARGET} 
        PRIVATE 
        ${GTEST_LIBRARIES}
        ${GMOCK_LIBRARIES}
        ${TEST_LIBRARIES}
    )
    
    # Set C++ standard for test
    target_compile_features(${TEST_TARGET} PRIVATE cxx_std_17)
    
    # Set up include directories for tests
    target_include_directories(${TEST_TARGET}
        PRIVATE
        ${CMAKE_SOURCE_DIR}/libs/core/include
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
    )
    
    # Add compiler options for tests
    target_compile_options(${TEST_TARGET} PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
    )
    
    # Discover and register tests with CTest
    gtest_discover_tests(${TEST_TARGET}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )
    
    # Add test to a custom target for running all tests
    add_dependencies(all_tests ${TEST_TARGET})
endfunction()

# Create custom target for running all tests
add_custom_target(all_tests)

# Add subdirectories for different test categories
add_subdirectory(unit)