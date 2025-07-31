# Testing configuration

# Enable CTest
enable_testing()

# Find required packages for testing
find_package(GTest CONFIG QUIET)

if(GTest_FOUND)
    # Add unit tests subdirectory
    add_subdirectory(unit)
    
    # Add integration tests subdirectory
    add_subdirectory(integration)
    
    message(STATUS "Google Test found - tests enabled")
    message(STATUS "CTest integration enabled")
else()
    message(WARNING "Google Test not found - tests disabled. Install via vcpkg: vcpkg install gtest")
endif()