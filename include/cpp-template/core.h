#pragma once

/**
 * @file core.h
 * @brief Public interface for the cpp-template core functionality
 * 
 * This header provides the main public API for the cpp-template project's
 * core functionality. It exposes the essential classes and functions
 * needed by client code.
 */

#include <string>
#include <memory>

namespace cpp_template {

/**
 * @brief Main core class for the cpp-template library
 * 
 * This class provides the primary interface for core functionality.
 * It encapsulates the internal core implementation and provides
 * a clean public API.
 */
class Core {
public:
    /**
     * @brief Construct a new Core object
     * 
     * @param name The name identifier for this core instance
     */
    explicit Core(const std::string& name);
    
    /**
     * @brief Destroy the Core object
     */
    ~Core();
    
    // Copy constructor and assignment
    Core(const Core& other);
    Core& operator=(const Core& other);
    
    // Move constructor and assignment
    Core(Core&& other) noexcept;
    Core& operator=(Core&& other) noexcept;
    
    /**
     * @brief Get the name of this core instance
     * 
     * @return const std::string& The name identifier
     */
    const std::string& getName() const;
    
    /**
     * @brief Set a new name for this core instance
     * 
     * @param name The new name identifier
     */
    void setName(const std::string& name);
    
    /**
     * @brief Process some data and return a result
     * 
     * @param input The input data to process
     * @return std::string The processed result
     */
    std::string process(const std::string& input) const;
    
    /**
     * @brief Initialize the core system
     * 
     * @return true if initialization was successful
     * @return false if initialization failed
     */
    bool initialize();
    
    /**
     * @brief Check if the core system is initialized
     * 
     * @return true if initialized
     * @return false if not initialized
     */
    bool isInitialized() const;

private:
    class Impl; // Forward declaration for PIMPL idiom
    std::unique_ptr<Impl> pImpl_;
};

/**
 * @brief Factory function to create Core instances
 * 
 * @param name The name for the new Core instance
 * @return std::unique_ptr<Core> A unique pointer to the created Core instance
 */
std::unique_ptr<Core> createCore(const std::string& name);

/**
 * @brief Get the version of the cpp-template library
 * 
 * @return std::string The version string
 */
std::string getVersion();

} // namespace cpp_template