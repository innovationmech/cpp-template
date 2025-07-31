/**
 * @file main.cpp
 * @brief Main application entry point for the cpp-template project
 * 
 * This file contains the main application logic that demonstrates the
 * capabilities of the cpp-template project including:
 * - Core library initialization and lifecycle management
 * - Modular architecture with inter-module communication
 * - Example usage of different project components
 * - Proper resource management and cleanup
 * 
 * The application serves as both a functional program and a comprehensive
 * example of how to use the various components provided by the template.
 * 
 * @author cpp-template project
 * @date 2024
 * @version 1.0.0
 * @since 1.0.0
 * 
 * @example Running the application:
 * @code
 * $ ./cpp-template
 * Welcome to Project: cpp-template (Version: 1.0.0)
 * Application started successfully!
 * ...
 * @endcode
 */

#include <iostream>
#include "cpp-template/cpp-template.hpp"
#include "modules/example/example_module.hpp"

/**
 * @brief Main application entry point
 * 
 * The main function serves as the entry point for the cpp-template application.
 * It demonstrates the complete lifecycle of a typical application built with
 * this template, including:
 * 
 * 1. **Initialization Phase:**
 *    - Core library initialization
 *    - System validation and setup
 *    - Resource allocation
 * 
 * 2. **Execution Phase:**
 *    - Module instantiation and configuration
 *    - Inter-module data processing demonstration
 *    - Statistics collection and reporting
 * 
 * 3. **Cleanup Phase:**
 *    - Resource deallocation
 *    - Proper shutdown procedures
 * 
 * The application showcases:
 * - Modern C++ practices and patterns
 * - Modular architecture design
 * - Error handling and validation
 * - Resource management (RAII)
 * - Comprehensive logging and user feedback
 * 
 * @return int Application exit code:
 *         - 0: Successful execution
 *         - 1: Core library initialization failure
 *         - Other: Unexpected error conditions
 * 
 * @note This function demonstrates best practices for application structure
 *       and can serve as a template for building larger applications.
 * 
 * @warning Ensure all required dependencies are properly installed before
 *          running the application.
 * 
 * @see cpp_template::core::Core for core library documentation
 * @see cpp_template::modules::ExampleModule for module documentation
 */
int main() {
    // Display welcome message with project information
    std::cout << "Welcome to " << cpp_template::getProjectInfo() << std::endl;
    
    // Initialize core library - this is the foundation for all other operations
    cpp_template::core::Core core;
    // Attempt core library initialization with error handling
    if (!core.initialize()) {
        std::cerr << "Failed to initialize core library" << std::endl;
        return 1;  // Exit with error code if initialization fails
    }
    
    // Confirm successful initialization and display feature overview
    std::cout << "Application started successfully!" << std::endl;
    std::cout << "This is a modern C++ project template demonstrating:" << std::endl;
    std::cout << "- Modular architecture" << std::endl;
    std::cout << "- CMake build system" << std::endl;
    std::cout << "- Multiple dependency management approaches" << std::endl;
    std::cout << "- Modern C++ practices" << std::endl;
    
    // === MODULAR ARCHITECTURE DEMONSTRATION ===
    // This section demonstrates how modules can be instantiated and used
    // to create a processing pipeline with inter-module communication
    std::cout << "\n=== Demonstrating Modular Architecture ===" << std::endl;
    
    // Create specialized module instances for different processing tasks
    // Each module maintains its own state and processing statistics
    cpp_template::modules::ExampleModule dataProcessor("DataProcessor");
    cpp_template::modules::ExampleModule textAnalyzer("TextAnalyzer");
    
    // === DATA PROCESSING PIPELINE ===
    // Demonstrate a multi-stage processing pipeline where data flows
    // through multiple modules, each adding its own transformation
    std::string testData = "Hello World";
    std::cout << "\nProcessing data: \"" << testData << "\"" << std::endl;
    
    // Stage 1: Initial data processing
    // The first module processes the raw input data
    std::string processedData = dataProcessor.processData(testData);
    std::cout << "Result from " << dataProcessor.getName() << ": " << processedData << std::endl;
    
    // Stage 2: Secondary processing
    // The second module processes the output from the first module
    std::string finalResult = textAnalyzer.processData(processedData);
    std::cout << "Final result from " << textAnalyzer.getName() << ": " << finalResult << std::endl;
    
    // === STATISTICS AND MONITORING ===
    // Display comprehensive statistics from each module to demonstrate
    // monitoring capabilities and module state tracking
    std::cout << "\n=== Module Statistics ===" << std::endl;
    
    // Collect and display statistics from the first module
    auto stats1 = dataProcessor.getStatistics();
    std::cout << "Statistics for " << dataProcessor.getName() << ":" << std::endl;
    for (const auto& stat : stats1) {
        std::cout << "  " << stat << std::endl;
    }
    
    // Collect and display statistics from the second module
    auto stats2 = textAnalyzer.getStatistics();
    std::cout << "Statistics for " << textAnalyzer.getName() << ":" << std::endl;
    for (const auto& stat : stats2) {
        std::cout << "  " << stat << std::endl;
    }
    
    std::cout << "\n=== Module Communication Complete ===" << std::endl;
    
    // === CLEANUP AND SHUTDOWN ===
    // Properly cleanup all resources and shutdown the core library
    // This demonstrates proper resource management and RAII principles
    core.cleanup();
    
    // Final success message
    std::cout << "Application finished successfully!" << std::endl;
    return 0;  // Return success exit code
}