#pragma once

/**
 * @file core.hpp
 * @brief Core library header file for cpp-template
 * 
 * This file contains the main interface for the core library.
 * It provides essential functionality that can be used across
 * the entire project.
 */

#include <string>
#include <memory>
#include <vector>

namespace core {

/**
 * @brief Core library version information
 */
struct Version {
    int major;
    int minor;
    int patch;
    
    /**
     * @brief Get version as string
     * @return Version string in format "major.minor.patch"
     */
    std::string toString() const;
};

/**
 * @brief Get the current version of the core library
 * @return Version information
 */
Version getVersion();

/**
 * @brief Core utility class
 * 
 * This class provides essential utility functions that can be
 * used throughout the project.
 */
class CoreUtils {
public:
    /**
     * @brief Initialize the core library
     * @return true if initialization was successful, false otherwise
     */
    static bool initialize();
    
    /**
     * @brief Cleanup the core library
     */
    static void cleanup();
    
    /**
     * @brief Check if the core library is initialized
     * @return true if initialized, false otherwise
     */
    static bool isInitialized();
    
    /**
     * @brief Get library information
     * @return String containing library information
     */
    static std::string getLibraryInfo();
    
    /**
     * @brief Utility function to format strings
     * @param format Format string
     * @param args Arguments for formatting
     * @return Formatted string
     */
    template<typename... Args>
    static std::string format(const std::string& format, Args&&... args);
    
private:
    static bool s_initialized;
};

/**
 * @brief Configuration management class
 * 
 * This class handles configuration loading and management
 * for the application.
 */
class Config {
public:
    /**
     * @brief Load configuration from file
     * @param filename Path to configuration file
     * @return true if loaded successfully, false otherwise
     */
    bool loadFromFile(const std::string& filename);
    
    /**
     * @brief Get string value from configuration
     * @param key Configuration key
     * @param defaultValue Default value if key not found
     * @return Configuration value or default
     */
    std::string getString(const std::string& key, const std::string& defaultValue = "") const;
    
    /**
     * @brief Get integer value from configuration
     * @param key Configuration key
     * @param defaultValue Default value if key not found
     * @return Configuration value or default
     */
    int getInt(const std::string& key, int defaultValue = 0) const;
    
    /**
     * @brief Get boolean value from configuration
     * @param key Configuration key
     * @param defaultValue Default value if key not found
     * @return Configuration value or default
     */
    bool getBool(const std::string& key, bool defaultValue = false) const;
    
    /**
     * @brief Set string value in configuration
     * @param key Configuration key
     * @param value Value to set
     */
    void setString(const std::string& key, const std::string& value);
    
    /**
     * @brief Set integer value in configuration
     * @param key Configuration key
     * @param value Value to set
     */
    void setInt(const std::string& key, int value);
    
    /**
     * @brief Set boolean value in configuration
     * @param key Configuration key
     * @param value Value to set
     */
    void setBool(const std::string& key, bool value);
    
    /**
     * @brief Save configuration to file
     * @param filename Path to save configuration
     * @return true if saved successfully, false otherwise
     */
    bool saveToFile(const std::string& filename) const;
    
private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
};

/**
 * @brief Logger class for the core library
 * 
 * This class provides logging functionality with different
 * log levels and output targets.
 */
class Logger {
public:
    enum class Level {
        Debug,
        Info,
        Warning,
        Error,
        Critical
    };
    
    /**
     * @brief Get the global logger instance
     * @return Reference to the logger instance
     */
    static Logger& getInstance();
    
    /**
     * @brief Set the minimum log level
     * @param level Minimum level to log
     */
    void setLevel(Level level);
    
    /**
     * @brief Log a message
     * @param level Log level
     * @param message Message to log
     */
    void log(Level level, const std::string& message);
    
    /**
     * @brief Log a debug message
     * @param message Message to log
     */
    void debug(const std::string& message);
    
    /**
     * @brief Log an info message
     * @param message Message to log
     */
    void info(const std::string& message);
    
    /**
     * @brief Log a warning message
     * @param message Message to log
     */
    void warning(const std::string& message);
    
    /**
     * @brief Log an error message
     * @param message Message to log
     */
    void error(const std::string& message);
    
    /**
     * @brief Log a critical message
     * @param message Message to log
     */
    void critical(const std::string& message);
    
private:
    Logger() = default;
    Level m_level = Level::Info;
};

} // namespace core

// Template implementation
template<typename... Args>
std::string core::CoreUtils::format(const std::string& format, Args&&... args) {
    // Simple implementation - in a real project you might use fmt library
    // For now, just return the format string
    return format;
}