# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

### Basic Build

```bash
# Configure build
cmake -B build -S .

# Build project
cmake --build build

# Run tests
cmake --build build --target test
# or
cd build && ctest --output-on-failure
```

### CMake Presets

```bash
# List available presets
cmake --list-presets

# Use default preset
cmake --preset default
cmake --build build

# Use vcpkg preset (requires VCPKG_ROOT env var)
cmake --preset vcpkg
cmake --build build
```

### Testing Commands

```bash
# Run all tests with verbose output
cmake --build build --target test_verbose

# Run tests in parallel
cmake --build build --target test_parallel

# Run specific test category
ctest -R "core_test"

# Run individual test executables
./build/tests/test_core
```

### Development Setup

```bash
# Run automated setup script
./scripts/setup-dev-env.sh

# Initialize git submodules
git submodule update --init --recursive

# Update submodules to latest
git submodule update --remote --merge

# Set up pre-commit hooks (recommended)
./scripts/setup-pre-commit.sh
```

## Architecture Overview

### Project Structure

- **`libs/core/`** - Internal static library providing fundamental functionality
- **`src/`** - Main application code with modular architecture
  - `cpp-template-impl` - Implementation library for public API
  - `cpp-template-app` - Main executable
  - `modules/` - Application modules (config-manager, data-processor)
- **`tests/`** - Comprehensive testing with Google Test framework
- **`third_party/`** - Multiple dependency management strategies
  - `submodules/` - Git submodule dependencies
  - `manual/` - Manually managed dependencies

### Key CMake Patterns

#### Target-Based Architecture

- Uses modern CMake with target_* commands
- Libraries create alias targets (e.g., `core::core`)
- Proper PUBLIC/PRIVATE/INTERFACE linkage and include directories

#### Dependency Management

The project supports 4 dependency strategies:

1. **vcpkg** (preferred) - Manifest mode with `vcpkg.json`
2. **System packages** - Platform package managers
3. **Git submodules** - Source-based dependencies
4. **Manual** - Direct library placement in `third_party/manual/`

#### Test Framework

- Uses `add_cpp_template_test()` function for consistent test creation
- Automatic test discovery with Google Test
- Supports unit, integration, and performance test categories
- Custom targets: `all_tests`, `test_verbose`, `test_parallel`

### Build Configuration

- **C++17 standard** enforced across all targets
- **CompilerOptions.CMake** provides standardized compiler flags
- **Position Independent Code** enabled globally
- **Export compile commands** for IDE integration
- **Cross-platform** support (Windows, macOS, Linux)

### Important Files

- `cmake/CompilerOptions.cmake` - Compiler settings and warnings
- `cmake/VcpkgHelpers.cmake` - vcpkg integration utilities
- `CMakePresets.json` - Build configuration presets
- `vcpkg.json` - Package manifest for vcpkg dependencies

## Development Notes

### Adding New Tests

Use the standardized test function:

```cmake
add_cpp_template_test(test_name
    SOURCES test_file.cpp
    LIBRARIES core config-manager
    # Optional: INTEGRATION or PERFORMANCE flags
)
```

### Conditional Linking

Third-party dependencies are linked conditionally:

```cmake
if(TARGET third_party::fmt)
    target_link_libraries(target PRIVATE third_party::fmt)
    target_compile_definitions(target PRIVATE HAVE_FMT=1)
endif()
```

### Platform-Specific Code

Uses generator expressions for cross-platform builds:

```cmake
$<$<PLATFORM_ID:Windows>:src/platform/windows_utils.cpp>
$<$<PLATFORM_ID:Linux>:src/platform/linux_utils.cpp>
$<$<PLATFORM_ID:Darwin>:src/platform/macos_utils.cpp>
```

## Automated Release and Packaging

### GitHub Actions Release Workflow

The project includes an automated release workflow that creates tags, builds packages, and publishes GitHub releases when changes are merged to the master branch.

#### Version Management

The release workflow uses semantic versioning based on commit messages:

- **Major version** (1.0.0 → 2.0.0): Commit messages containing `[major]`, `BREAKING CHANGE:`, or `!:`
- **Minor version** (1.0.0 → 1.1.0): Commit messages starting with `feat:` or containing `[minor]`
- **Patch version** (1.0.0 → 1.0.1): All other commits

#### Supported Platforms and Formats

The workflow builds and packages for multiple platforms:

- **Linux (Ubuntu)**: TGZ archive, DEB package
- **Windows**: ZIP archive, NSIS installer
- **macOS**: TGZ archive, DMG disk image

#### Release Commands

```bash
# Trigger a patch release
git commit -m "fix: resolve memory leak in parser"

# Trigger a minor release
git commit -m "feat: add new configuration options"

# Trigger a major release
git commit -m "feat!: redesign public API"
# or
git commit -m "feat: major refactor

BREAKING CHANGE: The public API has been completely redesigned"

# Manual package creation (local testing)
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build --target package
```

#### Release Process

1. **Automatic Trigger**: Push to master branch triggers the workflow
2. **Version Detection**: Analyzes commits to determine version bump
3. **Cross-Platform Build**: Builds on Ubuntu, Windows, and macOS
4. **Package Creation**: Uses CPack to generate platform-specific packages
5. **GitHub Release**: Creates tagged release with all package artifacts
6. **Changelog Generation**: Automatically generates changelog from commits

#### Configuration Files

- `.github/workflows/release.yml` - Main release workflow
- `cmake/PackagingConfig.cmake` - CPack packaging configuration
- `CMakeLists.txt` - Project version definition

#### Local Testing and Validation

Before pushing changes, you can validate the GitHub Actions workflows locally:

```bash
# Install required tools
brew install gh act  # macOS
# or for Linux:
# curl -s https://raw.githubusercontent.com/nektos/act/master/install.sh | sudo bash

# Run comprehensive validation
./scripts/validate-actions.sh

# Test specific validation methods
./scripts/validate-actions.sh gh      # GitHub CLI validation
./scripts/validate-actions.sh act     # Local workflow execution with act
./scripts/validate-actions.sh build   # Manual build and package testing
./scripts/validate-actions.sh syntax  # YAML syntax validation only

# Test local workflow (if using act)
act -j local-build-test              # Test build process
act -j local-version-test            # Test version detection
act -l                               # List all available jobs
act -n                               # Dry run without execution

# GitHub CLI workflow testing
gh workflow list                     # List workflows
gh workflow view release.yml         # View workflow details
gh run list                          # List recent runs
```

#### Validation Methods

1. **GitHub CLI (`gh`)**: Validates workflow syntax and simulates runs
2. **act Tool**: Runs workflows locally using Docker containers
3. **Manual Build**: Tests CMake build and CPack packaging locally
4. **Syntax Check**: Validates YAML syntax of workflow files

**Requirements:**

- `gh`: GitHub CLI for syntax validation and workflow inspection
- `act`: Local GitHub Actions runner (requires Docker)
- `cmake`, `ninja`: For manual build testing
- `docker`: Required by act for containerized workflow execution

## Code Quality Tools

### Formatting and Static Analysis

```bash
# Format all code
./scripts/format-code.sh

# Check code quality
./scripts/check-code.sh

# CMake targets
cmake --build build --target format-all      # Format all files
cmake --build build --target check-all       # Run all quality checks
cmake --build build --target fix-format      # Fix C++ formatting
cmake --build build --target clang-tidy-check # Run static analysis
```

### Pre-commit Hooks

```bash
# Set up pre-commit hooks
./scripts/setup-pre-commit.sh

# Run hooks manually
pre-commit run --all-files

# Skip hooks for emergency commits
git commit --no-verify
```

### Available Tools

- **clang-format**: C++ code formatting (Google style)
- **clang-tidy**: Static analysis and modernization
- **cppcheck**: Additional static analysis
- **CMake-format**: CMake file formatting

### Configuration Files

- `.clang-format` - C++ formatting rules
- `.clang-tidy` - Static analysis configuration
- `.cmake-format.yaml` - CMake formatting rules
- `.cppcheck-suppressions` - Static analysis suppressions
- `.pre-commit-config.yaml` - Pre-commit hook configuration
