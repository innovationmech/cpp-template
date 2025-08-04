#include "core/core.h"
#include <stdexcept>
#include "core/utils.h"

namespace cpp_template {
namespace core {

Core::Core(const std::string& name) : name_(name), initialized_(false) {
    if (utils::validation::isEmpty(name)) {
        throw std::invalid_argument("Core name cannot be empty");
    }
}

const std::string& Core::getName() const noexcept {
    return name_;
}

void Core::setName(const std::string& name) {
    if (utils::validation::isEmpty(name)) {
        throw std::invalid_argument("Core name cannot be empty");
    }
    name_ = name;
}

std::string Core::process(const std::string& input) const {
    if (!initialized_) {
        throw std::runtime_error("Core must be initialized before processing");
    }

    // Demonstrate processing by converting to uppercase and adding prefix
    std::string processed = utils::string::toUpper(input);
    return "[" + name_ + "] " + processed;
}

bool Core::initialize() {
    if (initialized_) {
        return true;  // Already initialized
    }

    // Simulate initialization process
    // In a real implementation, this might set up resources, connections, etc.
    initialized_ = true;
    return true;
}

bool Core::isInitialized() const noexcept {
    return initialized_;
}

std::unique_ptr<Core> createCore(const std::string& name) {
    return std::make_unique<Core>(name);
}

}  // namespace core
}  // namespace cpp_template
