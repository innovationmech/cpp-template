/**
 * Dependency Management Demo
 * This example demonstrates how to use different types of dependencies:
 * 1. vcpkg dependencies (gtest)
 * 2. Git submodules (nlohmann/json)
 * 3. Manual dependencies (example_lib)
 * 4. System dependencies (standard library)
 */

#include <iostream>
#include <string>
#include <vector>

// Include headers from different dependency sources
#include "cpp-template/core/core.hpp"  // Internal library
#include "example_lib/math_utils.h"     // Manual dependency

// Conditional includes for optional dependencies
#ifdef NLOHMANN_JSON_VERSION_MAJOR
#include <nlohmann/json.hpp>            // Git submodule
#endif

int main() {
    std::cout << "=== C++ Template Dependency Management Demo ===\n\n";
    
    // 1. Using internal core library
    std::cout << "1. Internal Core Library:\n";
    cpp_template::core::Core core;
    std::cout << "   Core version: " << core.getVersion() << "\n";
    std::cout << "   Core initialized: " << (core.isInitialized() ? "Yes" : "No") << "\n\n";
    
    // 2. Using manual dependency (example_lib)
    std::cout << "2. Manual Dependency (example_lib):\n";
    int factorial_input = 5;
    long long factorial_result = example_lib::MathUtils::factorial(factorial_input);
    std::cout << "   Factorial of " << factorial_input << " = " << factorial_result << "\n";
    
    int gcd_a = 48, gcd_b = 18;
    int gcd_result = example_lib::MathUtils::gcd(gcd_a, gcd_b);
    std::cout << "   GCD of " << gcd_a << " and " << gcd_b << " = " << gcd_result << "\n";
    
    int prime_check = 17;
    bool is_prime = example_lib::MathUtils::isPrime(prime_check);
    std::cout << "   Is " << prime_check << " prime? " << (is_prime ? "Yes" : "No") << "\n\n";
    
    // 3. Using Git submodule (nlohmann/json) if available
#ifdef NLOHMANN_JSON_VERSION_MAJOR
    std::cout << "3. Git Submodule (nlohmann/json):\n";
    nlohmann::json demo_json;
    demo_json["project"] = "cpp-template";
    demo_json["version"] = "1.0.0";
    demo_json["dependencies"] = nlohmann::json::array({
        "vcpkg", "git-submodules", "manual", "system"
    });
    demo_json["features"] = {
        {"testing", true},
        {"documentation", true},
        {"cross_platform", true}
    };
    
    std::cout << "   JSON data: " << demo_json.dump(2) << "\n\n";
#else
    std::cout << "3. Git Submodule (nlohmann/json): Not available\n";
    std::cout << "   Note: Include nlohmann/json in your build to enable JSON features\n\n";
#endif
    
    // 4. Using system dependencies (standard library)
    std::cout << "4. System Dependencies (Standard Library):\n";
    std::vector<std::string> dependency_types = {
        "vcpkg (Package Manager)",
        "Git Submodules", 
        "Manual Dependencies",
        "System Package Managers"
    };
    
    std::cout << "   Supported dependency management approaches:\n";
    for (size_t i = 0; i < dependency_types.size(); ++i) {
        std::cout << "   " << (i + 1) << ". " << dependency_types[i] << "\n";
    }
    
    std::cout << "\n=== Demo Complete ===\n";
    std::cout << "This example shows how to integrate multiple dependency types\n";
    std::cout << "in a single C++ project using modern CMake practices.\n";
    
    return 0;
}