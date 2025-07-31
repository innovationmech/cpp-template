#pragma once

/**
 * @file cpp-template.hpp
 * @brief Main header file for the cpp-template project
 * 
 * This header provides convenient access to all public interfaces
 * of the cpp-template project.
 */

// Core library
#include "core/core.hpp"

/**
 * @namespace cpp_template
 * @brief Main namespace for the cpp-template project
 */
namespace cpp_template {

/**
 * @brief Get the full project information
 * @return String containing project name and version
 */
inline std::string getProjectInfo() {
    return core::Core::getProjectName() + " v" + core::Core::getVersion();
}

} // namespace cpp_template