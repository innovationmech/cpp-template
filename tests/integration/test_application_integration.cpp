#include <gtest/gtest.h>
#include "cpp-template/cpp-template.hpp"
#include "cpp-template/core/core.hpp"
#include "example/example_module.hpp"
#include <sstream>
#include <iostream>

using namespace cpp_template;

class ApplicationIntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Capture cout for output verification
        original_cout = std::cout.rdbuf();
        std::cout.rdbuf(captured_output.rdbuf());
    }
    
    void TearDown() override {
        // Restore cout
        std::cout.rdbuf(original_cout);
    }
    
    std::stringstream captured_output;
    std::streambuf* original_cout;
};

TEST_F(ApplicationIntegrationTest, CompleteApplicationWorkflow) {
    // Test the complete application workflow similar to main.cpp
    
    // 1. Get project information
    std::string projectInfo = getProjectInfo();
    EXPECT_FALSE(projectInfo.empty());
    EXPECT_TRUE(projectInfo.find("cpp-template") != std::string::npos);
    EXPECT_TRUE(projectInfo.find("1.0.0") != std::string::npos);
    
    // 2. Initialize core library
    core::Core core;
    EXPECT_TRUE(core.initialize());
    
    // 3. Create and use modules
    modules::ExampleModule dataProcessor("DataProcessor");
    modules::ExampleModule textAnalyzer("TextAnalyzer");
    
    // 4. Process data through module chain
    std::string testData = "Hello World";
    std::string processedData = dataProcessor.processData(testData);
    std::string finalResult = textAnalyzer.processData(processedData);
    
    // Verify processing results
    EXPECT_FALSE(processedData.empty());
    EXPECT_FALSE(finalResult.empty());
    EXPECT_TRUE(processedData.find("DataProcessor") != std::string::npos);
    EXPECT_TRUE(finalResult.find("TextAnalyzer") != std::string::npos);
    
    // 5. Verify module statistics
    auto stats1 = dataProcessor.getStatistics();
    auto stats2 = textAnalyzer.getStatistics();
    
    EXPECT_FALSE(stats1.empty());
    EXPECT_FALSE(stats2.empty());
    
    // Check that both modules have processed data
    bool foundProcessCount1 = false, foundProcessCount2 = false;
    for (const auto& stat : stats1) {
        if (stat.find("Process Count: 1") != std::string::npos) {
            foundProcessCount1 = true;
            break;
        }
    }
    for (const auto& stat : stats2) {
        if (stat.find("Process Count: 1") != std::string::npos) {
            foundProcessCount2 = true;
            break;
        }
    }
    
    EXPECT_TRUE(foundProcessCount1);
    EXPECT_TRUE(foundProcessCount2);
    
    // 6. Cleanup
    EXPECT_NO_THROW(core.cleanup());
}

TEST_F(ApplicationIntegrationTest, DependencyResolution) {
    // Test that all dependencies are properly resolved and linked
    
    // Core library should be accessible
    EXPECT_EQ(core::Core::getProjectName(), "cpp-template");
    EXPECT_EQ(core::Core::getVersion(), "1.0.0");
    
    // Modules should be able to access core functionality
    modules::ExampleModule module("TestModule");
    auto stats = module.getStatistics();
    
    // Module should be able to get core version
    bool foundCoreVersion = false;
    for (const auto& stat : stats) {
        if (stat.find("Core Version: 1.0.0") != std::string::npos) {
            foundCoreVersion = true;
            break;
        }
    }
    
    EXPECT_TRUE(foundCoreVersion);
}

TEST_F(ApplicationIntegrationTest, BuildSystemIntegration) {
    // Test that the build system correctly links all components
    
    // Test that we can create multiple instances without conflicts
    core::Core core1, core2;
    modules::ExampleModule module1("Module1");
    modules::ExampleModule module2("Module2");
    
    EXPECT_TRUE(core1.initialize());
    EXPECT_TRUE(core2.initialize());
    
    // Test cross-module communication
    std::string data = "test data";
    std::string result1 = module1.processData(data);
    std::string result2 = module2.processData(result1);
    
    EXPECT_NE(result1, result2);
    EXPECT_TRUE(result1.find("Module1") != std::string::npos);
    EXPECT_TRUE(result2.find("Module2") != std::string::npos);
    
    core1.cleanup();
    core2.cleanup();
}