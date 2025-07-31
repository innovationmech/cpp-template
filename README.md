# C++ Project Template

A comprehensive C++ project template demonstrating modern C++ development practices with CMake build system. This template serves as a starting point for C++ projects, incorporating best practices for project structure, dependency management, and build configuration.

## Features

- **Modern C++ Standards**: Uses C++17 as the minimum standard with explicit version specification
- **Modular Architecture**: Clear separation of concerns with hierarchical project structure
- **Multiple Dependency Management**: Supports vcpkg, Git submodules, manual dependencies, and system packages
- **Comprehensive Testing**: Integrated Google Test framework with unit and integration tests
- **Custom CMake Modules**: Standardized compiler options and custom find modules
- **Cross-platform Support**: Compatible with major compilers (GCC, Clang, MSVC)
- **Complete Documentation**: Detailed setup instructions and troubleshooting guides
- **Example Code**: Demonstrates best practices and usage patterns

## Project Structure

```
cpp-template/
├── CMakeLists.txt              # Top-level build configuration
├── README.md                   # Project documentation
├── .gitmodules                 # Git submodules configuration
├── vcpkg.json                  # vcpkg manifest file
├── src/                        # Application source code
│   ├── CMakeLists.txt
│   ├── main.cpp
│   └── modules/                # Application modules
│       └── example/
├── include/                    # Public header files
│   └── cpp-template/
├── libs/                       # Internal project libraries
│   ├── CMakeLists.txt
│   └── core/                   # Core library
│       ├── include/
│       └── src/
├── tests/                      # Unit tests
│   ├── CMakeLists.txt
│   └── unit/
├── third_party/                # External dependencies
│   ├── CMakeLists.txt
│   ├── manual/                 # Manually managed dependencies
│   └── submodules/             # Git submodule dependencies
└── cmake/                      # Custom CMake modules
    ├── FindCustomLib.cmake
    └── CompilerOptions.cmake
```

## Environment Setup

### System Requirements

- **CMake**: Version 3.20 or higher
- **C++ Compiler**: C++17 compatible compiler
  - GCC 7+ (Linux/macOS)
  - Clang 5+ (Linux/macOS)
  - MSVC 2017+ (Windows)
- **Git**: For version control and submodule management
- **Build Tools**: Platform-specific build tools

### Platform-Specific Setup

#### macOS Setup

1. **Install Xcode Command Line Tools**:
   ```bash
   xcode-select --install
   ```

2. **Install Homebrew** (if not already installed):
   ```bash
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   ```

3. **Install CMake and dependencies**:
   ```bash
   brew install cmake git
   ```

4. **Optional: Install system dependencies**:
   ```bash
   brew install gtest boost openssl
   ```

#### Ubuntu/Debian Setup

1. **Update package manager**:
   ```bash
   sudo apt update
   ```

2. **Install build essentials**:
   ```bash
   sudo apt install build-essential cmake git
   ```

3. **Install C++ compiler**:
   ```bash
   sudo apt install g++ clang
   ```

4. **Optional: Install system dependencies**:
   ```bash
   sudo apt install libgtest-dev libboost-all-dev libssl-dev
   ```

#### Windows Setup

1. **Install Visual Studio 2017 or later** with C++ development tools

2. **Install CMake**:
   - Download from [cmake.org](https://cmake.org/download/)
   - Or use chocolatey: `choco install cmake`

3. **Install Git**:
   - Download from [git-scm.com](https://git-scm.com/)
   - Or use chocolatey: `choco install git`

4. **Install vcpkg** (recommended for Windows):
   ```cmd
   git clone https://github.com/Microsoft/vcpkg.git
   cd vcpkg
   .\bootstrap-vcpkg.bat
   .\vcpkg integrate install
   ```

## Quick Start

### Building the Project

1. **Clone the repository**:
   ```bash
   git clone <repository-url>
   cd cpp-template
   ```

2. **Create build directory**:
   ```bash
   mkdir build
   cd build
   ```

3. **Configure with CMake**:
   ```bash
   cmake ..
   ```

4. **Build the project**:
   ```bash
   cmake --build .
   ```

5. **Run the application**:
   ```bash
   ./cpp-template
   ```

6. **Run tests** (if Google Test is available):
   ```bash
   ctest
   ```

## Dependency Management

This template supports four distinct dependency management approaches, each with specific use cases and advantages:

### 1. vcpkg Package Manager (Recommended)

**Best for**: Popular C++ libraries with active maintenance and cross-platform support.

#### Setup vcpkg

1. **Clone vcpkg repository**:
   ```bash
   git clone https://github.com/Microsoft/vcpkg.git
   cd vcpkg
   ```

2. **Bootstrap vcpkg**:
   ```bash
   # Linux/macOS
   ./bootstrap-vcpkg.sh
   
   # Windows
   .\bootstrap-vcpkg.bat
   ```

3. **Integrate with system** (optional but recommended):
   ```bash
   ./vcpkg integrate install
   ```

#### Using vcpkg with this project

1. **Install dependencies** (automatically handled by CMake):
   ```bash
   # Dependencies are defined in vcpkg.json and installed automatically
   cmake -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake ..
   ```

2. **Add new dependencies**:
   - Edit `vcpkg.json` to add new packages
   - Update `third_party/vcpkg/CMakeLists.txt` to find and link the package
   - Example:
     ```json
     {
       "dependencies": [
         "gtest",
         "nlohmann-json",
         "boost-system"
       ]
     }
     ```

3. **Configure CMake targets**:
   ```cmake
   find_package(nlohmann_json CONFIG REQUIRED)
   target_link_libraries(your_target PRIVATE nlohmann_json::nlohmann_json)
   ```

### 2. Git Submodules

**Best for**: Libraries you need to modify, specific versions, or header-only libraries.

#### Adding a new Git submodule

1. **Add submodule**:
   ```bash
   git submodule add https://github.com/nlohmann/json.git third_party/submodules/nlohmann_json
   ```

2. **Initialize and update submodules**:
   ```bash
   git submodule update --init --recursive
   ```

3. **Configure in CMake**:
   - Edit `third_party/submodules/CMakeLists.txt`
   - Add subdirectory and create alias target:
     ```cmake
     # Add the submodule
     add_subdirectory(nlohmann_json)
     
     # Create alias for consistent naming
     if(TARGET nlohmann_json AND NOT TARGET submodules::json)
         add_library(submodules::json ALIAS nlohmann_json)
     endif()
     ```

#### Managing submodules

1. **Update submodules to latest**:
   ```bash
   git submodule update --remote
   ```

2. **Update specific submodule**:
   ```bash
   cd third_party/submodules/nlohmann_json
   git checkout v3.11.2
   cd ../../..
   git add third_party/submodules/nlohmann_json
   git commit -m "Update nlohmann_json to v3.11.2"
   ```

3. **Clone project with submodules**:
   ```bash
   git clone --recursive <repository-url>
   # or
   git clone <repository-url>
   git submodule update --init --recursive
   ```

### 3. Manual Dependencies

**Best for**: Custom libraries, modified third-party code, or libraries not available through package managers.

#### Adding a manual dependency

1. **Create library structure**:
   ```bash
   mkdir -p third_party/manual/my_library/{include,src}
   ```

2. **Add library files**:
   ```
   third_party/manual/my_library/
   ├── CMakeLists.txt
   ├── include/my_library/
   │   └── my_library.h
   └── src/
       └── my_library.cpp
   ```

3. **Create CMakeLists.txt for the library**:
   ```cmake
   # third_party/manual/my_library/CMakeLists.txt
   add_library(my_library STATIC
       src/my_library.cpp
   )
   
   target_include_directories(my_library
       PUBLIC include
       PRIVATE src
   )
   
   # Create alias for consistent naming
   add_library(manual::my_library ALIAS my_library)
   ```

4. **Integrate with main build**:
   - Edit `third_party/manual/CMakeLists.txt`:
     ```cmake
     # Add the library
     if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/my_library/CMakeLists.txt)
         add_subdirectory(my_library)
         if(TARGET my_library)
             target_link_libraries(manual_dependencies INTERFACE manual::my_library)
         endif()
     endif()
     ```

### 4. System Package Manager Dependencies

**Best for**: System-level libraries, development tools, and platform-specific dependencies.

#### macOS with Homebrew

1. **Install dependencies**:
   ```bash
   brew install boost openssl zlib curl
   ```

2. **Configure CMake to find system packages**:
   ```cmake
   # CMake will automatically find Homebrew-installed packages
   find_package(Boost REQUIRED COMPONENTS system filesystem)
   find_package(OpenSSL REQUIRED)
   ```

#### Ubuntu/Debian with apt

1. **Install development packages**:
   ```bash
   sudo apt install libboost-all-dev libssl-dev zlib1g-dev libcurl4-openssl-dev
   ```

2. **Use in CMake**:
   ```cmake
   find_package(Boost REQUIRED COMPONENTS system filesystem)
   find_package(OpenSSL REQUIRED)
   find_package(ZLIB REQUIRED)
   find_package(CURL REQUIRED)
   ```

#### Windows with vcpkg (System-wide)

1. **Install vcpkg globally**:
   ```cmd
   vcpkg install boost:x64-windows openssl:x64-windows zlib:x64-windows curl:x64-windows
   ```

2. **Use system-wide vcpkg**:
   ```bash
   cmake -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake ..
   ```

## Development

### Adding New Libraries

1. Create a new directory under `libs/`
2. Add CMakeLists.txt with library configuration
3. Update `libs/CMakeLists.txt` to include the new subdirectory
4. Add public headers to `include/cpp-template/`

### Adding New Modules

1. Create a new directory under `src/modules/`
2. Implement module functionality
3. Update `src/modules/CMakeLists.txt` to include the new module
4. Add corresponding tests in `tests/unit/`

### Testing

The project uses Google Test for unit testing:

- Unit tests are located in `tests/unit/`
- Each library/module should have corresponding tests
- Run tests with `ctest` or individual test executables

## Customization

### Compiler Options

Modify `cmake/CompilerOptions.cmake` to adjust:
- Warning levels
- Optimization flags
- Debug information
- Platform-specific options

### Project Configuration

Update the top-level `CMakeLists.txt` to change:
- Project name and version
- C++ standard requirement
- Global compile definitions

## Troubleshooting

### Common Build Issues

#### CMake Configuration Errors

**Problem**: `CMake Error: Could not find a package configuration file provided by "GTest"`

**Solution**:
1. Ensure vcpkg is properly configured:
   ```bash
   cmake -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake ..
   ```
2. Or install GTest via system package manager:
   ```bash
   # macOS
   brew install gtest
   
   # Ubuntu
   sudo apt install libgtest-dev
   ```

**Problem**: `CMake Error: The source directory does not appear to contain CMakeLists.txt`

**Solution**: Ensure you're running CMake from the correct directory:
```bash
cd cpp-template
mkdir build && cd build
cmake ..
```

#### Compiler Errors

**Problem**: `error: 'std::filesystem' has not been declared`

**Solution**: Ensure you're using a C++17 compatible compiler:
- GCC 8+ or Clang 7+ for full std::filesystem support
- Add filesystem linking if needed:
  ```cmake
  target_link_libraries(your_target PRIVATE std::filesystem)
  ```

**Problem**: `fatal error: 'nlohmann/json.hpp' file not found`

**Solution**: Initialize Git submodules:
```bash
git submodule update --init --recursive
```

#### Dependency Issues

**Problem**: vcpkg dependencies not found

**Solution**:
1. Verify vcpkg installation:
   ```bash
   ./vcpkg list
   ```
2. Ensure correct triplet for your platform:
   ```bash
   # Check available triplets
   ./vcpkg help triplet
   
   # Install for specific triplet
   ./vcpkg install gtest:x64-osx
   ```

**Problem**: Git submodule is empty or missing

**Solution**:
```bash
# Re-initialize submodules
git submodule deinit --all
git submodule update --init --recursive

# Or force update
git submodule foreach --recursive git reset --hard
git submodule update --init --recursive
```

### Platform-Specific Issues

#### macOS Issues

**Problem**: `xcrun: error: invalid active developer path`

**Solution**: Install or update Xcode Command Line Tools:
```bash
xcode-select --install
```

**Problem**: Homebrew packages not found by CMake

**Solution**: Set CMAKE_PREFIX_PATH:
```bash
export CMAKE_PREFIX_PATH="/opt/homebrew;/usr/local"
cmake ..
```

#### Windows Issues

**Problem**: `MSVC compiler not found`

**Solution**: 
1. Install Visual Studio with C++ development tools
2. Use Developer Command Prompt or PowerShell
3. Or specify generator explicitly:
   ```cmd
   cmake -G "Visual Studio 16 2019" ..
   ```

**Problem**: vcpkg integration issues

**Solution**:
```cmd
# Re-run integration
vcpkg integrate install

# Or use toolchain file explicitly
cmake -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake ..
```

#### Linux Issues

**Problem**: `Permission denied` when building

**Solution**: Check file permissions and ownership:
```bash
sudo chown -R $USER:$USER .
chmod -R 755 .
```

**Problem**: Missing development headers

**Solution**: Install development packages:
```bash
# Ubuntu/Debian
sudo apt install build-essential libc6-dev

# CentOS/RHEL
sudo yum groupinstall "Development Tools"
sudo yum install glibc-devel
```

### Performance Issues

**Problem**: Slow compilation times

**Solutions**:
1. Use parallel builds:
   ```bash
   cmake --build . --parallel $(nproc)
   ```
2. Enable ccache if available:
   ```bash
   export CMAKE_CXX_COMPILER_LAUNCHER=ccache
   ```
3. Use Ninja generator for faster builds:
   ```bash
   cmake -G Ninja ..
   ninja
   ```

### Getting Help

If you encounter issues not covered here:

1. **Check the logs**: Look at detailed CMake and compiler output
2. **Verify environment**: Ensure all prerequisites are installed
3. **Clean build**: Remove `build/` directory and reconfigure
4. **Check documentation**: Review dependency-specific documentation
5. **Search issues**: Look for similar problems in project issues
6. **Create issue**: Provide detailed error messages and system information

## API Reference

### Core Library (`src/core/`)

#### Core Class

```cpp
class Core {
public:
    Core();
    ~Core();
    
    // Initialize the core system
    bool initialize();
    
    // Process data through the core system
    std::string process(const std::string& input);
    
    // Get current version
    static std::string getVersion();
    
    // Cleanup resources
    void cleanup();
    
private:
    bool initialized_;
};
```

**Methods:**

- `Core()`: Default constructor. Initializes the core system.
- `~Core()`: Destructor. Automatically calls cleanup().
- `initialize()`: Initializes the core system. Returns `true` on success.
- `process(const std::string& input)`: Processes input data and returns processed result.
- `getVersion()`: Static method returning the current version string.
- `cleanup()`: Releases all allocated resources.

**Example Usage:**
```cpp
#include "core/core.h"

int main() {
    Core core;
    if (core.initialize()) {
        std::string result = core.process("Hello, World!");
        std::cout << "Result: " << result << std::endl;
    }
    return 0;
}
```

### Modules Library (`src/modules/`)

#### DataProcessor Class

```cpp
class DataProcessor {
public:
    DataProcessor();
    
    // Process raw data
    std::vector<std::string> processData(const std::vector<std::string>& data);
    
    // Set processing options
    void setOptions(const ProcessingOptions& options);
    
    // Get processing statistics
    ProcessingStats getStats() const;
    
private:
    ProcessingOptions options_;
    ProcessingStats stats_;
};
```

**Methods:**

- `DataProcessor()`: Constructor with default processing options.
- `processData(const std::vector<std::string>& data)`: Processes a vector of strings and returns processed results.
- `setOptions(const ProcessingOptions& options)`: Configures processing behavior.
- `getStats()`: Returns current processing statistics.

#### TextAnalyzer Class

```cpp
class TextAnalyzer {
public:
    TextAnalyzer();
    
    // Analyze text content
    AnalysisResult analyze(const std::string& text);
    
    // Get word count
    size_t getWordCount(const std::string& text);
    
    // Get character statistics
    CharacterStats getCharacterStats(const std::string& text);
    
private:
    void updateStats(const std::string& text);
};
```

**Methods:**

- `TextAnalyzer()`: Default constructor.
- `analyze(const std::string& text)`: Performs comprehensive text analysis.
- `getWordCount(const std::string& text)`: Returns word count for given text.
- `getCharacterStats(const std::string& text)`: Returns detailed character statistics.

**Example Usage:**
```cpp
#include "modules/data_processor.h"
#include "modules/text_analyzer.h"

int main() {
    // Data processing
    DataProcessor processor;
    std::vector<std::string> data = {"item1", "item2", "item3"};
    auto results = processor.processData(data);
    
    // Text analysis
    TextAnalyzer analyzer;
    std::string text = "Hello, World! This is a test.";
    auto analysis = analyzer.analyze(text);
    size_t wordCount = analyzer.getWordCount(text);
    
    return 0;
}
```

### Manual Dependencies (`third_party/manual/`)

#### ExampleLib

```cpp
namespace example_lib {
    // Mathematical operations
    int add(int a, int b);
    int multiply(int a, int b);
    double divide(double a, double b);
    
    // String utilities
    std::string toUpper(const std::string& str);
    std::string toLower(const std::string& str);
    std::vector<std::string> split(const std::string& str, char delimiter);
}
```

**Functions:**

- `add(int a, int b)`: Returns the sum of two integers.
- `multiply(int a, int b)`: Returns the product of two integers.
- `divide(double a, double b)`: Returns the quotient of two doubles. Throws exception if b is zero.
- `toUpper(const std::string& str)`: Converts string to uppercase.
- `toLower(const std::string& str)`: Converts string to lowercase.
- `split(const std::string& str, char delimiter)`: Splits string by delimiter.

### Git Submodules

#### nlohmann/json

This project uses the nlohmann/json library for JSON processing. For complete API documentation, see:
- [nlohmann/json GitHub Repository](https://github.com/nlohmann/json)
- [API Documentation](https://json.nlohmann.me/)

**Basic Usage:**
```cpp
#include <nlohmann/json.hpp>
using json = nlohmann::json;

// Create JSON object
json j;
j["name"] = "John Doe";
j["age"] = 30;
j["city"] = "New York";

// Parse JSON string
std::string jsonString = R"({"key": "value", "number": 42})";
json parsed = json::parse(jsonString);

// Access values
std::string name = j["name"];
int age = j["age"];
```

### vcpkg Dependencies

#### Google Test (GTest)

Used for unit testing. For complete documentation, see:
- [Google Test Primer](https://google.github.io/googletest/primer.html)
- [Advanced Guide](https://google.github.io/googletest/advanced.html)

**Basic Test Structure:**
```cpp
#include <gtest/gtest.h>

// Test fixture
class MyTestFixture : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code
    }
    
    void TearDown() override {
        // Cleanup code
    }
};

// Test case
TEST_F(MyTestFixture, TestName) {
    EXPECT_EQ(expected, actual);
    ASSERT_TRUE(condition);
}
```

### Error Handling

All API functions follow consistent error handling patterns:

1. **Return Values**: Functions return `bool` for success/failure or appropriate result types.
2. **Exceptions**: Critical errors throw `std::runtime_error` or derived exceptions.
3. **Logging**: Errors are logged using the internal logging system.
4. **Validation**: Input parameters are validated and documented.

### Thread Safety

- **Core Library**: Thread-safe for read operations, requires external synchronization for write operations.
- **Modules**: Not thread-safe by default. Use separate instances per thread or external synchronization.
- **Dependencies**: Refer to individual library documentation for thread safety guarantees.

## Examples and Tutorials

### Quick Start Tutorial

#### 1. Basic Project Setup

After cloning and building the project, let's create a simple application:

```cpp
// tutorial_basic.cpp
#include "core/core.h"
#include "modules/data_processor.h"
#include <iostream>

int main() {
    // Step 1: Initialize core system
    Core core;
    if (!core.initialize()) {
        std::cerr << "Failed to initialize core system" << std::endl;
        return 1;
    }
    
    // Step 2: Process some data
    std::string input = "Hello, C++ Template!";
    std::string result = core.process(input);
    std::cout << "Core processed: " << result << std::endl;
    
    // Step 3: Use modules
    DataProcessor processor;
    std::vector<std::string> data = {"item1", "item2", "item3"};
    auto processed = processor.processData(data);
    
    std::cout << "Processed items:" << std::endl;
    for (const auto& item : processed) {
        std::cout << "  - " << item << std::endl;
    }
    
    return 0;
}
```

#### 2. Working with Dependencies

```cpp
// tutorial_dependencies.cpp
#include "core/core.h"
#include "modules/text_analyzer.h"
#include "example_lib/example_lib.h"  // Manual dependency
#include <nlohmann/json.hpp>          // Git submodule
#include <iostream>

using json = nlohmann::json;

int main() {
    // Using manual dependency (example_lib)
    int sum = example_lib::add(10, 20);
    std::string upper = example_lib::toUpper("hello world");
    std::cout << "Sum: " << sum << ", Upper: " << upper << std::endl;
    
    // Using Git submodule (nlohmann/json)
    json config;
    config["app_name"] = "cpp-template";
    config["version"] = "1.0.0";
    config["features"] = json::array({"core", "modules", "dependencies"});
    
    std::cout << "Config: " << config.dump(2) << std::endl;
    
    // Using internal modules
    TextAnalyzer analyzer;
    std::string text = "This is a sample text for analysis.";
    size_t wordCount = analyzer.getWordCount(text);
    auto stats = analyzer.getCharacterStats(text);
    
    std::cout << "Text analysis - Words: " << wordCount << std::endl;
    
    return 0;
}
```

### Advanced Examples

#### 1. Custom Module Development

Create your own module following the project structure:

```cpp
// src/modules/custom_module.h
#pragma once
#include <string>
#include <vector>

class CustomModule {
public:
    CustomModule();
    
    // Your custom functionality
    std::string processCustomData(const std::string& input);
    std::vector<int> generateSequence(int start, int end);
    
private:
    void initialize();
    bool initialized_;
};
```

```cpp
// src/modules/custom_module.cpp
#include "custom_module.h"
#include <algorithm>
#include <numeric>

CustomModule::CustomModule() : initialized_(false) {
    initialize();
}

void CustomModule::initialize() {
    // Custom initialization logic
    initialized_ = true;
}

std::string CustomModule::processCustomData(const std::string& input) {
    if (!initialized_) {
        return "Error: Module not initialized";
    }
    
    // Custom processing logic
    std::string result = "Processed: " + input;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

std::vector<int> CustomModule::generateSequence(int start, int end) {
    std::vector<int> sequence(end - start + 1);
    std::iota(sequence.begin(), sequence.end(), start);
    return sequence;
}
```

#### 2. Integration Testing Example

```cpp
// tests/integration/custom_integration_test.cpp
#include <gtest/gtest.h>
#include "core/core.h"
#include "modules/custom_module.h"
#include "example_lib/example_lib.h"
#include <nlohmann/json.hpp>

class CustomIntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        core_.initialize();
    }
    
    Core core_;
    CustomModule custom_module_;
};

TEST_F(CustomIntegrationTest, FullWorkflow) {
    // Test complete workflow
    std::string input = "test data";
    
    // Process through core
    std::string core_result = core_.process(input);
    EXPECT_FALSE(core_result.empty());
    
    // Process through custom module
    std::string custom_result = custom_module_.processCustomData(core_result);
    EXPECT_TRUE(custom_result.find("PROCESSED") != std::string::npos);
    
    // Use external dependencies
    int calculation = example_lib::multiply(5, 10);
    EXPECT_EQ(calculation, 50);
    
    // JSON configuration
    nlohmann::json config;
    config["result"] = custom_result;
    config["calculation"] = calculation;
    
    EXPECT_TRUE(config.contains("result"));
    EXPECT_TRUE(config.contains("calculation"));
}
```

#### 3. Performance Optimization Example

```cpp
// examples/performance_demo.cpp
#include "core/core.h"
#include "modules/data_processor.h"
#include <chrono>
#include <iostream>
#include <vector>

class PerformanceTimer {
public:
    PerformanceTimer() : start_(std::chrono::high_resolution_clock::now()) {}
    
    ~PerformanceTimer() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start_);
        std::cout << "Execution time: " << duration.count() << " microseconds" << std::endl;
    }
    
private:
    std::chrono::high_resolution_clock::time_point start_;
};

int main() {
    Core core;
    core.initialize();
    
    DataProcessor processor;
    
    // Generate test data
    std::vector<std::string> large_dataset;
    for (int i = 0; i < 10000; ++i) {
        large_dataset.push_back("data_item_" + std::to_string(i));
    }
    
    std::cout << "Processing " << large_dataset.size() << " items..." << std::endl;
    
    {
        PerformanceTimer timer;
        auto results = processor.processData(large_dataset);
        std::cout << "Processed " << results.size() << " items." << std::endl;
    }
    
    // Display statistics
    auto stats = processor.getStats();
    std::cout << "Processing statistics available" << std::endl;
    
    return 0;
}
```

### Best Practices Tutorial

#### 1. Error Handling

```cpp
// examples/error_handling_demo.cpp
#include "core/core.h"
#include "modules/text_analyzer.h"
#include <iostream>
#include <stdexcept>

int main() {
    try {
        Core core;
        
        // Always check initialization
        if (!core.initialize()) {
            throw std::runtime_error("Core initialization failed");
        }
        
        // Validate input before processing
        std::string input = "";
        if (input.empty()) {
            std::cerr << "Warning: Empty input provided" << std::endl;
            input = "default input";
        }
        
        std::string result = core.process(input);
        std::cout << "Result: " << result << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
```

#### 2. Resource Management

```cpp
// examples/resource_management_demo.cpp
#include "core/core.h"
#include "modules/data_processor.h"
#include <memory>
#include <iostream>

class ResourceManager {
public:
    ResourceManager() {
        core_ = std::make_unique<Core>();
        processor_ = std::make_unique<DataProcessor>();
        
        if (!core_->initialize()) {
            throw std::runtime_error("Failed to initialize core");
        }
    }
    
    ~ResourceManager() {
        // Automatic cleanup through RAII
        if (core_) {
            core_->cleanup();
        }
    }
    
    void processData(const std::vector<std::string>& data) {
        if (!core_ || !processor_) {
            throw std::runtime_error("Resources not initialized");
        }
        
        auto results = processor_->processData(data);
        
        for (const auto& item : results) {
            std::string processed = core_->process(item);
            std::cout << "Processed: " << processed << std::endl;
        }
    }
    
private:
    std::unique_ptr<Core> core_;
    std::unique_ptr<DataProcessor> processor_;
};

int main() {
    try {
        ResourceManager manager;
        
        std::vector<std::string> data = {"item1", "item2", "item3"};
        manager.processData(data);
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    // Resources automatically cleaned up
    return 0;
}
```

### Building and Running Examples

All examples are located in the `examples/` directory and can be built using CMake:

```bash
# Build all examples
cd build
cmake --build . --target dependency_demo

# Run the dependency demonstration
./examples/dependency_demo

# Run custom examples (after adding them to CMakeLists.txt)
cmake --build . --target tutorial_basic
./examples/tutorial_basic
```

### Next Steps

1. **Explore the codebase**: Browse `src/` directory to understand the architecture
2. **Run tests**: Execute `cmake --build . --target test` to see all tests
3. **Add your own modules**: Follow the module structure in `src/modules/`
4. **Integrate new dependencies**: Use the established patterns for vcpkg, submodules, or manual dependencies
5. **Contribute**: Follow the contributing guidelines to submit improvements

For more detailed examples and advanced usage patterns, check the `examples/` directory and the test files in `tests/`.

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests for new functionality
5. Ensure all tests pass
6. Submit a pull request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- Modern CMake practices
- C++ Core Guidelines
- vcpkg package manager
- Google Test framework