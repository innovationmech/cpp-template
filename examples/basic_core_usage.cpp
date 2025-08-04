/**
 * @file basic_core_usage.cpp
 * @brief Basic example demonstrating core library usage
 *
 * This example shows the fundamental usage patterns of the cpp-template
 * core library, including initialization, basic processing, and error handling.
 */

#include <cpp-template/cpp-template.h>
#include <iostream>
#include <stdexcept>

int main() {
    std::cout << "=== Basic Core Usage Example ===" << std::endl;
    std::cout << "Library: " << cpp_template::info::getName() << " v"
              << cpp_template::info::getVersion() << std::endl;
    std::cout << "Description: " << cpp_template::info::getDescription() << std::endl;
    std::cout << std::endl;

    try {
        // Step 1: Create a core instance
        std::cout << "1. Creating core instance..." << std::endl;
        auto core = cpp_template::createCore("BasicExampleCore");
        std::cout << "   Created core: " << core->getName() << std::endl;

        // Step 2: Initialize the core
        std::cout << "\n2. Initializing core..." << std::endl;
        if (!core->initialize()) {
            std::cerr << "   ERROR: Failed to initialize core" << std::endl;
            return 1;
        }
        std::cout << "   Core initialized successfully" << std::endl;
        std::cout << "   Is initialized: " << (core->isInitialized() ? "Yes" : "No") << std::endl;

        // Step 3: Process some data
        std::cout << "\n3. Processing data..." << std::endl;
        std::vector<std::string> test_inputs = {
            "Hello, World!", "cpp-template example", "Modern C++ Development",
            ""  // Empty string test
        };

        for (const auto& input : test_inputs) {
            std::cout << "   Input: \"" << input << "\"" << std::endl;

            if (input.empty()) {
                std::cout << "   Output: (skipping empty input)" << std::endl;
                continue;
            }

            std::string result = core->process(input);
            std::cout << "   Output: \"" << result << "\"" << std::endl;
        }

        // Step 4: Demonstrate core properties
        std::cout << "\n4. Core properties:" << std::endl;
        std::cout << "   Name: " << core->getName() << std::endl;

        // Change the name
        core->setName("RenamedCore");
        std::cout << "   New name: " << core->getName() << std::endl;

        // Step 5: Demonstrate copy/move semantics
        std::cout << "\n5. Copy and move operations:" << std::endl;

        // Copy constructor
        auto core_copy = *core;
        std::cout << "   Copied core name: " << core_copy.getName() << std::endl;

        // Process with copied core
        std::string copy_result = core_copy.process("Copy test");
        std::cout << "   Copy result: \"" << copy_result << "\"" << std::endl;

        std::cout << "\n✓ Basic core usage completed successfully!" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "ERROR: Unknown exception occurred" << std::endl;
        return 1;
    }

    return 0;
}
