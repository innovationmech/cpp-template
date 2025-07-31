#include "example_module.hpp"
#include "cpp-template/core/core.hpp"
#include <iostream>
#include <sstream>

namespace cpp_template {
namespace modules {

ExampleModule::ExampleModule(const std::string& name) : m_name(name) {
    std::cout << "Created module: " << m_name << std::endl;
}

const std::string& ExampleModule::getName() const {
    return m_name;
}

std::string ExampleModule::processData(const std::string& input) {
    ++m_processCount;
    
    std::cout << "Module '" << m_name << "' processing: " << input << std::endl;
    
    // Simple processing: reverse the string and add module info
    std::string reversed(input.rbegin(), input.rend());
    
    std::ostringstream result;
    result << "[" << m_name << "] Processed: " << reversed;
    
    return result.str();
}

std::vector<std::string> ExampleModule::getStatistics() const {
    std::vector<std::string> stats;
    
    stats.push_back("Module Name: " + m_name);
    stats.push_back("Process Count: " + std::to_string(m_processCount));
    stats.push_back("Core Version: " + core::Core::getVersion());
    
    return stats;
}

} // namespace modules
} // namespace cpp_template