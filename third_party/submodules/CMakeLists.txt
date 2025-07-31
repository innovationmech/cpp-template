# Git Submodules CMakeLists.txt
# Configuration for dependencies managed as Git submodules

message(STATUS "Configuring Git submodule dependencies...")

# =============================================================================
# Submodule Availability Check
# =============================================================================

# Function to check if a submodule is available and initialized
function(check_submodule_available SUBMODULE_PATH RESULT_VAR)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${SUBMODULE_PATH}/.git" OR 
       EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${SUBMODULE_PATH}/CMakeLists.txt")
        set(${RESULT_VAR} TRUE PARENT_SCOPE)
    else()
        set(${RESULT_VAR} FALSE PARENT_SCOPE)
    endif()
endfunction()

# Function to warn about missing submodules
function(warn_missing_submodule SUBMODULE_NAME SUBMODULE_PATH)
    message(WARNING "Submodule '${SUBMODULE_NAME}' not found at ${SUBMODULE_PATH}")
    message(STATUS "To initialize submodules, run:")
    message(STATUS "  git submodule update --init --recursive")
    message(STATUS "Or to add this specific submodule:")
    message(STATUS "  git submodule update --init ${SUBMODULE_PATH}")
endfunction()

# =============================================================================
# nlohmann/json (Header-only JSON library)
# =============================================================================

check_submodule_available("json" JSON_AVAILABLE)
if(JSON_AVAILABLE)
    message(STATUS "Found nlohmann/json submodule")
    
    # nlohmann/json is header-only, so we can include it directly
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/json/single_include/nlohmann/json.hpp")
        # Use single header version if available
        add_library(submodule_json INTERFACE)
        target_include_directories(submodule_json INTERFACE 
            "${CMAKE_CURRENT_SOURCE_DIR}/json/single_include"
        )
        add_library(third_party::submodule_json ALIAS submodule_json)
        message(STATUS "Using nlohmann/json single header")
    elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/json/include/nlohmann/json.hpp")
        # Use multi-header version
        add_library(submodule_json INTERFACE)
        target_include_directories(submodule_json INTERFACE 
            "${CMAKE_CURRENT_SOURCE_DIR}/json/include"
        )
        add_library(third_party::submodule_json ALIAS submodule_json)
        message(STATUS "Using nlohmann/json multi-header")
    else()
        message(WARNING "nlohmann/json submodule found but headers not located")
    endif()
else()
    warn_missing_submodule("nlohmann/json" "third_party/submodules/json")
endif()

# =============================================================================
# Catch2 (Testing framework)
# =============================================================================

check_submodule_available("catch2" CATCH2_AVAILABLE)
if(CATCH2_AVAILABLE)
    message(STATUS "Found Catch2 submodule")
    
    # Check if Catch2 has CMake support
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/catch2/CMakeLists.txt")
        # Use Catch2's CMake configuration
        add_subdirectory(catch2)
        
        # Create alias for consistent naming
        if(TARGET Catch2::Catch2)
            add_library(third_party::catch2 ALIAS Catch2::Catch2)
        elseif(TARGET Catch2)
            add_library(third_party::catch2 ALIAS Catch2)
        endif()
        
        message(STATUS "Using Catch2 with CMake integration")
    elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/catch2/single_include/catch2/catch.hpp")
        # Use single header version
        add_library(submodule_catch2 INTERFACE)
        target_include_directories(submodule_catch2 INTERFACE 
            "${CMAKE_CURRENT_SOURCE_DIR}/catch2/single_include"
        )
        add_library(third_party::catch2 ALIAS submodule_catch2)
        message(STATUS "Using Catch2 single header")
    else()
        message(WARNING "Catch2 submodule found but configuration not recognized")
    endif()
else()
    warn_missing_submodule("Catch2" "third_party/submodules/catch2")
endif()

# =============================================================================
# cxxopts (Command line argument parsing)
# =============================================================================

check_submodule_available("cxxopts" CXXOPTS_AVAILABLE)
if(CXXOPTS_AVAILABLE)
    message(STATUS "Found cxxopts submodule")
    
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cxxopts/CMakeLists.txt")
        # Use cxxopts CMake configuration
        set(CXXOPTS_BUILD_EXAMPLES OFF CACHE BOOL "Build cxxopts examples")
        set(CXXOPTS_BUILD_TESTS OFF CACHE BOOL "Build cxxopts tests")
        add_subdirectory(cxxopts)
        
        if(TARGET cxxopts)
            add_library(third_party::cxxopts ALIAS cxxopts)
        endif()
        
        message(STATUS "Using cxxopts with CMake integration")
    elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cxxopts/include/cxxopts.hpp")
        # Use header-only version
        add_library(submodule_cxxopts INTERFACE)
        target_include_directories(submodule_cxxopts INTERFACE 
            "${CMAKE_CURRENT_SOURCE_DIR}/cxxopts/include"
        )
        add_library(third_party::cxxopts ALIAS submodule_cxxopts)
        message(STATUS "Using cxxopts header-only")
    else()
        message(WARNING "cxxopts submodule found but headers not located")
    endif()
else()
    warn_missing_submodule("cxxopts" "third_party/submodules/cxxopts")
endif()

# =============================================================================
# spdlog (Logging library - header-only mode)
# =============================================================================

check_submodule_available("spdlog-header-only" SPDLOG_HO_AVAILABLE)
if(SPDLOG_HO_AVAILABLE)
    message(STATUS "Found spdlog header-only submodule")
    
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/spdlog-header-only/include/spdlog/spdlog.h")
        # Use header-only version
        add_library(submodule_spdlog INTERFACE)
        target_include_directories(submodule_spdlog INTERFACE 
            "${CMAKE_CURRENT_SOURCE_DIR}/spdlog-header-only/include"
        )
        target_compile_definitions(submodule_spdlog INTERFACE 
            SPDLOG_HEADER_ONLY=1
        )
        add_library(third_party::submodule_spdlog ALIAS submodule_spdlog)
        message(STATUS "Using spdlog header-only mode")
    else()
        message(WARNING "spdlog submodule found but headers not located")
    endif()
else()
    warn_missing_submodule("spdlog-header-only" "third_party/submodules/spdlog-header-only")
endif()

# =============================================================================
# Submodule Management Utilities
# =============================================================================

# Create a custom target to update all submodules
add_custom_target(update-submodules
    COMMAND ${GIT_EXECUTABLE} submodule update --remote --merge
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Updating Git submodules to latest versions"
    VERBATIM
)

# Create a custom target to initialize submodules
add_custom_target(init-submodules
    COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Initializing Git submodules"
    VERBATIM
)

# =============================================================================
# Summary
# =============================================================================

message(STATUS "=== Git Submodule Summary ===")
get_property(imported_targets DIRECTORY PROPERTY IMPORTED_TARGETS)
foreach(target ${imported_targets})
    if(target MATCHES "^third_party::.*submodule.*" OR target MATCHES "^third_party::(catch2|cxxopts)$")
        message(STATUS "Available: ${target}")
    endif()
endforeach()

# Check for available non-alias targets too
if(TARGET submodule_json)
    message(STATUS "Available: third_party::submodule_json")
endif()
if(TARGET submodule_catch2)
    message(STATUS "Available: third_party::catch2 (submodule)")
endif()
if(TARGET submodule_cxxopts)
    message(STATUS "Available: third_party::cxxopts (submodule)")
endif()
if(TARGET submodule_spdlog)
    message(STATUS "Available: third_party::submodule_spdlog")
endif()

message(STATUS "==============================")

# Note: Actual submodule directories are not created by this script
# Users need to run: git submodule update --init --recursive