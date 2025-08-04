/**
 * @file performance_benchmark.cpp
 * @brief Performance benchmarking example
 *
 * This example demonstrates how to benchmark the cpp-template library
 * components and provides patterns for performance testing in your own code.
 */

#include <cpp-template/cpp-template.h>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

class Timer {
  public:
    Timer() : start_(std::chrono::high_resolution_clock::now()) {}

    void reset() { start_ = std::chrono::high_resolution_clock::now(); }

    double elapsed_ms() const {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start_);
        return duration.count() / 1000.0;  // Return milliseconds
    }

    double elapsed_us() const {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start_);
        return static_cast<double>(duration.count());  // Return microseconds
    }

  private:
    std::chrono::high_resolution_clock::time_point start_;
};

class BenchmarkResults {
  public:
    void addResult(const std::string& test_name, double time_ms, size_t operations = 1) {
        results_.push_back({test_name, time_ms, operations});
    }

    void printResults() const {
        std::cout << "\n=== Benchmark Results ===" << std::endl;
        std::cout << std::left << std::setw(30) << "Test Name" << std::setw(15) << "Time (ms)"
                  << std::setw(15) << "Operations" << std::setw(20) << "Avg per Op (μs)"
                  << std::endl;
        std::cout << std::string(80, '-') << std::endl;

        for (const auto& result : results_) {
            double avg_us = (result.time_ms * 1000.0) / result.operations;
            std::cout << std::left << std::setw(30) << result.name << std::setw(15) << std::fixed
                      << std::setprecision(3) << result.time_ms << std::setw(15)
                      << result.operations << std::setw(20) << std::fixed << std::setprecision(2)
                      << avg_us << std::endl;
        }
        std::cout << std::endl;
    }

  private:
    struct Result {
        std::string name;
        double time_ms;
        size_t operations;
    };

    std::vector<Result> results_;
};

std::vector<std::string> generateTestData(size_t count, size_t min_length = 5,
                                          size_t max_length = 50) {
    std::vector<std::string> data;
    data.reserve(count);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> length_dist(min_length, max_length);
    std::uniform_int_distribution<> char_dist('a', 'z');

    for (size_t i = 0; i < count; ++i) {
        size_t length = length_dist(gen);
        std::string str;
        str.reserve(length);

        for (size_t j = 0; j < length; ++j) {
            str += static_cast<char>(char_dist(gen));
        }

        data.push_back(std::move(str));
    }

    return data;
}

void benchmarkCoreOperations(BenchmarkResults& results) {
    std::cout << "=== Core Operations Benchmark ===" << std::endl;

    auto core = cpp_template::createCore("BenchmarkCore");
    core->initialize();

    // Test different input sizes
    std::vector<size_t> input_sizes = {10, 100, 1000, 10000};

    for (size_t size : input_sizes) {
        std::cout << "Testing with " << size << " operations..." << std::endl;

        auto test_data = generateTestData(size, 10, 30);

        Timer timer;
        for (const auto& data : test_data) {
            core->process(data);
        }
        double elapsed = timer.elapsed_ms();

        std::string test_name = "Core Process (" + std::to_string(size) + ")";
        results.addResult(test_name, elapsed, size);

        std::cout << "  " << size << " operations in " << elapsed << " ms" << std::endl;
    }
    std::cout << std::endl;
}

void benchmarkStringUtilities(BenchmarkResults& results) {
    std::cout << "=== String Utilities Benchmark ===" << std::endl;

    const size_t iterations = 10000;
    auto test_data = generateTestData(iterations, 20, 100);

    // Benchmark toUpper
    {
        Timer timer;
        for (const auto& str : test_data) {
            cpp_template::string_utils::toUpper(str);
        }
        double elapsed = timer.elapsed_ms();
        results.addResult("String toUpper", elapsed, iterations);
        std::cout << "toUpper: " << iterations << " operations in " << elapsed << " ms"
                  << std::endl;
    }

    // Benchmark toLower
    {
        Timer timer;
        for (const auto& str : test_data) {
            cpp_template::string_utils::toLower(str);
        }
        double elapsed = timer.elapsed_ms();
        results.addResult("String toLower", elapsed, iterations);
        std::cout << "toLower: " << iterations << " operations in " << elapsed << " ms"
                  << std::endl;
    }

    // Benchmark split
    {
        std::vector<std::string> split_data;
        for (size_t i = 0; i < iterations / 10; ++i) {
            split_data.push_back("word1 word2 word3 word4 word5");
        }

        Timer timer;
        for (const auto& str : split_data) {
            cpp_template::string_utils::split(str, ' ');
        }
        double elapsed = timer.elapsed_ms();
        results.addResult("String split", elapsed, split_data.size());
        std::cout << "split: " << split_data.size() << " operations in " << elapsed << " ms"
                  << std::endl;
    }

    // Benchmark join
    {
        std::vector<std::string> words = {"word1", "word2", "word3", "word4", "word5"};

        Timer timer;
        for (size_t i = 0; i < iterations / 10; ++i) {
            cpp_template::string_utils::join(words, " ");
        }
        double elapsed = timer.elapsed_ms();
        results.addResult("String join", elapsed, iterations / 10);
        std::cout << "join: " << (iterations / 10) << " operations in " << elapsed << " ms"
                  << std::endl;
    }

    std::cout << std::endl;
}

void benchmarkValidation(BenchmarkResults& results) {
    std::cout << "=== Validation Benchmark ===" << std::endl;

    const size_t iterations = 50000;

    // Generate test data for different validation types
    auto alphanumeric_data = generateTestData(iterations, 5, 20);

    std::vector<std::string> email_data;
    for (size_t i = 0; i < iterations; ++i) {
        email_data.push_back("user" + std::to_string(i) + "@example.com");
    }

    std::vector<std::string> mixed_data;
    for (size_t i = 0; i < iterations; ++i) {
        if (i % 3 == 0) {
            mixed_data.push_back("");  // Empty
        } else if (i % 3 == 1) {
            mixed_data.push_back("   ");  // Whitespace
        } else {
            mixed_data.push_back("valid_data");
        }
    }

    // Benchmark isEmpty
    {
        Timer timer;
        for (const auto& str : mixed_data) {
            cpp_template::validation::isEmpty(str);
        }
        double elapsed = timer.elapsed_ms();
        results.addResult("Validation isEmpty", elapsed, iterations);
        std::cout << "isEmpty: " << iterations << " operations in " << elapsed << " ms"
                  << std::endl;
    }

    // Benchmark isAlphanumeric
    {
        Timer timer;
        for (const auto& str : alphanumeric_data) {
            cpp_template::validation::isAlphanumeric(str);
        }
        double elapsed = timer.elapsed_ms();
        results.addResult("Validation isAlphanumeric", elapsed, iterations);
        std::cout << "isAlphanumeric: " << iterations << " operations in " << elapsed << " ms"
                  << std::endl;
    }

    // Benchmark isValidEmail
    {
        Timer timer;
        for (const auto& str : email_data) {
            cpp_template::validation::isValidEmail(str);
        }
        double elapsed = timer.elapsed_ms();
        results.addResult("Validation isValidEmail", elapsed, iterations);
        std::cout << "isValidEmail: " << iterations << " operations in " << elapsed << " ms"
                  << std::endl;
    }

    std::cout << std::endl;
}

void benchmarkMemoryUsage() {
    std::cout << "=== Memory Usage Analysis ===" << std::endl;

    // This is a simplified memory usage demonstration
    // In a real application, you might use tools like valgrind or custom allocators

    const size_t large_count = 100000;

    std::cout << "Creating " << large_count << " Core instances..." << std::endl;

    Timer timer;
    std::vector<std::unique_ptr<cpp_template::Core>> cores;
    cores.reserve(large_count);

    for (size_t i = 0; i < large_count; ++i) {
        cores.push_back(cpp_template::createCore("Core" + std::to_string(i)));
    }

    double creation_time = timer.elapsed_ms();
    std::cout << "Creation time: " << creation_time << " ms" << std::endl;

    // Initialize all cores
    timer.reset();
    for (auto& core : cores) {
        core->initialize();
    }
    double init_time = timer.elapsed_ms();
    std::cout << "Initialization time: " << init_time << " ms" << std::endl;

    // Process data with all cores
    timer.reset();
    for (auto& core : cores) {
        core->process("test data");
    }
    double process_time = timer.elapsed_ms();
    std::cout << "Processing time: " << process_time << " ms" << std::endl;

    // Cleanup
    timer.reset();
    cores.clear();
    double cleanup_time = timer.elapsed_ms();
    std::cout << "Cleanup time: " << cleanup_time << " ms" << std::endl;

    std::cout << std::endl;
}

void benchmarkConcurrency() {
    std::cout << "=== Concurrency Analysis ===" << std::endl;
    std::cout << "Note: This is a single-threaded benchmark." << std::endl;
    std::cout << "For true concurrency testing, consider using threading libraries." << std::endl;

    // Simulate concurrent-like workload by processing multiple cores
    const size_t num_cores = 10;
    const size_t operations_per_core = 1000;

    std::vector<std::unique_ptr<cpp_template::Core>> cores;
    for (size_t i = 0; i < num_cores; ++i) {
        auto core = cpp_template::createCore("ConcurrentCore" + std::to_string(i));
        core->initialize();
        cores.push_back(std::move(core));
    }

    auto test_data = generateTestData(operations_per_core, 10, 50);

    Timer timer;

    // Sequential processing (simulating single-threaded)
    for (size_t core_idx = 0; core_idx < num_cores; ++core_idx) {
        for (const auto& data : test_data) {
            cores[core_idx]->process(data);
        }
    }

    double sequential_time = timer.elapsed_ms();
    size_t total_operations = num_cores * operations_per_core;

    std::cout << "Sequential processing:" << std::endl;
    std::cout << "  " << total_operations << " operations across " << num_cores << " cores"
              << std::endl;
    std::cout << "  Total time: " << sequential_time << " ms" << std::endl;
    std::cout << "  Throughput: " << (total_operations / sequential_time * 1000) << " ops/sec"
              << std::endl;

    std::cout << std::endl;
}

void runComprehensiveBenchmark() {
    std::cout << "=== Comprehensive Performance Benchmark ===" << std::endl;
    std::cout << "This benchmark tests various aspects of the cpp-template library." << std::endl;
    std::cout << std::endl;

    BenchmarkResults results;

    benchmarkCoreOperations(results);
    benchmarkStringUtilities(results);
    benchmarkValidation(results);
    benchmarkMemoryUsage();
    benchmarkConcurrency();

    results.printResults();
}

int main() {
    std::cout << "=== cpp-template Performance Benchmark ===" << std::endl;
    std::cout << "This example demonstrates performance testing patterns" << std::endl;
    std::cout << "and benchmarks the cpp-template library components." << std::endl;
    std::cout << std::endl;

    try {
        runComprehensiveBenchmark();

        std::cout << "=== Benchmark Recommendations ===" << std::endl;
        std::cout << "1. Run benchmarks multiple times for consistent results" << std::endl;
        std::cout << "2. Use release builds for accurate performance measurements" << std::endl;
        std::cout << "3. Consider system load when interpreting results" << std::endl;
        std::cout << "4. Profile with tools like perf, gprof, or Instruments for detailed analysis"
                  << std::endl;
        std::cout << "5. Test with realistic data sizes for your use case" << std::endl;
        std::cout << std::endl;

        std::cout << "✓ Performance benchmark completed successfully!" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
