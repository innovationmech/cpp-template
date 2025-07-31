#include <gtest/gtest.h>
#include "example/example_module.hpp"

using namespace cpp_template::modules;

class ModuleTest : public ::testing::Test {
protected:
    void SetUp() override {
        module = std::make_unique<ExampleModule>("TestModule");
    }
    
    void TearDown() override {
        module.reset();
    }
    
    std::unique_ptr<ExampleModule> module;
};

TEST_F(ModuleTest, GetName) {
    EXPECT_EQ(module->getName(), "TestModule");
}

TEST_F(ModuleTest, ProcessData) {
    std::string input = "hello";
    std::string result = module->processData(input);
    
    EXPECT_TRUE(result.find("TestModule") != std::string::npos);
    EXPECT_TRUE(result.find("olleh") != std::string::npos); // reversed "hello"
}

TEST_F(ModuleTest, ProcessMultipleData) {
    module->processData("first");
    module->processData("second");
    module->processData("third");
    
    auto stats = module->getStatistics();
    
    // Should have at least 3 statistics entries
    EXPECT_GE(stats.size(), 3);
    
    // Check if process count is tracked
    bool foundProcessCount = false;
    for (const auto& stat : stats) {
        if (stat.find("Process Count: 3") != std::string::npos) {
            foundProcessCount = true;
            break;
        }
    }
    EXPECT_TRUE(foundProcessCount);
}

TEST_F(ModuleTest, GetStatistics) {
    auto stats = module->getStatistics();
    
    EXPECT_FALSE(stats.empty());
    
    // Check if module name is in statistics
    bool foundModuleName = false;
    for (const auto& stat : stats) {
        if (stat.find("TestModule") != std::string::npos) {
            foundModuleName = true;
            break;
        }
    }
    EXPECT_TRUE(foundModuleName);
}

TEST_F(ModuleTest, EmptyInput) {
    std::string result = module->processData("");
    EXPECT_TRUE(result.find("TestModule") != std::string::npos);
}