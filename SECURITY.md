# Security Policy

## Supported Versions

We actively support the following versions of cpp-template with security updates:

| Version | Supported          |
| ------- | ------------------ |
| 0.0.x   | :white_check_mark: |
| < 0.0.2 | :x:                |

## Reporting a Vulnerability

We take the security of cpp-template seriously. If you believe you have found a security vulnerability, please report it to us as described below.

### How to Report

**Please do not report security vulnerabilities through public GitHub issues.**

Instead, please report them via one of the following methods:

1. **Email**: Send an email to [jackelyj@gmail.com](mailto:jackelyj@gmail.com)
2. **GitHub Security Advisories**: Use GitHub's private vulnerability reporting feature
   - Go to the Security tab in this repository
   - Click "Report a vulnerability"
   - Fill out the advisory form

### What to Include

Please include the following information in your report:

- Type of issue (e.g. buffer overflow, SQL injection, cross-site scripting, etc.)
- Full paths of source file(s) related to the manifestation of the issue
- The location of the affected source code (tag/branch/commit or direct URL)
- Any special configuration required to reproduce the issue
- Step-by-step instructions to reproduce the issue
- Proof-of-concept or exploit code (if possible)
- Impact of the issue, including how an attacker might exploit the issue

### Response Timeline

We will acknowledge receipt of your vulnerability report within 48 hours and will send a more detailed response within 72 hours indicating the next steps in handling your report.

After the initial reply to your report, we will:

- Confirm the problem and determine the affected versions
- Audit code to find any potential similar problems
- Prepare fixes for all supported versions
- Release patched versions as soon as possible

### Disclosure Policy

We follow a coordinated disclosure policy:

1. **Initial Report**: You report the vulnerability privately
2. **Acknowledgment**: We acknowledge receipt within 48 hours
3. **Investigation**: We investigate and develop a fix
4. **Coordination**: We coordinate with you on disclosure timing
5. **Public Disclosure**: We publicly disclose the vulnerability after a fix is available

Typically, we aim to disclose vulnerabilities within 90 days of the initial report, but this may vary depending on the complexity of the issue.

## Security Best Practices

When using cpp-template in your projects, we recommend:

### For Developers

- Always use the latest stable version
- Regularly update dependencies managed by vcpkg
- Enable compiler security features:
  - Stack protection (`-fstack-protector-strong`)
  - Position Independent Executables (PIE)
  - Address Space Layout Randomization (ASLR)
- Use static analysis tools (included in our CI pipeline)
- Follow secure coding practices

### For Build Systems

- Use our provided CMake presets which include security-hardened compiler flags
- Enable all available warnings and treat them as errors
- Use sanitizers during development and testing:
  - AddressSanitizer (ASan)
  - UndefinedBehaviorSanitizer (UBSan)
  - ThreadSanitizer (TSan) for multi-threaded code

### For Dependencies

- Regularly audit and update third-party dependencies
- Use vcpkg's security features and vulnerability scanning
- Monitor security advisories for used libraries
- Consider using dependency pinning for production builds

## Security Features

This project includes several security-oriented features:

- **Static Analysis**: Integrated clang-tidy and cppcheck
- **Code Formatting**: Enforced code style reduces certain vulnerability classes
- **Dependency Management**: Controlled through vcpkg with version tracking
- **CI/CD Security**: Automated security checks in GitHub Actions
- **Compiler Hardening**: Security flags enabled by default in release builds

## Known Security Considerations

### Memory Safety

- This is a C++ project and inherits C++'s memory safety characteristics
- We use modern C++ practices (C++17) to minimize common pitfalls
- Smart pointers and RAII are preferred over raw memory management
- Bounds checking is enabled where possible

### Input Validation

- All external inputs should be validated
- Use the provided utility functions for common validation tasks
- Be cautious with user-provided format strings
- Sanitize data before logging or displaying

## Security Updates

Security updates will be:

- Released as patch versions (e.g., 1.0.1, 1.1.1)
- Documented in the CHANGELOG.md with a "Security" section
- Announced through GitHub releases with security labels
- Backported to supported versions when feasible

## Contact

For general security questions or concerns, please contact:

- **Security Team**: [jackelyj@gmail.com](mailto:jackelyj@gmail.com)
- **Project Maintainer**: [jackelyj@gmail.com](mailto:jackelyj@gmail.com)

---

*This security policy is based on industry best practices and will be updated as needed to reflect the current threat landscape and project evolution.*
