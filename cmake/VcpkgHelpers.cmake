# VcpkgHelpers.cmake
# Utility functions for vcpkg integration and dependency management

# Function to check if vcpkg is being used
function(check_vcpkg_integration)
    if(DEFINED CMAKE_TOOLCHAIN_FILE AND CMAKE_TOOLCHAIN_FILE MATCHES "vcpkg")
        set(USING_VCPKG TRUE PARENT_SCOPE)
        message(STATUS "vcpkg integration detected")
    else()
        set(USING_VCPKG FALSE PARENT_SCOPE)
        message(STATUS "vcpkg integration not detected")
        message(STATUS "To use vcpkg, set CMAKE_TOOLCHAIN_FILE to vcpkg.cmake:")
        message(STATUS "  cmake -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake ..")
    endif()
endfunction()

# Function to find a package with fallback options
# Usage: find_package_with_fallback(PackageName VCPKG_NAME vcpkg_name SYSTEM_NAME system_name)
function(find_package_with_fallback PACKAGE_NAME)
    cmake_parse_arguments(ARGS "" "VCPKG_NAME;SYSTEM_NAME" "" ${ARGN})
    
    set(VCPKG_NAME ${ARGS_VCPKG_NAME})
    set(SYSTEM_NAME ${ARGS_SYSTEM_NAME})
    
    if(NOT VCPKG_NAME)
        set(VCPKG_NAME ${PACKAGE_NAME})
    endif()
    
    if(NOT SYSTEM_NAME)
        set(SYSTEM_NAME ${PACKAGE_NAME})
    endif()
    
    # Try to find the package
    find_package(${PACKAGE_NAME} QUIET)
    
    if(NOT ${PACKAGE_NAME}_FOUND)
        message(STATUS "${PACKAGE_NAME} not found. Trying alternative names...")
        
        # Try vcpkg name
        if(NOT "${VCPKG_NAME}" STREQUAL "${PACKAGE_NAME}")
            find_package(${VCPKG_NAME} QUIET)
            if(${VCPKG_NAME}_FOUND)
                message(STATUS "Found ${PACKAGE_NAME} as ${VCPKG_NAME}")
                return()
            endif()
        endif()
        
        # Try system name
        if(NOT "${SYSTEM_NAME}" STREQUAL "${PACKAGE_NAME}")
            find_package(${SYSTEM_NAME} QUIET)
            if(${SYSTEM_NAME}_FOUND)
                message(STATUS "Found ${PACKAGE_NAME} as ${SYSTEM_NAME}")
                return()
            endif()
        endif()
        
        message(WARNING "${PACKAGE_NAME} not found with any naming convention")
        message(STATUS "Install options:")
        message(STATUS "  vcpkg: vcpkg install ${VCPKG_NAME}")
        message(STATUS "  brew:  brew install ${SYSTEM_NAME}")
        message(STATUS "  apt:   sudo apt-get install lib${SYSTEM_NAME}-dev")
    else()
        message(STATUS "Found ${PACKAGE_NAME}")
    endif()
endfunction()

# Function to setup vcpkg manifest mode
function(setup_vcpkg_manifest)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg.json")
        message(STATUS "vcpkg manifest found: ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg.json")
        
        # Enable manifest mode
        set(VCPKG_MANIFEST_MODE ON CACHE BOOL "Enable vcpkg manifest mode")
        
        # Set manifest directory
        set(VCPKG_MANIFEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE PATH "vcpkg manifest directory")
        
        message(STATUS "vcpkg manifest mode enabled")
    else()
        message(STATUS "No vcpkg.json manifest found")
    endif()
endfunction()

# Function to print vcpkg information
function(print_vcpkg_info)
    if(DEFINED CMAKE_TOOLCHAIN_FILE AND CMAKE_TOOLCHAIN_FILE MATCHES "vcpkg")
        message(STATUS "=== vcpkg Configuration ===")
        message(STATUS "Toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
        
        if(DEFINED VCPKG_TARGET_TRIPLET)
            message(STATUS "Target triplet: ${VCPKG_TARGET_TRIPLET}")
        endif()
        
        if(DEFINED VCPKG_MANIFEST_MODE)
            message(STATUS "Manifest mode: ${VCPKG_MANIFEST_MODE}")
        endif()
        
        if(DEFINED VCPKG_MANIFEST_DIR)
            message(STATUS "Manifest directory: ${VCPKG_MANIFEST_DIR}")
        endif()
        
        message(STATUS "===========================")
    endif()
endfunction()

# Automatically setup vcpkg if toolchain is detected
check_vcpkg_integration()
if(USING_VCPKG)
    setup_vcpkg_manifest()
    print_vcpkg_info()
endif()