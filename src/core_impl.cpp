#include <stdexcept>
#include "core/core.h"  // Internal core library
#include "cpp-template/core.h"

namespace cpp_template {

// PIMPL implementation class
class Core::Impl {
  public:
    explicit Impl(const std::string& name) : internal_core_(name) {}

    core::Core internal_core_;
};

Core::Core(const std::string& name) : pImpl_(std::make_unique<Impl>(name)) {}

Core::~Core() = default;

Core::Core(const Core& other)
    : pImpl_(std::make_unique<Impl>(other.pImpl_->internal_core_.getName())) {
    pImpl_->internal_core_ = other.pImpl_->internal_core_;
}

Core& Core::operator=(const Core& other) {
    if (this != &other) {
        pImpl_ = std::make_unique<Impl>(other.pImpl_->internal_core_.getName());
        pImpl_->internal_core_ = other.pImpl_->internal_core_;
    }
    return *this;
}

Core::Core(Core&& other) noexcept = default;
Core& Core::operator=(Core&& other) noexcept = default;

const std::string& Core::getName() const {
    return pImpl_->internal_core_.getName();
}

void Core::setName(const std::string& name) {
    pImpl_->internal_core_.setName(name);
}

std::string Core::process(const std::string& input) const {
    return pImpl_->internal_core_.process(input);
}

bool Core::initialize() {
    return pImpl_->internal_core_.initialize();
}

bool Core::isInitialized() const {
    return pImpl_->internal_core_.isInitialized();
}

std::unique_ptr<Core> createCore(const std::string& name) {
    return std::make_unique<Core>(name);
}

std::string getVersion() {
    return "1.0.0";
}

}  // namespace cpp_template
