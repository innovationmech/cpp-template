#include "core/utils.h"
#include <algorithm>
#include <cctype>
#include <sstream>

namespace cpp_template {
namespace core {
namespace utils {
namespace string {

std::string toUpper(const std::string& input) {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return result;
}

std::string toLower(const std::string& input) {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

std::vector<std::string> split(const std::string& input, char delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(input);
    std::string item;

    while (std::getline(ss, item, delimiter)) {
        result.push_back(item);
    }

    return result;
}

std::string join(const std::vector<std::string>& strings, const std::string& delimiter) {
    if (strings.empty()) {
        return "";
    }

    std::string result = strings[0];
    for (size_t i = 1; i < strings.size(); ++i) {
        result += delimiter + strings[i];
    }

    return result;
}

}  // namespace string

namespace validation {

bool isEmpty(const std::string& input) {
    return input.empty() ||
           std::all_of(input.begin(), input.end(), [](unsigned char c) { return std::isspace(c); });
}

bool isAlphanumeric(const std::string& input) {
    if (input.empty()) {
        return false;
    }

    return std::all_of(input.begin(), input.end(), [](unsigned char c) { return std::isalnum(c); });
}

}  // namespace validation
}  // namespace utils
}  // namespace core
}  // namespace cpp_template
