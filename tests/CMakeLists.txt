# Testing configuration

# Find required packages for testing
find_package(GTest QUIET)

if(GTest_FOUND)
    # Add unit tests subdirectory
    add_subdirectory(unit)
    
    message(STATUS "Google Test found - tests enabled")
else()
    message(WARNING "Google Test not found - tests disabled. Install via vcpkg: vcpkg install gtest")
endif()