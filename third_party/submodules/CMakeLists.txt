# Git Submodules Integration
# This file demonstrates how to integrate Git submodules into the build system

# Add nlohmann/json library as a submodule
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/nlohmann_json/CMakeLists.txt")
    message(STATUS "Adding nlohmann/json submodule")
    
    # Disable nlohmann_json tests and other unnecessary components
    set(JSON_BuildTests OFF CACHE INTERNAL "")
    set(JSON_Install OFF CACHE INTERNAL "")
    set(JSON_MultipleHeaders OFF CACHE INTERNAL "")
    
    # Add the submodule directory
    add_subdirectory(nlohmann_json)
    
    # Create an alias for easier usage
    if(TARGET nlohmann_json AND NOT TARGET submodules::json)
        add_library(submodules::json ALIAS nlohmann_json)
    elseif(TARGET nlohmann_json::nlohmann_json AND NOT TARGET submodules::json)
        # nlohmann_json::nlohmann_json is already an alias, so we reference the original target
        if(TARGET nlohmann_json)
            add_library(submodules::json ALIAS nlohmann_json)
        endif()
    endif()
    
    message(STATUS "nlohmann/json submodule added successfully")
else()
    message(WARNING "nlohmann/json submodule not found. Run 'git submodule update --init --recursive' to initialize submodules.")
endif()

# Example of how to add more submodules:
# if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/another_submodule/CMakeLists.txt")
#     add_subdirectory(another_submodule)
# endif()

# Note: When adding new submodules, follow these steps:
# 1. git submodule add <repository_url> third_party/submodules/<name>
# 2. Add the subdirectory configuration above
# 3. Create alias targets for consistent naming
# 4. Update the main CMakeLists.txt to include this directory if not already done