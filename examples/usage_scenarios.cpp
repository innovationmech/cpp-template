/**
 * @file usage_scenarios.cpp
 * @brief Comprehensive usage scenarios demonstrating the cpp-template capabilities
 * 
 * This file provides practical examples of how to use the cpp-template in various
 * real-world scenarios, showcasing different features and best practices.
 * 
 * @author cpp-template
 * @version 1.0.0
 * @date 2024
 */

#include "core/core.h"
#include "modules/example/example_module.h"
#include "example_lib/math_utils.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include <fstream>
#include <sstream>
#include <iomanip>

using json = nlohmann::json;

/**
 * @brief Scenario 1: Basic Application Development
 * 
 * Demonstrates how to use the template for a simple application that processes
 * user data using the core library and modules.
 */
namespace scenario_basic {
    
    class UserDataProcessor {
    private:
        std::unique_ptr<Core> core_;
        std::unique_ptr<ExampleModule> module_;
        
    public:
        UserDataProcessor() {
            core_ = std::make_unique<Core>();
            module_ = std::make_unique<ExampleModule>("UserProcessor");
            
            if (!core_->initialize()) {
                throw std::runtime_error("Failed to initialize core system");
            }
        }
        
        std::string processUserInput(const std::string& input) {
            // First process through core
            std::string core_result = core_->process(input);
            
            // Then process through module
            std::string final_result = module_->process(core_result);
            
            return final_result;
        }
        
        void displayStatistics() {
            auto stats = module_->getStatistics();
            std::cout << "Processing Statistics:\n";
            std::cout << "  Module: " << stats.module_name << "\n";
            std::cout << "  Processed: " << stats.process_count << " items\n";
            std::cout << "  Core Version: " << stats.core_version << "\n";
        }
    };
    
    void run_scenario() {
        std::cout << "\n=== Scenario 1: Basic Application Development ===\n";
        
        try {
            UserDataProcessor processor;
            
            std::vector<std::string> user_inputs = {
                "Hello World",
                "Process this data",
                "Template demonstration"
            };
            
            for (const auto& input : user_inputs) {
                std::string result = processor.processUserInput(input);
                std::cout << "Input: \"" << input << "\" -> Output: \"" << result << "\"\n";
            }
            
            processor.displayStatistics();
            
        } catch (const std::exception& e) {
            std::cerr << "Error in basic scenario: " << e.what() << std::endl;
        }
    }
}

/**
 * @brief Scenario 2: Mathematical Computing Application
 * 
 * Shows how to integrate manual dependencies (example_lib) for mathematical
 * computations in a scientific or engineering application.
 */
namespace scenario_math {
    
    class MathematicalProcessor {
    private:
        Core core_;
        
    public:
        MathematicalProcessor() {
            if (!core_.initialize()) {
                throw std::runtime_error("Failed to initialize math processor");
            }
        }
        
        void performCalculations() {
            std::cout << "\n=== Mathematical Calculations ===\n";
            
            // Basic arithmetic
            int a = 15, b = 25;
            std::cout << "Factorial of 7: " << example_lib::factorial(7) << "\n";
            std::cout << "GCD of " << a << " and " << b << ": " << example_lib::gcd(a, b) << "\n";
            
            // Prime number checking
            std::vector<int> test_numbers = {17, 25, 31, 42, 97};
            std::cout << "Prime number tests:\n";
            for (int num : test_numbers) {
                bool is_prime = example_lib::is_prime(num);
                std::cout << "  " << num << " is " << (is_prime ? "prime" : "not prime") << "\n";
            }
            
            // Power calculations
            std::cout << "Power calculations:\n";
            std::cout << "  2^10 = " << example_lib::power(2, 10) << "\n";
            std::cout << "  3^5 = " << example_lib::power(3, 5) << "\n";
        }
        
        json generateMathReport() {
            json report;
            report["application"] = "Mathematical Processor";
            report["core_version"] = Core::getVersion();
            report["timestamp"] = std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
            
            // Add calculation results
            report["calculations"] = {
                {"factorial_7", example_lib::factorial(7)},
                {"gcd_15_25", example_lib::gcd(15, 25)},
                {"is_17_prime", example_lib::is_prime(17)},
                {"power_2_10", example_lib::power(2, 10)}
            };
            
            return report;
        }
    };
    
    void run_scenario() {
        std::cout << "\n=== Scenario 2: Mathematical Computing Application ===\n";
        
        try {
            MathematicalProcessor processor;
            processor.performCalculations();
            
            // Generate and display JSON report
            json report = processor.generateMathReport();
            std::cout << "\nMath Report (JSON):\n" << report.dump(2) << "\n";
            
        } catch (const std::exception& e) {
            std::cerr << "Error in math scenario: " << e.what() << std::endl;
        }
    }
}

/**
 * @brief Scenario 3: Configuration Management System
 * 
 * Demonstrates using JSON (Git submodule) for configuration management
 * in a larger application system.
 */
namespace scenario_config {
    
    class ConfigurationManager {
    private:
        json config_;
        Core core_;
        std::vector<std::unique_ptr<ExampleModule>> modules_;
        
    public:
        ConfigurationManager() {
            if (!core_.initialize()) {
                throw std::runtime_error("Failed to initialize configuration manager");
            }
            loadDefaultConfiguration();
        }
        
        void loadDefaultConfiguration() {
            config_ = {
                {"application", {
                    {"name", "cpp-template-app"},
                    {"version", "1.0.0"},
                    {"debug_mode", false},
                    {"max_threads", 4}
                }},
                {"modules", {
                    {"data_processor", {"enabled", true}},
                    {"text_analyzer", {"enabled", true}},
                    {"file_handler", {"enabled", false}}
                }},
                {"logging", {
                    {"level", "info"},
                    {"file", "app.log"},
                    {"console", true}
                }}
            };
        }
        
        void loadConfigurationFromFile(const std::string& filename) {
            std::ifstream file(filename);
            if (file.is_open()) {
                file >> config_;
                std::cout << "Configuration loaded from: " << filename << "\n";
            } else {
                std::cout << "Using default configuration (file not found: " << filename << ")\n";
            }
        }
        
        void initializeModules() {
            auto modules_config = config_["modules"];
            
            if (modules_config["data_processor"]["enabled"]) {
                modules_.push_back(std::make_unique<ExampleModule>("DataProcessor"));
                std::cout << "Initialized DataProcessor module\n";
            }
            
            if (modules_config["text_analyzer"]["enabled"]) {
                modules_.push_back(std::make_unique<ExampleModule>("TextAnalyzer"));
                std::cout << "Initialized TextAnalyzer module\n";
            }
            
            if (modules_config["file_handler"]["enabled"]) {
                modules_.push_back(std::make_unique<ExampleModule>("FileHandler"));
                std::cout << "Initialized FileHandler module\n";
            }
        }
        
        void processData(const std::string& data) {
            std::string current_data = data;
            
            // Process through core first
            current_data = core_.process(current_data);
            
            // Then through each enabled module
            for (auto& module : modules_) {
                current_data = module->process(current_data);
            }
            
            std::cout << "Final processed data: " << current_data << "\n";
        }
        
        void displayConfiguration() {
            std::cout << "\nCurrent Configuration:\n" << config_.dump(2) << "\n";
        }
        
        void saveConfiguration(const std::string& filename) {
            std::ofstream file(filename);
            if (file.is_open()) {
                file << config_.dump(2);
                std::cout << "Configuration saved to: " << filename << "\n";
            }
        }
    };
    
    void run_scenario() {
        std::cout << "\n=== Scenario 3: Configuration Management System ===\n";
        
        try {
            ConfigurationManager manager;
            
            // Try to load configuration from file (will use default if not found)
            manager.loadConfigurationFromFile("app_config.json");
            
            // Display current configuration
            manager.displayConfiguration();
            
            // Initialize modules based on configuration
            manager.initializeModules();
            
            // Process some data through the configured pipeline
            std::vector<std::string> test_data = {
                "Configuration test data",
                "Module pipeline processing",
                "JSON-based configuration"
            };
            
            for (const auto& data : test_data) {
                std::cout << "\nProcessing: \"" << data << "\"\n";
                manager.processData(data);
            }
            
            // Save current configuration
            manager.saveConfiguration("output_config.json");
            
        } catch (const std::exception& e) {
            std::cerr << "Error in config scenario: " << e.what() << std::endl;
        }
    }
}

/**
 * @brief Scenario 4: Performance Benchmarking
 * 
 * Shows how to use the template for performance testing and benchmarking
 * applications, demonstrating timing and measurement capabilities.
 */
namespace scenario_performance {
    
    class PerformanceBenchmark {
    private:
        Core core_;
        std::vector<std::unique_ptr<ExampleModule>> modules_;
        
        struct BenchmarkResult {
            std::string operation;
            std::chrono::microseconds duration;
            size_t iterations;
            double avg_time_per_operation;
        };
        
        std::vector<BenchmarkResult> results_;
        
    public:
        PerformanceBenchmark() {
            if (!core_.initialize()) {
                throw std::runtime_error("Failed to initialize benchmark");
            }
            
            // Create multiple modules for testing
            for (int i = 0; i < 3; ++i) {
                modules_.push_back(std::make_unique<ExampleModule>("BenchModule" + std::to_string(i)));
            }
        }
        
        template<typename Func>
        BenchmarkResult measurePerformance(const std::string& operation_name, 
                                          size_t iterations, 
                                          Func&& operation) {
            auto start = std::chrono::high_resolution_clock::now();
            
            for (size_t i = 0; i < iterations; ++i) {
                operation();
            }
            
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            
            BenchmarkResult result;
            result.operation = operation_name;
            result.duration = duration;
            result.iterations = iterations;
            result.avg_time_per_operation = static_cast<double>(duration.count()) / iterations;
            
            results_.push_back(result);
            return result;
        }
        
        void runBenchmarks() {
            std::cout << "\n=== Performance Benchmarks ===\n";
            
            const size_t iterations = 10000;
            const std::string test_data = "Performance test data for benchmarking";
            
            // Benchmark core processing
            measurePerformance("Core Processing", iterations, [&]() {
                core_.process(test_data);
            });
            
            // Benchmark module processing
            measurePerformance("Module Processing", iterations, [&]() {
                modules_[0]->process(test_data);
            });
            
            // Benchmark mathematical operations
            measurePerformance("Mathematical Operations", iterations, [&]() {
                example_lib::factorial(10);
                example_lib::gcd(48, 18);
                example_lib::is_prime(97);
            });
            
            // Benchmark JSON operations
            measurePerformance("JSON Operations", iterations / 10, [&]() {
                json test_json;
                test_json["data"] = test_data;
                test_json["timestamp"] = std::chrono::duration_cast<std::chrono::seconds>(
                    std::chrono::system_clock::now().time_since_epoch()).count();
                std::string serialized = test_json.dump();
                json parsed = json::parse(serialized);
            });
        }
        
        void displayResults() {
            std::cout << "\n=== Benchmark Results ===\n";
            std::cout << std::left << std::setw(25) << "Operation" 
                      << std::setw(15) << "Total Time (μs)" 
                      << std::setw(12) << "Iterations" 
                      << std::setw(20) << "Avg Time/Op (μs)" << "\n";
            std::cout << std::string(72, '-') << "\n";
            
            for (const auto& result : results_) {
                std::cout << std::left << std::setw(25) << result.operation
                          << std::setw(15) << result.duration.count()
                          << std::setw(12) << result.iterations
                          << std::setw(20) << std::fixed << std::setprecision(3) 
                          << result.avg_time_per_operation << "\n";
            }
        }
        
        json generateBenchmarkReport() {
            json report;
            report["benchmark_info"] = {
                {"core_version", Core::getVersion()},
                {"timestamp", std::chrono::duration_cast<std::chrono::seconds>(
                    std::chrono::system_clock::now().time_since_epoch()).count()},
                {"total_operations", results_.size()}
            };
            
            json results_json = json::array();
            for (const auto& result : results_) {
                results_json.push_back({
                    {"operation", result.operation},
                    {"total_time_microseconds", result.duration.count()},
                    {"iterations", result.iterations},
                    {"average_time_per_operation", result.avg_time_per_operation}
                });
            }
            report["results"] = results_json;
            
            return report;
        }
    };
    
    void run_scenario() {
        std::cout << "\n=== Scenario 4: Performance Benchmarking ===\n";
        
        try {
            PerformanceBenchmark benchmark;
            benchmark.runBenchmarks();
            benchmark.displayResults();
            
            // Generate JSON report
            json report = benchmark.generateBenchmarkReport();
            std::cout << "\nBenchmark Report (JSON):\n" << report.dump(2) << "\n";
            
        } catch (const std::exception& e) {
            std::cerr << "Error in performance scenario: " << e.what() << std::endl;
        }
    }
}

/**
 * @brief Main function demonstrating all usage scenarios
 */
int main() {
    std::cout << "=== cpp-template Usage Scenarios Demonstration ===\n";
    std::cout << "This program demonstrates various real-world usage scenarios\n";
    std::cout << "for the cpp-template project, showcasing different features\n";
    std::cout << "and integration patterns.\n";
    
    try {
        // Run all scenarios
        scenario_basic::run_scenario();
        scenario_math::run_scenario();
        scenario_config::run_scenario();
        scenario_performance::run_scenario();
        
        std::cout << "\n=== All Scenarios Completed Successfully ===\n";
        std::cout << "\nThese scenarios demonstrate:\n";
        std::cout << "1. Basic application development with core and modules\n";
        std::cout << "2. Mathematical computing with manual dependencies\n";
        std::cout << "3. Configuration management with JSON (Git submodule)\n";
        std::cout << "4. Performance benchmarking and measurement\n";
        std::cout << "\nEach scenario shows different aspects of using the cpp-template\n";
        std::cout << "for real-world C++ application development.\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Fatal error in usage scenarios: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}