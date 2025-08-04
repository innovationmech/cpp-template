# FindCustomLib.cmake Example CMake find module for system package management This demonstrates how
# to create custom find modules for libraries that don't provide their own CMake configuration files

#[=======================================================================[.rst:
FindCustomLib
-------------

Find the CustomLib library installed via system package managers.

This module supports finding CustomLib installed via:
- Homebrew (macOS)
- APT (Ubuntu/Debian)
- DNF/YUM (Fedora/RHEL)
- Manual installation

Installation commands:
- macOS: brew install customlib
- Ubuntu: sudo apt install libcustomlib-dev
- Fedora: sudo dnf install customlib-devel

This module defines the following variables:

``CustomLib_FOUND``
  True if CustomLib is found
``CUSTOMLIB_INCLUDE_DIR``
  Include directory for CustomLib headers
``CUSTOMLIB_LIBRARY``
  CustomLib library file
``CUSTOMLIB_VERSION``
  Version of CustomLib if available

This module defines the following imported targets:

``CustomLib::CustomLib``
  The CustomLib library, if found

#]=======================================================================]

# Set the module description
set(CUSTOMLIB_DESCRIPTION "Example custom library find module for system packages")

# Use pkg-config if available (common on Linux systems)
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(PC_CUSTOMLIB QUIET customlib)
endif()

# Define possible library names and paths
set(CUSTOMLIB_NAMES customlib custom_lib)
set(CUSTOMLIB_HEADER_NAMES customlib.h custom_lib.h)

# Search for the library in common system locations
find_library(
    CUSTOMLIB_LIBRARY
    NAMES ${CUSTOMLIB_NAMES}
    PATHS ${PC_CUSTOMLIB_LIBRARY_DIRS} # pkg-config paths
          ${CUSTOMLIB_ROOT} # User-specified root
          ${CMAKE_CURRENT_SOURCE_DIR}/third_party/manual/customlib # Manual fallback
          # macOS Homebrew paths
          /opt/homebrew/lib # Apple Silicon Homebrew
          /usr/local/lib # Intel Homebrew / Linux standard
          /usr/local/Cellar/customlib/*/lib # Homebrew versioned path
          # Linux standard paths
          /usr/lib
          /usr/lib64
          /usr/lib/x86_64-linux-gnu # Ubuntu/Debian multiarch
          /usr/lib/aarch64-linux-gnu # Ubuntu/Debian ARM64
          # MacPorts
          /opt/local/lib
          # Other common paths
          ${CMAKE_PREFIX_PATH}/lib
    PATH_SUFFIXES lib lib64 customlib
    DOC "CustomLib library")

# Search for the header files in common system locations
find_path(
    CUSTOMLIB_INCLUDE_DIR
    NAMES ${CUSTOMLIB_HEADER_NAMES}
    PATHS ${PC_CUSTOMLIB_INCLUDE_DIRS} # pkg-config paths
          ${CUSTOMLIB_ROOT} # User-specified root
          ${CMAKE_CURRENT_SOURCE_DIR}/third_party/manual/customlib # Manual fallback
          # macOS Homebrew paths
          /opt/homebrew/include # Apple Silicon Homebrew
          /usr/local/include # Intel Homebrew / Linux standard
          /usr/local/Cellar/customlib/*/include # Homebrew versioned path
          # Linux standard paths
          /usr/include
          /usr/include/x86_64-linux-gnu # Ubuntu/Debian multiarch
          /usr/include/aarch64-linux-gnu # Ubuntu/Debian ARM64
          # MacPorts
          /opt/local/include
          # Other common paths
          ${CMAKE_PREFIX_PATH}/include
    PATH_SUFFIXES include customlib CustomLib
    DOC "CustomLib include directory")

# Try to extract version information from multiple sources
set(CUSTOMLIB_VERSION "")

# First try pkg-config version
if(PC_CUSTOMLIB_VERSION)
    set(CUSTOMLIB_VERSION ${PC_CUSTOMLIB_VERSION})
endif()

# Then try to extract from header file
if(CUSTOMLIB_INCLUDE_DIR AND EXISTS "${CUSTOMLIB_INCLUDE_DIR}/customlib.h")
    file(STRINGS "${CUSTOMLIB_INCLUDE_DIR}/customlib.h" CUSTOMLIB_VERSION_LINES
         REGEX "^#define[ \t]+CUSTOMLIB_VERSION")

    foreach(line ${CUSTOMLIB_VERSION_LINES})
        # Try different version definition patterns
        if(line MATCHES "^#define[ \t]+CUSTOMLIB_VERSION[ \t]+\"([^\"]+)\"")
            set(CUSTOMLIB_VERSION ${CMAKE_MATCH_1})
            break()
        elseif(line MATCHES "^#define[ \t]+CUSTOMLIB_VERSION_MAJOR[ \t]+([0-9]+)")
            set(CUSTOMLIB_VERSION_MAJOR ${CMAKE_MATCH_1})
        elseif(line MATCHES "^#define[ \t]+CUSTOMLIB_VERSION_MINOR[ \t]+([0-9]+)")
            set(CUSTOMLIB_VERSION_MINOR ${CMAKE_MATCH_1})
        elseif(line MATCHES "^#define[ \t]+CUSTOMLIB_VERSION_PATCH[ \t]+([0-9]+)")
            set(CUSTOMLIB_VERSION_PATCH ${CMAKE_MATCH_1})
        endif()
    endforeach()

    # Construct version from components if not found as string
    if(NOT CUSTOMLIB_VERSION AND CUSTOMLIB_VERSION_MAJOR)
        set(CUSTOMLIB_VERSION "${CUSTOMLIB_VERSION_MAJOR}")
        if(CUSTOMLIB_VERSION_MINOR)
            set(CUSTOMLIB_VERSION "${CUSTOMLIB_VERSION}.${CUSTOMLIB_VERSION_MINOR}")
            if(CUSTOMLIB_VERSION_PATCH)
                set(CUSTOMLIB_VERSION "${CUSTOMLIB_VERSION}.${CUSTOMLIB_VERSION_PATCH}")
            endif()
        endif()
    endif()
endif()

# Use FindPackageHandleStandardArgs to handle the REQUIRED and QUIET arguments and set
# CUSTOMLIB_FOUND to TRUE if all required variables are set
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    CustomLib
    FOUND_VAR CUSTOMLIB_FOUND
    REQUIRED_VARS CUSTOMLIB_LIBRARY CUSTOMLIB_INCLUDE_DIR
    VERSION_VAR CUSTOMLIB_VERSION
    FAIL_MESSAGE
        "Could not find CustomLib library. Set CUSTOMLIB_ROOT to the installation directory.")

# Create imported target if found
if(CUSTOMLIB_FOUND AND NOT TARGET CustomLib::CustomLib)
    add_library(CustomLib::CustomLib UNKNOWN IMPORTED)
    set_target_properties(
        CustomLib::CustomLib PROPERTIES IMPORTED_LOCATION "${CUSTOMLIB_LIBRARY}"
                                        INTERFACE_INCLUDE_DIRECTORIES "${CUSTOMLIB_INCLUDE_DIR}")

    # Set additional properties if needed
    if(CUSTOMLIB_VERSION)
        set_target_properties(
            CustomLib::CustomLib PROPERTIES INTERFACE_COMPILE_DEFINITIONS
                                            "CUSTOMLIB_VERSION=\"${CUSTOMLIB_VERSION}\"")
    endif()
endif()

# Mark variables as advanced (they won't show up in cmake-gui by default)
mark_as_advanced(CUSTOMLIB_LIBRARY CUSTOMLIB_INCLUDE_DIR CUSTOMLIB_VERSION)

# Provide usage information and installation instructions
if(CUSTOMLIB_FOUND)
    if(NOT CustomLib_FIND_QUIETLY)
        message(STATUS "Found CustomLib: ${CUSTOMLIB_LIBRARY}")
        if(CUSTOMLIB_VERSION)
            message(STATUS "CustomLib version: ${CUSTOMLIB_VERSION}")
        endif()
        message(STATUS "CustomLib include dir: ${CUSTOMLIB_INCLUDE_DIR}")
    endif()
else()
    if(CustomLib_FIND_REQUIRED)
        message(STATUS "CustomLib not found.")
        message(STATUS "")
        message(STATUS "Installation options:")
        if(APPLE)
            message(STATUS "  macOS (Homebrew): brew install customlib")
        elseif(UNIX AND NOT APPLE)
            # Detect Linux distribution
            if(EXISTS "/etc/debian_version")
                message(
                    STATUS "  Ubuntu/Debian: sudo apt update && sudo apt install libcustomlib-dev")
            elseif(EXISTS "/etc/redhat-release" OR EXISTS "/etc/fedora-release")
                message(STATUS "  Fedora/RHEL: sudo dnf install customlib-devel")
            else()
                message(
                    STATUS "  Linux: Install customlib development package via your package manager"
                )
            endif()
        elseif(WIN32)
            message(STATUS "  Windows (Chocolatey): choco install customlib")
        endif()
        message(STATUS "")
        message(STATUS "Alternative: Set CUSTOMLIB_ROOT to specify custom installation path")
        message(STATUS "  cmake -DCUSTOMLIB_ROOT=/path/to/customlib ..")
    endif()
endif()
