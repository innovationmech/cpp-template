#pragma once

/**
 * @file cpp-template.hpp
 * @brief Main header file for the cpp-template project
 * 
 * This header provides convenient access to all public interfaces
 * of the cpp-template project, following proper namespace organization
 * and header guard conventions. It serves as the primary entry point
 * for applications using the cpp-template library.
 * 
 * Key features provided by this header:
 * - Unified access to all core functionality
 * - Convenient wrapper functions for common operations
 * - Project-wide initialization and cleanup utilities
 * - Consistent namespace organization
 * - Modern C++ interface design
 * 
 * This header is designed to be the only include needed for most
 * applications using the cpp-template library, providing a clean
 * and simple API surface.
 * 
 * @author cpp-template project
 * @date 2024
 * @version 1.0.0
 * @since 1.0.0
 * 
 * @example Basic usage:
 * @code
 * #include "cpp-template/cpp-template.hpp"
 * 
 * int main() {
 *     if (cpp_template::initialize()) {
 *         std::cout << cpp_template::getProjectInfo() << std::endl;
 *         cpp_template::cleanup();
 *         return 0;
 *     }
 *     return 1;
 * }
 * @endcode
 */

// Core library public interface
#include "cpp-template/core/core.hpp"

/**
 * @namespace cpp_template
 * @brief Main namespace for the cpp-template project
 * 
 * This namespace contains all public interfaces and utilities
 * provided by the cpp-template project. It serves as the primary
 * API surface for applications and provides:
 * 
 * - **Core Functionality**: Access to fundamental library features
 * - **Utility Functions**: Helper functions for common operations
 * - **Project Management**: Initialization, cleanup, and lifecycle management
 * - **Information Services**: Project metadata and version information
 * - **Configuration**: Validation and setup utilities
 * 
 * The namespace is designed to provide a clean, intuitive API that
 * follows modern C++ best practices and conventions. All functions
 * are designed to be safe, efficient, and easy to use.
 * 
 * @note All functions in this namespace are thread-safe unless
 *       explicitly documented otherwise.
 * 
 * @see cpp_template::core for lower-level core functionality
 * @since 1.0.0
 */
namespace cpp_template {

/**
 * @brief Retrieves concise project information
 * 
 * Returns a compact string containing the project name and version
 * in a simple "name vversion" format. This function is ideal for
 * quick identification and logging purposes.
 * 
 * @return std::string Project information in format "cpp-template v1.0.0"
 * 
 * @note This function is thread-safe and has no side effects.
 *       It combines the project name and version with a simple format.
 * 
 * @thread_safety This function is thread-safe.
 * 
 * @example
 * @code
 * std::string info = cpp_template::getProjectInfo();
 * std::cout << "Running: " << info << std::endl;
 * // Output: "Running: cpp-template v1.0.0"
 * @endcode
 * 
 * @see getFormattedProjectInfo() for more detailed formatting
 * @see cpp_template::core::Core::getProjectName()
 * @see cpp_template::core::Core::getVersion()
 */
inline std::string getProjectInfo() {
    return core::Core::getProjectName() + " v" + core::Core::getVersion();
}

/**
 * @brief Retrieves detailed formatted project information
 * 
 * Returns a comprehensive, human-readable string containing project
 * information in a standardized format. This function provides more
 * detailed formatting than getProjectInfo() and is suitable for
 * user interfaces, reports, and detailed logging.
 * 
 * @return std::string Detailed project information in format:
 *         "Project: cpp-template (Version: 1.0.0)"
 * 
 * @note This function delegates to the core utilities for consistent
 *       formatting across the entire project. It's thread-safe and
 *       has no side effects.
 * 
 * @thread_safety This function is thread-safe.
 * 
 * @example
 * @code
 * std::string info = cpp_template::getFormattedProjectInfo();
 * std::cout << info << std::endl;
 * // Output: "Project: cpp-template (Version: 1.0.0)"
 * @endcode
 * 
 * @see getProjectInfo() for simpler formatting
 * @see cpp_template::core::utils::getFormattedProjectInfo()
 */
inline std::string getFormattedProjectInfo() {
    return core::utils::getFormattedProjectInfo();
}

/**
 * @brief Initializes the entire cpp-template project
 * 
 * Performs comprehensive initialization of the cpp-template project,
 * including configuration validation and core library setup. This
 * function should be called once at the beginning of your application
 * before using any other project functionality.
 * 
 * The initialization process includes:
 * 1. **Configuration Validation**: Ensures all project settings are valid
 * 2. **Core Library Setup**: Initializes the fundamental library components
 * 3. **Resource Allocation**: Sets up necessary internal resources
 * 4. **System Validation**: Verifies system compatibility and requirements
 * 
 * @return bool true if initialization completed successfully,
 *              false if any step failed
 * 
 * @note This function uses a static core instance to ensure consistent
 *       state across the application. It's safe to call multiple times
 *       (idempotent behavior).
 * 
 * @warning This function is NOT thread-safe for the initial call.
 *          Ensure it's called from a single thread during application startup.
 * 
 * @post If this function returns true, all project functionality is
 *       available for use. If it returns false, no project functions
 *       should be called except cleanup().
 * 
 * @example
 * @code
 * if (!cpp_template::initialize()) {
 *     std::cerr << "Failed to initialize cpp-template" << std::endl;
 *     return 1;
 * }
 * // Project is now ready for use
 * @endcode
 * 
 * @see cleanup() for proper shutdown
 * @see cpp_template::core::utils::validateConfiguration()
 * @see cpp_template::core::Core::initialize()
 */
inline bool initialize() {
    // Step 1: Validate configuration first
    if (!core::utils::validateConfiguration()) {
        return false;
    }
    
    // Step 2: Initialize core library with static instance
    static core::Core coreInstance;
    return coreInstance.initialize();
}

/**
 * @brief Cleans up all project resources
 * 
 * Performs comprehensive cleanup of all resources allocated by the
 * cpp-template project. This function should be called when the
 * project functionality is no longer needed, typically at application
 * shutdown.
 * 
 * The cleanup process includes:
 * - **Resource Deallocation**: Frees all allocated memory and handles
 * - **State Reset**: Resets all internal state to initial conditions
 * - **System Cleanup**: Performs any necessary system-level cleanup
 * - **Graceful Shutdown**: Ensures all operations complete properly
 * 
 * @note This function is idempotent and safe to call multiple times.
 *       It uses the same static core instance as initialize() to ensure
 *       consistent state management.
 * 
 * @warning This function is NOT thread-safe. Ensure no other threads
 *          are using project functionality when cleanup() is called.
 * 
 * @post After calling this function, initialize() must be called again
 *       before using any other project functionality.
 * 
 * @example
 * @code
 * // At application shutdown
 * cpp_template::cleanup();
 * std::cout << "Project cleanup complete" << std::endl;
 * @endcode
 * 
 * @see initialize() for project setup
 * @see cpp_template::core::Core::cleanup()
 */
inline void cleanup() {
    // Use the same static instance as initialize() for consistency
    static core::Core coreInstance;
    coreInstance.cleanup();
}

} // namespace cpp_template