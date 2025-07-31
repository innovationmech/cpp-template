# Manual dependency management

# This directory is for manually managed dependencies
# Place header-only libraries or pre-built libraries here

# Example for header-only library:
# add_library(manual-header-lib INTERFACE)
# target_include_directories(manual-header-lib INTERFACE
#     ${CMAKE_CURRENT_SOURCE_DIR}/header-only-lib/include
# )
# add_library(third-party::manual-header-lib ALIAS manual-header-lib)

# Example for pre-built library:
# find_library(MANUAL_LIB_PATH
#     NAMES manual-lib
#     PATHS ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/lib
#     NO_DEFAULT_PATH
# )
# 
# if(MANUAL_LIB_PATH)
#     add_library(manual-prebuilt-lib UNKNOWN IMPORTED)
#     set_target_properties(manual-prebuilt-lib PROPERTIES
#         IMPORTED_LOCATION ${MANUAL_LIB_PATH}
#         INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/include
#     )
#     add_library(third-party::manual-prebuilt-lib ALIAS manual-prebuilt-lib)
# endif()

# Add example manual dependency
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/example_lib/CMakeLists.txt")
    message(STATUS "Adding example_lib manual dependency")
    add_subdirectory(example_lib)
else()
    message(WARNING "example_lib manual dependency not found")
endif()

# Create a convenience target that links all manual dependencies
if(NOT TARGET manual_dependencies)
    add_library(manual_dependencies INTERFACE)
endif()

# Link all manual dependencies to the convenience target
if(TARGET example_lib)
    target_link_libraries(manual_dependencies INTERFACE manual::example_lib)
endif()

message(STATUS "Manual dependency directory configured")