# Third-party dependencies

# Manual dependencies
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/manual)
    add_subdirectory(manual)
endif()

# Git submodule dependencies
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/submodules)
    # Add submodule directories here when they exist
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/submodules/CMakeLists.txt)
        add_subdirectory(submodules)
    endif()
endif()