# ============================================================================= Tests CMakeLists.txt
# Comprehensive testing framework configuration with Google Test and CTest
#
# This file establishes the testing infrastructure for the cpp-template project: - Google Test
# framework integration with multiple installation methods - CTest configuration for test discovery
# and execution - Standardized test creation utilities and conventions - Support for different test
# categories (unit, integration, performance) - Cross-platform test execution and reporting
#
# Testing Strategy: - Unit tests: Test individual components in isolation - Integration tests: Test
# component interactions and workflows - Performance tests: Benchmark critical functionality -
# System tests: End-to-end application testing
# =============================================================================

message(STATUS "Configuring testing framework...")

# ============================================================================= Google Test
# Framework Detection and Configuration
# =============================================================================

# Find Google Test package with support for multiple installation methods: 1. vcpkg package manager
# (preferred for cross-platform development) 2. System package managers (apt, brew, etc.) 3. Manual
# installation or build from source 4. FetchContent for automatic download and build (fallback)

# First, try to find Google Test using standard CMake find_package
find_package(GTest QUIET)

if(GTest_FOUND)
    message(STATUS "Found Google Test via find_package")
else()
    message(STATUS "Google Test not found via find_package, trying alternative methods...")

    # Try to find GTest using PkgConfig (common on Linux systems)
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(GTEST_PC QUIET gtest)
        if(GTEST_PC_FOUND)
            message(STATUS "Found Google Test via pkg-config")
            # Create imported targets for consistency
            add_library(GTest::gtest INTERFACE IMPORTED)
            target_link_libraries(GTest::gtest INTERFACE ${GTEST_PC_LIBRARIES})
            target_include_directories(GTest::gtest INTERFACE ${GTEST_PC_INCLUDE_DIRS})

            # Also create gtest_main target
            add_library(GTest::gtest_main INTERFACE IMPORTED)
            target_link_libraries(GTest::gtest_main INTERFACE ${GTEST_PC_LIBRARIES})
            target_include_directories(GTest::gtest_main INTERFACE ${GTEST_PC_INCLUDE_DIRS})

            set(GTest_FOUND TRUE)
        endif()
    endif()

    # If still not found, use FetchContent to download and build Google Test
    if(NOT GTest_FOUND)
        message(STATUS "Downloading Google Test via FetchContent...")
        include(FetchContent)

        FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG v1.14.0 # Use a specific version for reproducibility
            GIT_SHALLOW TRUE # Shallow clone for faster download
        )

        # Configure Google Test options before making it available
        set(gtest_force_shared_crt
            ON
            CACHE BOOL "" FORCE) # Use shared runtime on Windows
        set(BUILD_GMOCK
            ON
            CACHE BOOL "" FORCE) # Build Google Mock
        set(INSTALL_GTEST
            OFF
            CACHE BOOL "" FORCE) # Don't install Google Test

        FetchContent_MakeAvailable(googletest)

        # Create aliases for consistency with find_package results (only if they don't already
        # exist) Modern GoogleTest versions may already create these targets
        if(NOT TARGET GTest::gtest)
            add_library(GTest::gtest ALIAS gtest)
        endif()
        if(NOT TARGET GTest::gtest_main)
            add_library(GTest::gtest_main ALIAS gtest_main)
        endif()
        if(NOT TARGET GTest::gmock)
            add_library(GTest::gmock ALIAS gmock)
        endif()
        if(NOT TARGET GTest::gmock_main)
            add_library(GTest::gmock_main ALIAS gmock_main)
        endif()

        set(GTest_FOUND TRUE)
        message(STATUS "Google Test downloaded and configured successfully")
    endif()
endif()

# Verify that Google Test was found and configured
if(NOT GTest_FOUND)
    message(FATAL_ERROR "Google Test is required for testing but could not be found or downloaded")
endif()

# ============================================================================= Google Test Target
# Configuration
# =============================================================================

# Handle different Google Test target naming conventions This ensures compatibility with various
# installation methods
if(TARGET GTest::gtest)
    # Modern CMake targets (preferred approach)
    set(GTEST_LIBRARIES GTest::gtest GTest::gtest_main)
    message(STATUS "Using modern Google Test CMake targets")

    # Check if GMock targets are available (they may not be in some installations)
    if(TARGET GTest::gmock)
        set(GMOCK_LIBRARIES GTest::gmock GTest::gmock_main)
        message(STATUS "GMock targets available: GTest::gmock, GTest::gmock_main")
    else()
        set(GMOCK_LIBRARIES "")
        message(STATUS "GMock targets not available - tests will build without GMock linking")
    endif()
elseif(TARGET gtest)
    # FetchContent or older installations
    set(GTEST_LIBRARIES gtest gtest_main)
    message(STATUS "Using legacy Google Test targets")

    # Check if GMock targets are available
    if(TARGET gmock)
        set(GMOCK_LIBRARIES gmock gmock_main)
        message(STATUS "GMock targets available: gmock, gmock_main")
    else()
        set(GMOCK_LIBRARIES "")
        message(STATUS "GMock targets not available - tests will build without GMock linking")
    endif()
else()
    # Legacy variables (very old installations)
    if(DEFINED GTEST_LIBRARIES AND DEFINED GTEST_MAIN_LIBRARIES)
        set(GTEST_LIBRARIES ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})
        set(GMOCK_LIBRARIES "")
        message(STATUS "Using Google Test legacy variables (GMock not available)")
    else()
        message(FATAL_ERROR "Google Test targets not properly configured")
    endif()
endif()

# ============================================================================= CTest Integration
# and Configuration
# =============================================================================

# Include Google Test's CMake utilities for test discovery This provides the gtest_discover_tests
# function for automatic test registration
include(GoogleTest)

# Configure CTest settings for better test execution and reporting
set(CTEST_OUTPUT_ON_FAILURE ON) # Show test output when tests fail
set(CTEST_PARALLEL_LEVEL 4) # Run tests in parallel (adjust based on system)
set(CTEST_TEST_TIMEOUT 300) # Set test timeout to 5 minutes

# Configure test result formatting
set(CTEST_CUSTOM_MAXIMUM_PASSED_TEST_OUTPUT_SIZE 1024)
set(CTEST_CUSTOM_MAXIMUM_FAILED_TEST_OUTPUT_SIZE 4096)

message(STATUS "CTest configuration:")
message(STATUS "  - Parallel level: ${CTEST_PARALLEL_LEVEL}")
message(STATUS "  - Test timeout: ${CTEST_TEST_TIMEOUT} seconds")
message(STATUS "  - Output on failure: ${CTEST_OUTPUT_ON_FAILURE}")

# ============================================================================= Test Naming and
# Organization Conventions
# =============================================================================

# Set up consistent test executable naming convention This helps organize test binaries and makes
# them easily identifiable
set(TEST_TARGET_PREFIX "test_")
set(TEST_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests")

# Create test output directory
file(MAKE_DIRECTORY ${TEST_OUTPUT_DIRECTORY})

message(STATUS "Test configuration:")
message(STATUS "  - Test prefix: ${TEST_TARGET_PREFIX}")
message(STATUS "  - Test output directory: ${TEST_OUTPUT_DIRECTORY}")

# ============================================================================= Standardized Test
# Creation Function
# =============================================================================

# Create a comprehensive function to add tests with consistent configuration This function
# encapsulates best practices and reduces boilerplate code
function(add_cpp_template_test TEST_NAME)
    # Parse function arguments using cmake_parse_arguments This provides a clean interface for
    # specifying test properties
    cmake_parse_arguments(
        TEST # Prefix for parsed variables
        "INTEGRATION;PERFORMANCE" # Boolean options
        "TIMEOUT;WORKING_DIRECTORY" # Single-value options
        "SOURCES;LIBRARIES;COMPILE_DEFINITIONS;COMPILE_OPTIONS" # Multi-value options
        ${ARGN} # Arguments to parse
    )

    # Validate required arguments
    if(NOT TEST_SOURCES)
        message(FATAL_ERROR "add_cpp_template_test: SOURCES argument is required")
    endif()

    # Create test executable with descriptive name
    set(TEST_TARGET ${TEST_TARGET_PREFIX}${TEST_NAME})
    add_executable(${TEST_TARGET} ${TEST_SOURCES})

    # Set test executable properties
    set_target_properties(
        ${TEST_TARGET}
        PROPERTIES # Output configuration
                   RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY}
                   OUTPUT_NAME ${TEST_NAME}
                   # C++ standard consistency
                   CXX_STANDARD 17
                   CXX_STANDARD_REQUIRED ON
                   CXX_EXTENSIONS OFF
                   # Test categorization
                   FOLDER "Tests"
                   LABELS "${TEST_NAME}")

    # Add test category labels for filtering
    if(TEST_INTEGRATION)
        set_target_properties(${TEST_TARGET} PROPERTIES LABELS "integration;${TEST_NAME}")
    endif()

    if(TEST_PERFORMANCE)
        set_target_properties(${TEST_TARGET} PROPERTIES LABELS "performance;${TEST_NAME}")
    endif()

    # ============================================================================= Test
    # Dependencies and Linking
    # =============================================================================

    # Link against Google Test framework
    target_link_libraries(
        ${TEST_TARGET} PRIVATE ${GTEST_LIBRARIES} ${GMOCK_LIBRARIES}
                               ${TEST_LIBRARIES} # User-specified libraries
    )

    # ============================================================================= Test Include
    # Directories
    # =============================================================================

    # Set up comprehensive include directories for tests
    target_include_directories(
        ${TEST_TARGET}
        PRIVATE # Project include directories
                ${CMAKE_SOURCE_DIR}/include # Public API headers
                ${CMAKE_SOURCE_DIR}/libs/core/include # Internal library headers
                ${CMAKE_SOURCE_DIR}/src # Application source headers
                ${CMAKE_SOURCE_DIR}/src/modules # Module headers
                # Test-specific directories
                ${CMAKE_CURRENT_SOURCE_DIR} # Test source directory
                ${CMAKE_CURRENT_SOURCE_DIR}/fixtures # Test fixtures and data
                ${CMAKE_CURRENT_SOURCE_DIR}/helpers # Test helper utilities
    )

    # ============================================================================= Test Compiler
    # Configuration
    # =============================================================================

    # Apply comprehensive compiler options for tests
    target_compile_options(
        ${TEST_TARGET}
        PRIVATE # GCC and Clang options
                $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
                -Wall
                -Wextra
                -Wpedantic # Enable comprehensive warnings
                -Wno-unused-parameter # Allow unused parameters in test functions
                -Wno-sign-compare # Allow sign comparison in test assertions
                >
                # MSVC options
                $<$<CXX_COMPILER_ID:MSVC>:
                /W4 # High warning level
                /wd4100 # Disable unused parameter warning
                /wd4127 # Disable constant conditional expression warning
                >
                # User-specified compile options
                ${TEST_COMPILE_OPTIONS})

    # Apply compile definitions
    target_compile_definitions(
        ${TEST_TARGET}
        PRIVATE # Test framework definitions
                GTEST_HAS_STD_WSTRING=1 # Enable wide string support
                # Debug mode for tests
                $<$<CONFIG:Debug>:DEBUG_TESTS=1>
                # User-specified definitions
                ${TEST_COMPILE_DEFINITIONS})

    # ============================================================================= Test Discovery
    # and Registration
    # =============================================================================

    # Configure test discovery with Google Test
    set(DISCOVERY_TIMEOUT ${TEST_TIMEOUT})
    if(NOT DISCOVERY_TIMEOUT)
        set(DISCOVERY_TIMEOUT 60) # Default timeout for test discovery
    endif()

    set(DISCOVERY_WORKING_DIR ${TEST_WORKING_DIRECTORY})
    if(NOT DISCOVERY_WORKING_DIR)
        set(DISCOVERY_WORKING_DIR ${CMAKE_CURRENT_SOURCE_DIR})
    endif()

    # Discover and register individual test cases with CTest
    gtest_discover_tests(
        ${TEST_TARGET}
        # Test execution configuration
        WORKING_DIRECTORY ${DISCOVERY_WORKING_DIR}
        TIMEOUT ${DISCOVERY_TIMEOUT}
        # Test properties for CTest
        PROPERTIES
        TIMEOUT ${DISCOVERY_TIMEOUT} ENVIRONMENT "GTEST_COLOR=1" # Enable colored output
        # IDE integration
        EXTRA_ARGS --gtest_color=yes
        # Visual Studio debugging support
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${DISCOVERY_WORKING_DIR}")

    # Add test to custom targets for batch execution
    add_dependencies(all_tests ${TEST_TARGET})

    # Add to category-specific targets
    if(TEST_INTEGRATION)
        if(NOT TARGET integration_tests)
            add_custom_target(integration_tests)
        endif()
        add_dependencies(integration_tests ${TEST_TARGET})
    endif()

    if(TEST_PERFORMANCE)
        if(NOT TARGET performance_tests)
            add_custom_target(performance_tests)
        endif()
        add_dependencies(performance_tests ${TEST_TARGET})
    endif()

    # Log test creation
    message(STATUS "Created test: ${TEST_TARGET}")
    if(TEST_INTEGRATION)
        message(STATUS "  - Type: Integration test")
    elseif(TEST_PERFORMANCE)
        message(STATUS "  - Type: Performance test")
    else()
        message(STATUS "  - Type: Unit test")
    endif()
    message(STATUS "  - Sources: ${TEST_SOURCES}")
    message(STATUS "  - Libraries: ${TEST_LIBRARIES}")

endfunction()

# ============================================================================= Custom Test Targets
# =============================================================================

# Create custom target for running all tests
add_custom_target(all_tests COMMENT "Build all test executables")

# Create custom target for running tests with verbose output
add_custom_target(
    test_verbose
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure --verbose
    DEPENDS all_tests
    COMMENT "Run all tests with verbose output"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

# Create custom target for running tests in parallel
add_custom_target(
    test_parallel
    COMMAND ${CMAKE_CTEST_COMMAND} --parallel ${CTEST_PARALLEL_LEVEL}
    DEPENDS all_tests
    COMMENT "Run all tests in parallel"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

# Create custom target for generating test coverage (if supported)
if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    find_program(GCOV_EXECUTABLE gcov)
    find_program(LCOV_EXECUTABLE lcov)

    if(GCOV_EXECUTABLE AND LCOV_EXECUTABLE)
        add_custom_target(
            test_coverage
            COMMAND ${CMAKE_CTEST_COMMAND}
            COMMAND ${LCOV_EXECUTABLE} --capture --directory . --output-file coverage.info
            COMMAND ${LCOV_EXECUTABLE} --remove coverage.info '/usr/*' --output-file coverage.info
            COMMAND ${LCOV_EXECUTABLE} --list coverage.info
            DEPENDS all_tests
            COMMENT "Generate test coverage report"
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
        message(STATUS "Test coverage target available: test_coverage")
    endif()
endif()

# ============================================================================= Test Subdirectories
# =============================================================================

# Add subdirectories for different test categories Each subdirectory contains tests of a specific
# type or for specific components

message(STATUS "Including test subdirectories...")

# Unit tests - Test individual components in isolation
add_subdirectory(unit)

# Integration tests - Test component interactions (if directory exists)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/integration")
    add_subdirectory(integration)
    message(STATUS "  - Integration tests included")
endif()

# Performance tests - Benchmark critical functionality (if directory exists)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/performance")
    add_subdirectory(performance)
    message(STATUS "  - Performance tests included")
endif()

# System tests - End-to-end testing (if directory exists)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/system")
    add_subdirectory(system)
    message(STATUS "  - System tests included")
endif()

# ============================================================================= Test Configuration
# Summary
# =============================================================================

message(STATUS "Testing framework configuration complete:")
message(STATUS "  - Framework: Google Test ${GTEST_VERSION}")
message(STATUS "  - Test discovery: Enabled")
message(STATUS "  - Parallel execution: ${CTEST_PARALLEL_LEVEL} jobs")
message(STATUS "  - Test timeout: ${CTEST_TEST_TIMEOUT} seconds")
message(STATUS "  - Custom targets: all_tests, test_verbose, test_parallel")

# Display available test targets
get_property(
    test_targets
    DIRECTORY
    PROPERTY BUILDSYSTEM_TARGETS)
list(FILTER test_targets INCLUDE REGEX "^${TEST_TARGET_PREFIX}")
if(test_targets)
    message(STATUS "  - Test executables: ${test_targets}")
endif()
