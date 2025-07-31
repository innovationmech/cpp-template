/**
 * @file example_module.cpp
 * @brief Implementation of the ExampleModule class
 * 
 * This file contains the implementation of all ExampleModule methods,
 * demonstrating modular architecture patterns and best practices for
 * C++ module development within the cpp-template project.
 * 
 * @author cpp-template project
 * @date 2024
 * @version 1.0.0
 */

#include "example_module.hpp"
#include "cpp-template/core/core.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace cpp_template {
namespace modules {

ExampleModule::ExampleModule(const std::string& name) : m_name(name) {
    // Validate input parameter
    if (name.empty()) {
        throw std::invalid_argument("Module name cannot be empty");
    }
    
    // Log module creation for debugging purposes
    // In production code, consider using a proper logging framework
    std::cout << "Created module: " << m_name << std::endl;
}

const std::string& ExampleModule::getName() const {
    // Simple getter with no side effects - thread-safe
    return m_name;
}

std::string ExampleModule::processData(const std::string& input) {
    // Increment process counter for statistics tracking
    // Note: This operation is NOT thread-safe
    ++m_processCount;
    
    // Log processing operation for debugging
    std::cout << "Module '" << m_name << "' processing: " << input << std::endl;
    
    // Demonstrate data processing by reversing the input string
    // In real implementations, this would contain domain-specific logic
    std::string reversed(input.rbegin(), input.rend());
    
    // Format the result with module identification
    std::ostringstream result;
    result << "[" << m_name << "] Processed: " << reversed;
    
    return result.str();
}

std::vector<std::string> ExampleModule::getStatistics() const {
    // Create statistics collection
    std::vector<std::string> stats;
    
    // Add module identification
    stats.push_back("Module Name: " + m_name);
    
    // Add processing statistics
    stats.push_back("Process Count: " + std::to_string(m_processCount));
    
    // Add core library version information
    stats.push_back("Core Version: " + core::Core::getVersion());
    
    return stats;
}

} // namespace modules
} // namespace cpp_template