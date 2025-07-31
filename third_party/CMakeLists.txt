# Third party dependencies CMakeLists.txt
# Configuration for external dependencies using multiple management approaches

message(STATUS "Configuring third-party dependencies...")

# Include vcpkg helpers for dependency management
include(VcpkgHelpers)

# =============================================================================
# vcpkg Dependencies (Requirement 3.3)
# =============================================================================

# Check if vcpkg is available and configure accordingly
check_vcpkg_integration()

if(USING_VCPKG)
    message(STATUS "Configuring vcpkg dependencies...")
    
    # Core dependencies (always available)
    find_package(fmt CONFIG QUIET)
    if(fmt_FOUND)
        message(STATUS "Found fmt via vcpkg")
        # Create an interface target for easier usage
        if(NOT TARGET third_party::fmt)
            add_library(third_party::fmt INTERFACE IMPORTED)
            target_link_libraries(third_party::fmt INTERFACE fmt::fmt)
        endif()
    endif()
    
    find_package(spdlog CONFIG QUIET)
    if(spdlog_FOUND)
        message(STATUS "Found spdlog via vcpkg")
        if(NOT TARGET third_party::spdlog)
            add_library(third_party::spdlog INTERFACE IMPORTED)
            target_link_libraries(third_party::spdlog INTERFACE spdlog::spdlog)
        endif()
    endif()
    
    find_package(nlohmann_json CONFIG QUIET)
    if(nlohmann_json_FOUND)
        message(STATUS "Found nlohmann_json via vcpkg")
        if(NOT TARGET third_party::json)
            add_library(third_party::json INTERFACE IMPORTED)
            target_link_libraries(third_party::json INTERFACE nlohmann_json::nlohmann_json)
        endif()
    endif()
    
    # Optional feature dependencies
    if("networking" IN_LIST VCPKG_MANIFEST_FEATURES)
        find_package(CURL CONFIG QUIET)
        if(CURL_FOUND)
            message(STATUS "Found CURL via vcpkg (networking feature)")
            if(NOT TARGET third_party::curl)
                add_library(third_party::curl INTERFACE IMPORTED)
                target_link_libraries(third_party::curl INTERFACE CURL::libcurl)
            endif()
        endif()
        
        find_package(OpenSSL REQUIRED)
        if(OpenSSL_FOUND)
            message(STATUS "Found OpenSSL via vcpkg (networking feature)")
            if(NOT TARGET third_party::openssl)
                add_library(third_party::openssl INTERFACE IMPORTED)
                target_link_libraries(third_party::openssl INTERFACE OpenSSL::SSL OpenSSL::Crypto)
            endif()
        endif()
    endif()
    
    if("database" IN_LIST VCPKG_MANIFEST_FEATURES)
        find_package(unofficial-sqlite3 CONFIG QUIET)
        if(unofficial-sqlite3_FOUND)
            message(STATUS "Found SQLite3 via vcpkg (database feature)")
            if(NOT TARGET third_party::sqlite3)
                add_library(third_party::sqlite3 INTERFACE IMPORTED)
                target_link_libraries(third_party::sqlite3 INTERFACE unofficial::sqlite3::sqlite3)
            endif()
        endif()
    endif()
    
else()
    message(STATUS "vcpkg not detected, using alternative dependency resolution...")
endif()

# =============================================================================
# Git Submodules (Requirement 3.4)
# =============================================================================

# Include submodules if they exist
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/submodules/CMakeLists.txt")
    message(STATUS "Including Git submodule dependencies...")
    add_subdirectory(submodules)
else()
    message(STATUS "No Git submodule dependencies found")
endif()

# =============================================================================
# Manual Dependencies (Requirement 3.1)
# =============================================================================

# Include manual dependencies if they exist
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/manual/CMakeLists.txt")
    message(STATUS "Including manual dependencies...")
    add_subdirectory(manual)
else()
    message(STATUS "No manual dependencies found")
endif()

# =============================================================================
# System Package Dependencies (Requirement 3.2)
# =============================================================================

# Example system package finding with fallbacks
find_package_with_fallback(Threads 
    VCPKG_NAME threads 
    SYSTEM_NAME pthread
)

if(Threads_FOUND AND NOT TARGET third_party::threads)
    add_library(third_party::threads INTERFACE IMPORTED)
    target_link_libraries(third_party::threads INTERFACE Threads::Threads)
endif()

# =============================================================================
# Dependency Summary
# =============================================================================

# Print summary of found dependencies
message(STATUS "=== Dependency Summary ===")
get_property(imported_targets DIRECTORY PROPERTY IMPORTED_TARGETS)
foreach(target ${imported_targets})
    if(target MATCHES "^third_party::")
        message(STATUS "Available: ${target}")
    endif()
endforeach()
message(STATUS "==========================")

# Export targets for use by other parts of the project
set(THIRD_PARTY_TARGETS "" CACHE INTERNAL "List of third-party targets")
get_property(all_targets DIRECTORY PROPERTY IMPORTED_TARGETS)
foreach(target ${all_targets})
    if(target MATCHES "^third_party::")
        list(APPEND THIRD_PARTY_TARGETS ${target})
    endif()
endforeach()
set(THIRD_PARTY_TARGETS ${THIRD_PARTY_TARGETS} CACHE INTERNAL "List of third-party targets")