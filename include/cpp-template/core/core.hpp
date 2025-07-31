#pragma once

/**
 * @file core.hpp
 * @brief Public interface for the core library
 * 
 * This header exposes the public interface of the core library
 * following proper namespace conventions and header guard practices.
 */

#include <string>

namespace cpp_template {
namespace core {

/**
 * @brief Core utility class demonstrating the template structure
 * 
 * This class provides basic functionality for the cpp-template project,
 * including project information and lifecycle management.
 */
class Core {
public:
    /**
     * @brief Get the project name
     * @return Project name as string
     */
    static std::string getProjectName();
    
    /**
     * @brief Get the project version
     * @return Project version as string
     */
    static std::string getVersion();
    
    /**
     * @brief Initialize the core library
     * @return true if initialization successful, false otherwise
     */
    bool initialize();
    
    /**
     * @brief Cleanup resources
     */
    void cleanup();
    
    /**
     * @brief Check if the core library is initialized
     * @return true if initialized, false otherwise
     */
    bool isInitialized() const;

private:
    bool m_initialized = false;
};

/**
 * @brief Utility functions for core library operations
 */
namespace utils {
    /**
     * @brief Get formatted project information
     * @return Formatted string with project name and version
     */
    std::string getFormattedProjectInfo();
    
    /**
     * @brief Validate project configuration
     * @return true if configuration is valid, false otherwise
     */
    bool validateConfiguration();
}

} // namespace core
} // namespace cpp_template