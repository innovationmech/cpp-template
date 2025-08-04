#pragma once

/**
 * @file data_processor.h
 * @brief Data processing module
 *
 * This module demonstrates data processing capabilities and shows
 * inter-module dependencies within the cpp-template project.
 */

#include <memory>
#include <string>
#include <vector>
#include "config_manager.h"

namespace cpp_template {
namespace modules {

/**
 * @brief Data processing operations
 */
enum class ProcessingMode {
    SIMPLE,
    ADVANCED,
    BATCH
};

/**
 * @brief Result of a data processing operation
 */
struct ProcessingResult {
    bool success;
    std::string result;
    std::string error_message;
    size_t processed_items;

    ProcessingResult() : success(false), processed_items(0) {}
};

/**
 * @brief Data processor for various data transformation operations
 *
 * This class demonstrates modular architecture by depending on the
 * ConfigManager module and providing data processing capabilities.
 */
class DataProcessor {
  public:
    /**
     * @brief Construct a new Data Processor object
     *
     * @param config_manager Shared pointer to configuration manager
     */
    explicit DataProcessor(std::shared_ptr<ConfigManager> config_manager);

    /**
     * @brief Destroy the Data Processor object
     */
    ~DataProcessor() = default;

    // Non-copyable but movable
    DataProcessor(const DataProcessor&) = delete;
    DataProcessor& operator=(const DataProcessor&) = delete;
    DataProcessor(DataProcessor&&) = default;
    DataProcessor& operator=(DataProcessor&&) = default;

    /**
     * @brief Process a single data item
     *
     * @param input The input data to process
     * @param mode The processing mode to use
     * @return ProcessingResult The result of the processing operation
     */
    ProcessingResult processItem(const std::string& input,
                                 ProcessingMode mode = ProcessingMode::SIMPLE);

    /**
     * @brief Process multiple data items
     *
     * @param inputs Vector of input data to process
     * @param mode The processing mode to use
     * @return ProcessingResult The result of the batch processing operation
     */
    ProcessingResult processBatch(const std::vector<std::string>& inputs,
                                  ProcessingMode mode = ProcessingMode::BATCH);

    /**
     * @brief Set the processing configuration
     *
     * @param key Configuration key
     * @param value Configuration value
     */
    void setProcessingConfig(const std::string& key, const std::string& value);

    /**
     * @brief Get current processing statistics
     *
     * @return std::string Statistics as a formatted string
     */
    std::string getStatistics() const;

    /**
     * @brief Reset processing statistics
     */
    void resetStatistics();

  private:
    std::shared_ptr<ConfigManager> config_manager_;
    size_t total_processed_;
    size_t successful_operations_;
    size_t failed_operations_;

    /**
     * @brief Internal method to apply processing based on mode
     *
     * @param input The input string
     * @param mode The processing mode
     * @return std::string The processed result
     */
    std::string applyProcessing(const std::string& input, ProcessingMode mode);
};

/**
 * @brief Factory function to create a DataProcessor instance
 *
 * @param config_manager Shared pointer to configuration manager
 * @return std::unique_ptr<DataProcessor> A unique pointer to the created instance
 */
std::unique_ptr<DataProcessor> createDataProcessor(std::shared_ptr<ConfigManager> config_manager);

}  // namespace modules
}  // namespace cpp_template
