#pragma once

#include <string>
#include <vector>

namespace cpp_template {
namespace modules {

/**
 * @brief Example module demonstrating modular architecture and inter-module communication
 * 
 * This class serves as a template for creating modular components within the cpp-template
 * project. It demonstrates best practices for:
 * - Module initialization and lifecycle management
 * - Data processing with statistics tracking
 * - Inter-module communication patterns
 * - Resource management and cleanup
 * 
 * The module can be used as a foundation for creating specialized processors like
 * DataProcessor, TextAnalyzer, or any other domain-specific modules.
 * 
 * @note This class is thread-safe for read operations but requires external
 *       synchronization for write operations (processData calls).
 * 
 * @example
 * @code
 * // Create a specialized data processor module
 * ExampleModule dataProcessor("DataProcessor");
 * 
 * // Process some input data
 * std::string input = "Hello, World!";
 * std::string result = dataProcessor.processData(input);
 * 
 * // Get processing statistics
 * auto stats = dataProcessor.getStatistics();
 * for (const auto& stat : stats) {
 *     std::cout << stat << std::endl;
 * }
 * @endcode
 * 
 * @see cpp_template::core::Core
 * @since 1.0.0
 */
class ExampleModule {
public:
    /**
     * @brief Constructs a new ExampleModule with the specified name
     * 
     * Initializes the module with a given name and sets up internal state.
     * The module name is used for identification and logging purposes.
     * 
     * @param name The unique identifier for this module instance.
     *             Should be descriptive and follow naming conventions.
     *             Examples: "DataProcessor", "TextAnalyzer", "ImageProcessor"
     * 
     * @throws std::invalid_argument if name is empty
     * 
     * @note The constructor outputs a creation message to stdout for debugging purposes.
     *       In production code, consider using a proper logging framework.
     * 
     * @example
     * @code
     * ExampleModule processor("DataProcessor");  // Creates module with name "DataProcessor"
     * @endcode
     */
    explicit ExampleModule(const std::string& name);
    
    /**
     * @brief Gets the module's unique identifier name
     * 
     * Returns the name that was assigned to this module during construction.
     * This name is used for identification, logging, and statistics reporting.
     * 
     * @return const std::string& Reference to the module's name
     * 
     * @note This method is thread-safe and has no side effects.
     * 
     * @example
     * @code
     * ExampleModule module("TextAnalyzer");
     * std::cout << "Module name: " << module.getName() << std::endl;  // Outputs: "TextAnalyzer"
     * @endcode
     */
    const std::string& getName() const;
    
    /**
     * @brief Processes input data and returns the transformed result
     * 
     * This method performs the core functionality of the module by processing
     * the provided input data. The current implementation:
     * 1. Increments the internal process counter for statistics
     * 2. Logs the processing operation to stdout
     * 3. Reverses the input string as a demonstration
     * 4. Wraps the result with module identification
     * 
     * @param input The data to be processed. Can be any valid string.
     *              Empty strings are handled gracefully.
     * 
     * @return std::string The processed result in the format:
     *         "[ModuleName] Processed: <reversed_input>"
     * 
     * @note This method is NOT thread-safe due to the increment of m_processCount.
     *       Use external synchronization if calling from multiple threads.
     * 
     * @warning The current implementation reverses the string for demonstration.
     *          In real modules, implement domain-specific processing logic.
     * 
     * @example
     * @code
     * ExampleModule processor("DataProcessor");
     * std::string result = processor.processData("Hello");
     * // result will be: "[DataProcessor] Processed: olleH"
     * @endcode
     */
    std::string processData(const std::string& input);
    
    /**
     * @brief Retrieves comprehensive statistics about the module's operation
     * 
     * Returns a vector of formatted strings containing various statistics
     * about the module's current state and processing history. The statistics include:
     * - Module name for identification
     * - Total number of processData() calls made
     * - Core library version information
     * 
     * @return std::vector<std::string> A collection of formatted statistic strings.
     *         Each string represents one statistic in human-readable format.
     * 
     * @note This method is thread-safe for reading current statistics.
     *       However, statistics may change between calls if processData()
     *       is called concurrently.
     * 
     * @example
     * @code
     * ExampleModule module("TextAnalyzer");
     * module.processData("test1");
     * module.processData("test2");
     * 
     * auto stats = module.getStatistics();
     * for (const auto& stat : stats) {
     *     std::cout << stat << std::endl;
     * }
     * // Output:
     * // Module Name: TextAnalyzer
     * // Process Count: 2
     * // Core Version: 1.0.0
     * @endcode
     */
    std::vector<std::string> getStatistics() const;
    
private:
    /**
     * @brief The unique identifier name for this module instance
     * 
     * Set during construction and remains constant throughout the module's lifetime.
     * Used for identification, logging, and statistics reporting.
     */
    std::string m_name;
    
    /**
     * @brief Counter tracking the number of processData() calls
     * 
     * Incremented each time processData() is called, providing usage statistics.
     * Initialized to 0 and grows monotonically during the module's lifetime.
     * 
     * @note This member is not thread-safe and requires external synchronization
     *       if the module is used from multiple threads.
     */
    size_t m_processCount = 0;
};

} // namespace modules
} // namespace cpp_template