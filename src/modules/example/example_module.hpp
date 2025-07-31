#pragma once

#include <string>
#include <vector>

namespace cpp_template {
namespace modules {

/**
 * @brief Example module demonstrating modular architecture
 */
class ExampleModule {
public:
    /**
     * @brief Constructor
     * @param name Module name
     */
    explicit ExampleModule(const std::string& name);
    
    /**
     * @brief Get module name
     * @return Module name
     */
    const std::string& getName() const;
    
    /**
     * @brief Process some data
     * @param input Input data
     * @return Processed result
     */
    std::string processData(const std::string& input);
    
    /**
     * @brief Get module statistics
     * @return Vector of statistics
     */
    std::vector<std::string> getStatistics() const;
    
private:
    std::string m_name;
    size_t m_processCount = 0;
};

} // namespace modules
} // namespace cpp_template