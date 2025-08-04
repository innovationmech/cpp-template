#include "core/utils.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace cpp_template::core::utils;

// Test fixture for string utilities
class StringUtilsTest : public ::testing::Test {
  protected:
    void SetUp() override {
        empty_string_ = "";
        simple_string_ = "hello";
        mixed_case_string_ = "HeLLo WoRLd";
        special_chars_string_ = "Hello@World#123!";
        whitespace_string_ = "  hello world  ";
        unicode_string_ = "héllo wörld";
    }

    std::string empty_string_;
    std::string simple_string_;
    std::string mixed_case_string_;
    std::string special_chars_string_;
    std::string whitespace_string_;
    std::string unicode_string_;
};

// Test string::toUpper function
TEST_F(StringUtilsTest, ToUpperEmptyString) {
    EXPECT_EQ(string::toUpper(empty_string_), "");
}

TEST_F(StringUtilsTest, ToUpperSimpleString) {
    EXPECT_EQ(string::toUpper(simple_string_), "HELLO");
}

TEST_F(StringUtilsTest, ToUpperMixedCase) {
    EXPECT_EQ(string::toUpper(mixed_case_string_), "HELLO WORLD");
}

TEST_F(StringUtilsTest, ToUpperSpecialCharacters) {
    EXPECT_EQ(string::toUpper(special_chars_string_), "HELLO@WORLD#123!");
}

TEST_F(StringUtilsTest, ToUpperWithWhitespace) {
    EXPECT_EQ(string::toUpper(whitespace_string_), "  HELLO WORLD  ");
}

// Test string::toLower function
TEST_F(StringUtilsTest, ToLowerEmptyString) {
    EXPECT_EQ(string::toLower(empty_string_), "");
}

TEST_F(StringUtilsTest, ToLowerSimpleString) {
    EXPECT_EQ(string::toLower("HELLO"), "hello");
}

TEST_F(StringUtilsTest, ToLowerMixedCase) {
    EXPECT_EQ(string::toLower(mixed_case_string_), "hello world");
}

TEST_F(StringUtilsTest, ToLowerSpecialCharacters) {
    EXPECT_EQ(string::toLower("HELLO@WORLD#123!"), "hello@world#123!");
}

TEST_F(StringUtilsTest, ToLowerWithWhitespace) {
    EXPECT_EQ(string::toLower("  HELLO WORLD  "), "  hello world  ");
}

// Test string::split function
TEST_F(StringUtilsTest, SplitEmptyString) {
    auto result = string::split(empty_string_, ',');
    EXPECT_EQ(result.size(), 0);
}

TEST_F(StringUtilsTest, SplitSimpleString) {
    auto result = string::split("a,b,c", ',');
    EXPECT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "a");
    EXPECT_EQ(result[1], "b");
    EXPECT_EQ(result[2], "c");
}

TEST_F(StringUtilsTest, SplitWithSpaces) {
    auto result = string::split("hello world test", ' ');
    EXPECT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "hello");
    EXPECT_EQ(result[1], "world");
    EXPECT_EQ(result[2], "test");
}

TEST_F(StringUtilsTest, SplitWithEmptyParts) {
    auto result = string::split("a,,b,", ',');
    EXPECT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "a");
    EXPECT_EQ(result[1], "");
    EXPECT_EQ(result[2], "b");
    // Note: trailing empty part after last delimiter is not included
}

TEST_F(StringUtilsTest, SplitNoDelimiter) {
    auto result = string::split("hello", ',');
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], "hello");
}

TEST_F(StringUtilsTest, SplitOnlyDelimiters) {
    auto result = string::split(",,,", ',');
    EXPECT_EQ(result.size(), 3);
    for (const auto& part : result) {
        EXPECT_EQ(part, "");
    }
}

// Test string::join function
TEST_F(StringUtilsTest, JoinEmptyVector) {
    std::vector<std::string> empty_vec;
    EXPECT_EQ(string::join(empty_vec, ","), "");
}

TEST_F(StringUtilsTest, JoinSingleElement) {
    std::vector<std::string> single_vec = {"hello"};
    EXPECT_EQ(string::join(single_vec, ","), "hello");
}

TEST_F(StringUtilsTest, JoinMultipleElements) {
    std::vector<std::string> multi_vec = {"a", "b", "c"};
    EXPECT_EQ(string::join(multi_vec, ","), "a,b,c");
}

TEST_F(StringUtilsTest, JoinWithSpaceDelimiter) {
    std::vector<std::string> words = {"hello", "world", "test"};
    EXPECT_EQ(string::join(words, " "), "hello world test");
}

TEST_F(StringUtilsTest, JoinWithMultiCharDelimiter) {
    std::vector<std::string> parts = {"part1", "part2", "part3"};
    EXPECT_EQ(string::join(parts, " -> "), "part1 -> part2 -> part3");
}

TEST_F(StringUtilsTest, JoinWithEmptyStrings) {
    std::vector<std::string> with_empty = {"a", "", "b", ""};
    EXPECT_EQ(string::join(with_empty, ","), "a,,b,");
}

// Test round-trip split and join
TEST_F(StringUtilsTest, SplitJoinRoundTrip) {
    std::string original = "hello,world,test";
    auto split_result = string::split(original, ',');
    std::string joined = string::join(split_result, ",");
    EXPECT_EQ(joined, original);
}

// Test fixture for validation utilities
class ValidationUtilsTest : public ::testing::Test {
  protected:
    void SetUp() override {
        empty_string_ = "";
        whitespace_only_ = "   \t\n  ";
        mixed_whitespace_ = " \t hello \n ";
        normal_string_ = "hello";
        alphanumeric_string_ = "Hello123";
        special_chars_string_ = "hello@world";
        numbers_only_ = "12345";
        letters_only_ = "abcDEF";
    }

    std::string empty_string_;
    std::string whitespace_only_;
    std::string mixed_whitespace_;
    std::string normal_string_;
    std::string alphanumeric_string_;
    std::string special_chars_string_;
    std::string numbers_only_;
    std::string letters_only_;
};

// Test validation::isEmpty function
TEST_F(ValidationUtilsTest, IsEmptyWithEmptyString) {
    EXPECT_TRUE(validation::isEmpty(empty_string_));
}

TEST_F(ValidationUtilsTest, IsEmptyWithWhitespaceOnly) {
    EXPECT_TRUE(validation::isEmpty(whitespace_only_));
}

TEST_F(ValidationUtilsTest, IsEmptyWithMixedWhitespace) {
    EXPECT_FALSE(validation::isEmpty(mixed_whitespace_));
}

TEST_F(ValidationUtilsTest, IsEmptyWithNormalString) {
    EXPECT_FALSE(validation::isEmpty(normal_string_));
}

TEST_F(ValidationUtilsTest, IsEmptyWithSingleSpace) {
    EXPECT_TRUE(validation::isEmpty(" "));
}

TEST_F(ValidationUtilsTest, IsEmptyWithSingleTab) {
    EXPECT_TRUE(validation::isEmpty("\t"));
}

TEST_F(ValidationUtilsTest, IsEmptyWithSingleNewline) {
    EXPECT_TRUE(validation::isEmpty("\n"));
}

// Test validation::isAlphanumeric function
TEST_F(ValidationUtilsTest, IsAlphanumericWithEmptyString) {
    EXPECT_FALSE(validation::isAlphanumeric(empty_string_));
}

TEST_F(ValidationUtilsTest, IsAlphanumericWithLettersOnly) {
    EXPECT_TRUE(validation::isAlphanumeric(letters_only_));
}

TEST_F(ValidationUtilsTest, IsAlphanumericWithNumbersOnly) {
    EXPECT_TRUE(validation::isAlphanumeric(numbers_only_));
}

TEST_F(ValidationUtilsTest, IsAlphanumericWithMixed) {
    EXPECT_TRUE(validation::isAlphanumeric(alphanumeric_string_));
}

TEST_F(ValidationUtilsTest, IsAlphanumericWithSpecialChars) {
    EXPECT_FALSE(validation::isAlphanumeric(special_chars_string_));
}

TEST_F(ValidationUtilsTest, IsAlphanumericWithWhitespace) {
    EXPECT_FALSE(validation::isAlphanumeric("hello world"));
}

TEST_F(ValidationUtilsTest, IsAlphanumericWithPunctuation) {
    EXPECT_FALSE(validation::isAlphanumeric("hello!"));
}

TEST_F(ValidationUtilsTest, IsAlphanumericWithUnderscore) {
    EXPECT_FALSE(validation::isAlphanumeric("hello_world"));
}

TEST_F(ValidationUtilsTest, IsAlphanumericWithHyphen) {
    EXPECT_FALSE(validation::isAlphanumeric("hello-world"));
}

// Edge case tests
TEST_F(ValidationUtilsTest, IsAlphanumericSingleCharacter) {
    EXPECT_TRUE(validation::isAlphanumeric("a"));
    EXPECT_TRUE(validation::isAlphanumeric("Z"));
    EXPECT_TRUE(validation::isAlphanumeric("5"));
    EXPECT_FALSE(validation::isAlphanumeric("@"));
}

// Performance and stress tests
TEST_F(StringUtilsTest, LargeStringSplit) {
    // Create a large string with many parts
    std::string large_string;
    for (int i = 0; i < 1000; ++i) {
        if (i > 0)
            large_string += ",";
        large_string += "part" + std::to_string(i);
    }

    auto result = string::split(large_string, ',');
    EXPECT_EQ(result.size(), 1000);
    EXPECT_EQ(result[0], "part0");
    EXPECT_EQ(result[999], "part999");
}

TEST_F(StringUtilsTest, LargeVectorJoin) {
    // Create a large vector
    std::vector<std::string> large_vector;
    for (int i = 0; i < 1000; ++i) {
        large_vector.push_back("item" + std::to_string(i));
    }

    std::string result = string::join(large_vector, ",");
    EXPECT_TRUE(result.find("item0,item1") == 0);
    EXPECT_TRUE(result.find("item999") != std::string::npos);
}
