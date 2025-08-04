/**
 * @file config_management_demo.cpp
 * @brief Configuration management demonstration
 *
 * This example shows advanced configuration management patterns using
 * the cpp-template configuration module, including file loading,
 * runtime updates, and validation.
 */

#include <ctime>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
#include "modules/config_manager.h"

// Helper function to create a sample configuration file
void createSampleConfigFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not create config file: " + filename);
    }

    file << "# Sample configuration file for cpp-template\n";
    file << "# Lines starting with # are comments\n";
    file << "\n";
    file << "# Application settings\n";
    file << "app.name=ConfigDemo\n";
    file << "app.version=2.1.0\n";
    file << "app.debug=true\n";
    file << "\n";
    file << "# Database configuration\n";
    file << "database.host=localhost\n";
    file << "database.port=5432\n";
    file << "database.name=demo_db\n";
    file << "database.username=demo_user\n";
    file << "database.pool_size=10\n";
    file << "\n";
    file << "# Logging configuration\n";
    file << "logging.level=debug\n";
    file << "logging.file=app.log\n";
    file << "logging.console=true\n";
    file << "logging.max_file_size=10MB\n";
    file << "\n";
    file << "# Processing settings\n";
    file << "processing.threads=4\n";
    file << "processing.batch_size=100\n";
    file << "processing.timeout=30000\n";
    file << "processing.retry_count=3\n";
    file << "\n";
    file << "# Cache settings\n";
    file << "cache.enabled=true\n";
    file << "cache.size=1024\n";
    file << "cache.ttl=3600\n";
    file << "\n";
    file.close();
}

void demonstrateBasicConfiguration() {
    std::cout << "=== Basic Configuration Management ===" << std::endl;

    // Create configuration manager
    auto config = cpp_template::modules::createConfigManager();

    // Show default configuration
    std::cout << "Default configuration:" << std::endl;
    auto default_keys = config->getAllKeys();
    for (const auto& key : default_keys) {
        std::cout << "  " << key << " = " << config->getValue(key) << std::endl;
    }
    std::cout << std::endl;

    // Add some runtime configuration
    std::cout << "Adding runtime configuration..." << std::endl;
    config->setValue("runtime.start_time", std::to_string(std::time(nullptr)));
    config->setValue("runtime.mode", "demo");
    config->setValue("runtime.user", "demo_user");

    // Show updated configuration
    std::cout << "Updated configuration:" << std::endl;
    auto updated_keys = config->getAllKeys();
    for (const auto& key : updated_keys) {
        std::cout << "  " << key << " = " << config->getValue(key) << std::endl;
    }
    std::cout << std::endl;
}

void demonstrateFileConfiguration() {
    std::cout << "=== File-Based Configuration ===" << std::endl;

    const std::string config_file = "demo_config.txt";

    try {
        // Create sample configuration file
        std::cout << "Creating sample configuration file: " << config_file << std::endl;
        createSampleConfigFile(config_file);

        // Load configuration from file
        auto config = cpp_template::modules::createConfigManager();

        std::cout << "Loading configuration from file..." << std::endl;
        if (config->loadFromFile(config_file)) {
            std::cout << "✓ Configuration loaded successfully" << std::endl;
        } else {
            std::cout << "✗ Failed to load configuration file" << std::endl;
            return;
        }

        // Display loaded configuration
        std::cout << "\nLoaded configuration:" << std::endl;
        auto keys = config->getAllKeys();
        std::cout << "Total configuration keys: " << keys.size() << std::endl;

        // Group by category
        std::map<std::string, std::vector<std::pair<std::string, std::string>>> categories;

        for (const auto& key : keys) {
            std::string value = config->getValue(key);
            size_t dot_pos = key.find('.');

            if (dot_pos != std::string::npos) {
                std::string category = key.substr(0, dot_pos);
                std::string sub_key = key.substr(dot_pos + 1);
                categories[category].push_back({sub_key, value});
            } else {
                categories["general"].push_back({key, value});
            }
        }

        // Display by category
        for (const auto& [category, items] : categories) {
            std::cout << "\n[" << category << "]" << std::endl;
            for (const auto& [sub_key, value] : items) {
                std::cout << "  " << sub_key << " = " << value << std::endl;
            }
        }

        // Clean up
        std::remove(config_file.c_str());
        std::cout << "\nCleaned up configuration file" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error in file configuration demo: " << e.what() << std::endl;
        std::remove(config_file.c_str());  // Clean up on error
    }

    std::cout << std::endl;
}

void demonstrateConfigurationValidation() {
    std::cout << "=== Configuration Validation ===" << std::endl;

    auto config = cpp_template::modules::createConfigManager();

    // Set up test configuration
    config->setValue("server.port", "8080");
    config->setValue("server.host", "localhost");
    config->setValue("database.pool_size", "10");
    config->setValue("cache.ttl", "3600");
    config->setValue("logging.level", "info");

    // Define validation rules
    struct ValidationRule {
        std::string key;
        std::string description;
        std::function<bool(const std::string&)> validator;
        std::string error_message;
    };

    std::vector<ValidationRule> rules = {
        {       "server.port",      "Server port must be a valid port number (1-65535)",
         [](const std::string& value) {
         try {
         int port = std::stoi(value);
         return port >= 1 && port <= 65535;
         } catch (...) {
         return false;
         }
         },                  "Invalid port number"},
        {"database.pool_size",          "Database pool size must be a positive integer",
         [](const std::string& value) {
         try {
         int size = std::stoi(value);
         return size > 0;
         } catch (...) {
         return false;
         }
         }, "Pool size must be a positive integer"},
        {         "cache.ttl",               "Cache TTL must be a non-negative integer",
         [](const std::string& value) {
         try {
         int ttl = std::stoi(value);
         return ttl >= 0;
         } catch (...) {
         return false;
         }
         },   "TTL must be a non-negative integer"},
        {     "logging.level", "Logging level must be one of: debug, info, warn, error",
         [](const std::string& value) {
         return value == "debug" || value == "info" || value == "warn" || value == "error";
         },                "Invalid logging level"}
    };

    // Validate configuration
    std::cout << "Validating configuration..." << std::endl;
    bool all_valid = true;

    for (const auto& rule : rules) {
        std::cout << "\nValidating: " << rule.key << std::endl;
        std::cout << "  Rule: " << rule.description << std::endl;

        if (config->hasKey(rule.key)) {
            std::string value = config->getValue(rule.key);
            std::cout << "  Value: " << value << std::endl;

            if (rule.validator(value)) {
                std::cout << "  Status: ✓ Valid" << std::endl;
            } else {
                std::cout << "  Status: ✗ Invalid - " << rule.error_message << std::endl;
                all_valid = false;
            }
        } else {
            std::cout << "  Status: ✗ Missing required configuration" << std::endl;
            all_valid = false;
        }
    }

    std::cout << "\nOverall validation: " << (all_valid ? "✓ PASSED" : "✗ FAILED") << std::endl;
    std::cout << std::endl;
}

void demonstrateConfigurationTemplates() {
    std::cout << "=== Configuration Templates ===" << std::endl;

    // Define configuration templates for different environments
    std::map<std::string, std::map<std::string, std::string>> templates = {
        {"development",
         {{"app.debug", "true"},
         {"database.host", "localhost"},
         {"database.port", "5432"},
         {"logging.level", "debug"},
         {"cache.enabled", "false"},
         {"processing.threads", "2"}}},
        {    "testing",
         {{"app.debug", "true"},
         {"database.host", "test-db"},
         {"database.port", "5433"},
         {"logging.level", "info"},
         {"cache.enabled", "true"},
         {"processing.threads", "1"}}},
        { "production",
         {{"app.debug", "false"},
         {"database.host", "prod-db.example.com"},
         {"database.port", "5432"},
         {"logging.level", "warn"},
         {"cache.enabled", "true"},
         {"processing.threads", "8"}}}
    };

    // Demonstrate loading different templates
    for (const auto& [env_name, template_config] : templates) {
        std::cout << "Environment: " << env_name << std::endl;

        auto config = cpp_template::modules::createConfigManager();

        // Apply template
        for (const auto& [key, value] : template_config) {
            config->setValue(key, value);
        }

        // Display configuration
        std::cout << "Configuration:" << std::endl;
        for (const auto& [key, value] : template_config) {
            std::cout << "  " << key << " = " << value << std::endl;
        }

        // Show some computed values based on configuration
        bool debug_mode = config->getValue("app.debug") == "true";
        int thread_count = std::stoi(config->getValue("processing.threads", "1"));
        bool cache_enabled = config->getValue("cache.enabled") == "true";

        std::cout << "Computed settings:" << std::endl;
        std::cout << "  Debug mode: " << (debug_mode ? "enabled" : "disabled") << std::endl;
        std::cout << "  Thread pool size: " << thread_count << std::endl;
        std::cout << "  Caching: " << (cache_enabled ? "enabled" : "disabled") << std::endl;
        std::cout << std::endl;
    }
}

void demonstrateConfigurationHierarchy() {
    std::cout << "=== Configuration Hierarchy ===" << std::endl;

    // Simulate configuration hierarchy: defaults < file < environment < command line
    auto config = cpp_template::modules::createConfigManager();

    std::cout << "1. Default configuration:" << std::endl;
    auto keys = config->getAllKeys();
    for (const auto& key : keys) {
        std::cout << "  " << key << " = " << config->getValue(key) << std::endl;
    }

    // Simulate file-based configuration (higher priority)
    std::cout << "\n2. Applying file-based configuration..." << std::endl;
    std::map<std::string, std::string> file_config = {
        {        "app.name", "FileConfiguredApp"},
        {     "app.version",             "2.0.0"},
        {   "database.host",      "file-db-host"},
        {"new.file.setting",         "from_file"}
    };

    for (const auto& [key, value] : file_config) {
        config->setValue(key, value);
        std::cout << "  Set " << key << " = " << value << std::endl;
    }

    // Simulate environment variables (even higher priority)
    std::cout << "\n3. Applying environment variable overrides..." << std::endl;
    std::map<std::string, std::string> env_config = {
        {  "database.host", "env-db-host"},
        {      "app.debug",        "true"},
        {"new.env.setting",    "from_env"}
    };

    for (const auto& [key, value] : env_config) {
        config->setValue(key, value);
        std::cout << "  Override " << key << " = " << value << std::endl;
    }

    // Simulate command line arguments (highest priority)
    std::cout << "\n4. Applying command line overrides..." << std::endl;
    std::map<std::string, std::string> cli_config = {
        {         "app.debug",    "false"},
        {"processing.threads",       "16"},
        {   "new.cli.setting", "from_cli"}
    };

    for (const auto& [key, value] : cli_config) {
        config->setValue(key, value);
        std::cout << "  Override " << key << " = " << value << std::endl;
    }

    // Show final configuration
    std::cout << "\n5. Final configuration (after all overrides):" << std::endl;
    auto final_keys = config->getAllKeys();
    for (const auto& key : final_keys) {
        std::cout << "  " << key << " = " << config->getValue(key) << std::endl;
    }

    std::cout << std::endl;
}

int main() {
    std::cout << "=== Configuration Management Comprehensive Demo ===" << std::endl;
    std::cout << "This example demonstrates advanced configuration management" << std::endl;
    std::cout << "patterns using the cpp-template configuration module." << std::endl;
    std::cout << std::endl;

    try {
        demonstrateBasicConfiguration();
        std::cout << std::string(60, '=') << std::endl;

        demonstrateFileConfiguration();
        std::cout << std::string(60, '=') << std::endl;

        demonstrateConfigurationValidation();
        std::cout << std::string(60, '=') << std::endl;

        demonstrateConfigurationTemplates();
        std::cout << std::string(60, '=') << std::endl;

        demonstrateConfigurationHierarchy();
        std::cout << std::string(60, '=') << std::endl;

        std::cout << "=== Configuration Best Practices ===" << std::endl;
        std::cout << "1. Use hierarchical configuration (defaults < file < env < cli)" << std::endl;
        std::cout << "2. Validate configuration values at startup" << std::endl;
        std::cout << "3. Use environment-specific templates" << std::endl;
        std::cout << "4. Provide sensible defaults for all settings" << std::endl;
        std::cout << "5. Document all configuration options" << std::endl;
        std::cout << "6. Use type-safe configuration access when possible" << std::endl;
        std::cout << std::endl;

        std::cout << "✓ All configuration management demonstrations completed successfully!"
                  << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
