#pragma once

/**
 * @file utils.h
 * @brief Public utility functions for the cpp-template library
 *
 * This header provides utility functions that are commonly needed
 * when working with the cpp-template library. These utilities are
 * designed to be lightweight and efficient.
 */

#include <string>
#include <vector>

namespace cpp_template {

/**
 * @brief Utility functions for string manipulation
 */
namespace string_utils {

/**
 * @brief Convert a string to uppercase
 *
 * @param input The input string
 * @return std::string The uppercase version of the input
 */
std::string toUpper(const std::string& input);

/**
 * @brief Convert a string to lowercase
 *
 * @param input The input string
 * @return std::string The lowercase version of the input
 */
std::string toLower(const std::string& input);

/**
 * @brief Split a string by a delimiter
 *
 * @param input The input string to split
 * @param delimiter The delimiter character
 * @return std::vector<std::string> Vector of split strings
 */
std::vector<std::string> split(const std::string& input, char delimiter);

/**
 * @brief Join a vector of strings with a delimiter
 *
 * @param strings The vector of strings to join
 * @param delimiter The delimiter string
 * @return std::string The joined string
 */
std::string join(const std::vector<std::string>& strings, const std::string& delimiter);

}  // namespace string_utils

/**
 * @brief Utility functions for validation
 */
namespace validation {

/**
 * @brief Check if a string is empty or contains only whitespace
 *
 * @param input The string to check
 * @return true if the string is empty or whitespace-only
 * @return false otherwise
 */
bool isEmpty(const std::string& input);

/**
 * @brief Validate that a string contains only alphanumeric characters
 *
 * @param input The string to validate
 * @return true if the string is alphanumeric
 * @return false otherwise
 */
bool isAlphanumeric(const std::string& input);

/**
 * @brief Validate an email address format
 *
 * @param email The email string to validate
 * @return true if the email format is valid
 * @return false otherwise
 */
bool isValidEmail(const std::string& email);

}  // namespace validation

}  // namespace cpp_template
