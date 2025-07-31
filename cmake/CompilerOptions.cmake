# CompilerOptions.cmake
# Custom CMake module for standardized compiler settings

# Function to set common compiler options for a target
function(set_project_compiler_options target_name)
    target_compile_options(${target_name} PRIVATE
        # GCC/Clang warnings
        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
            -Wall -Wextra -Wpedantic -Werror
            -Wcast-align -Wcast-qual -Wctor-dtor-privacy
            -Wdisabled-optimization -Wformat=2 -Winit-self
            -Wmissing-declarations -Wmissing-include-dirs
            -Wold-style-cast -Woverloaded-virtual -Wredundant-decls
            -Wshadow -Wsign-conversion -Wsign-promo
            -Wstrict-overflow=5 -Wswitch-default -Wundef
        >
        # MSVC warnings
        $<$<CXX_COMPILER_ID:MSVC>:
            /W4 /WX /permissive-
        >
    )

    # Debug-specific options
    target_compile_options(${target_name} PRIVATE
        $<$<CONFIG:Debug>:
            $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-g -O0>
            $<$<CXX_COMPILER_ID:MSVC>:/Od /Zi>
        >
    )

    # Release-specific options
    target_compile_options(${target_name} PRIVATE
        $<$<CONFIG:Release>:
            $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-O3 -DNDEBUG>
            $<$<CXX_COMPILER_ID:MSVC>:/O2 /DNDEBUG>
        >
    )
endfunction()

# Set global compiler-specific options
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # GCC-specific options
    add_compile_options(-fdiagnostics-color=always)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Clang-specific options
    add_compile_options(-fcolor-diagnostics)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # MSVC-specific options
    add_compile_options(/utf-8)
endif()

# Platform-specific definitions
if(WIN32)
    add_compile_definitions(
        WIN32_LEAN_AND_MEAN
        NOMINMAX
        _CRT_SECURE_NO_WARNINGS
    )
endif()