# FindSystemDependencies.cmake
# This module demonstrates how to find and configure system-installed dependencies

# Function to find and configure system dependencies
function(find_system_dependencies)
    message(STATUS "Searching for system dependencies...")
    
    # Try to find pkg-config first (useful for many libraries)
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        message(STATUS "Found pkg-config: ${PKG_CONFIG_EXECUTABLE}")
    endif()
    
    # OpenSSL - commonly available on most systems
    find_package(OpenSSL QUIET)
    if(OpenSSL_FOUND)
        message(STATUS "Found system OpenSSL: ${OPENSSL_VERSION}")
        # Create a target for easier usage
        if(NOT TARGET system::openssl)
            add_library(system::openssl INTERFACE IMPORTED)
            target_link_libraries(system::openssl INTERFACE OpenSSL::SSL OpenSSL::Crypto)
        endif()
    else()
        message(STATUS "System OpenSSL not found")
    endif()
    
    # ZLIB - compression library
    find_package(ZLIB QUIET)
    if(ZLIB_FOUND)
        message(STATUS "Found system ZLIB: ${ZLIB_VERSION_STRING}")
        if(NOT TARGET system::zlib)
            add_library(system::zlib INTERFACE IMPORTED)
            target_link_libraries(system::zlib INTERFACE ZLIB::ZLIB)
        endif()
    else()
        message(STATUS "System ZLIB not found")
    endif()
    
    # CURL - HTTP client library
    find_package(CURL QUIET)
    if(CURL_FOUND)
        message(STATUS "Found system CURL: ${CURL_VERSION_STRING}")
        if(NOT TARGET system::curl)
            add_library(system::curl INTERFACE IMPORTED)
            target_link_libraries(system::curl INTERFACE CURL::libcurl)
        endif()
    else()
        message(STATUS "System CURL not found")
    endif()
    
    # Threads - usually available on all systems
    find_package(Threads QUIET)
    if(Threads_FOUND)
        message(STATUS "Found system Threads support")
        if(NOT TARGET system::threads)
            add_library(system::threads INTERFACE IMPORTED)
            target_link_libraries(system::threads INTERFACE Threads::Threads)
        endif()
    endif()
    
    # Boost (optional) - large C++ library collection
    find_package(Boost QUIET COMPONENTS system filesystem thread chrono)
    if(Boost_FOUND)
        message(STATUS "Found system Boost: ${Boost_VERSION}")
        if(NOT TARGET system::boost)
            add_library(system::boost INTERFACE IMPORTED)
            target_link_libraries(system::boost INTERFACE 
                Boost::system 
                Boost::filesystem 
                Boost::thread 
                Boost::chrono
            )
        endif()
    else()
        message(STATUS "System Boost not found (optional)")
    endif()
    
    # Platform-specific libraries
    if(APPLE)
        # macOS-specific libraries
        find_library(COREFOUNDATION_LIBRARY CoreFoundation)
        if(COREFOUNDATION_LIBRARY)
            message(STATUS "Found CoreFoundation: ${COREFOUNDATION_LIBRARY}")
        endif()
        
        find_library(SECURITY_LIBRARY Security)
        if(SECURITY_LIBRARY)
            message(STATUS "Found Security framework: ${SECURITY_LIBRARY}")
        endif()
    elseif(UNIX)
        # Linux-specific libraries
        find_library(DL_LIBRARY dl)
        if(DL_LIBRARY)
            message(STATUS "Found libdl: ${DL_LIBRARY}")
        endif()
        
        find_library(RT_LIBRARY rt)
        if(RT_LIBRARY)
            message(STATUS "Found librt: ${RT_LIBRARY}")
        endif()
    endif()
    
    # Create a convenience target that includes all found system dependencies
    if(NOT TARGET system_dependencies)
        add_library(system_dependencies INTERFACE)
        
        # Link available system libraries
        if(TARGET system::openssl)
            target_link_libraries(system_dependencies INTERFACE system::openssl)
        endif()
        
        if(TARGET system::zlib)
            target_link_libraries(system_dependencies INTERFACE system::zlib)
        endif()
        
        if(TARGET system::curl)
            target_link_libraries(system_dependencies INTERFACE system::curl)
        endif()
        
        if(TARGET system::threads)
            target_link_libraries(system_dependencies INTERFACE system::threads)
        endif()
        
        if(TARGET system::boost)
            target_link_libraries(system_dependencies INTERFACE system::boost)
        endif()
        
        # Platform-specific linking
        if(APPLE)
            if(COREFOUNDATION_LIBRARY)
                target_link_libraries(system_dependencies INTERFACE ${COREFOUNDATION_LIBRARY})
            endif()
            if(SECURITY_LIBRARY)
                target_link_libraries(system_dependencies INTERFACE ${SECURITY_LIBRARY})
            endif()
        elseif(UNIX)
            if(DL_LIBRARY)
                target_link_libraries(system_dependencies INTERFACE ${DL_LIBRARY})
            endif()
            if(RT_LIBRARY)
                target_link_libraries(system_dependencies INTERFACE ${RT_LIBRARY})
            endif()
        endif()
    endif()
    
    message(STATUS "System dependency search completed")
endfunction()

# Function to print system dependency summary
function(print_system_dependency_summary)
    message(STATUS "=== System Dependency Summary ===")
    
    if(OpenSSL_FOUND)
        message(STATUS "  ✓ OpenSSL ${OPENSSL_VERSION}")
    else()
        message(STATUS "  ✗ OpenSSL")
    endif()
    
    if(ZLIB_FOUND)
        message(STATUS "  ✓ ZLIB ${ZLIB_VERSION_STRING}")
    else()
        message(STATUS "  ✗ ZLIB")
    endif()
    
    if(CURL_FOUND)
        message(STATUS "  ✓ CURL ${CURL_VERSION_STRING}")
    else()
        message(STATUS "  ✗ CURL")
    endif()
    
    if(Threads_FOUND)
        message(STATUS "  ✓ Threads")
    else()
        message(STATUS "  ✗ Threads")
    endif()
    
    if(Boost_FOUND)
        message(STATUS "  ✓ Boost ${Boost_VERSION}")
    else()
        message(STATUS "  ✗ Boost (optional)")
    endif()
    
    message(STATUS "=================================")
endfunction()

# Automatically find system dependencies when this module is included
if(NOT SYSTEM_DEPENDENCIES_FOUND)
    find_system_dependencies()
    set(SYSTEM_DEPENDENCIES_FOUND TRUE CACHE INTERNAL "System dependencies have been searched")
endif()