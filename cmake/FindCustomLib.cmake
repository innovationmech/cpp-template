# FindCustomLib.cmake
# Example CMake find module for manual dependency management
# This demonstrates how to create custom find modules for libraries
# that don't provide their own CMake configuration files

# Set the module description
set(CUSTOMLIB_DESCRIPTION "Example custom library find module")

# Define possible library names and paths
set(CUSTOMLIB_NAMES customlib custom_lib)
set(CUSTOMLIB_HEADER_NAMES customlib.h custom_lib.h)

# Search for the library in common locations
find_library(CUSTOMLIB_LIBRARY
    NAMES ${CUSTOMLIB_NAMES}
    PATHS
        ${CUSTOMLIB_ROOT}
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/manual/customlib
        /usr/local/lib
        /usr/lib
        /opt/local/lib
        /opt/homebrew/lib
    PATH_SUFFIXES
        lib
        lib64
    DOC "CustomLib library"
)

# Search for the header files
find_path(CUSTOMLIB_INCLUDE_DIR
    NAMES ${CUSTOMLIB_HEADER_NAMES}
    PATHS
        ${CUSTOMLIB_ROOT}
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/manual/customlib
        /usr/local/include
        /usr/include
        /opt/local/include
        /opt/homebrew/include
    PATH_SUFFIXES
        include
        customlib
    DOC "CustomLib include directory"
)

# Try to extract version information if available
if(CUSTOMLIB_INCLUDE_DIR AND EXISTS "${CUSTOMLIB_INCLUDE_DIR}/customlib.h")
    file(STRINGS "${CUSTOMLIB_INCLUDE_DIR}/customlib.h" CUSTOMLIB_VERSION_LINE
        REGEX "^#define[ \t]+CUSTOMLIB_VERSION[ \t]+\"[^\"]*\"")
    if(CUSTOMLIB_VERSION_LINE)
        string(REGEX REPLACE "^#define[ \t]+CUSTOMLIB_VERSION[ \t]+\"([^\"]*)\".*" "\\1"
            CUSTOMLIB_VERSION "${CUSTOMLIB_VERSION_LINE}")
    endif()
endif()

# Use FindPackageHandleStandardArgs to handle the REQUIRED and QUIET arguments
# and set CUSTOMLIB_FOUND to TRUE if all required variables are set
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CustomLib
    FOUND_VAR CUSTOMLIB_FOUND
    REQUIRED_VARS
        CUSTOMLIB_LIBRARY
        CUSTOMLIB_INCLUDE_DIR
    VERSION_VAR CUSTOMLIB_VERSION
    FAIL_MESSAGE "Could not find CustomLib library. Set CUSTOMLIB_ROOT to the installation directory."
)

# Create imported target if found
if(CUSTOMLIB_FOUND AND NOT TARGET CustomLib::CustomLib)
    add_library(CustomLib::CustomLib UNKNOWN IMPORTED)
    set_target_properties(CustomLib::CustomLib PROPERTIES
        IMPORTED_LOCATION "${CUSTOMLIB_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${CUSTOMLIB_INCLUDE_DIR}"
    )
    
    # Set additional properties if needed
    if(CUSTOMLIB_VERSION)
        set_target_properties(CustomLib::CustomLib PROPERTIES
            INTERFACE_COMPILE_DEFINITIONS "CUSTOMLIB_VERSION=\"${CUSTOMLIB_VERSION}\""
        )
    endif()
endif()

# Mark variables as advanced (they won't show up in cmake-gui by default)
mark_as_advanced(
    CUSTOMLIB_LIBRARY
    CUSTOMLIB_INCLUDE_DIR
    CUSTOMLIB_VERSION
)

# Provide usage information
if(CUSTOMLIB_FOUND)
    if(NOT CustomLib_FIND_QUIETLY)
        message(STATUS "Found CustomLib: ${CUSTOMLIB_LIBRARY} (version ${CUSTOMLIB_VERSION})")
        message(STATUS "CustomLib include dir: ${CUSTOMLIB_INCLUDE_DIR}")
    endif()
else()
    if(CustomLib_FIND_REQUIRED)
        message(STATUS "CustomLib not found. To specify a custom location, set:")
        message(STATUS "  CUSTOMLIB_ROOT=/path/to/customlib")
        message(STATUS "Or install CustomLib to a standard system location.")
    endif()
endif()