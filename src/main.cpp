#include <iostream>
#include "cpp-template/cpp-template.hpp"
#include "modules/example/example_module.hpp"

int main() {
    std::cout << "Welcome to " << cpp_template::getProjectInfo() << std::endl;
    
    // Initialize core library
    cpp_template::core::Core core;
    if (!core.initialize()) {
        std::cerr << "Failed to initialize core library" << std::endl;
        return 1;
    }
    
    std::cout << "Application started successfully!" << std::endl;
    std::cout << "This is a modern C++ project template demonstrating:" << std::endl;
    std::cout << "- Modular architecture" << std::endl;
    std::cout << "- CMake build system" << std::endl;
    std::cout << "- Multiple dependency management approaches" << std::endl;
    std::cout << "- Modern C++ practices" << std::endl;
    
    // Demonstrate modular architecture with example module
    std::cout << "\n=== Demonstrating Modular Architecture ===" << std::endl;
    
    // Create example modules to show inter-module communication
    cpp_template::modules::ExampleModule dataProcessor("DataProcessor");
    cpp_template::modules::ExampleModule textAnalyzer("TextAnalyzer");
    
    // Process some data through modules
    std::string testData = "Hello World";
    std::cout << "\nProcessing data: \"" << testData << "\"" << std::endl;
    
    // First module processes the data
    std::string processedData = dataProcessor.processData(testData);
    std::cout << "Result from " << dataProcessor.getName() << ": " << processedData << std::endl;
    
    // Second module processes the result from first module
    std::string finalResult = textAnalyzer.processData(processedData);
    std::cout << "Final result from " << textAnalyzer.getName() << ": " << finalResult << std::endl;
    
    // Show module statistics
    std::cout << "\n=== Module Statistics ===" << std::endl;
    auto stats1 = dataProcessor.getStatistics();
    std::cout << "Statistics for " << dataProcessor.getName() << ":" << std::endl;
    for (const auto& stat : stats1) {
        std::cout << "  " << stat << std::endl;
    }
    
    auto stats2 = textAnalyzer.getStatistics();
    std::cout << "Statistics for " << textAnalyzer.getName() << ":" << std::endl;
    for (const auto& stat : stats2) {
        std::cout << "  " << stat << std::endl;
    }
    
    std::cout << "\n=== Module Communication Complete ===" << std::endl;
    
    // Cleanup
    core.cleanup();
    
    std::cout << "Application finished successfully!" << std::endl;
    return 0;
}