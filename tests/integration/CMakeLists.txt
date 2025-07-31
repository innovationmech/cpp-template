# ==============================================================================
# cpp-template Integration Tests Configuration
# ==============================================================================
#
# @file tests/integration/CMakeLists.txt
# @brief Integration testing configuration for component interactions
#
# This file configures integration tests for the cpp-template project.
# Integration tests focus on testing interactions between different components,
# end-to-end workflows, and system-level behavior to ensure proper integration
# of all project parts.
#
# Test Coverage:
# - Application-level integration and workflows
# - Inter-module communication and data flow
# - Core library and module system interactions
# - End-to-end functionality validation
# - System behavior under realistic usage scenarios
#
# Test Organization:
# - test_application_integration.cpp: Application-level integration tests
# - test_module_interactions.cpp: Module interaction and communication tests
#
# Dependencies:
# - Google Test framework (GTest::gtest, GTest::gtest_main)
# - All project libraries (core, modules)
# - System dependencies as required
#
# Test Characteristics:
# - Longer execution time than unit tests
# - More complex test scenarios
# - Real-world usage simulation
# - Cross-component validation
#
# Usage:
#   # Run all integration tests
#   ctest -L integration
#   
#   # Run integration tests with verbose output
#   ctest -L integration --verbose
#   
#   # Run specific integration test executable
#   ./test_integration
#
# @author cpp-template project
# @version 1.0.0
# @since 1.0.0
# ==============================================================================

if(GTest_FOUND)
    # ==========================================================================
    # Integration Test Suite
    # ==========================================================================
    
    # Create comprehensive integration test executable
    # Combines multiple test files for complete integration coverage
    add_executable(test_integration
        test_application_integration.cpp  # Application-level integration tests
        test_module_interactions.cpp      # Module interaction tests
    )
    
    # Link all required libraries for comprehensive testing
    # Integration tests need access to all project components
    target_link_libraries(test_integration
        PRIVATE
            cpp-template::core           # Core library functionality
            cpp-template::example-module # Module system components
            GTest::gtest                 # Google Test framework
            GTest::gtest_main           # Test runner with main() function
    )
    
    # Include directories for accessing all project headers
    # Integration tests may need access to both public and internal interfaces
    target_include_directories(test_integration
        PRIVATE
            ${PROJECT_SOURCE_DIR}/include      # Public API headers
            ${PROJECT_SOURCE_DIR}/src/modules  # Module implementation headers
    )
    
    # Register integration tests with CTest
    # Integration tests are run separately from unit tests
    add_test(NAME IntegrationTests COMMAND test_integration)
    
    # Configure integration test properties
    # Integration tests typically require more time and resources
    set_tests_properties(IntegrationTests PROPERTIES
        TIMEOUT 30                    # Allow longer execution time
        LABELS "integration"          # Label for test categorization
        ENVIRONMENT "TEST_ENV=integration"  # Set test environment
    )
    
    # Additional test configuration for CI/CD environments
    if(DEFINED ENV{CI})
        # In CI environments, may need different timeout or parallel settings
        set_tests_properties(IntegrationTests PROPERTIES
            TIMEOUT 60                # Extended timeout for CI
            RUN_SERIAL TRUE          # Run serially to avoid resource conflicts
        )
    endif()
    
    # Log integration test configuration
    message(STATUS "Integration tests configured: IntegrationTests")
    message(STATUS "Integration test timeout: 30 seconds")
    
endif()