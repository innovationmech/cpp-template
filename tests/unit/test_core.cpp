#include <gtest/gtest.h>
#include "cpp-template/core/core.hpp"

using namespace cpp_template::core;

class CoreTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code for each test
    }
    
    void TearDown() override {
        // Cleanup code for each test
    }
};

TEST_F(CoreTest, GetProjectName) {
    EXPECT_EQ(Core::getProjectName(), "cpp-template");
}

TEST_F(CoreTest, GetVersion) {
    EXPECT_EQ(Core::getVersion(), "1.0.0");
}

TEST_F(CoreTest, Initialize) {
    Core core;
    EXPECT_TRUE(core.initialize());
    
    // Second initialization should also return true
    EXPECT_TRUE(core.initialize());
}

TEST_F(CoreTest, InitializeAndCleanup) {
    Core core;
    
    // Initialize
    EXPECT_TRUE(core.initialize());
    
    // Cleanup should work without issues
    EXPECT_NO_THROW(core.cleanup());
    
    // Multiple cleanups should be safe
    EXPECT_NO_THROW(core.cleanup());
}

TEST_F(CoreTest, MultipleInstances) {
    Core core1;
    Core core2;
    
    EXPECT_TRUE(core1.initialize());
    EXPECT_TRUE(core2.initialize());
    
    EXPECT_NO_THROW(core1.cleanup());
    EXPECT_NO_THROW(core2.cleanup());
}