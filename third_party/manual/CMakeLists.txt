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

message(STATUS "Manual dependency directory configured")