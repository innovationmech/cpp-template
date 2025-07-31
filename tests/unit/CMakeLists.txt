# ==============================================================================
# cpp-template Unit Tests Configuration
# ==============================================================================
#
# @file tests/unit/CMakeLists.txt
# @brief Unit testing configuration for individual components
#
# This file configures unit tests for the cpp-template project components.
# Unit tests focus on testing individual classes, functions, and modules
# in isolation to ensure correctness of basic functionality.
#
# Test Coverage:
# - Core library functionality (Core class, utilities)
# - Module system (ExampleModule and other modules)
# - Individual component behavior and edge cases
# - API contract validation
#
# Test Organization:
# - test_core: Tests for core library components
# - test_modules: Tests for modular components
#
# Dependencies:
# - Google Test framework (GTest::gtest, GTest::gtest_main)
# - Project libraries (cpp-template::core, cpp-template::example-module)
#
# Usage:
#   # Run all unit tests
#   ctest -R "CoreTests|ModuleTests"
#   
#   # Run specific test executable
#   ./test_core
#   ./test_modules
#
# @author cpp-template project
# @version 1.0.0
# @since 1.0.0
# ==============================================================================

if(GTest_FOUND)
    # ==========================================================================
    # Core Library Unit Tests
    # ==========================================================================
    
    # Create test executable for core library components
    # Tests fundamental functionality like Core class methods and utilities
    add_executable(test_core
        test_core.cpp
    )
    
    # Link required libraries for core testing
    # - cpp-template::core: The core library being tested
    # - GTest::gtest: Google Test framework
    # - GTest::gtest_main: Provides main() function for tests
    target_link_libraries(test_core
        PRIVATE
            cpp-template::core
            GTest::gtest
            GTest::gtest_main
    )
    
    # Include directories for accessing public headers
    target_include_directories(test_core
        PRIVATE
            ${PROJECT_SOURCE_DIR}/include
    )
    
    # Register test with CTest for automated test discovery
    # Allows running with 'ctest' command and IDE integration
    add_test(NAME CoreTests COMMAND test_core)
    
    # Set test properties for better organization
    set_tests_properties(CoreTests PROPERTIES
        LABELS "unit;core"
        TIMEOUT 10
    )
    
    # ==========================================================================
    # Module System Unit Tests
    # ==========================================================================
    
    # Create test executable for module components
    # Tests modular architecture and individual module functionality
    add_executable(test_modules
        test_modules.cpp
    )
    
    # Link required libraries for module testing
    # - cpp-template::example-module: The module library being tested
    # - GTest::gtest: Google Test framework
    # - GTest::gtest_main: Provides main() function for tests
    target_link_libraries(test_modules
        PRIVATE
            cpp-template::example-module
            GTest::gtest
            GTest::gtest_main
    )
    
    # Include directories for accessing module headers
    target_include_directories(test_modules
        PRIVATE
            ${PROJECT_SOURCE_DIR}/src/modules
    )
    
    # Register test with CTest for automated test discovery
    add_test(NAME ModuleTests COMMAND test_modules)
    
    # Set test properties for better organization
    set_tests_properties(ModuleTests PROPERTIES
        LABELS "unit;modules"
        TIMEOUT 10
    )
    
    # Log unit test configuration
    message(STATUS "Unit tests configured: CoreTests, ModuleTests")
    
endif()