/**
 * @file string_utilities_demo.cpp
 * @brief Comprehensive demonstration of string utility functions
 *
 * This example showcases all the string manipulation utilities provided
 * by the cpp-template library, including case conversion, splitting,
 * joining, and validation functions.
 */

#include <cpp-template/utils.h>
#include <iomanip>
#include <iostream>
#include <vector>

void demonstrateCaseConversion() {
    std::cout << "=== Case Conversion Demo ===" << std::endl;

    std::vector<std::string> test_strings = {
        "Hello World",
        "ALREADY UPPERCASE",
        "already lowercase",
        "MiXeD cAsE sTrInG",
        "Numbers123AndSymbols!@#",
        ""  // Empty string
    };

    std::cout << std::left << std::setw(25) << "Original" << std::setw(25) << "Uppercase"
              << std::setw(25) << "Lowercase" << std::endl;
    std::cout << std::string(75, '-') << std::endl;

    for (const auto& str : test_strings) {
        std::string display_str = str.empty() ? "(empty)" : str;
        std::string upper = cpp_template::string_utils::toUpper(str);
        std::string lower = cpp_template::string_utils::toLower(str);

        std::cout << std::left << std::setw(25) << display_str << std::setw(25) << upper
                  << std::setw(25) << lower << std::endl;
    }
    std::cout << std::endl;
}

void demonstrateSplitAndJoin() {
    std::cout << "=== Split and Join Demo ===" << std::endl;

    struct TestCase {
        std::string input;
        char delimiter;
        std::string join_delimiter;
    };

    std::vector<TestCase> test_cases = {
        {  "Hello World Example", ' ',    "-"},
        {   "one,two,three,four", ',',  " | "},
        {"path/to/some/file.txt", '/', " -> "},
        {               "single", ' ',   ", "},
        {                     "", ' ',    "-"}, // Empty string
        {   "no-delimiters-here", ',',  " + "}  // No delimiters found
    };

    for (const auto& test : test_cases) {
        std::cout << "Input: \"" << test.input << "\"" << std::endl;
        std::cout << "Split by '" << test.delimiter << "': ";

        auto parts = cpp_template::string_utils::split(test.input, test.delimiter);

        std::cout << "[";
        for (size_t i = 0; i < parts.size(); ++i) {
            std::cout << "\"" << parts[i] << "\"";
            if (i < parts.size() - 1)
                std::cout << ", ";
        }
        std::cout << "] (" << parts.size() << " parts)" << std::endl;

        std::string rejoined = cpp_template::string_utils::join(parts, test.join_delimiter);
        std::cout << "Rejoined with \"" << test.join_delimiter << "\": \"" << rejoined << "\""
                  << std::endl;
        std::cout << std::endl;
    }
}

void demonstrateValidation() {
    std::cout << "=== Validation Demo ===" << std::endl;

    struct TestString {
        std::string value;
        std::string description;
    };

    std::vector<TestString> test_strings = {
        {        "hello123",            "alphanumeric"},
        {     "Hello World",          "contains space"},
        {             "   ",         "whitespace only"},
        {                "",            "empty string"},
        {"user@example.com",            "email format"},
        {   "invalid-email",           "invalid email"},
        {           "test@",        "incomplete email"},
        {    "@example.com",        "missing username"},
        {       "ABC123def", "mixed case alphanumeric"},
        {      "special!@#",      "special characters"},
        {          "123456",            "numbers only"},
        {          "abcdef",            "letters only"}
    };

    std::cout << std::left << std::setw(20) << "String" << std::setw(15) << "Empty?"
              << std::setw(15) << "Alphanumeric?" << std::setw(15) << "Valid Email?"
              << std::setw(20) << "Description" << std::endl;
    std::cout << std::string(85, '-') << std::endl;

    for (const auto& test : test_strings) {
        std::string display_str =
            test.value.empty()
                ? "(empty)"
                : (test.value.find_first_not_of(" \t\n\r") == std::string::npos ? "(whitespace)"
                                                                                : test.value);

        bool is_empty = cpp_template::validation::isEmpty(test.value);
        bool is_alphanum = cpp_template::validation::isAlphanumeric(test.value);
        bool is_email = cpp_template::validation::isValidEmail(test.value);

        std::cout << std::left << std::setw(20) << display_str << std::setw(15)
                  << (is_empty ? "Yes" : "No") << std::setw(15) << (is_alphanum ? "Yes" : "No")
                  << std::setw(15) << (is_email ? "Yes" : "No") << std::setw(20) << test.description
                  << std::endl;
    }
    std::cout << std::endl;
}

void demonstrateComplexScenarios() {
    std::cout << "=== Complex Scenarios Demo ===" << std::endl;

    // Scenario 1: Processing a CSV-like string
    std::cout << "Scenario 1: Processing CSV-like data" << std::endl;
    std::string csv_data = "John Doe,john@example.com,Manager,Active";
    auto fields = cpp_template::string_utils::split(csv_data, ',');

    std::cout << "CSV Input: " << csv_data << std::endl;
    std::cout << "Parsed fields:" << std::endl;
    std::vector<std::string> field_names = {"Name", "Email", "Role", "Status"};

    for (size_t i = 0; i < fields.size() && i < field_names.size(); ++i) {
        std::cout << "  " << field_names[i] << ": " << fields[i];

        // Validate email field
        if (field_names[i] == "Email") {
            bool valid = cpp_template::validation::isValidEmail(fields[i]);
            std::cout << " (Valid: " << (valid ? "Yes" : "No") << ")";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // Scenario 2: Text processing pipeline
    std::cout << "Scenario 2: Text processing pipeline" << std::endl;
    std::string text = "  Hello, WORLD! This is a TEST string.  ";

    std::cout << "Original: \"" << text << "\"" << std::endl;

    // Step 1: Split into words
    auto words = cpp_template::string_utils::split(text, ' ');
    std::cout << "Split into words: " << words.size() << " words" << std::endl;

    // Step 2: Clean and process each word
    std::vector<std::string> processed_words;
    for (const auto& word : words) {
        if (!cpp_template::validation::isEmpty(word)) {
            // Remove punctuation (simple approach)
            std::string clean_word = word;
            while (!clean_word.empty() && !std::isalnum(clean_word.back())) {
                clean_word.pop_back();
            }
            while (!clean_word.empty() && !std::isalnum(clean_word.front())) {
                clean_word.erase(0, 1);
            }

            if (!clean_word.empty()) {
                processed_words.push_back(cpp_template::string_utils::toLower(clean_word));
            }
        }
    }

    // Step 3: Rejoin processed words
    std::string result = cpp_template::string_utils::join(processed_words, "_");
    std::cout << "Processed result: \"" << result << "\"" << std::endl;
    std::cout << std::endl;
}

int main() {
    std::cout << "=== String Utilities Comprehensive Demo ===" << std::endl;
    std::cout << "This example demonstrates all string utility functions" << std::endl;
    std::cout << "provided by the cpp-template library." << std::endl;
    std::cout << std::endl;

    try {
        demonstrateCaseConversion();
        demonstrateSplitAndJoin();
        demonstrateValidation();
        demonstrateComplexScenarios();

        std::cout << "✓ All string utility demonstrations completed successfully!" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
