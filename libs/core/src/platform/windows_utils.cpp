/**
 * @file windows_utils.cpp
 * @brief Windows-specific utility functions implementation
 * 
 * This file contains platform-specific implementations for Windows systems.
 * It provides Windows-specific functionality that complements the cross-platform
 * core utilities.
 */

#ifdef _WIN32
#include <windows.h>
#endif

#include <string>
#include <vector>

namespace core {
namespace platform {

/**
 * @brief Get Windows system information
 * @return String containing system information
 */
std::string getSystemInfo() {
    return "Windows System";
}

/**
 * @brief Get Windows-specific temporary directory
 * @return Path to temporary directory
 */
std::string getTempDirectory() {
#ifdef _WIN32
    char tempPath[MAX_PATH];
    DWORD result = GetTempPathA(MAX_PATH, tempPath);
    if (result > 0 && result < MAX_PATH) {
        return std::string(tempPath);
    }
#endif
    
    // Fallback to environment variables
    const char* temp = std::getenv("TEMP");
    if (temp) return std::string(temp);
    
    const char* tmp = std::getenv("TMP");
    if (tmp) return std::string(tmp);
    
    return "C:\\temp";
}

/**
 * @brief Get Windows version information
 * @return Version string
 */
std::string getWindowsVersion() {
#ifdef _WIN32
    OSVERSIONINFOA osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOA));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
    
    // Note: GetVersionExA is deprecated, but used here for simplicity
    // In production code, consider using VerifyVersionInfo or other methods
    if (GetVersionExA(&osvi)) {
        return "Windows " + std::to_string(osvi.dwMajorVersion) + 
               "." + std::to_string(osvi.dwMinorVersion);
    }
#endif
    return "Windows (version detection not available)";
}

/**
 * @brief Check if running as administrator
 * @return True if running with administrator privileges
 */
bool isRunningAsAdmin() {
#ifdef _WIN32
    BOOL isAdmin = FALSE;
    PSID adminGroup = NULL;
    
    SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
    if (AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID,
                                DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &adminGroup)) {
        CheckTokenMembership(NULL, adminGroup, &isAdmin);
        FreeSid(adminGroup);
    }
    
    return isAdmin == TRUE;
#else
    return false;
#endif
}

} // namespace platform
} // namespace core