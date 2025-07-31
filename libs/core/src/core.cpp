#include "cpp-template/core/core.hpp"
#include <iostream>

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

} // namespace core
} // namespace cpp_template