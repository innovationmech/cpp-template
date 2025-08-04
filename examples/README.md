# cpp-template Examples

This directory contains comprehensive examples demonstrating how to use the cpp-template library and how to customize the template for your own projects.

## Table of Contents

- [Basic Usage Examples](#basic-usage-examples)
- [Advanced Usage Scenarios](#advanced-usage-scenarios)
- [Template Customization Guide](#template-customization-guide)
- [Integration Examples](#integration-examples)
- [Performance Examples](#performance-examples)

## Basic Usage Examples

### 1. Simple Core Usage (`basic_core_usage.cpp`)

Demonstrates the fundamental usage of the core library:

```cpp
#include <cpp-template/cpp-template.h>
#include <iostream>

int main() {
    // Create a core instance
    auto core = cpp_template::createCore("ExampleCore");

    // Initialize the core
    if (!core->initialize()) {
        std::cerr << "Failed to initialize core" << std::endl;
        return 1;
    }

    // Process some data
    std::string result = core->process("Hello, World!");
    std::cout << "Processed result: " << result << std::endl;

    return 0;
}
```

### 2. String Utilities (`string_utilities_demo.cpp`)

Shows how to use the built-in string utility functions:

```cpp
#include <cpp-template/utils.h>
#include <iostream>
#include <vector>

int main() {
    using namespace cpp_template::string_utils;

    std::string text = "Hello World Example";

    // Case conversion
    std::cout << "Original: " << text << std::endl;
    std::cout << "Uppercase: " << toUpper(text) << std::endl;
    std::cout << "Lowercase: " << toLower(text) << std::endl;

    // String splitting and joining
    auto words = split(text, ' ');
    std::cout << "Split into " << words.size() << " words" << std::endl;

    std::string rejoined = join(words, "-");
    std::cout << "Rejoined: " << rejoined << std::endl;

    return 0;
}
```

### 3. Validation Examples (`validation_demo.cpp`)

Demonstrates input validation capabilities:

```cpp
#include <cpp-template/utils.h>
#include <iostream>
#include <vector>

int main() {
    using namespace cpp_template::validation;

    std::vector<std::string> test_strings = {
        "hello123",      // alphanumeric
        "hello world",   // not alphanumeric
        "   ",          // empty/whitespace
        "user@example.com", // email
        "invalid-email"  // invalid email
    };

    for (const auto& str : test_strings) {
        std::cout << "Testing: '" << str << "'" << std::endl;
        std::cout << "  Empty: " << (isEmpty(str) ? "yes" : "no") << std::endl;
        std::cout << "  Alphanumeric: " << (isAlphanumeric(str) ? "yes" : "no") << std::endl;
        std::cout << "  Valid email: " << (isValidEmail(str) ? "yes" : "no") << std::endl;
        std::cout << std::endl;
    }

    return 0;
}
```

## Advanced Usage Scenarios

### 4. Modular Architecture (`modular_architecture_demo.cpp`)

Shows how to use the modular components together:

```cpp
#include <cpp-template/cpp-template.h>
#include <iostream>
#include <memory>
#include "modules/config_manager.h"
#include "modules/data_processor.h"

int main() {
    try {
        // Create configuration manager
        auto config = cpp_template::modules::createConfigManager();

        // Configure processing parameters
        config->setValue("processing.mode", "advanced");
        config->setValue("processing.batch_size", "3");
        config->setValue("app.debug", "true");

        // Create data processor with configuration dependency
        auto processor = cpp_template::modules::createDataProcessor(
            std::shared_ptr<cpp_template::modules::ConfigManager>(config.release())
        );

        // Process individual items
        std::vector<std::string> test_data = {
            "First item",
            "Second item",
            "Third item"
        };

        std::cout << "=== Individual Processing ===" << std::endl;
        for (const auto& item : test_data) {
            auto result = processor->processItem(item,
                cpp_template::modules::ProcessingMode::ADVANCED);

            if (result.success) {
                std::cout << "✓ " << result.result << std::endl;
            } else {
                std::cout << "✗ Error: " << result.error_message << std::endl;
            }
        }

        // Process as batch
        std::cout << "\n=== Batch Processing ===" << std::endl;
        auto batch_result = processor->processBatch(test_data,
            cpp_template::modules::ProcessingMode::BATCH);

        if (batch_result.success) {
            std::cout << "Batch result: " << batch_result.result << std::endl;
            std::cout << "Items processed: " << batch_result.processed_items << std::endl;
        }

        // Show statistics
        std::cout << "\n" << processor->getStatistics() << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
```

### 5. Configuration Management (`config_management_demo.cpp`)

Demonstrates advanced configuration management:

```cpp
#include "modules/config_manager.h"
#include <iostream>
#include <fstream>

// Helper function to create a sample config file
void createSampleConfigFile(const std::string& filename) {
    std::ofstream file(filename);
    file << "# Sample configuration file\n";
    file << "app.name=MyApplication\n";
    file << "app.version=2.0.0\n";
    file << "database.host=localhost\n";
    file << "database.port=5432\n";
    file << "logging.level=debug\n";
    file << "processing.threads=4\n";
    file.close();
}

int main() {
    auto config = cpp_template::modules::createConfigManager();

    // Create and load from file
    std::string config_file = "sample_config.txt";
    createSampleConfigFile(config_file);

    if (config->loadFromFile(config_file)) {
        std::cout << "Configuration loaded successfully!" << std::endl;
    } else {
        std::cout << "Using default configuration" << std::endl;
    }

    // Display all configuration
    std::cout << "\n=== Current Configuration ===" << std::endl;
    auto keys = config->getAllKeys();
    for (const auto& key : keys) {
        std::cout << key << " = " << config->getValue(key) << std::endl;
    }

    // Demonstrate runtime configuration changes
    std::cout << "\n=== Runtime Configuration Changes ===" << std::endl;
    config->setValue("runtime.setting", "dynamic_value");
    config->setValue("processing.mode", "optimized");

    std::cout << "Added runtime settings:" << std::endl;
    std::cout << "runtime.setting = " << config->getValue("runtime.setting") << std::endl;
    std::cout << "processing.mode = " << config->getValue("processing.mode") << std::endl;

    // Demonstrate default values
    std::cout << "\n=== Default Value Handling ===" << std::endl;
    std::string missing_key = config->getValue("non.existent.key", "default_value");
    std::cout << "Missing key with default: " << missing_key << std::endl;

    // Clean up
    std::remove(config_file.c_str());

    return 0;
}
```

## Template Customization Guide

### 6. Creating Your Own Project (`customization_guide.md`)

See the detailed customization guide in `customization_guide.md`.

### 7. Custom Module Example (`custom_module_example/`)

A complete example of adding a new module to the template.

## Integration Examples

### 8. Third-Party Integration (`third_party_integration.cpp`)

Shows how to integrate with external libraries:

```cpp
#include <cpp-template/cpp-template.h>
#include <nlohmann/json.hpp>  // Example third-party library
#include <iostream>

int main() {
    // Use cpp-template with third-party library
    auto core = cpp_template::createCore("JsonProcessor");
    core->initialize();

    // Create JSON data
    nlohmann::json data = {
        {"name", "cpp-template"},
        {"version", "1.0.0"},
        {"features", {"modular", "modern", "tested"}}
    };

    // Process JSON string with cpp-template
    std::string json_str = data.dump();
    std::string processed = core->process(json_str);

    std::cout << "Original JSON: " << json_str << std::endl;
    std::cout << "Processed: " << processed << std::endl;

    // Parse processed result back to JSON
    try {
        auto result_json = nlohmann::json::parse(processed);
        std::cout << "Successfully parsed processed JSON" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Processed result is not valid JSON: " << e.what() << std::endl;
    }

    return 0;
}
```

## Performance Examples

### 9. Benchmarking (`performance_benchmark.cpp`)

Demonstrates how to benchmark your code:

```cpp
#include <cpp-template/cpp-template.h>
#include <chrono>
#include <iostream>
#include <vector>

class Timer {
public:
    Timer() : start_(std::chrono::high_resolution_clock::now()) {}

    double elapsed() const {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start_);
        return duration.count() / 1000.0; // Return milliseconds
    }

private:
    std::chrono::high_resolution_clock::time_point start_;
};

int main() {
    auto core = cpp_template::createCore("BenchmarkCore");
    core->initialize();

    // Benchmark single operations
    std::cout << "=== Single Operation Benchmark ===" << std::endl;
    std::vector<std::string> test_data = {
        "short",
        "medium length string",
        "this is a much longer string that will take more time to process",
        std::string(1000, 'x') // Very long string
    };

    for (const auto& data : test_data) {
        Timer timer;
        std::string result = core->process(data);
        double elapsed = timer.elapsed();

        std::cout << "Input length: " << data.length()
                  << ", Time: " << elapsed << " ms" << std::endl;
    }

    // Benchmark batch operations
    std::cout << "\n=== Batch Operation Benchmark ===" << std::endl;
    std::vector<int> batch_sizes = {10, 100, 1000, 10000};

    for (int size : batch_sizes) {
        std::vector<std::string> batch_data(size, "test string");

        Timer timer;
        for (const auto& item : batch_data) {
            core->process(item);
        }
        double elapsed = timer.elapsed();

        std::cout << "Batch size: " << size
                  << ", Total time: " << elapsed << " ms"
                  << ", Avg per item: " << (elapsed / size) << " ms" << std::endl;
    }

    return 0;
}
```

## Building and Running Examples

### Prerequisites

Make sure you have built the main cpp-template project first:

```bash
cmake -B build -S .
cmake --build build
```

### Building Examples

```bash
# Build all examples
cmake --build build --target examples

# Or build individual examples
cmake --build build --target basic_core_usage
cmake --build build --target modular_architecture_demo
```

### Running Examples

```bash
# Run from the build directory
cd build/examples

# Basic examples
./basic_core_usage
./string_utilities_demo
./validation_demo

# Advanced examples
./modular_architecture_demo
./config_management_demo
./performance_benchmark

# Integration examples (requires third-party libraries)
./third_party_integration
```

## Next Steps

After exploring these examples:

1. **Read the customization guide** to adapt the template for your project
2. **Check the developer guide** for detailed architecture information
3. **Look at the test files** for more usage patterns
4. **Experiment with the build system** to understand CMake configuration

For more information, see the main [README.md](../README.md) and [DEVELOPER_GUIDE.md](../DEVELOPER_GUIDE.md).
