# Manual Dependencies CMakeLists.txt
# Configuration for manually managed external dependencies

message(STATUS "Configuring manual dependencies...")

# =============================================================================
# Manual Dependency Management Utilities
# =============================================================================

# Function to check if a manual dependency is available
function(check_manual_dependency DEP_NAME DEP_PATH RESULT_VAR)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${DEP_PATH}")
        set(${RESULT_VAR} TRUE PARENT_SCOPE)
        message(STATUS "Found manual dependency: ${DEP_NAME}")
    else()
        set(${RESULT_VAR} FALSE PARENT_SCOPE)
        message(STATUS "Manual dependency not found: ${DEP_NAME} at ${DEP_PATH}")
    endif()
endfunction()

# Function to create interface library for header-only dependencies
function(create_header_only_target TARGET_NAME INCLUDE_DIR)
    add_library(${TARGET_NAME} INTERFACE)
    target_include_directories(${TARGET_NAME} INTERFACE "${INCLUDE_DIR}")
    message(STATUS "Created header-only target: ${TARGET_NAME}")
endfunction()

# Function to create compiled library target
function(create_compiled_target TARGET_NAME SOURCE_DIR INCLUDE_DIR)
    file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/*.cpp" "${SOURCE_DIR}/*.c")
    if(SOURCES)
        add_library(${TARGET_NAME} STATIC ${SOURCES})
        target_include_directories(${TARGET_NAME} 
            PUBLIC "${INCLUDE_DIR}"
            PRIVATE "${SOURCE_DIR}"
        )
        message(STATUS "Created compiled target: ${TARGET_NAME} with ${list(LENGTH SOURCES)} source files")
    else()
        message(WARNING "No source files found for ${TARGET_NAME} in ${SOURCE_DIR}")
    endif()
endfunction()

# =============================================================================
# Example: Header-only Math Library
# =============================================================================

check_manual_dependency("MathUtils" "mathutils" MATHUTILS_AVAILABLE)
if(MATHUTILS_AVAILABLE)
    # Check for different possible structures
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/mathutils/include")
        create_header_only_target(manual_mathutils 
            "${CMAKE_CURRENT_SOURCE_DIR}/mathutils/include"
        )
    elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/mathutils/mathutils.h")
        create_header_only_target(manual_mathutils 
            "${CMAKE_CURRENT_SOURCE_DIR}/mathutils"
        )
    else()
        message(WARNING "MathUtils found but no recognizable structure")
    endif()
    
    if(TARGET manual_mathutils)
        add_library(third_party::mathutils ALIAS manual_mathutils)
    endif()
else()
    message(STATUS "To add MathUtils manually:")
    message(STATUS "  1. Download or copy library to third_party/manual/mathutils/")
    message(STATUS "  2. Ensure headers are in mathutils/include/ or mathutils/")
    message(STATUS "  3. Reconfigure CMake")
endif()

# =============================================================================
# Example: Compiled Utility Library
# =============================================================================

check_manual_dependency("UtilLib" "utillib" UTILLIB_AVAILABLE)
if(UTILLIB_AVAILABLE)
    set(UTILLIB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/utillib/include")
    set(UTILLIB_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/utillib/src")
    
    if(EXISTS "${UTILLIB_INCLUDE_DIR}" AND EXISTS "${UTILLIB_SOURCE_DIR}")
        create_compiled_target(manual_utillib 
            "${UTILLIB_SOURCE_DIR}"
            "${UTILLIB_INCLUDE_DIR}"
        )
        
        if(TARGET manual_utillib)
            # Set additional properties for compiled library
            target_compile_features(manual_utillib PUBLIC cxx_std_17)
            set_target_properties(manual_utillib PROPERTIES
                CXX_STANDARD 17
                CXX_STANDARD_REQUIRED ON
                CXX_EXTENSIONS OFF
            )
            add_library(third_party::utillib ALIAS manual_utillib)
        endif()
    else()
        message(WARNING "UtilLib found but missing include/ or src/ directories")
    endif()
else()
    message(STATUS "To add UtilLib manually:")
    message(STATUS "  1. Create third_party/manual/utillib/")
    message(STATUS "  2. Add headers to utillib/include/")
    message(STATUS "  3. Add sources to utillib/src/")
    message(STATUS "  4. Reconfigure CMake")
endif()

# =============================================================================
# Example: Pre-built Library Integration
# =============================================================================

check_manual_dependency("PrebuiltLib" "prebuilt" PREBUILT_AVAILABLE)
if(PREBUILT_AVAILABLE)
    set(PREBUILT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/include")
    set(PREBUILT_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/lib")
    
    if(EXISTS "${PREBUILT_INCLUDE_DIR}")
        # Create imported library target
        add_library(manual_prebuilt UNKNOWN IMPORTED)
        
        # Set include directories
        target_include_directories(manual_prebuilt INTERFACE "${PREBUILT_INCLUDE_DIR}")
        
        # Find and set library file
        if(WIN32)
            set(LIB_EXTENSIONS ".lib" ".dll.a")
            set(LIB_NAMES "prebuilt" "libprebuilt")
        else()
            set(LIB_EXTENSIONS ".a" ".so" ".dylib")
            set(LIB_NAMES "libprebuilt" "prebuilt")
        endif()
        
        foreach(LIB_NAME ${LIB_NAMES})
            foreach(LIB_EXT ${LIB_EXTENSIONS})
                set(LIB_PATH "${PREBUILT_LIB_DIR}/${LIB_NAME}${LIB_EXT}")
                if(EXISTS "${LIB_PATH}")
                    set_target_properties(manual_prebuilt PROPERTIES
                        IMPORTED_LOCATION "${LIB_PATH}"
                    )
                    message(STATUS "Found prebuilt library: ${LIB_PATH}")
                    break()
                endif()
            endforeach()
            if(TARGET manual_prebuilt AND DEFINED IMPORTED_LOCATION)
                break()
            endif()
        endforeach()
        
        if(TARGET manual_prebuilt)
            add_library(third_party::prebuilt ALIAS manual_prebuilt)
        endif()
    else()
        message(WARNING "PrebuiltLib found but missing include directory")
    endif()
else()
    message(STATUS "To add PrebuiltLib manually:")
    message(STATUS "  1. Create third_party/manual/prebuilt/")
    message(STATUS "  2. Add headers to prebuilt/include/")
    message(STATUS "  3. Add library files to prebuilt/lib/")
    message(STATUS "  4. Reconfigure CMake")
endif()

# =============================================================================
# Example: Custom CMake Integration
# =============================================================================

check_manual_dependency("CustomLib" "customlib" CUSTOMLIB_AVAILABLE)
if(CUSTOMLIB_AVAILABLE)
    # Check if the dependency has its own CMakeLists.txt
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/customlib/CMakeLists.txt")
        message(STATUS "Using CustomLib's own CMake configuration")
        
        # Set options to disable unwanted features
        set(CUSTOMLIB_BUILD_TESTS OFF CACHE BOOL "Disable CustomLib tests")
        set(CUSTOMLIB_BUILD_EXAMPLES OFF CACHE BOOL "Disable CustomLib examples")
        
        # Add subdirectory
        add_subdirectory(customlib)
        
        # Create alias if the library target exists
        if(TARGET customlib)
            add_library(third_party::customlib ALIAS customlib)
        elseif(TARGET CustomLib::CustomLib)
            add_library(third_party::customlib ALIAS CustomLib::CustomLib)
        endif()
    else()
        message(STATUS "CustomLib found but no CMakeLists.txt - using manual configuration")
        # Fallback to manual configuration similar to other examples
    endif()
else()
    message(STATUS "To add CustomLib manually:")
    message(STATUS "  1. Download library to third_party/manual/customlib/")
    message(STATUS "  2. If it has CMakeLists.txt, it will be used automatically")
    message(STATUS "  3. Otherwise, ensure proper include/src structure")
endif()

# =============================================================================
# Manual Dependency Template Generator
# =============================================================================

# Function to generate template structure for new manual dependencies
function(generate_manual_dependency_template DEP_NAME)
    set(DEP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${DEP_NAME}")
    
    if(NOT EXISTS "${DEP_DIR}")
        message(STATUS "Generating template structure for ${DEP_NAME}:")
        
        # Create directory structure
        file(MAKE_DIRECTORY "${DEP_DIR}/include/${DEP_NAME}")
        file(MAKE_DIRECTORY "${DEP_DIR}/src")
        file(MAKE_DIRECTORY "${DEP_DIR}/lib")
        
        # Create example header file
        file(WRITE "${DEP_DIR}/include/${DEP_NAME}/${DEP_NAME}.h"
            "// ${DEP_NAME} - Manual dependency header\n"
            "#pragma once\n\n"
            "namespace ${DEP_NAME} {\n"
            "    void hello();\n"
            "}\n"
        )
        
        # Create example source file
        file(WRITE "${DEP_DIR}/src/${DEP_NAME}.cpp"
            "// ${DEP_NAME} - Manual dependency implementation\n"
            "#include \"${DEP_NAME}/${DEP_NAME}.h\"\n"
            "#include <iostream>\n\n"
            "namespace ${DEP_NAME} {\n"
            "    void hello() {\n"
            "        std::cout << \"Hello from ${DEP_NAME}!\" << std::endl;\n"
            "    }\n"
            "}\n"
        )
        
        # Create README
        file(WRITE "${DEP_DIR}/README.md"
            "# ${DEP_NAME}\n\n"
            "This is a manually managed dependency.\n\n"
            "## Structure\n"
            "- `include/`: Public header files\n"
            "- `src/`: Source files (for compiled libraries)\n"
            "- `lib/`: Pre-built library files (for pre-built libraries)\n\n"
            "## Usage\n"
            "Replace this template with your actual dependency files.\n"
        )
        
        message(STATUS "  Created: ${DEP_DIR}/")
        message(STATUS "  - include/${DEP_NAME}/${DEP_NAME}.h")
        message(STATUS "  - src/${DEP_NAME}.cpp")
        message(STATUS "  - README.md")
    endif()
endfunction()

# Uncomment to generate templates for new dependencies
# generate_manual_dependency_template("newlib")

# =============================================================================
# Summary and Documentation
# =============================================================================

message(STATUS "=== Manual Dependencies Summary ===")

# List all available manual dependency targets
get_property(all_targets DIRECTORY PROPERTY BUILDSYSTEM_TARGETS)
foreach(target ${all_targets})
    if(target MATCHES "^manual_.*")
        get_target_property(target_type ${target} TYPE)
        message(STATUS "Available: ${target} (${target_type})")
    endif()
endforeach()

# List all alias targets
get_property(imported_targets DIRECTORY PROPERTY IMPORTED_TARGETS)
foreach(target ${imported_targets})
    if(target MATCHES "^third_party::(mathutils|utillib|prebuilt|customlib)$")
        message(STATUS "Available: ${target}")
    endif()
endforeach()

message(STATUS "====================================")

# Create documentation target
add_custom_target(manual-deps-info
    COMMAND ${CMAKE_COMMAND} -E echo "Manual Dependencies Information:"
    COMMAND ${CMAKE_COMMAND} -E echo "================================"
    COMMAND ${CMAKE_COMMAND} -E echo "Directory: ${CMAKE_CURRENT_SOURCE_DIR}"
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "To add a new manual dependency:"
    COMMAND ${CMAKE_COMMAND} -E echo "1. Create directory: third_party/manual/<name>/"
    COMMAND ${CMAKE_COMMAND} -E echo "2. Add headers to: <name>/include/"
    COMMAND ${CMAKE_COMMAND} -E echo "3. Add sources to: <name>/src/ (if needed)"
    COMMAND ${CMAKE_COMMAND} -E echo "4. Add libraries to: <name>/lib/ (if pre-built)"
    COMMAND ${CMAKE_COMMAND} -E echo "5. Reconfigure CMake"
    COMMENT "Displaying manual dependencies information"
    VERBATIM
)