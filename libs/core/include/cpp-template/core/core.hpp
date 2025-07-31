#pragma once

#include <string>

namespace cpp_template {
namespace core {

/**
 * @brief Core utility class demonstrating the template structure
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
    
private:
    bool m_initialized = false;
};

} // namespace core
} // namespace cpp_template