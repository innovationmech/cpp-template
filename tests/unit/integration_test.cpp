#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "modules/config_manager.h"
#include "modules/data_processor.h"
#include "core/core.h"
#include "core/utils.h"
#include <memory>
#include <fstream>
#include <filesystem>

using namespace cpp_template::modules;
using namespace cpp_template::core;

class IntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create temporary directory for test files
        test_dir_ = std::filesystem::temp_directory_path() / "cpp_template_test";
        std::filesystem::create_directories(test_dir_);
        
        // Create test configuration file
        config_file_ = test_dir_ / "test_config.txt";
        createTestConfigFile();
        
        // Create shared components for integration testing
        config_manager_ = std::shared_ptr<ConfigManager>(createConfigManager().release());
        data_processor_ = createDataProcessor(config_manager_);
        core_instance_ = createCore("IntegrationTestCore");
    }

    void TearDown() override {
        // Clean up test files
        std::filesystem::remove_all(test_dir_);
    }

    void createTestConfigFile() {
        std::ofstream file(config_file_);
        file << "# Test configuration file\n";
        file << "app.name = integration-test\n";
        file << "app.version = 2.0.0\n";
        file << "processing.mode = advanced\n";
        file << "processing.batch_size = 3\n";
        file << "logging.level = debug\n";
        file << "test.setting = test_value\n";
        file.close();
    }

    std::filesystem::path test_dir_;
    std::filesystem::path config_file_;
    std::shared_ptr<ConfigManager> config_manager_;
    std::unique_ptr<DataProcessor> data_processor_;
    std::unique_ptr<Core> core_instance_;
};

// Test module interaction: ConfigManager and DataProcessor
TEST_F(IntegrationTest, ConfigManagerDataProcessorIntegration) {
    // Test that DataProcessor correctly uses ConfigManager settings
    
    // Set specific configuration for processing
    config_manager_->setValue("processing.batch_size", "2");
    config_manager_->setValue("processing.mode", "advanced");
    
    // Test single item processing
    auto result = data_processor_->processItem("test input", ProcessingMode::SIMPLE);
    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.processed_items, 1);
    EXPECT_TRUE(result.result.find("[SIMPLE]") != std::string::npos);
    EXPECT_TRUE(result.result.find("TEST INPUT") != std::string::npos);
    
    // Test batch processing with configuration limits
    std::vector<std::string> small_batch = {"item1", "item2"};
    auto batch_result = data_processor_->processBatch(small_batch, ProcessingMode::BATCH);
    EXPECT_TRUE(batch_result.success);
    EXPECT_EQ(batch_result.processed_items, 2);
    
    // Test batch processing exceeding configured limit
    std::vector<std::string> large_batch = {"item1", "item2", "item3", "item4"};
    auto large_batch_result = data_processor_->processBatch(large_batch, ProcessingMode::BATCH);
    EXPECT_FALSE(large_batch_result.success);
    EXPECT_TRUE(large_batch_result.error_message.find("exceeds configured limit") != std::string::npos);
}

// Test configuration file loading and module integration
TEST_F(IntegrationTest, ConfigFileLoadingIntegration) {
    // Load configuration from file
    EXPECT_TRUE(config_manager_->loadFromFile(config_file_.string()));
    
    // Verify configuration values were loaded
    EXPECT_EQ(config_manager_->getValue("app.name"), "integration-test");
    EXPECT_EQ(config_manager_->getValue("app.version"), "2.0.0");
    EXPECT_EQ(config_manager_->getValue("processing.batch_size"), "3");
    EXPECT_EQ(config_manager_->getValue("test.setting"), "test_value");
    
    // Test that DataProcessor uses the loaded configuration
    std::vector<std::string> batch = {"a", "b", "c"};
    auto result = data_processor_->processBatch(batch, ProcessingMode::BATCH);
    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.processed_items, 3);
    
    // Test exceeding the loaded batch size limit
    std::vector<std::string> large_batch = {"a", "b", "c", "d"};
    auto large_result = data_processor_->processBatch(large_batch, ProcessingMode::BATCH);
    EXPECT_FALSE(large_result.success);
}

// Test Core library integration with modules
TEST_F(IntegrationTest, CoreLibraryModuleIntegration) {
    // Initialize core
    EXPECT_TRUE(core_instance_->initialize());
    EXPECT_TRUE(core_instance_->isInitialized());
    
    // Test that core processing works with utility functions
    std::string input = "integration test";
    std::string core_result = core_instance_->process(input);
    
    // Core should use utils::string::toUpper internally
    std::string expected_upper = utils::string::toUpper(input);
    EXPECT_TRUE(core_result.find(expected_upper) != std::string::npos);
    EXPECT_TRUE(core_result.find("[IntegrationTestCore]") != std::string::npos);
    
    // Test that DataProcessor can use the same utility functions
    auto processor_result = data_processor_->processItem(input, ProcessingMode::SIMPLE);
    EXPECT_TRUE(processor_result.success);
    EXPECT_TRUE(processor_result.result.find(expected_upper) != std::string::npos);
}

// Test complete processing pipeline
TEST_F(IntegrationTest, CompleteProcessingPipeline) {
    // Set up configuration
    config_manager_->setValue("processing.mode", "pipeline");
    config_manager_->setValue("processing.batch_size", "5");
    
    // Initialize core
    EXPECT_TRUE(core_instance_->initialize());
    
    // Create test data
    std::vector<std::string> test_data = {
        "hello world",
        "integration test",
        "module interaction",
        "pipeline processing"
    };
    
    // Process each item through different processing modes
    std::vector<std::string> processed_results;
    
    for (const auto& item : test_data) {
        // First process through core
        std::string core_processed = core_instance_->process(item);
        
        // Then process through data processor
        auto processor_result = data_processor_->processItem(core_processed, ProcessingMode::ADVANCED);
        EXPECT_TRUE(processor_result.success);
        
        processed_results.push_back(processor_result.result);
    }
    
    // Verify all items were processed
    EXPECT_EQ(processed_results.size(), test_data.size());
    
    // Process as batch
    auto batch_result = data_processor_->processBatch(test_data, ProcessingMode::BATCH);
    EXPECT_TRUE(batch_result.success);
    EXPECT_EQ(batch_result.processed_items, test_data.size());
    
    // Verify statistics
    std::string stats = data_processor_->getStatistics();
    EXPECT_TRUE(stats.find("Total Processed:") != std::string::npos);
    EXPECT_TRUE(stats.find("Successful Operations:") != std::string::npos);
}

// Test error handling across modules
TEST_F(IntegrationTest, ErrorHandlingIntegration) {
    // Test Core error handling
    EXPECT_THROW(core_instance_->process("test"), std::runtime_error); // Not initialized
    
    // Test DataProcessor error handling with invalid config
    config_manager_->setValue("processing.batch_size", "invalid");
    std::vector<std::string> batch = {"test"};
    auto result = data_processor_->processBatch(batch, ProcessingMode::BATCH);
    EXPECT_FALSE(result.success);
    EXPECT_FALSE(result.error_message.empty());
    
    // Test DataProcessor with null config (should be caught in constructor)
    EXPECT_THROW({
        auto null_processor = createDataProcessor(nullptr);
    }, std::invalid_argument);
    
    // Test empty input handling
    auto empty_result = data_processor_->processItem("", ProcessingMode::SIMPLE);
    EXPECT_FALSE(empty_result.success);
    EXPECT_TRUE(empty_result.error_message.find("empty") != std::string::npos);
}

// Test dependency resolution and linking
TEST_F(IntegrationTest, DependencyResolutionTest) {
    // This test verifies that all dependencies are properly linked
    // by exercising functionality that requires different libraries
    
    // Test core library dependency
    auto core = createCore("DependencyTest");
    EXPECT_NE(core, nullptr);
    EXPECT_EQ(core->getName(), "DependencyTest");
    
    // Test utils library dependency
    std::string test_string = "Dependency Test";
    std::string upper = utils::string::toUpper(test_string);
    std::string lower = utils::string::toLower(test_string);
    EXPECT_EQ(upper, "DEPENDENCY TEST");
    EXPECT_EQ(lower, "dependency test");
    
    // Test string split and join
    auto parts = utils::string::split("a,b,c", ',');
    EXPECT_EQ(parts.size(), 3);
    std::string joined = utils::string::join(parts, "-");
    EXPECT_EQ(joined, "a-b-c");
    
    // Test validation functions
    EXPECT_TRUE(utils::validation::isEmpty("   "));
    EXPECT_FALSE(utils::validation::isEmpty("not empty"));
    EXPECT_TRUE(utils::validation::isAlphanumeric("Test123"));
    EXPECT_FALSE(utils::validation::isAlphanumeric("Test@123"));
    
    // Test module dependencies
    auto config = createConfigManager();
    EXPECT_NE(config, nullptr);
    
    auto shared_config = std::shared_ptr<ConfigManager>(config.release());
    auto processor = createDataProcessor(shared_config);
    EXPECT_NE(processor, nullptr);
    
    // Test that processor can use config
    shared_config->setValue("test.key", "test.value");
    EXPECT_EQ(shared_config->getValue("test.key"), "test.value");
    
    auto process_result = processor->processItem("dependency test", ProcessingMode::SIMPLE);
    EXPECT_TRUE(process_result.success);
}

// Test build and execution pipeline
TEST_F(IntegrationTest, BuildExecutionPipeline) {
    // This test simulates the complete application workflow
    
    // Step 1: Configuration setup
    config_manager_->setValue("app.name", "pipeline-test");
    config_manager_->setValue("processing.mode", "advanced");
    config_manager_->setValue("processing.batch_size", "10");
    
    // Step 2: Core initialization
    EXPECT_TRUE(core_instance_->initialize());
    
    // Step 3: Data processing setup
    data_processor_->setProcessingConfig("mode", "batch");
    
    // Step 4: Execute processing workflow
    std::vector<std::string> workflow_data = {
        "step1: initialization",
        "step2: configuration",
        "step3: processing",
        "step4: validation",
        "step5: completion"
    };
    
    // Process each step
    for (size_t i = 0; i < workflow_data.size(); ++i) {
        // Core processing
        std::string core_result = core_instance_->process(workflow_data[i]);
        EXPECT_FALSE(core_result.empty());
        
        // Module processing
        ProcessingMode mode = (i % 2 == 0) ? ProcessingMode::SIMPLE : ProcessingMode::ADVANCED;
        auto module_result = data_processor_->processItem(workflow_data[i], mode);
        EXPECT_TRUE(module_result.success);
    }
    
    // Batch processing
    auto batch_result = data_processor_->processBatch(workflow_data, ProcessingMode::BATCH);
    EXPECT_TRUE(batch_result.success);
    EXPECT_EQ(batch_result.processed_items, workflow_data.size());
    
    // Verify final statistics
    std::string final_stats = data_processor_->getStatistics();
    EXPECT_TRUE(final_stats.find("Total Processed:") != std::string::npos);
    
    // Verify configuration state
    EXPECT_EQ(config_manager_->getValue("app.name"), "pipeline-test");
    EXPECT_TRUE(config_manager_->hasKey("processing.mode"));
    
    auto all_keys = config_manager_->getAllKeys();
    EXPECT_GT(all_keys.size(), 0);
}

// Test cross-module data flow
TEST_F(IntegrationTest, CrossModuleDataFlow) {
    // Test data flowing between different modules and libraries
    
    // Start with raw data
    std::string raw_data = "cross module data flow test";
    
    // Step 1: Process through utils
    std::string upper_data = utils::string::toUpper(raw_data);
    EXPECT_EQ(upper_data, "CROSS MODULE DATA FLOW TEST");
    
    // Step 2: Split and process parts
    auto parts = utils::string::split(upper_data, ' ');
    EXPECT_EQ(parts.size(), 5);
    
    // Step 3: Process each part through core
    core_instance_->initialize();
    std::vector<std::string> core_processed;
    for (const auto& part : parts) {
        std::string processed = core_instance_->process(part);
        core_processed.push_back(processed);
    }
    
    // Step 4: Join and process through data processor
    std::string joined = utils::string::join(parts, "_");
    auto final_result = data_processor_->processItem(joined, ProcessingMode::ADVANCED);
    EXPECT_TRUE(final_result.success);
    
    // Step 5: Verify the complete data flow
    EXPECT_TRUE(final_result.result.find("[ADVANCED]") != std::string::npos);
    EXPECT_FALSE(final_result.result.empty());
    
    // Verify statistics were updated
    std::string stats = data_processor_->getStatistics();
    EXPECT_TRUE(stats.find("Successful Operations: 1") != std::string::npos);
}