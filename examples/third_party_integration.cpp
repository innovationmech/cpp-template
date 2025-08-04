/**
 * @file third_party_integration.cpp
 * @brief Example demonstrating integration with third-party libraries
 *
 * This example shows how to integrate the cpp-template library with
 * external dependencies, specifically demonstrating JSON processing
 * with nlohmann/json library.
 */

#include <cpp-template/cpp-template.h>
#include <iostream>
#include <map>
#include <vector>

// Conditional compilation based on available third-party libraries
#ifdef HAS_NLOHMANN_JSON
    #include <nlohmann/json.hpp>
using json = nlohmann::json;
#endif

void demonstrateBasicIntegration() {
    std::cout << "=== Basic Third-Party Integration ===" << std::endl;

    // Create core instance
    auto core = cpp_template::createCore("IntegrationCore");
    core->initialize();

#ifdef HAS_NLOHMANN_JSON
    std::cout << "JSON library available - demonstrating JSON processing" << std::endl;

    // Create sample JSON data
    json sample_data = {
        {        "name",                             "cpp-template"                        },
        {     "version",                                                            "1.0.0"},
        {    "features",                      {"modular", "modern", "tested", "documented"}},
        {"dependencies", {{"required", {"cmake", "c++17"}}, {"optional", {"vcpkg", "git"}}}},
        {  "statistics",
         {{"lines_of_code", 2500}, {"test_coverage", 95.5}, {"build_time_seconds", 45}}    }
    };

    // Convert JSON to string and process with cpp-template
    std::string json_string = sample_data.dump(2);  // Pretty print with 2-space indent
    std::cout << "\nOriginal JSON data:" << std::endl;
    std::cout << json_string << std::endl;

    // Process the JSON string with our core library
    std::string processed = core->process(json_string);
    std::cout << "\nProcessed by cpp-template core:" << std::endl;
    std::cout << processed << std::endl;

    // Try to parse the processed result back to JSON
    try {
        json parsed_result = json::parse(processed);
        std::cout << "\n✓ Processed result is valid JSON" << std::endl;

        // Extract some information
        if (parsed_result.contains("name")) {
            std::cout << "Project name: " << parsed_result["name"] << std::endl;
        }
        if (parsed_result.contains("features") && parsed_result["features"].is_array()) {
            std::cout << "Features (" << parsed_result["features"].size()
                      << " total):" << std::endl;
            for (const auto& feature : parsed_result["features"]) {
                std::cout << "  - " << feature << std::endl;
            }
        }
    } catch (const json::parse_error& e) {
        std::cout << "\n✗ Processed result is not valid JSON: " << e.what() << std::endl;
        std::cout << "This is expected if the core processing modifies the format" << std::endl;
    }

#else
    std::cout << "JSON library not available - using string processing only" << std::endl;

    // Simulate JSON-like data as strings
    std::string json_like_data = R"({
  "name": "cpp-template",
  "version": "1.0.0",
  "features": ["modular", "modern", "tested"]
})";

    std::cout << "\nSimulated JSON data:" << std::endl;
    std::cout << json_like_data << std::endl;

    std::string processed = core->process(json_like_data);
    std::cout << "\nProcessed result:" << std::endl;
    std::cout << processed << std::endl;
#endif

    std::cout << std::endl;
}

void demonstrateDataTransformation() {
    std::cout << "=== Data Transformation Example ===" << std::endl;

#ifdef HAS_NLOHMANN_JSON
    // Create a more complex data transformation scenario
    auto core = cpp_template::createCore("TransformCore");
    core->initialize();

    // Sample input data (could come from API, file, etc.)
    std::vector<json> input_records = {
        {{"id", 1},
         {"name", "Alice Johnson"},
         {"email", "alice@example.com"},
         {"department", "Engineering"},
         {"salary", 75000}},
        {{"id", 2},
         {"name", "Bob Smith"},
         {"email", "bob@example.com"},
         {"department", "Marketing"},
         {"salary", 65000}},
        {{"id", 3},
         {"name", "Carol Davis"},
         {"email", "carol@example.com"},
         {"department", "Engineering"},
         {"salary", 80000}}
    };

    std::cout << "Processing " << input_records.size() << " employee records..." << std::endl;

    // Transform each record
    std::vector<json> transformed_records;
    for (const auto& record : input_records) {
        // Convert record to string, process it, then parse back
        std::string record_str = record.dump();
        std::string processed_str = core->process(record_str);

        try {
            json transformed = json::parse(processed_str);

            // Add some computed fields
            transformed["processed_timestamp"] = std::time(nullptr);
            transformed["name_length"] = record["name"].get<std::string>().length();

            // Validate email using cpp-template validation
            std::string email = record["email"].get<std::string>();
            transformed["email_valid"] = cpp_template::validation::isValidEmail(email);

            transformed_records.push_back(transformed);

        } catch (const json::parse_error& e) {
            std::cout << "Warning: Could not parse processed record: " << e.what() << std::endl;
            // Add original record with error flag
            json error_record = record;
            error_record["processing_error"] = true;
            transformed_records.push_back(error_record);
        }
    }

    // Display results
    std::cout << "\nTransformed records:" << std::endl;
    for (size_t i = 0; i < transformed_records.size(); ++i) {
        std::cout << "Record " << (i + 1) << ":" << std::endl;
        std::cout << transformed_records[i].dump(2) << std::endl;
        std::cout << std::endl;
    }

    // Generate summary statistics
    json summary = {
        {       "total_records", transformed_records.size()},
        {"processing_timestamp",         std::time(nullptr)},
        {         "departments",              json::array()},
        {      "average_salary",                        0.0},
        {        "valid_emails",                          0}
    };

    std::map<std::string, int> dept_count;
    double total_salary = 0.0;
    int valid_email_count = 0;

    for (const auto& record : transformed_records) {
        if (record.contains("department")) {
            std::string dept = record["department"];
            dept_count[dept]++;
        }
        if (record.contains("salary")) {
            total_salary += record["salary"].get<double>();
        }
        if (record.contains("email_valid") && record["email_valid"].get<bool>()) {
            valid_email_count++;
        }
    }

    // Add department statistics
    for (const auto& [dept, count] : dept_count) {
        summary["departments"].push_back({
            { "name",  dept},
            {"count", count}
        });
    }

    summary["average_salary"] = total_salary / transformed_records.size();
    summary["valid_emails"] = valid_email_count;

    std::cout << "Summary Statistics:" << std::endl;
    std::cout << summary.dump(2) << std::endl;

#else
    std::cout << "JSON library not available - skipping complex data transformation" << std::endl;
#endif

    std::cout << std::endl;
}

void demonstrateConfigurationWithJSON() {
    std::cout << "=== Configuration Management with JSON ===" << std::endl;

#ifdef HAS_NLOHMANN_JSON
    // Demonstrate how to use JSON for configuration
    json config = {
        {"application", {{"name", "IntegrationDemo"}, {"version", "1.0.0"}, {"debug_mode", true}}},
        { "processing",           {{"batch_size", 100}, {"timeout_ms", 5000}, {"retry_count", 3}}},
        {    "logging",               {{"level", "info"}, {"file", "app.log"}, {"console", true}}}
    };

    std::cout << "Configuration loaded from JSON:" << std::endl;
    std::cout << config.dump(2) << std::endl;

    // Extract configuration values
    std::string app_name = config["application"]["name"];
    bool debug_mode = config["application"]["debug_mode"];
    int batch_size = config["processing"]["batch_size"];
    std::string log_level = config["logging"]["level"];

    std::cout << "\nExtracted configuration:" << std::endl;
    std::cout << "App name: " << app_name << std::endl;
    std::cout << "Debug mode: " << (debug_mode ? "enabled" : "disabled") << std::endl;
    std::cout << "Batch size: " << batch_size << std::endl;
    std::cout << "Log level: " << log_level << std::endl;

    // Use configuration with cpp-template components
    auto core = cpp_template::createCore(app_name);
    core->initialize();

    // Process some data based on configuration
    std::vector<std::string> test_data = {"item1", "item2", "item3", "item4", "item5"};

    std::cout << "\nProcessing data with batch size " << batch_size << ":" << std::endl;
    for (size_t i = 0; i < test_data.size(); i += batch_size) {
        size_t end = std::min(i + batch_size, test_data.size());
        std::cout << "Batch " << (i / batch_size + 1) << ": ";

        for (size_t j = i; j < end; ++j) {
            std::string result = core->process(test_data[j]);
            std::cout << result;
            if (j < end - 1)
                std::cout << ", ";
        }
        std::cout << std::endl;
    }

#else
    std::cout << "JSON library not available - using simple configuration" << std::endl;

    // Simple key-value configuration
    std::map<std::string, std::string> config = {
        {             "app.name", "IntegrationDemo"},
        {          "app.version",           "1.0.0"},
        {"processing.batch_size",             "100"},
        {        "logging.level",            "info"}
    };

    std::cout << "Simple configuration:" << std::endl;
    for (const auto& [key, value] : config) {
        std::cout << key << " = " << value << std::endl;
    }
#endif

    std::cout << std::endl;
}

void demonstrateErrorHandling() {
    std::cout << "=== Error Handling with Third-Party Libraries ===" << std::endl;

    auto core = cpp_template::createCore("ErrorHandlingCore");
    core->initialize();

#ifdef HAS_NLOHMANN_JSON
    // Test various error scenarios with JSON
    std::vector<std::string> test_inputs = {
        R"({"valid": "json"})",                   // Valid JSON
        R"({"invalid": json})",                   // Invalid JSON (unquoted value)
        R"({"incomplete":)",                      // Incomplete JSON
        "",                                       // Empty string
        "not json at all",                        // Plain text
        R"({"nested": {"deep": {"value": 42}}})"  // Valid nested JSON
    };

    std::cout << "Testing error handling with various inputs:" << std::endl;

    for (size_t i = 0; i < test_inputs.size(); ++i) {
        const auto& input = test_inputs[i];
        std::cout << "\nTest " << (i + 1) << ": ";

        if (input.empty()) {
            std::cout << "(empty string)" << std::endl;
        } else if (input.length() > 50) {
            std::cout << input.substr(0, 47) << "..." << std::endl;
        } else {
            std::cout << input << std::endl;
        }

        try {
            // First, try to parse as JSON
            json parsed = json::parse(input);
            std::cout << "  ✓ Valid JSON parsed successfully" << std::endl;

            // Process with cpp-template
            std::string processed = core->process(input);
            std::cout << "  ✓ Processed by cpp-template: " << processed.substr(0, 50);
            if (processed.length() > 50)
                std::cout << "...";
            std::cout << std::endl;

            // Try to parse processed result
            try {
                json reparsed = json::parse(processed);
                std::cout << "  ✓ Processed result is still valid JSON" << std::endl;
            } catch (const json::parse_error&) {
                std::cout << "  ! Processed result is no longer valid JSON (expected)" << std::endl;
            }

        } catch (const json::parse_error& e) {
            std::cout << "  ✗ JSON parse error: " << e.what() << std::endl;

            // Still try to process with cpp-template
            try {
                std::string processed = core->process(input);
                std::cout << "  ✓ Still processed by cpp-template: " << processed.substr(0, 50);
                if (processed.length() > 50)
                    std::cout << "...";
                std::cout << std::endl;
            } catch (const std::exception& e) {
                std::cout << "  ✗ cpp-template processing also failed: " << e.what() << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "  ✗ Unexpected error: " << e.what() << std::endl;
        }
    }

#else
    std::cout << "JSON library not available - testing basic error handling" << std::endl;

    std::vector<std::string> test_inputs = {
        "valid input", "", "very long input string that might cause issues with processing",
        "special characters: !@#$%^&*()"};

    for (size_t i = 0; i < test_inputs.size(); ++i) {
        const auto& input = test_inputs[i];
        std::cout << "\nTest " << (i + 1) << ": ";

        if (input.empty()) {
            std::cout << "(empty string)" << std::endl;
        } else {
            std::cout << input << std::endl;
        }

        try {
            std::string processed = core->process(input);
            std::cout << "  ✓ Processed successfully: " << processed << std::endl;
        } catch (const std::exception& e) {
            std::cout << "  ✗ Processing failed: " << e.what() << std::endl;
        }
    }
#endif

    std::cout << std::endl;
}

int main() {
    std::cout << "=== Third-Party Integration Comprehensive Demo ===" << std::endl;

#ifdef HAS_NLOHMANN_JSON
    std::cout << "Running with nlohmann/json library support" << std::endl;
#else
    std::cout << "Running without third-party JSON library" << std::endl;
    std::cout << "To enable JSON features, install nlohmann-json via vcpkg:" << std::endl;
    std::cout << "  vcpkg install nlohmann-json" << std::endl;
    std::cout << "Then rebuild with vcpkg toolchain." << std::endl;
#endif

    std::cout << std::endl;

    try {
        demonstrateBasicIntegration();
        std::cout << std::string(60, '=') << std::endl;

        demonstrateDataTransformation();
        std::cout << std::string(60, '=') << std::endl;

        demonstrateConfigurationWithJSON();
        std::cout << std::string(60, '=') << std::endl;

        demonstrateErrorHandling();
        std::cout << std::string(60, '=') << std::endl;

        std::cout << "=== Integration Best Practices ===" << std::endl;
        std::cout << "1. Use conditional compilation for optional dependencies" << std::endl;
        std::cout << "2. Provide fallback functionality when libraries are unavailable"
                  << std::endl;
        std::cout << "3. Handle parsing and processing errors gracefully" << std::endl;
        std::cout << "4. Validate data at integration boundaries" << std::endl;
        std::cout << "5. Use modern C++ features for type safety" << std::endl;
        std::cout << std::endl;

        std::cout << "✓ All third-party integration demonstrations completed successfully!"
                  << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
