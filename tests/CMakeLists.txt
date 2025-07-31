# ==============================================================================
# cpp-template Testing Configuration
# ==============================================================================
#
# @file tests/CMakeLists.txt
# @brief Testing framework configuration for the cpp-template project
#
# This file configures the testing infrastructure for the cpp-template project,
# including Google Test integration, CTest setup, and test subdirectory management.
# It provides a comprehensive testing framework for both unit and integration tests.
#
# Key Features:
# - Google Test framework integration
# - CTest compatibility for IDE integration
# - Modular test organization (unit/integration)
# - Conditional test building based on dependencies
# - Comprehensive test coverage support
#
# Test Structure:
# - unit/: Unit tests for individual components
# - integration/: Integration tests for component interactions
#
# Dependencies:
# - Google Test (GTest): Primary testing framework
# - CTest: CMake's built-in testing infrastructure
#
# Usage:
#   # Build and run all tests
#   cmake --build . --target test
#   
#   # Run tests with CTest
#   ctest --verbose
#   
#   # Run specific test categories
#   ctest -L unit
#   ctest -L integration
#
# @author cpp-template project
# @version 1.0.0
# @since 1.0.0
# ==============================================================================

# Enable CTest for this directory and subdirectories
# CTest provides test discovery, execution, and reporting capabilities
enable_testing()

# ==============================================================================
# Testing Framework Detection
# ==============================================================================

# Find Google Test package using CONFIG mode for better vcpkg compatibility
# QUIET flag prevents warnings if GTest is not found
find_package(GTest CONFIG QUIET)

# ==============================================================================
# Conditional Test Building
# ==============================================================================

if(GTest_FOUND)
    # Google Test found - enable all testing functionality
    
    # Add unit tests subdirectory
    # Contains tests for individual classes and functions
    add_subdirectory(unit)
    
    # Add integration tests subdirectory  
    # Contains tests for component interactions and workflows
    add_subdirectory(integration)
    
    # Log successful test configuration
    message(STATUS "Google Test found - tests enabled")
    message(STATUS "CTest integration enabled")
    message(STATUS "Test categories: unit, integration")
    
else()
    # Google Test not found - provide helpful installation guidance
    message(WARNING "Google Test not found - tests disabled")
    message(STATUS "To enable tests, install Google Test:")
    message(STATUS "  vcpkg: vcpkg install gtest")
    message(STATUS "  apt: sudo apt-get install libgtest-dev")
    message(STATUS "  brew: brew install googletest")
endif()