#include "data_processor.h"
#include <core/utils.h>
#include <sstream>
#include <algorithm>
#include <iostream>

namespace cpp_template {
namespace modules {

DataProcessor::DataProcessor(std::shared_ptr<ConfigManager> config_manager)
    : config_manager_(config_manager)
    , total_processed_(0)
    , successful_operations_(0)
    , failed_operations_(0) {
    
    if (!config_manager_) {
        throw std::invalid_argument("ConfigManager cannot be null");
    }
}

ProcessingResult DataProcessor::processItem(const std::string& input, ProcessingMode mode) {
    ProcessingResult result;
    
    try {
        // Check if input is valid
        if (input.empty()) {
            result.success = false;
            result.error_message = "Input cannot be empty";
            failed_operations_++;
            return result;
        }
        
        // Apply processing based on mode
        result.result = applyProcessing(input, mode);
        result.success = true;
        result.processed_items = 1;
        
        successful_operations_++;
        total_processed_++;
        
    } catch (const std::exception& e) {
        result.success = false;
        result.error_message = e.what();
        failed_operations_++;
    }
    
    return result;
}

ProcessingResult DataProcessor::processBatch(const std::vector<std::string>& inputs, ProcessingMode mode) {
    ProcessingResult result;
    std::vector<std::string> processed_items;
    
    try {
        // Get batch size from configuration
        int batch_size = std::stoi(config_manager_->getValue("processing.batch_size", "10"));
        
        if (inputs.size() > static_cast<size_t>(batch_size)) {
            result.success = false;
            result.error_message = "Batch size exceeds configured limit of " + std::to_string(batch_size);
            failed_operations_++;
            return result;
        }
        
        for (const auto& input : inputs) {
            if (!input.empty()) {
                std::string processed = applyProcessing(input, mode);
                processed_items.push_back(processed);
            }
        }
        
        // Join all processed items
        result.result = cpp_template::core::utils::string::join(processed_items, ", ");
        result.success = true;
        result.processed_items = processed_items.size();
        
        successful_operations_++;
        total_processed_ += processed_items.size();
        
    } catch (const std::exception& e) {
        result.success = false;
        result.error_message = e.what();
        failed_operations_++;
    }
    
    return result;
}

void DataProcessor::setProcessingConfig(const std::string& key, const std::string& value) {
    config_manager_->setValue("processing." + key, value);
}

std::string DataProcessor::getStatistics() const {
    std::ostringstream stats;
    stats << "Processing Statistics:\n";
    stats << "  Total Processed: " << total_processed_ << "\n";
    stats << "  Successful Operations: " << successful_operations_ << "\n";
    stats << "  Failed Operations: " << failed_operations_ << "\n";
    stats << "  Success Rate: ";
    
    if (successful_operations_ + failed_operations_ > 0) {
        double success_rate = static_cast<double>(successful_operations_) / 
                             (successful_operations_ + failed_operations_) * 100.0;
        stats << success_rate << "%";
    } else {
        stats << "N/A";
    }
    
    return stats.str();
}

void DataProcessor::resetStatistics() {
    total_processed_ = 0;
    successful_operations_ = 0;
    failed_operations_ = 0;
}

std::string DataProcessor::applyProcessing(const std::string& input, ProcessingMode mode) {
    std::string processing_mode = config_manager_->getValue("processing.mode", "simple");
    
    switch (mode) {
        case ProcessingMode::SIMPLE:
            // Simple processing: convert to uppercase and add prefix
            return "[SIMPLE] " + cpp_template::core::utils::string::toUpper(input);
            
        case ProcessingMode::ADVANCED:
            // Advanced processing: reverse string and convert to lowercase
            {
                std::string reversed = input;
                std::reverse(reversed.begin(), reversed.end());
                return "[ADVANCED] " + cpp_template::core::utils::string::toLower(reversed);
            }
            
        case ProcessingMode::BATCH:
            // Batch processing: add batch identifier and trim
            {
                std::string trimmed = input;
                // Simple trim implementation
                trimmed.erase(0, trimmed.find_first_not_of(" \t\n\r"));
                trimmed.erase(trimmed.find_last_not_of(" \t\n\r") + 1);
                return "[BATCH] " + trimmed;
            }
            
        default:
            return "[UNKNOWN] " + input;
    }
}

std::unique_ptr<DataProcessor> createDataProcessor(std::shared_ptr<ConfigManager> config_manager) {
    return std::make_unique<DataProcessor>(config_manager);
}

} // namespace modules
} // namespace cpp_template