#include "cpp-template/core/core.hpp"
#include <iostream>
#include <sstream>

namespace cpp_template {
namespace core {

std::string Core::getProjectName() {
    return "cpp-template";
}

std::string Core::getVersion() {
    return "1.0.0";
}

bool Core::initialize() {
    if (m_initialized) {
        std::cout << "Core library already initialized" << std::endl;
        return true;
    }
    
    std::cout << "Initializing " << getProjectName() << " v" << getVersion() << std::endl;
    m_initialized = true;
    return true;
}

void Core::cleanup() {
    if (m_initialized) {
        std::cout << "Cleaning up core library" << std::endl;
        m_initialized = false;
    }
}

bool Core::isInitialized() const {
    return m_initialized;
}

namespace utils {

std::string getFormattedProjectInfo() {
    std::ostringstream oss;
    oss << "Project: " << Core::getProjectName() 
        << " (Version: " << Core::getVersion() << ")";
    return oss.str();
}

bool validateConfiguration() {
    // Basic configuration validation
    if (Core::getProjectName().empty()) {
        std::cerr << "Error: Project name is empty" << std::endl;
        return false;
    }
    
    if (Core::getVersion().empty()) {
        std::cerr << "Error: Project version is empty" << std::endl;
        return false;
    }
    
    std::cout << "Configuration validation passed" << std::endl;
    return true;
}

} // namespace utils

} // namespace core
} // namespace cpp_template