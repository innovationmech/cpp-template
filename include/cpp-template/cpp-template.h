#pragma once

/**
 * @file cpp-template.h
 * @brief Main header for the cpp-template library
 *
 * This is the main header file that includes all public interfaces
 * of the cpp-template library. Include this file to access all
 * functionality provided by the library.
 *
 * @version 1.0.0
 * @author cpp-template project
 */

// Core functionality
#include "core.h"

// Utility functions
#include "utils.h"

/**
 * @brief Main namespace for the cpp-template library
 *
 * All public APIs of the cpp-template library are contained within
 * this namespace to avoid naming conflicts with other libraries.
 */
namespace cpp_template {

/**
 * @brief Library information and metadata
 */
namespace info {

/**
 * @brief Get the library name
 *
 * @return const char* The library name
 */
constexpr const char* getName() {
    return "cpp-template";
}

/**
 * @brief Get the library version
 *
 * @return const char* The version string
 */
constexpr const char* getVersion() {
    return "1.0.0";
}

/**
 * @brief Get the library description
 *
 * @return const char* The description string
 */
constexpr const char* getDescription() {
    return "A comprehensive C++ project template with modern practices";
}

}  // namespace info

}  // namespace cpp_template
