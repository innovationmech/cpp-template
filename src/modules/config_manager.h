#pragma once

/**
 * @file config_manager.h
 * @brief Configuration management module
 * 
 * This module demonstrates how to structure configuration management
 * within the modular architecture of the cpp-template project.
 */

#include <string>
#include <map>
#include <memory>

namespace cpp_template {
namespace modules {

/**
 * @brief Configuration manager for application settings
 * 
 * This class provides a centralized way to manage application configuration
 * and demonstrates modular architecture patterns.
 */
class ConfigManager {
public:
    /**
     * @brief Construct a new Config Manager object
     */
    ConfigManager();
    
    /**
     * @brief Destroy the Config Manager object
     */
    ~ConfigManager() = default;
    
    // Non-copyable but movable
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;
    ConfigManager(ConfigManager&&) = default;
    ConfigManager& operator=(ConfigManager&&) = default;
    
    /**
     * @brief Load configuration from a file
     * 
     * @param filename The configuration file path
     * @return true if loading was successful
     * @return false if loading failed
     */
    bool loadFromFile(const std::string& filename);
    
    /**
     * @brief Set a configuration value
     * 
     * @param key The configuration key
     * @param value The configuration value
     */
    void setValue(const std::string& key, const std::string& value);
    
    /**
     * @brief Get a configuration value
     * 
     * @param key The configuration key
     * @param defaultValue The default value if key is not found
     * @return std::string The configuration value
     */
    std::string getValue(const std::string& key, const std::string& defaultValue = "") const;
    
    /**
     * @brief Check if a configuration key exists
     * 
     * @param key The configuration key to check
     * @return true if the key exists
     * @return false if the key does not exist
     */
    bool hasKey(const std::string& key) const;
    
    /**
     * @brief Get all configuration keys
     * 
     * @return std::vector<std::string> Vector of all configuration keys
     */
    std::vector<std::string> getAllKeys() const;
    
    /**
     * @brief Clear all configuration values
     */
    void clear();

private:
    std::map<std::string, std::string> config_data_;
    bool is_loaded_;
};

/**
 * @brief Factory function to create a ConfigManager instance
 * 
 * @return std::unique_ptr<ConfigManager> A unique pointer to the created instance
 */
std::unique_ptr<ConfigManager> createConfigManager();

} // namespace modules
} // namespace cpp_template