#include "cpp-template/utils.h"
#include "core/utils.h" // Internal core library utils
#include <regex>

namespace cpp_template {
namespace string_utils {

std::string toUpper(const std::string& input) {
    return core::utils::string::toUpper(input);
}

std::string toLower(const std::string& input) {
    return core::utils::string::toLower(input);
}

std::vector<std::string> split(const std::string& input, char delimiter) {
    return core::utils::string::split(input, delimiter);
}

std::string join(const std::vector<std::string>& strings, const std::string& delimiter) {
    return core::utils::string::join(strings, delimiter);
}

} // namespace string_utils

namespace validation {

bool isEmpty(const std::string& input) {
    return core::utils::validation::isEmpty(input);
}

bool isAlphanumeric(const std::string& input) {
    return core::utils::validation::isAlphanumeric(input);
}

bool isValidEmail(const std::string& email) {
    // Simple email validation using regex
    const std::regex email_pattern(
        R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)"
    );
    return std::regex_match(email, email_pattern);
}

} // namespace validation
} // namespace cpp_template