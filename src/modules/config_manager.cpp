#include "config_manager.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

namespace cpp_template {
namespace modules {

ConfigManager::ConfigManager() : is_loaded_(false) {
    // Set some default configuration values
    config_data_["app.name"] = "cpp-template";
    config_data_["app.version"] = "1.0.0";
    config_data_["processing.mode"] = "simple";
    config_data_["processing.batch_size"] = "10";
    config_data_["logging.level"] = "info";
}

bool ConfigManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Warning: Could not open config file: " << filename << std::endl;
        return false;
    }

    // Clear existing configuration to ensure file values take precedence
    config_data_.clear();

    std::string line;
    while (std::getline(file, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') {
            continue;
        }

        // Find the '=' separator
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);

            // Trim whitespace
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            config_data_[key] = value;
        }
    }

    is_loaded_ = true;
    return true;
}

void ConfigManager::setValue(const std::string& key, const std::string& value) {
    config_data_[key] = value;
}

std::string ConfigManager::getValue(const std::string& key, const std::string& defaultValue) const {
    auto it = config_data_.find(key);
    if (it != config_data_.end()) {
        return it->second;
    }
    return defaultValue;
}

bool ConfigManager::hasKey(const std::string& key) const {
    return config_data_.find(key) != config_data_.end();
}

std::vector<std::string> ConfigManager::getAllKeys() const {
    std::vector<std::string> keys;
    keys.reserve(config_data_.size());

    for (const auto& pair : config_data_) {
        keys.push_back(pair.first);
    }

    return keys;
}

void ConfigManager::clear() {
    config_data_.clear();
    is_loaded_ = false;
}

std::unique_ptr<ConfigManager> createConfigManager() {
    return std::make_unique<ConfigManager>();
}

}  // namespace modules
}  // namespace cpp_template
