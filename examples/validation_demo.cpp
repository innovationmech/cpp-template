/**
 * @file validation_demo.cpp
 * @brief Comprehensive demonstration of validation utilities
 * 
 * This example shows how to use the validation functions provided
 * by the cpp-template library for input validation and data verification.
 */

#include <cpp-template/utils.h>
#include <iostream>
#include <vector>
#include <iomanip>

void demonstrateBasicValidation() {
    std::cout << "=== Basic Validation Demo ===" << std::endl;
    
    struct TestCase {
        std::string input;
        std::string description;
    };
    
    std::vector<TestCase> test_cases = {
        {"hello123", "Simple alphanumeric"},
        {"Hello World", "Contains space"},
        {"   ", "Whitespace only"},
        {"", "Empty string"},
        {"ABC123def", "Mixed case alphanumeric"},
        {"special!@#", "Special characters"},
        {"123456", "Numbers only"},
        {"abcdef", "Letters only"},
        {"\t\n\r ", "Various whitespace"},
        {"user@example.com", "Email format"},
        {"invalid-email", "Invalid email"},
        {"test@", "Incomplete email"},
        {"@example.com", "Missing username"},
        {"user@example", "Missing TLD"},
        {"user.name@example.com", "Email with dot"},
        {"user+tag@example.co.uk", "Email with plus and multiple TLD"}
    };
    
    std::cout << std::left 
              << std::setw(25) << "Input" 
              << std::setw(12) << "Empty?" 
              << std::setw(15) << "Alphanumeric?" 
              << std::setw(13) << "Valid Email?" 
              << std::setw(20) << "Description" << std::endl;
    std::cout << std::string(85, '-') << std::endl;
    
    for (const auto& test : test_cases) {
        std::string display_input = test.input;
        if (display_input.empty()) {
            display_input = "(empty)";
        } else if (display_input.find_first_not_of(" \t\n\r") == std::string::npos) {
            display_input = "(whitespace)";
        }
        
        bool is_empty = cpp_template::validation::isEmpty(test.input);
        bool is_alphanum = cpp_template::validation::isAlphanumeric(test.input);
        bool is_email = cpp_template::validation::isValidEmail(test.input);
        
        std::cout << std::left 
                  << std::setw(25) << display_input
                  << std::setw(12) << (is_empty ? "Yes" : "No")
                  << std::setw(15) << (is_alphanum ? "Yes" : "No")
                  << std::setw(13) << (is_email ? "Yes" : "No")
                  << std::setw(20) << test.description << std::endl;
    }
    std::cout << std::endl;
}

void demonstrateFormValidation() {
    std::cout << "=== Form Validation Scenario ===" << std::endl;
    
    struct FormData {
        std::string username;
        std::string email;
        std::string password;
        std::string confirm_password;
    };
    
    std::vector<FormData> test_forms = {
        {"john123", "john@example.com", "password123", "password123"},
        {"", "john@example.com", "password123", "password123"},  // Empty username
        {"john123", "invalid-email", "password123", "password123"},  // Invalid email
        {"john123", "john@example.com", "", "password123"},  // Empty password
        {"john123", "john@example.com", "password123", "different"},  // Password mismatch
        {"user with spaces", "user@example.com", "pass", "pass"},  // Invalid username
        {"validuser", "user@example.com", "validpass", "validpass"}  // All valid
    };
    
    std::cout << "Validating form submissions:" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    
    for (size_t i = 0; i < test_forms.size(); ++i) {
        const auto& form = test_forms[i];
        std::cout << "Form " << (i + 1) << ":" << std::endl;
        
        bool valid = true;
        std::vector<std::string> errors;
        
        // Validate username
        if (cpp_template::validation::isEmpty(form.username)) {
            errors.push_back("Username cannot be empty");
            valid = false;
        } else if (!cpp_template::validation::isAlphanumeric(form.username)) {
            errors.push_back("Username must be alphanumeric");
            valid = false;
        }
        
        // Validate email
        if (cpp_template::validation::isEmpty(form.email)) {
            errors.push_back("Email cannot be empty");
            valid = false;
        } else if (!cpp_template::validation::isValidEmail(form.email)) {
            errors.push_back("Email format is invalid");
            valid = false;
        }
        
        // Validate password
        if (cpp_template::validation::isEmpty(form.password)) {
            errors.push_back("Password cannot be empty");
            valid = false;
        }
        
        // Validate password confirmation
        if (form.password != form.confirm_password) {
            errors.push_back("Passwords do not match");
            valid = false;
        }
        
        // Display results
        std::cout << "  Username: " << (form.username.empty() ? "(empty)" : form.username) << std::endl;
        std::cout << "  Email: " << (form.email.empty() ? "(empty)" : form.email) << std::endl;
        std::cout << "  Password: " << (form.password.empty() ? "(empty)" : "[hidden]") << std::endl;
        std::cout << "  Status: " << (valid ? "✓ VALID" : "✗ INVALID") << std::endl;
        
        if (!valid) {
            std::cout << "  Errors:" << std::endl;
            for (const auto& error : errors) {
                std::cout << "    - " << error << std::endl;
            }
        }
        std::cout << std::endl;
    }
}

void demonstrateDataCleaning() {
    std::cout << "=== Data Cleaning Scenario ===" << std::endl;
    
    std::vector<std::string> raw_data = {
        "  john@example.com  ",
        "JANE@EXAMPLE.COM",
        "",
        "   ",
        "invalid-email",
        "user@domain",
        "valid.user@example.co.uk",
        "user+tag@example.com",
        "  ADMIN@COMPANY.COM  "
    };
    
    std::cout << "Cleaning and validating email list:" << std::endl;
    std::cout << std::string(50, '-') << std::endl;
    
    std::vector<std::string> clean_emails;
    
    for (const auto& raw_email : raw_data) {
        std::cout << "Raw: \"" << raw_email << "\"" << std::endl;
        
        // Step 1: Check if empty or whitespace
        if (cpp_template::validation::isEmpty(raw_email)) {
            std::cout << "  → Skipped (empty/whitespace)" << std::endl;
            continue;
        }
        
        // Step 2: Clean the email (trim and lowercase)
        std::string cleaned = raw_email;
        
        // Simple trim implementation
        cleaned.erase(0, cleaned.find_first_not_of(" \t\n\r"));
        cleaned.erase(cleaned.find_last_not_of(" \t\n\r") + 1);
        
        // Convert to lowercase
        cleaned = cpp_template::string_utils::toLower(cleaned);
        
        std::cout << "  Cleaned: \"" << cleaned << "\"" << std::endl;
        
        // Step 3: Validate the cleaned email
        if (cpp_template::validation::isValidEmail(cleaned)) {
            clean_emails.push_back(cleaned);
            std::cout << "  → ✓ Added to clean list" << std::endl;
        } else {
            std::cout << "  → ✗ Invalid email format" << std::endl;
        }
        std::cout << std::endl;
    }
    
    std::cout << "Final clean email list (" << clean_emails.size() << " emails):" << std::endl;
    for (size_t i = 0; i < clean_emails.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << clean_emails[i] << std::endl;
    }
    std::cout << std::endl;
}

void demonstrateCustomValidation() {
    std::cout << "=== Custom Validation Patterns ===" << std::endl;
    
    // Custom validation using the basic validation functions
    auto isValidUsername = [](const std::string& username) -> std::pair<bool, std::string> {
        if (cpp_template::validation::isEmpty(username)) {
            return {false, "Username cannot be empty"};
        }
        
        if (username.length() < 3) {
            return {false, "Username must be at least 3 characters"};
        }
        
        if (username.length() > 20) {
            return {false, "Username cannot exceed 20 characters"};
        }
        
        if (!cpp_template::validation::isAlphanumeric(username)) {
            return {false, "Username must contain only letters and numbers"};
        }
        
        return {true, ""};
    };
    
    auto isStrongPassword = [](const std::string& password) -> std::pair<bool, std::string> {
        if (cpp_template::validation::isEmpty(password)) {
            return {false, "Password cannot be empty"};
        }
        
        if (password.length() < 8) {
            return {false, "Password must be at least 8 characters"};
        }
        
        bool has_upper = false, has_lower = false, has_digit = false, has_special = false;
        
        for (char c : password) {
            if (std::isupper(c)) has_upper = true;
            else if (std::islower(c)) has_lower = true;
            else if (std::isdigit(c)) has_digit = true;
            else if (!std::isalnum(c)) has_special = true;
        }
        
        if (!has_upper) return {false, "Password must contain at least one uppercase letter"};
        if (!has_lower) return {false, "Password must contain at least one lowercase letter"};
        if (!has_digit) return {false, "Password must contain at least one digit"};
        if (!has_special) return {false, "Password must contain at least one special character"};
        
        return {true, ""};
    };
    
    std::vector<std::pair<std::string, std::string>> test_credentials = {
        {"john123", "Password123!"},
        {"ab", "weak"},
        {"validuser", "StrongPass123!"},
        {"user with spaces", "ValidPass123!"},
        {"toolongusernamethatexceedslimit", "ValidPass123!"},
        {"gooduser", "NoUppercase123!"},
        {"gooduser", "NOLOWERCASE123!"},
        {"gooduser", "NoDigits!"},
        {"gooduser", "NoSpecialChars123"}
    };
    
    std::cout << "Testing custom validation rules:" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    
    for (size_t i = 0; i < test_credentials.size(); ++i) {
        const auto& [username, password] = test_credentials[i];
        
        std::cout << "Test " << (i + 1) << ":" << std::endl;
        std::cout << "  Username: " << username << std::endl;
        std::cout << "  Password: [hidden]" << std::endl;
        
        auto [username_valid, username_error] = isValidUsername(username);
        auto [password_valid, password_error] = isStrongPassword(password);
        
        std::cout << "  Username validation: " << (username_valid ? "✓ Valid" : "✗ Invalid") << std::endl;
        if (!username_valid) {
            std::cout << "    Error: " << username_error << std::endl;
        }
        
        std::cout << "  Password validation: " << (password_valid ? "✓ Valid" : "✗ Invalid") << std::endl;
        if (!password_valid) {
            std::cout << "    Error: " << password_error << std::endl;
        }
        
        bool overall_valid = username_valid && password_valid;
        std::cout << "  Overall: " << (overall_valid ? "✓ VALID CREDENTIALS" : "✗ INVALID CREDENTIALS") << std::endl;
        std::cout << std::endl;
    }
}

int main() {
    std::cout << "=== Validation Utilities Comprehensive Demo ===" << std::endl;
    std::cout << "This example demonstrates validation functions and patterns" << std::endl;
    std::cout << "provided by the cpp-template library." << std::endl;
    std::cout << std::endl;
    
    try {
        demonstrateBasicValidation();
        demonstrateFormValidation();
        demonstrateDataCleaning();
        demonstrateCustomValidation();
        
        std::cout << "✓ All validation demonstrations completed successfully!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}