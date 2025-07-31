# Third-party dependencies

# Manual dependencies
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/manual)
    add_subdirectory(manual)
endif()

# Git submodule dependencies
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/submodules)
    # Add submodule directories here when they exist
    # Example:
    # if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/submodules/some-library)
    #     add_subdirectory(submodules/some-library)
    # endif()
endif()