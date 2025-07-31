#pragma once

/**
 * @file cpp-template.hpp
 * @brief Main header file for the cpp-template project
 * 
 * This header provides convenient access to all public interfaces
 * of the cpp-template project, following proper namespace organization
 * and header guard conventions.
 */

// Core library public interface
#include "cpp-template/core/core.hpp"

/**
 * @namespace cpp_template
 * @brief Main namespace for the cpp-template project
 * 
 * This namespace contains all public interfaces and utilities
 * provided by the cpp-template project.
 */
namespace cpp_template {

/**
 * @brief Get the full project information
 * @return String containing project name and version
 */
inline std::string getProjectInfo() {
    return core::Core::getProjectName() + " v" + core::Core::getVersion();
}

/**
 * @brief Get formatted project information using core utilities
 * @return Formatted string with detailed project information
 */
inline std::string getFormattedProjectInfo() {
    return core::utils::getFormattedProjectInfo();
}

/**
 * @brief Initialize the entire project
 * @return true if initialization successful, false otherwise
 */
inline bool initialize() {
    // Validate configuration first
    if (!core::utils::validateConfiguration()) {
        return false;
    }
    
    // Initialize core library
    static core::Core coreInstance;
    return coreInstance.initialize();
}

/**
 * @brief Cleanup project resources
 */
inline void cleanup() {
    static core::Core coreInstance;
    coreInstance.cleanup();
}

} // namespace cpp_template