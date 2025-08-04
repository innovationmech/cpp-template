/**
 * @file linux_utils.cpp
 * @brief Linux-specific utility functions implementation
 *
 * This file contains platform-specific implementations for Linux systems.
 * It provides Linux-specific functionality that complements the cross-platform
 * core utilities.
 */

#include <fstream>
#include <string>
#include <vector>

namespace core {
namespace platform {

/**
 * @brief Get Linux system information
 * @return String containing system information
 */
std::string getSystemInfo() {
    return "Linux System";
}

/**
 * @brief Get Linux-specific temporary directory
 * @return Path to temporary directory
 */
std::string getTempDirectory() {
    const char* tmpdir = std::getenv("TMPDIR");
    if (tmpdir)
        return std::string(tmpdir);

    const char* tmp = std::getenv("TMP");
    if (tmp)
        return std::string(tmp);

    return "/tmp";
}

/**
 * @brief Get Linux distribution information
 * @return Distribution name and version
 */
std::string getLinuxDistribution() {
    std::ifstream file("/etc/os-release");
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (line.find("PRETTY_NAME=") == 0) {
                return line.substr(12);  // Remove "PRETTY_NAME="
            }
        }
    }
    return "Unknown Linux Distribution";
}

/**
 * @brief Check if running in a container
 * @return True if running in a container environment
 */
bool isRunningInContainer() {
    std::ifstream file("/proc/1/cgroup");
    if (file.is_open()) {
        std::string content((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());
        return content.find("docker") != std::string::npos ||
               content.find("lxc") != std::string::npos;
    }
    return false;
}

}  // namespace platform
}  // namespace core
