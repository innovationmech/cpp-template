// Main application entry point
// Demonstrates usage of the cpp-template library

#include <iostream>
#include <cpp-template/cpp-template.h>
#include "modules/config_manager.h"
#include "modules/data_processor.h"

int main() {
    std::cout << "=== " << cpp_template::info::getName() 
              << " v" << cpp_template::info::getVersion() << " ===" << std::endl;
    std::cout << cpp_template::info::getDescription() << std::endl << std::endl;
    
    try {
        // Create and initialize a core instance
        auto core = cpp_template::createCore("MainCore");
        
        std::cout << "Created core instance: " << core->getName() << std::endl;
        
        // Initialize the core
        if (core->initialize()) {
            std::cout << "Core initialized successfully" << std::endl;
        } else {
            std::cout << "Core initialization failed" << std::endl;
            return 1;
        }
        
        // Demonstrate string processing
        std::string input = "hello world";
        std::string processed = core->process(input);
        std::cout << "Processed '" << input << "' -> '" << processed << "'" << std::endl;
        
        // Demonstrate utility functions
        std::cout << "\n=== String Utilities Demo ===" << std::endl;
        std::string test_string = "Hello World";
        std::cout << "Original: " << test_string << std::endl;
        std::cout << "Uppercase: " << cpp_template::string_utils::toUpper(test_string) << std::endl;
        std::cout << "Lowercase: " << cpp_template::string_utils::toLower(test_string) << std::endl;
        
        // Demonstrate split and join
        auto words = cpp_template::string_utils::split(test_string, ' ');
        std::cout << "Split words: ";
        for (const auto& word : words) {
            std::cout << "[" << word << "] ";
        }
        std::cout << std::endl;
        
        std::string joined = cpp_template::string_utils::join(words, "-");
        std::cout << "Joined with '-': " << joined << std::endl;
        
        // Demonstrate validation
        std::cout << "\n=== Validation Demo ===" << std::endl;
        std::string empty_str = "   ";
        std::string alphanumeric_str = "Hello123";
        std::string email = "test@example.com";
        
        std::cout << "'" << empty_str << "' is empty: " 
                  << (cpp_template::validation::isEmpty(empty_str) ? "true" : "false") << std::endl;
        std::cout << "'" << alphanumeric_str << "' is alphanumeric: " 
                  << (cpp_template::validation::isAlphanumeric(alphanumeric_str) ? "true" : "false") << std::endl;
        std::cout << "'" << email << "' is valid email: " 
                  << (cpp_template::validation::isValidEmail(email) ? "true" : "false") << std::endl;
        
        // Demonstrate modular architecture with modules
        std::cout << "\n=== Modular Architecture Demo ===" << std::endl;
        
        // Create configuration manager
        auto config_manager_unique = cpp_template::modules::createConfigManager();
        std::shared_ptr<cpp_template::modules::ConfigManager> config_manager(config_manager_unique.release());
        std::cout << "Created configuration manager" << std::endl;
        
        // Set some configuration values
        config_manager->setValue("processing.mode", "advanced");
        config_manager->setValue("processing.batch_size", "5");
        std::cout << "Set configuration values" << std::endl;
        
        // Create data processor (demonstrates inter-module dependency)
        auto data_processor = cpp_template::modules::createDataProcessor(config_manager);
        std::cout << "Created data processor with config dependency" << std::endl;
        
        // Process single item
        auto result1 = data_processor->processItem("Hello Module", cpp_template::modules::ProcessingMode::SIMPLE);
        if (result1.success) {
            std::cout << "Single item processing: " << result1.result << std::endl;
        }
        
        // Process batch
        std::vector<std::string> batch_data = {"item1", "item2", "item3"};
        auto result2 = data_processor->processBatch(batch_data, cpp_template::modules::ProcessingMode::BATCH);
        if (result2.success) {
            std::cout << "Batch processing: " << result2.result << std::endl;
            std::cout << "Processed " << result2.processed_items << " items" << std::endl;
        }
        
        // Show processing statistics
        std::cout << "\n" << data_processor->getStatistics() << std::endl;
        
        // Demonstrate configuration access
        std::cout << "\n=== Configuration Demo ===" << std::endl;
        auto keys = config_manager->getAllKeys();
        std::cout << "Configuration keys (" << keys.size() << " total):" << std::endl;
        for (const auto& key : keys) {
            std::cout << "  " << key << " = " << config_manager->getValue(key) << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "\nApplication completed successfully!" << std::endl;
    return 0;
}