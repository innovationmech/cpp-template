#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "core/core.h"
#include <stdexcept>

using namespace cpp_template::core;

class CoreTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up test fixtures
        test_name_ = "TestCore";
        empty_name_ = "";
        whitespace_name_ = "   \t\n  ";
    }

    void TearDown() override {
        // Clean up after tests
    }

    std::string test_name_;
    std::string empty_name_;
    std::string whitespace_name_;
};

// Test Core constructor
TEST_F(CoreTest, ConstructorWithValidName) {
    EXPECT_NO_THROW({
        Core core(test_name_);
        EXPECT_EQ(core.getName(), test_name_);
        EXPECT_FALSE(core.isInitialized());
    });
}

TEST_F(CoreTest, ConstructorWithEmptyName) {
    EXPECT_THROW({
        Core core(empty_name_);
    }, std::invalid_argument);
}

TEST_F(CoreTest, ConstructorWithWhitespaceName) {
    EXPECT_THROW({
        Core core(whitespace_name_);
    }, std::invalid_argument);
}

// Test getName method
TEST_F(CoreTest, GetName) {
    Core core(test_name_);
    EXPECT_EQ(core.getName(), test_name_);
    
    // Test that getName is noexcept
    EXPECT_TRUE(noexcept(core.getName()));
}

// Test setName method
TEST_F(CoreTest, SetNameValid) {
    Core core(test_name_);
    std::string new_name = "NewTestCore";
    
    EXPECT_NO_THROW(core.setName(new_name));
    EXPECT_EQ(core.getName(), new_name);
}

TEST_F(CoreTest, SetNameEmpty) {
    Core core(test_name_);
    
    EXPECT_THROW(core.setName(empty_name_), std::invalid_argument);
    // Name should remain unchanged after failed set
    EXPECT_EQ(core.getName(), test_name_);
}

TEST_F(CoreTest, SetNameWhitespace) {
    Core core(test_name_);
    
    EXPECT_THROW(core.setName(whitespace_name_), std::invalid_argument);
    // Name should remain unchanged after failed set
    EXPECT_EQ(core.getName(), test_name_);
}

// Test initialization methods
TEST_F(CoreTest, InitiallyNotInitialized) {
    Core core(test_name_);
    EXPECT_FALSE(core.isInitialized());
    
    // Test that isInitialized is noexcept
    EXPECT_TRUE(noexcept(core.isInitialized()));
}

TEST_F(CoreTest, Initialize) {
    Core core(test_name_);
    
    EXPECT_TRUE(core.initialize());
    EXPECT_TRUE(core.isInitialized());
}

TEST_F(CoreTest, InitializeMultipleTimes) {
    Core core(test_name_);
    
    EXPECT_TRUE(core.initialize());
    EXPECT_TRUE(core.isInitialized());
    
    // Second initialization should also return true
    EXPECT_TRUE(core.initialize());
    EXPECT_TRUE(core.isInitialized());
}

// Test process method
TEST_F(CoreTest, ProcessWithoutInitialization) {
    Core core(test_name_);
    std::string input = "test input";
    
    EXPECT_THROW(core.process(input), std::runtime_error);
}

TEST_F(CoreTest, ProcessAfterInitialization) {
    Core core(test_name_);
    core.initialize();
    
    std::string input = "hello world";
    std::string result = core.process(input);
    
    // Should return "[TestCore] HELLO WORLD"
    std::string expected = "[" + test_name_ + "] HELLO WORLD";
    EXPECT_EQ(result, expected);
}

TEST_F(CoreTest, ProcessEmptyString) {
    Core core(test_name_);
    core.initialize();
    
    std::string input = "";
    std::string result = core.process(input);
    
    std::string expected = "[" + test_name_ + "] ";
    EXPECT_EQ(result, expected);
}

TEST_F(CoreTest, ProcessSpecialCharacters) {
    Core core(test_name_);
    core.initialize();
    
    std::string input = "hello@world#123";
    std::string result = core.process(input);
    
    std::string expected = "[" + test_name_ + "] HELLO@WORLD#123";
    EXPECT_EQ(result, expected);
}

// Test copy constructor and assignment
TEST_F(CoreTest, CopyConstructor) {
    Core original(test_name_);
    original.initialize();
    
    Core copy(original);
    
    EXPECT_EQ(copy.getName(), original.getName());
    EXPECT_EQ(copy.isInitialized(), original.isInitialized());
    
    // Test that they are independent
    copy.setName("CopyCore");
    EXPECT_NE(copy.getName(), original.getName());
}

TEST_F(CoreTest, CopyAssignment) {
    Core original(test_name_);
    original.initialize();
    
    Core copy("TempName");
    copy = original;
    
    EXPECT_EQ(copy.getName(), original.getName());
    EXPECT_EQ(copy.isInitialized(), original.isInitialized());
}

// Test move constructor and assignment
TEST_F(CoreTest, MoveConstructor) {
    Core original(test_name_);
    original.initialize();
    
    std::string original_name = original.getName();
    bool original_initialized = original.isInitialized();
    
    Core moved(std::move(original));
    
    EXPECT_EQ(moved.getName(), original_name);
    EXPECT_EQ(moved.isInitialized(), original_initialized);
}

TEST_F(CoreTest, MoveAssignment) {
    Core original(test_name_);
    original.initialize();
    
    std::string original_name = original.getName();
    bool original_initialized = original.isInitialized();
    
    Core moved("TempName");
    moved = std::move(original);
    
    EXPECT_EQ(moved.getName(), original_name);
    EXPECT_EQ(moved.isInitialized(), original_initialized);
}

// Test factory function
TEST_F(CoreTest, CreateCoreFactory) {
    auto core_ptr = createCore(test_name_);
    
    ASSERT_NE(core_ptr, nullptr);
    EXPECT_EQ(core_ptr->getName(), test_name_);
    EXPECT_FALSE(core_ptr->isInitialized());
}

TEST_F(CoreTest, CreateCoreFactoryWithEmptyName) {
    EXPECT_THROW({
        auto core_ptr = createCore(empty_name_);
    }, std::invalid_argument);
}