# ============================================================================= CodeQuality.cmake -
# Code Formatting and Static Analysis Integration Comprehensive code quality tools integration for
# modern C++ projects
#
# This module provides CMake integration for: - clang-format (code formatting) - clang-tidy (static
# analysis) - cppcheck (additional static analysis) - cmake-format (CMake file formatting)
#
# Features: - Automatic tool detection - Custom targets for manual execution - Optional integration
# with build process - Cross-platform support
# =============================================================================

# Only configure code quality tools if this is the main project
if(PROJECT_IS_TOP_LEVEL OR CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    message(STATUS "Configuring code quality tools for ${PROJECT_NAME}")

    # ============================================================================= Tool Detection
    # =============================================================================

    # Find clang-format
    find_program(
        CLANG_FORMAT_EXECUTABLE
        NAMES clang-format clang-format-17 clang-format-16 clang-format-15 clang-format-14
        DOC "Path to clang-format executable")

    # Find clang-tidy
    find_program(
        CLANG_TIDY_EXECUTABLE
        NAMES clang-tidy clang-tidy-17 clang-tidy-16 clang-tidy-15 clang-tidy-14
        DOC "Path to clang-tidy executable")

    # Find cppcheck
    find_program(
        CPPCHECK_EXECUTABLE
        NAMES cppcheck
        DOC "Path to cppcheck executable")

    # Find cmake-format (Python package)
    find_program(
        CMAKE_FORMAT_EXECUTABLE
        NAMES cmake-format
        DOC "Path to cmake-format executable")

    # Display tool detection results
    if(CLANG_FORMAT_EXECUTABLE)
        message(STATUS "Found clang-format: ${CLANG_FORMAT_EXECUTABLE}")
        execute_process(
            COMMAND ${CLANG_FORMAT_EXECUTABLE} --version
            OUTPUT_VARIABLE CLANG_FORMAT_VERSION
            OUTPUT_STRIP_TRAILING_WHITESPACE)
        message(STATUS "clang-format version: ${CLANG_FORMAT_VERSION}")
    else()
        message(STATUS "clang-format not found - formatting targets will not be available")
    endif()

    if(CLANG_TIDY_EXECUTABLE)
        message(STATUS "Found clang-tidy: ${CLANG_TIDY_EXECUTABLE}")
        execute_process(
            COMMAND ${CLANG_TIDY_EXECUTABLE} --version
            OUTPUT_VARIABLE CLANG_TIDY_VERSION
            OUTPUT_STRIP_TRAILING_WHITESPACE)
        message(STATUS "clang-tidy version: ${CLANG_TIDY_VERSION}")
    else()
        message(STATUS "clang-tidy not found - static analysis will not be available")
    endif()

    if(CPPCHECK_EXECUTABLE)
        message(STATUS "Found cppcheck: ${CPPCHECK_EXECUTABLE}")
        execute_process(
            COMMAND ${CPPCHECK_EXECUTABLE} --version
            OUTPUT_VARIABLE CPPCHECK_VERSION
            OUTPUT_STRIP_TRAILING_WHITESPACE)
        message(STATUS "cppcheck version: ${CPPCHECK_VERSION}")
    else()
        message(STATUS "cppcheck not found - additional static analysis will not be available")
    endif()

    if(CMAKE_FORMAT_EXECUTABLE)
        message(STATUS "Found cmake-format: ${CMAKE_FORMAT_EXECUTABLE}")
    else()
        message(STATUS "cmake-format not found - CMake file formatting will not be available")
    endif()

    # ============================================================================= Source File
    # Collection
    # =============================================================================

    # Collect all C++ source files for formatting and analysis
    file(
        GLOB_RECURSE
        ALL_CXX_SOURCE_FILES
        "${CMAKE_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_SOURCE_DIR}/src/*.cxx"
        "${CMAKE_SOURCE_DIR}/src/*.cc"
        "${CMAKE_SOURCE_DIR}/src/*.c++"
        "${CMAKE_SOURCE_DIR}/libs/*.cpp"
        "${CMAKE_SOURCE_DIR}/libs/*.cxx"
        "${CMAKE_SOURCE_DIR}/libs/*.cc"
        "${CMAKE_SOURCE_DIR}/libs/*.c++"
        "${CMAKE_SOURCE_DIR}/examples/*.cpp"
        "${CMAKE_SOURCE_DIR}/examples/*.cxx"
        "${CMAKE_SOURCE_DIR}/examples/*.cc"
        "${CMAKE_SOURCE_DIR}/examples/*.c++"
        "${CMAKE_SOURCE_DIR}/tests/*.cpp"
        "${CMAKE_SOURCE_DIR}/tests/*.cxx"
        "${CMAKE_SOURCE_DIR}/tests/*.cc"
        "${CMAKE_SOURCE_DIR}/tests/*.c++")

    # Collect all C++ header files
    file(
        GLOB_RECURSE
        ALL_CXX_HEADER_FILES
        "${CMAKE_SOURCE_DIR}/src/*.h"
        "${CMAKE_SOURCE_DIR}/src/*.hpp"
        "${CMAKE_SOURCE_DIR}/src/*.hxx"
        "${CMAKE_SOURCE_DIR}/src/*.h++"
        "${CMAKE_SOURCE_DIR}/libs/*.h"
        "${CMAKE_SOURCE_DIR}/libs/*.hpp"
        "${CMAKE_SOURCE_DIR}/libs/*.hxx"
        "${CMAKE_SOURCE_DIR}/libs/*.h++"
        "${CMAKE_SOURCE_DIR}/include/*.h"
        "${CMAKE_SOURCE_DIR}/include/*.hpp"
        "${CMAKE_SOURCE_DIR}/include/*.hxx"
        "${CMAKE_SOURCE_DIR}/include/*.h++"
        "${CMAKE_SOURCE_DIR}/examples/*.h"
        "${CMAKE_SOURCE_DIR}/examples/*.hpp"
        "${CMAKE_SOURCE_DIR}/examples/*.hxx"
        "${CMAKE_SOURCE_DIR}/examples/*.h++"
        "${CMAKE_SOURCE_DIR}/tests/*.h"
        "${CMAKE_SOURCE_DIR}/tests/*.hpp"
        "${CMAKE_SOURCE_DIR}/tests/*.hxx"
        "${CMAKE_SOURCE_DIR}/tests/*.h++")

    # Combine all C++ files
    set(ALL_CXX_FILES ${ALL_CXX_SOURCE_FILES} ${ALL_CXX_HEADER_FILES})

    # Collect CMake files
    file(GLOB_RECURSE ALL_CMAKE_FILES "${CMAKE_SOURCE_DIR}/CMakeLists.txt"
         "${CMAKE_SOURCE_DIR}/*/CMakeLists.txt" "${CMAKE_SOURCE_DIR}/*/*.cmake"
         "${CMAKE_SOURCE_DIR}/cmake/*.cmake")

    # Remove files from third_party and build directories
    list(FILTER ALL_CXX_FILES EXCLUDE REGEX ".*/third_party/.*")
    list(FILTER ALL_CXX_FILES EXCLUDE REGEX ".*/build/.*")
    list(FILTER ALL_CMAKE_FILES EXCLUDE REGEX ".*/third_party/.*")
    list(FILTER ALL_CMAKE_FILES EXCLUDE REGEX ".*/build/.*")

    message(STATUS "Found ${CMAKE_LISTLEN} C++ files for analysis")
    message(STATUS "Found ${CMAKE_LISTLEN} CMake files for formatting")

    # ============================================================================= clang-format
    # Integration
    # =============================================================================

    if(CLANG_FORMAT_EXECUTABLE AND ALL_CXX_FILES)
        # Format target - show what would be changed
        add_custom_target(
            format
            COMMAND ${CLANG_FORMAT_EXECUTABLE} --style=file --output-replacements-xml
                    ${ALL_CXX_FILES}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Checking C++ code formatting with clang-format"
            VERBATIM)

        # Check-format target - fail if formatting is needed (for CI)
        add_custom_target(
            check-format
            COMMAND ${CLANG_FORMAT_EXECUTABLE} --style=file --dry-run --Werror ${ALL_CXX_FILES}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Checking C++ code formatting compliance"
            VERBATIM)

        # Fix-format target - apply formatting changes
        add_custom_target(
            fix-format
            COMMAND ${CLANG_FORMAT_EXECUTABLE} --style=file -i ${ALL_CXX_FILES}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Applying clang-format to C++ files"
            VERBATIM)

        message(STATUS "clang-format targets available: format, check-format, fix-format")
    endif()

    # ============================================================================= clang-tidy
    # Integration
    # =============================================================================

    if(CLANG_TIDY_EXECUTABLE)
        # Option to enable clang-tidy during build
        option(ENABLE_CLANG_TIDY "Enable clang-tidy analysis during build" OFF)

        if(ENABLE_CLANG_TIDY)
            # Configure clang-tidy to run automatically during build
            set(CMAKE_CXX_CLANG_TIDY
                ${CLANG_TIDY_EXECUTABLE};
                --config-file=${CMAKE_SOURCE_DIR}/.clang-tidy;
                --header-filter=${CMAKE_SOURCE_DIR}/
                (src|libs|include)
                /.*\\.
                (h|hpp|hxx)
                ;
                --warnings-as-errors=*)
            message(STATUS "clang-tidy enabled for build process")
        endif()

        # Manual clang-tidy target
        if(ALL_CXX_SOURCE_FILES)
            add_custom_target(
                clang-tidy-check
                COMMAND
                    ${CLANG_TIDY_EXECUTABLE} --config-file=${CMAKE_SOURCE_DIR}/.clang-tidy
                    --header-filter=${CMAKE_SOURCE_DIR}/ (src|libs|include) /.*\\\\. (h|hpp|hxx) -p
                    ${CMAKE_BINARY_DIR} ${ALL_CXX_SOURCE_FILES}
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                COMMENT "Running clang-tidy static analysis"
                VERBATIM)

            # clang-tidy with fixes
            add_custom_target(
                clang-tidy-fix
                COMMAND
                    ${CLANG_TIDY_EXECUTABLE} --config-file=${CMAKE_SOURCE_DIR}/.clang-tidy
                    --header-filter=${CMAKE_SOURCE_DIR}/ (src|libs|include) /.*\\\\. (h|hpp|hxx)
                    --fix --fix-errors -p ${CMAKE_BINARY_DIR} ${ALL_CXX_SOURCE_FILES}
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                COMMENT "Running clang-tidy with automatic fixes"
                VERBATIM)

            message(STATUS "clang-tidy targets available: clang-tidy-check, clang-tidy-fix")
        endif()
    endif()

    # ============================================================================= cppcheck
    # Integration
    # =============================================================================

    if(CPPCHECK_EXECUTABLE)
        # Option to enable cppcheck during build
        option(ENABLE_CPPCHECK "Enable cppcheck analysis during build" OFF)

        if(ENABLE_CPPCHECK)
            # Configure cppcheck to run automatically during build
            set(CMAKE_CXX_CPPCHECK
                ${CPPCHECK_EXECUTABLE};
                --enable=warning,style,performance,portability,unusedFunction; --std=c++17;
                --verbose; --quiet; --suppressions-list=${CMAKE_SOURCE_DIR}/.cppcheck-suppressions;)
            message(STATUS "cppcheck enabled for build process")
        endif()

        # Manual cppcheck target
        if(ALL_CXX_FILES)
            add_custom_target(
                cppcheck-check
                COMMAND
                    ${CPPCHECK_EXECUTABLE} --enable=all --std=c++17 --verbose
                    --project=${CMAKE_BINARY_DIR}/compile_commands.json
                    --suppressions-list=${CMAKE_SOURCE_DIR}/.cppcheck-suppressions
                    --error-exitcode=1
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                COMMENT "Running cppcheck static analysis"
                VERBATIM)

            message(STATUS "cppcheck targets available: cppcheck-check")
        endif()
    endif()

    # ============================================================================= cmake-format
    # Integration
    # =============================================================================

    if(CMAKE_FORMAT_EXECUTABLE AND ALL_CMAKE_FILES)
        # Format CMake files target
        add_custom_target(
            cmake-format
            COMMAND ${CMAKE_FORMAT_EXECUTABLE} --config-file ${CMAKE_SOURCE_DIR}/.cmake-format.yaml
                    --in-place ${ALL_CMAKE_FILES}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Formatting CMake files with cmake-format"
            VERBATIM)

        # Check CMake format target
        add_custom_target(
            check-cmake-format
            COMMAND ${CMAKE_FORMAT_EXECUTABLE} --config-file ${CMAKE_SOURCE_DIR}/.cmake-format.yaml
                    --check ${ALL_CMAKE_FILES}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Checking CMake file formatting compliance"
            VERBATIM)

        message(STATUS "cmake-format targets available: cmake-format, check-cmake-format")
    endif()

    # ============================================================================= Combined Targets
    # =============================================================================

    # All format target - format everything
    set(ALL_FORMAT_TARGETS)
    if(TARGET fix-format)
        list(APPEND ALL_FORMAT_TARGETS fix-format)
    endif()
    if(TARGET cmake-format)
        list(APPEND ALL_FORMAT_TARGETS cmake-format)
    endif()

    if(ALL_FORMAT_TARGETS)
        add_custom_target(
            format-all
            DEPENDS ${ALL_FORMAT_TARGETS}
            COMMENT "Formatting all code and CMake files")
        message(STATUS "Combined target available: format-all")
    endif()

    # All check target - check everything
    set(ALL_CHECK_TARGETS)
    if(TARGET check-format)
        list(APPEND ALL_CHECK_TARGETS check-format)
    endif()
    if(TARGET check-cmake-format)
        list(APPEND ALL_CHECK_TARGETS check-cmake-format)
    endif()
    if(TARGET clang-tidy-check)
        list(APPEND ALL_CHECK_TARGETS clang-tidy-check)
    endif()
    if(TARGET cppcheck-check)
        list(APPEND ALL_CHECK_TARGETS cppcheck-check)
    endif()

    if(ALL_CHECK_TARGETS)
        add_custom_target(
            check-all
            DEPENDS ${ALL_CHECK_TARGETS}
            COMMENT "Running all code quality checks")
        message(STATUS "Combined target available: check-all")
    endif()

    # ============================================================================= Configuration
    # Summary
    # =============================================================================

    message(STATUS "Code quality tools configuration:")
    message(STATUS "  clang-format: ${CLANG_FORMAT_EXECUTABLE}")
    message(STATUS "  clang-tidy: ${CLANG_TIDY_EXECUTABLE}")
    message(STATUS "  cppcheck: ${CPPCHECK_EXECUTABLE}")
    message(STATUS "  cmake-format: ${CMAKE_FORMAT_EXECUTABLE}")

    if(ENABLE_CLANG_TIDY)
        message(STATUS "  Build-time clang-tidy: ENABLED")
    else()
        message(STATUS "  Build-time clang-tidy: DISABLED (use -DENABLE_CLANG_TIDY=ON)")
    endif()

    if(ENABLE_CPPCHECK)
        message(STATUS "  Build-time cppcheck: ENABLED")
    else()
        message(STATUS "  Build-time cppcheck: DISABLED (use -DENABLE_CPPCHECK=ON)")
    endif()

    # Create suppressions file if it doesn't exist
    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/.cppcheck-suppressions")
        file(
            WRITE "${CMAKE_SOURCE_DIR}/.cppcheck-suppressions"
            "# cppcheck suppressions file\n"
            "# Add suppressions in the format:\n"
            "# suppression-type:file:line\n"
            "# or\n"
            "# suppression-type\n"
            "\n"
            "# Suppress specific warnings that may not be relevant\n"
            "missingIncludeSystem\n"
            "unusedFunction:*/tests/*\n"
            "unmatchedSuppression\n")
    endif()

else()
    message(STATUS "Code quality tools disabled - not top-level project")
endif()
