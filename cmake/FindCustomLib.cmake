# FindCustomLib.cmake - Example CMake find module
# This is an example of how to create a custom find module for manual dependencies

# Find CustomLib
#
# This module defines:
#  CustomLib_FOUND - True if CustomLib is found
#  CustomLib_INCLUDE_DIRS - Include directories for CustomLib
#  CustomLib_LIBRARIES - Libraries to link against CustomLib
#  CustomLib_VERSION - Version of CustomLib
#
# You can set these variables to help guide the search:
#  CustomLib_ROOT_DIR - Root directory to search for CustomLib

# Search for header files
find_path(CustomLib_INCLUDE_DIR
    NAMES custom_lib.h customlib/custom_lib.h
    PATHS
        ${CustomLib_ROOT_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/manual/custom-lib/include
        /usr/local/include
        /usr/include
    PATH_SUFFIXES
        customlib
        custom-lib
)

# Search for library files
find_library(CustomLib_LIBRARY
    NAMES custom_lib customlib
    PATHS
        ${CustomLib_ROOT_DIR}/lib
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/manual/custom-lib/lib
        /usr/local/lib
        /usr/lib
    PATH_SUFFIXES
        lib
        lib64
)

# Try to extract version from header file
if(CustomLib_INCLUDE_DIR)
    file(STRINGS "${CustomLib_INCLUDE_DIR}/custom_lib.h" CustomLib_VERSION_LINE
        REGEX "^#define[ \t]+CUSTOM_LIB_VERSION[ \t]+\"[^\"]*\"")
    
    if(CustomLib_VERSION_LINE)
        string(REGEX REPLACE "^#define[ \t]+CUSTOM_LIB_VERSION[ \t]+\"([^\"]*)\""
               "\\1" CustomLib_VERSION "${CustomLib_VERSION_LINE}")
    endif()
endif()

# Handle standard find_package arguments
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CustomLib
    REQUIRED_VARS
        CustomLib_LIBRARY
        CustomLib_INCLUDE_DIR
    VERSION_VAR
        CustomLib_VERSION
)

# Set output variables
if(CustomLib_FOUND)
    set(CustomLib_LIBRARIES ${CustomLib_LIBRARY})
    set(CustomLib_INCLUDE_DIRS ${CustomLib_INCLUDE_DIR})
    
    # Create imported target
    if(NOT TARGET CustomLib::CustomLib)
        add_library(CustomLib::CustomLib UNKNOWN IMPORTED)
        set_target_properties(CustomLib::CustomLib PROPERTIES
            IMPORTED_LOCATION "${CustomLib_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${CustomLib_INCLUDE_DIR}"
        )
    endif()
    
    mark_as_advanced(
        CustomLib_INCLUDE_DIR
        CustomLib_LIBRARY
    )
endif()

# Usage example:
# find_package(CustomLib REQUIRED)
# target_link_libraries(your_target PRIVATE CustomLib::CustomLib)