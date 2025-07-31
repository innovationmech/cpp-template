#include <iostream>
#include "cpp-template/cpp-template.hpp"

int main() {
    std::cout << "Welcome to " << cpp_template::getProjectInfo() << std::endl;
    
    // Initialize core library
    cpp_template::core::Core core;
    if (!core.initialize()) {
        std::cerr << "Failed to initialize core library" << std::endl;
        return 1;
    }
    
    std::cout << "Application started successfully!" << std::endl;
    std::cout << "This is a modern C++ project template demonstrating:" << std::endl;
    std::cout << "- Modular architecture" << std::endl;
    std::cout << "- CMake build system" << std::endl;
    std::cout << "- Multiple dependency management approaches" << std::endl;
    std::cout << "- Modern C++ practices" << std::endl;
    
    // Cleanup
    core.cleanup();
    
    std::cout << "Application finished successfully!" << std::endl;
    return 0;
}