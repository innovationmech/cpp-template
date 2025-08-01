/**
 * @file modular_architecture_demo.cpp
 * @brief Comprehensive demonstration of modular architecture
 * 
 * This example shows how to use the modular components of the cpp-template
 * library together, demonstrating inter-module dependencies, configuration
 * management, and data processing workflows.
 */

#include <cpp-template/cpp-template.h>
#include <iostream>
#include <memory>
#include <vector>
#include <chrono>
#include <thread>
#include "modules/config_manager.h"
#include "modules/data_processor.h"

void demonstrateBasicModularUsage() {
    std::cout << "=== Basic Modular Usage ===" << std::endl;
    
    try {
        // Step 1: Create configuration manager
        std::cout << "1. Creating configuration manager..." << std::endl;
        auto config = cpp_template::modules::createConfigManager();
        
        // Configure basic settings
        config->setValue("app.name", "ModularDemo");
        config->setValue("app.version", "1.0.0");
        config->setValue("processing.mode", "advanced");
        config->setValue("processing.batch_size", "5");
        config->setValue("logging.level", "info");
        
        std::cout << "   Configuration created and initialized" << std::endl;
        
        // Step 2: Create data processor with configuration dependency
        std::cout << "\n2. Creating data processor..." << std::endl;
        auto processor = cpp_template::modules::createDataProcessor(
            std::shared_ptr<cpp_template::modules::ConfigManager>(config.release())
        );
        
        std::cout << "   Data processor created with configuration dependency" << std::endl;
        
        // Step 3: Process individual items
        std::cout << "\n3. Processing individual items..." << std::endl;
        std::vector<std::string> test_items = {
            "Hello World",
            "Modular Architecture",
            "C++ Template",
            "Data Processing"
        };
        
        for (const auto& item : test_items) {
            auto result = processor->processItem(item, 
                cpp_template::modules::ProcessingMode::SIMPLE);
            
            if (result.success) {
                std::cout << "   ✓ \"" << item << "\" → \"" << result.result << "\"" << std::endl;
            } else {
                std::cout << "   ✗ Error processing \"" << item << "\": " << result.error_message << std::endl;
            }
        }
        
        // Step 4: Show processing statistics
        std::cout << "\n4. Processing statistics:" << std::endl;
        std::cout << processor->getStatistics() << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error in basic modular usage: " << e.what() << std::endl;
    }
}

void demonstrateAdvancedProcessing() {
    std::cout << "=== Advanced Processing Modes ===" << std::endl;
    
    try {
        // Create configuration with advanced settings
        auto config = cpp_template::modules::createConfigManager();
        config->setValue("processing.mode", "advanced");
        config->setValue("processing.batch_size", "3");
        config->setValue("processing.timeout", "5000");
        
        auto processor = cpp_template::modules::createDataProcessor(
            std::shared_ptr<cpp_template::modules::ConfigManager>(config.release())
        );
        
        std::vector<std::string> test_data = {
            "Simple text",
            "Complex data structure",
            "Multi-word processing example"
        };
        
        // Test different processing modes
        std::vector<cpp_template::modules::ProcessingMode> modes = {
            cpp_template::modules::ProcessingMode::SIMPLE,
            cpp_template::modules::ProcessingMode::ADVANCED,
            cpp_template::modules::ProcessingMode::BATCH
        };
        
        std::vector<std::string> mode_names = {"SIMPLE", "ADVANCED", "BATCH"};
        
        for (size_t i = 0; i < modes.size(); ++i) {
            std::cout << "\nTesting " << mode_names[i] << " mode:" << std::endl;
            
            for (const auto& data : test_data) {
                auto result = processor->processItem(data, modes[i]);
                
                if (result.success) {
                    std::cout << "  \"" << data << "\" → \"" << result.result << "\"" << std::endl;
                } else {
                    std::cout << "  Error: " << result.error_message << std::endl;
                }
            }
        }
        
        std::cout << "\nFinal statistics:" << std::endl;
        std::cout << processor->getStatistics() << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error in advanced processing: " << e.what() << std::endl;
    }
}

void demonstrateBatchProcessing() {
    std::cout << "=== Batch Processing Demo ===" << std::endl;
    
    try {
        auto config = cpp_template::modules::createConfigManager();
        config->setValue("processing.batch_size", "10");
        config->setValue("processing.mode", "batch");
        
        auto processor = cpp_template::modules::createDataProcessor(
            std::shared_ptr<cpp_template::modules::ConfigManager>(config.release())
        );
        
        // Test different batch sizes
        std::vector<std::vector<std::string>> test_batches = {
            {"item1", "item2"},  // Small batch
            {"batch1", "batch2", "batch3", "batch4", "batch5"},  // Medium batch
            {"large1", "large2", "large3", "large4", "large5", 
             "large6", "large7", "large8", "large9", "large10"},  // At limit
            {"exceed1", "exceed2", "exceed3", "exceed4", "exceed5",
             "exceed6", "exceed7", "exceed8", "exceed9", "exceed10", "exceed11"}  // Over limit
        };
        
        for (size_t i = 0; i < test_batches.size(); ++i) {
            const auto& batch = test_batches[i];
            std::cout << "\nBatch " << (i + 1) << " (size: " << batch.size() << "):" << std::endl;
            
            auto result = processor->processBatch(batch, 
                cpp_template::modules::ProcessingMode::BATCH);
            
            if (result.success) {
                std::cout << "  ✓ Success: " << result.result << std::endl;
                std::cout << "  Items processed: " << result.processed_items << std::endl;
            } else {
                std::cout << "  ✗ Error: " << result.error_message << std::endl;
            }
        }
        
        std::cout << "\nBatch processing statistics:" << std::endl;
        std::cout << processor->getStatistics() << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error in batch processing: " << e.what() << std::endl;
    }
}

void demonstrateConfigurationManagement() {
    std::cout << "=== Configuration Management Demo ===" << std::endl;
    
    try {
        auto config = cpp_template::modules::createConfigManager();
        
        // Set initial configuration
        std::cout << "Setting initial configuration..." << std::endl;
        config->setValue("processing.threads", "4");
        config->setValue("processing.timeout", "30000");
        config->setValue("cache.size", "1024");
        config->setValue("cache.ttl", "3600");
        
        // Create processor with initial config
        auto processor = cpp_template::modules::createDataProcessor(
            std::shared_ptr<cpp_template::modules::ConfigManager>(config.release())
        );
        
        // Process some data with initial settings
        std::cout << "\nProcessing with initial configuration:" << std::endl;
        auto result1 = processor->processItem("Initial test", 
            cpp_template::modules::ProcessingMode::SIMPLE);
        if (result1.success) {
            std::cout << "  Result: " << result1.result << std::endl;
        }
        
        // Update configuration at runtime
        std::cout << "\nUpdating configuration at runtime..." << std::endl;
        processor->setProcessingConfig("mode", "optimized");
        processor->setProcessingConfig("batch_size", "20");
        
        // Process with updated settings
        std::cout << "Processing with updated configuration:" << std::endl;
        auto result2 = processor->processItem("Updated test", 
            cpp_template::modules::ProcessingMode::ADVANCED);
        if (result2.success) {
            std::cout << "  Result: " << result2.result << std::endl;
        }
        
        std::cout << "\nConfiguration management statistics:" << std::endl;
        std::cout << processor->getStatistics() << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error in configuration management: " << e.what() << std::endl;
    }
}

void demonstrateErrorHandling() {
    std::cout << "=== Error Handling Demo ===" << std::endl;
    
    try {
        auto config = cpp_template::modules::createConfigManager();
        config->setValue("processing.batch_size", "2");  // Small batch size for testing
        
        auto processor = cpp_template::modules::createDataProcessor(
            std::shared_ptr<cpp_template::modules::ConfigManager>(config.release())
        );
        
        // Test error scenarios
        std::cout << "Testing error scenarios:" << std::endl;
        
        // 1. Empty input
        std::cout << "\n1. Empty input test:" << std::endl;
        auto result1 = processor->processItem("", cpp_template::modules::ProcessingMode::SIMPLE);
        if (!result1.success) {
            std::cout << "  ✓ Correctly handled empty input: " << result1.error_message << std::endl;
        }
        
        // 2. Batch size exceeded
        std::cout << "\n2. Batch size limit test:" << std::endl;
        std::vector<std::string> large_batch = {"item1", "item2", "item3", "item4", "item5"};
        auto result2 = processor->processBatch(large_batch, cpp_template::modules::ProcessingMode::BATCH);
        if (!result2.success) {
            std::cout << "  ✓ Correctly handled batch size limit: " << result2.error_message << std::endl;
        }
        
        // 3. Valid processing after errors
        std::cout << "\n3. Recovery after errors:" << std::endl;
        auto result3 = processor->processItem("Valid input", cpp_template::modules::ProcessingMode::SIMPLE);
        if (result3.success) {
            std::cout << "  ✓ Successfully processed after errors: " << result3.result << std::endl;
        }
        
        std::cout << "\nError handling statistics:" << std::endl;
        std::cout << processor->getStatistics() << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error in error handling demo: " << e.what() << std::endl;
    }
}

void demonstratePerformanceMonitoring() {
    std::cout << "=== Performance Monitoring Demo ===" << std::endl;
    
    try {
        auto config = cpp_template::modules::createConfigManager();
        config->setValue("processing.batch_size", "100");
        
        auto processor = cpp_template::modules::createDataProcessor(
            std::shared_ptr<cpp_template::modules::ConfigManager>(config.release())
        );
        
        // Generate test data
        std::vector<std::string> performance_data;
        for (int i = 0; i < 50; ++i) {
            performance_data.push_back("Performance test item " + std::to_string(i));
        }
        
        std::cout << "Processing " << performance_data.size() << " items..." << std::endl;
        
        auto start_time = std::chrono::high_resolution_clock::now();
        
        // Process all items
        for (const auto& item : performance_data) {
            processor->processItem(item, cpp_template::modules::ProcessingMode::SIMPLE);
        }
        
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        
        std::cout << "Processing completed in " << duration.count() << " ms" << std::endl;
        std::cout << "Average time per item: " << (duration.count() / static_cast<double>(performance_data.size())) << " ms" << std::endl;
        
        std::cout << "\nPerformance statistics:" << std::endl;
        std::cout << processor->getStatistics() << std::endl;
        
        // Reset statistics and test batch processing
        processor->resetStatistics();
        std::cout << "\nTesting batch processing performance..." << std::endl;
        
        start_time = std::chrono::high_resolution_clock::now();
        auto batch_result = processor->processBatch(performance_data, cpp_template::modules::ProcessingMode::BATCH);
        end_time = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        
        if (batch_result.success) {
            std::cout << "Batch processing completed in " << duration.count() << " ms" << std::endl;
            std::cout << "Items processed: " << batch_result.processed_items << std::endl;
        }
        
        std::cout << "\nBatch performance statistics:" << std::endl;
        std::cout << processor->getStatistics() << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error in performance monitoring: " << e.what() << std::endl;
    }
}

int main() {
    std::cout << "=== Modular Architecture Comprehensive Demo ===" << std::endl;
    std::cout << "This example demonstrates the modular architecture of the" << std::endl;
    std::cout << "cpp-template library, showing how components work together." << std::endl;
    std::cout << std::endl;
    
    try {
        demonstrateBasicModularUsage();
        std::cout << std::string(60, '=') << std::endl;
        
        demonstrateAdvancedProcessing();
        std::cout << std::string(60, '=') << std::endl;
        
        demonstrateBatchProcessing();
        std::cout << std::string(60, '=') << std::endl;
        
        demonstrateConfigurationManagement();
        std::cout << std::string(60, '=') << std::endl;
        
        demonstrateErrorHandling();
        std::cout << std::string(60, '=') << std::endl;
        
        demonstratePerformanceMonitoring();
        std::cout << std::string(60, '=') << std::endl;
        
        std::cout << "✓ All modular architecture demonstrations completed successfully!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}