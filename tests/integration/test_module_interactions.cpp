#include <gtest/gtest.h>
#include "cpp-template/core/core.hpp"
#include "example/example_module.hpp"
#include <vector>
#include <memory>

using namespace cpp_template;

class ModuleInteractionTest : public ::testing::Test {
protected:
    void SetUp() override {
        core = std::make_unique<core::Core>();
        ASSERT_TRUE(core->initialize());
    }
    
    void TearDown() override {
        if (core) {
            core->cleanup();
        }
    }
    
    std::unique_ptr<core::Core> core;
};

TEST_F(ModuleInteractionTest, ModuleChainProcessing) {
    // Test data processing through a chain of modules
    
    std::vector<std::unique_ptr<modules::ExampleModule>> moduleChain;
    moduleChain.push_back(std::make_unique<modules::ExampleModule>("Preprocessor"));
    moduleChain.push_back(std::make_unique<modules::ExampleModule>("Transformer"));
    moduleChain.push_back(std::make_unique<modules::ExampleModule>("Postprocessor"));
    
    std::string data = "Original Data";
    std::string currentData = data;
    
    // Process data through the chain
    for (auto& module : moduleChain) {
        currentData = module->processData(currentData);
        EXPECT_FALSE(currentData.empty());
        EXPECT_TRUE(currentData.find("[" + module->getName() + "]") != std::string::npos);
    }
    
    // Verify that data has been processed by all modules
    EXPECT_TRUE(currentData.find("[Postprocessor]") != std::string::npos);
    // Note: Only the last module's output format is preserved in chain processing
    
    // Verify processing counts
    for (const auto& module : moduleChain) {
        auto stats = module->getStatistics();
        bool foundProcessCount = false;
        for (const auto& stat : stats) {
            if (stat.find("Process Count: 1") != std::string::npos) {
                foundProcessCount = true;
                break;
            }
        }
        EXPECT_TRUE(foundProcessCount);
    }
}

TEST_F(ModuleInteractionTest, ParallelModuleProcessing) {
    // Test parallel processing with multiple modules
    
    std::vector<std::unique_ptr<modules::ExampleModule>> parallelModules;
    parallelModules.push_back(std::make_unique<modules::ExampleModule>("Worker1"));
    parallelModules.push_back(std::make_unique<modules::ExampleModule>("Worker2"));
    parallelModules.push_back(std::make_unique<modules::ExampleModule>("Worker3"));
    
    std::string inputData = "Parallel Test Data";
    std::vector<std::string> results;
    
    // Process same data with different modules
    for (auto& module : parallelModules) {
        std::string result = module->processData(inputData);
        results.push_back(result);
        
        EXPECT_FALSE(result.empty());
        EXPECT_TRUE(result.find("[" + module->getName() + "]") != std::string::npos);
    }
    
    // Verify that each module produced different results
    EXPECT_EQ(results.size(), 3);
    EXPECT_NE(results[0], results[1]);
    EXPECT_NE(results[1], results[2]);
    EXPECT_NE(results[0], results[2]);
    
    // But all should contain the original reversed data
    for (const auto& result : results) {
        EXPECT_TRUE(result.find("ataD tseT lellaraP") != std::string::npos); // reversed input
    }
}

TEST_F(ModuleInteractionTest, ModuleCoreIntegration) {
    // Test integration between modules and core library
    
    modules::ExampleModule module("CoreIntegrationTest");
    
    // Module should be able to access core version through statistics
    auto stats = module.getStatistics();
    
    bool foundCoreVersion = false;
    bool foundModuleName = false;
    
    for (const auto& stat : stats) {
        if (stat.find("Core Version:") != std::string::npos) {
            foundCoreVersion = true;
            EXPECT_TRUE(stat.find(core::Core::getVersion()) != std::string::npos);
        }
        if (stat.find("Module Name: CoreIntegrationTest") != std::string::npos) {
            foundModuleName = true;
        }
    }
    
    EXPECT_TRUE(foundCoreVersion);
    EXPECT_TRUE(foundModuleName);
}

TEST_F(ModuleInteractionTest, ModuleStateManagement) {
    // Test module state management and persistence
    
    modules::ExampleModule module("StateTest");
    
    // Process multiple data items
    std::vector<std::string> testInputs = {
        "First Input",
        "Second Input",
        "Third Input"
    };
    
    for (size_t i = 0; i < testInputs.size(); ++i) {
        module.processData(testInputs[i]);
        
        // Check that process count is correctly maintained
        auto stats = module.getStatistics();
        bool foundCorrectCount = false;
        
        for (const auto& stat : stats) {
            std::string expectedCount = "Process Count: " + std::to_string(i + 1);
            if (stat.find(expectedCount) != std::string::npos) {
                foundCorrectCount = true;
                break;
            }
        }
        
        EXPECT_TRUE(foundCorrectCount);
    }
}

TEST_F(ModuleInteractionTest, ModuleErrorHandling) {
    // Test module behavior with edge cases
    
    modules::ExampleModule module("ErrorHandlingTest");
    
    // Test with empty input
    std::string emptyResult = module.processData("");
    EXPECT_FALSE(emptyResult.empty());
    EXPECT_TRUE(emptyResult.find("[ErrorHandlingTest]") != std::string::npos);
    
    // Test with very long input
    std::string longInput(1000, 'A');
    std::string longResult = module.processData(longInput);
    EXPECT_FALSE(longResult.empty());
    EXPECT_TRUE(longResult.find("[ErrorHandlingTest]") != std::string::npos);
    
    // Test with special characters
    std::string specialInput = "!@#$%^&*()_+-=[]{}|;':,.<>?";
    std::string specialResult = module.processData(specialInput);
    EXPECT_FALSE(specialResult.empty());
    EXPECT_TRUE(specialResult.find("[ErrorHandlingTest]") != std::string::npos);
    
    // Verify all processing was counted
    auto stats = module.getStatistics();
    bool foundProcessCount = false;
    for (const auto& stat : stats) {
        if (stat.find("Process Count: 3") != std::string::npos) {
            foundProcessCount = true;
            break;
        }
    }
    EXPECT_TRUE(foundProcessCount);
}