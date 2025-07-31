#pragma once

/**
 * @file core.hpp
 * @brief Public interface for the core library
 * 
 * This header exposes the public interface of the core library following
 * proper namespace conventions and header guard practices. It provides
 * the foundational functionality for the cpp-template project including:
 * - Project information and versioning
 * - Core library lifecycle management
 * - Utility functions for common operations
 * - Configuration validation
 * 
 * The core library serves as the foundation for all other modules and
 * provides essential services that other components depend on.
 * 
 * @author cpp-template project
 * @date 2024
 * @version 1.0.0
 * @since 1.0.0
 * 
 * @example Basic usage:
 * @code
 * #include "cpp-template/core/core.hpp"
 * 
 * int main() {
 *     cpp_template::core::Core core;
 *     if (core.initialize()) {
 *         std::cout << "Project: " << cpp_template::core::Core::getProjectName() << std::endl;
 *         std::cout << "Version: " << cpp_template::core::Core::getVersion() << std::endl;
 *         core.cleanup();
 *     }
 *     return 0;
 * }
 * @endcode
 */

#include <string>

namespace cpp_template {
namespace core {

/**
 * @brief Core utility class providing foundational functionality
 * 
 * This class serves as the central component of the cpp-template project,
 * providing essential services including:
 * - Project metadata and version information
 * - Library initialization and cleanup
 * - State management and validation
 * - Integration point for other modules
 * 
 * The Core class follows the RAII (Resource Acquisition Is Initialization)
 * pattern and provides both static utility methods and instance-based
 * lifecycle management.
 * 
 * @note This class is designed to be lightweight and can be instantiated
 *       multiple times, though typically only one instance is needed per
 *       application.
 * 
 * @warning Always call initialize() before using other instance methods
 *          and cleanup() when done to ensure proper resource management.
 * 
 * @thread_safety This class is thread-safe for read operations (static methods)
 *                but requires external synchronization for write operations
 *                (initialize/cleanup calls).
 * 
 * @see cpp_template::core::utils for additional utility functions
 * @since 1.0.0
 */
class Core {
public:
    /**
     * @brief Retrieves the project's canonical name
     * 
     * Returns the official name of the cpp-template project. This name
     * is used for identification, logging, and display purposes throughout
     * the application.
     * 
     * @return std::string The project name ("cpp-template")
     * 
     * @note This is a static method and can be called without instantiating
     *       the Core class. The returned value is constant and will not
     *       change during runtime.
     * 
     * @thread_safety This method is thread-safe and has no side effects.
     * 
     * @example
     * @code
     * std::string name = cpp_template::core::Core::getProjectName();
     * std::cout << "Running " << name << std::endl;
     * @endcode
     */
    static std::string getProjectName();
    
    /**
     * @brief Retrieves the current project version
     * 
     * Returns the semantic version string of the cpp-template project.
     * The version follows semantic versioning (SemVer) format: MAJOR.MINOR.PATCH
     * 
     * @return std::string The current version (e.g., "1.0.0")
     * 
     * @note This is a static method that returns the compile-time version.
     *       The version is embedded during build and reflects the state
     *       of the codebase at compilation time.
     * 
     * @thread_safety This method is thread-safe and has no side effects.
     * 
     * @see https://semver.org/ for semantic versioning specification
     * 
     * @example
     * @code
     * std::string version = cpp_template::core::Core::getVersion();
     * std::cout << "Version: " << version << std::endl;
     * @endcode
     */
    static std::string getVersion();
    
    /**
     * @brief Initializes the core library instance
     * 
     * Performs necessary initialization steps to prepare the core library
     * for operation. This method should be called before using any other
     * instance methods. The initialization process includes:
     * - Setting up internal state
     * - Validating system requirements
     * - Preparing logging and output systems
     * - Marking the instance as ready for use
     * 
     * @return bool true if initialization completed successfully,
     *              false if an error occurred during initialization
     * 
     * @note This method is idempotent - calling it multiple times on the
     *       same instance will not cause errors, but subsequent calls will
     *       have no effect and will return true.
     * 
     * @warning This method is NOT thread-safe. Ensure that only one thread
     *          calls initialize() on a given instance.
     * 
     * @example
     * @code
     * Core core;
     * if (!core.initialize()) {
     *     std::cerr << "Failed to initialize core library" << std::endl;
     *     return 1;
     * }
     * // Core is now ready for use
     * @endcode
     */
    bool initialize();
    
    /**
     * @brief Cleans up resources and shuts down the core library
     * 
     * Performs cleanup operations to properly shut down the core library
     * instance. This method should be called when the core library is no
     * longer needed. The cleanup process includes:
     * - Releasing allocated resources
     * - Flushing any pending operations
     * - Resetting internal state
     * - Marking the instance as uninitialized
     * 
     * @note This method is idempotent - calling it multiple times will not
     *       cause errors. If the instance is not initialized, this method
     *       will have no effect.
     * 
     * @warning This method is NOT thread-safe. Ensure that no other threads
     *          are using the core instance when cleanup() is called.
     * 
     * @post After calling this method, isInitialized() will return false
     *       and initialize() must be called again before using other methods.
     * 
     * @example
     * @code
     * Core core;
     * core.initialize();
     * // ... use core ...
     * core.cleanup();  // Proper cleanup
     * @endcode
     */
    void cleanup();
    
    /**
     * @brief Checks the initialization status of the core library
     * 
     * Returns the current initialization state of this core library instance.
     * This method can be used to verify that initialize() has been called
     * successfully before attempting to use other functionality.
     * 
     * @return bool true if the core library is initialized and ready for use,
     *              false if initialize() has not been called or cleanup()
     *              has been called since the last initialization
     * 
     * @note This method is thread-safe and has no side effects. It simply
     *       reads the current state without modifying anything.
     * 
     * @thread_safety This method is thread-safe for concurrent reads.
     * 
     * @example
     * @code
     * Core core;
     * assert(!core.isInitialized());  // Not initialized yet
     * 
     * core.initialize();
     * assert(core.isInitialized());   // Now initialized
     * 
     * core.cleanup();
     * assert(!core.isInitialized());  // No longer initialized
     * @endcode
     */
    bool isInitialized() const;

private:
    /**
     * @brief Internal flag tracking the initialization state
     * 
     * This member variable tracks whether the core library instance
     * has been successfully initialized. It is set to true by initialize()
     * and reset to false by cleanup().
     * 
     * @note This member is not thread-safe and should only be accessed
     *       through the public interface methods.
     */
    bool m_initialized = false;
};

/**
 * @brief Utility functions for core library operations
 * 
 * This namespace contains helper functions that provide additional
 * functionality related to the core library. These functions are
 * designed to be stateless and can be used independently of the
 * Core class instances.
 * 
 * The utilities include:
 * - Project information formatting
 * - Configuration validation
 * - Common operations and helpers
 * 
 * @note All functions in this namespace are thread-safe unless
 *       explicitly documented otherwise.
 * 
 * @since 1.0.0
 */
namespace utils {
    /**
     * @brief Retrieves formatted project information string
     * 
     * Creates a human-readable string containing both the project name
     * and version in a standardized format. This is useful for display
     * purposes, logging, and user interfaces.
     * 
     * @return std::string Formatted string in the format:
     *         "Project: <name> (Version: <version>)"
     *         Example: "Project: cpp-template (Version: 1.0.0)"
     * 
     * @note This function combines the results of Core::getProjectName()
     *       and Core::getVersion() into a single formatted string.
     * 
     * @thread_safety This function is thread-safe and has no side effects.
     * 
     * @example
     * @code
     * std::string info = cpp_template::core::utils::getFormattedProjectInfo();
     * std::cout << info << std::endl;
     * // Output: "Project: cpp-template (Version: 1.0.0)"
     * @endcode
     */
    std::string getFormattedProjectInfo();
    
    /**
     * @brief Validates the current project configuration
     * 
     * Performs comprehensive validation of the project's configuration
     * to ensure all required components are properly set up. The validation
     * includes:
     * - Checking that project name is not empty
     * - Verifying that version information is available
     * - Validating internal consistency
     * - Ensuring all required dependencies are accessible
     * 
     * @return bool true if all validation checks pass and the configuration
     *              is valid, false if any validation check fails
     * 
     * @note This function outputs validation messages to stdout/stderr
     *       for debugging purposes. In production environments, consider
     *       using a proper logging framework.
     * 
     * @thread_safety This function is thread-safe and can be called
     *                concurrently from multiple threads.
     * 
     * @example
     * @code
     * if (!cpp_template::core::utils::validateConfiguration()) {
     *     std::cerr << "Configuration validation failed!" << std::endl;
     *     return 1;
     * }
     * std::cout << "Configuration is valid" << std::endl;
     * @endcode
     */
    bool validateConfiguration();
}

} // namespace core
} // namespace cpp_template