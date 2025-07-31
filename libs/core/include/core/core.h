#pragma once

#include <string>
#include <memory>

namespace cpp_template {
namespace core {

/**
 * @brief Core functionality class demonstrating library structure
 * 
 * This class provides basic functionality to demonstrate how internal
 * libraries should be structured within the cpp-template project.
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
    ~Core() = default;
    
    // Copy constructor and assignment
    Core(const Core& other) = default;
    Core& operator=(const Core& other) = default;
    
    // Move constructor and assignment
    Core(Core&& other) noexcept = default;
    Core& operator=(Core&& other) noexcept = default;
    
    /**
     * @brief Get the name of this core instance
     * 
     * @return const std::string& The name identifier
     */
    const std::string& getName() const noexcept;
    
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
    bool isInitialized() const noexcept;

private:
    std::string name_;
    bool initialized_;
};

/**
 * @brief Factory function to create Core instances
 * 
 * @param name The name for the new Core instance
 * @return std::unique_ptr<Core> A unique pointer to the created Core instance
 */
std::unique_ptr<Core> createCore(const std::string& name);

} // namespace core
} // namespace cpp_template