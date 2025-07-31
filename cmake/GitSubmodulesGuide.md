# Git Submodules Guide

This document provides comprehensive instructions for using Git submodules with the cpp-template project.

## Overview

Git submodules allow you to include external Git repositories as subdirectories within your project. This approach is useful for:

- Dependencies that need to be built from source
- Libraries not available in package managers
- Specific versions or forks of libraries
- Header-only libraries that don't require complex build systems

## Advantages and Disadvantages

### Advantages
- **Source control integration**: Dependencies are versioned with your project
- **Reproducible builds**: Exact commit hashes ensure consistency
- **Offline development**: All source code is available locally
- **Custom modifications**: Easy to fork and modify dependencies

### Disadvantages
- **Repository size**: Increases repository size and clone time
- **Complexity**: Requires understanding of submodule workflows
- **Update overhead**: Manual process to update dependencies
- **Build time**: Dependencies must be compiled from source

## Project Configuration

### Submodule Structure

The project organizes submodules in `third_party/submodules/`:

```
third_party/submodules/
├── CMakeLists.txt          # Submodule build configuration
├── json/                   # nlohmann/json library
├── catch2/                 # Catch2 testing framework
├── cxxopts/                # Command line argument parsing
└── spdlog-header-only/     # spdlog logging library
```

### .gitmodules Configuration

The `.gitmodules` file defines all submodules:

```ini
[submodule "third_party/submodules/json"]
    path = third_party/submodules/json
    url = https://github.com/nlohmann/json.git
    branch = develop
```

## Working with Submodules

### Initial Setup

When cloning the project for the first time:

```bash
# Clone the main repository
git clone <repository-url>
cd cpp-template

# Initialize and update all submodules
git submodule update --init --recursive
```

Or clone with submodules in one command:

```bash
git clone --recurse-submodules <repository-url>
```

### Adding New Submodules

To add a new submodule:

```bash
# Add the submodule
git submodule add <repository-url> third_party/submodules/<name>

# Optionally specify a branch
git submodule add -b <branch-name> <repository-url> third_party/submodules/<name>

# Commit the changes
git add .gitmodules third_party/submodules/<name>
git commit -m "Add <name> submodule"
```

### Updating Submodules

#### Update to Latest Commits

```bash
# Update all submodules to their latest commits
git submodule update --remote --merge

# Update a specific submodule
git submodule update --remote --merge third_party/submodules/<name>
```

#### Update to Specific Commit

```bash
# Navigate to submodule directory
cd third_party/submodules/<name>

# Checkout specific commit or tag
git checkout <commit-hash-or-tag>

# Return to main project
cd ../../..

# Commit the submodule update
git add third_party/submodules/<name>
git commit -m "Update <name> submodule to <version>"
```

### Removing Submodules

To remove a submodule:

```bash
# Remove submodule entry from .gitmodules
git config -f .gitmodules --remove-section submodule.third_party/submodules/<name>

# Remove submodule entry from .git/config
git config -f .git/config --remove-section submodule.third_party/submodules/<name>

# Remove the submodule directory
git rm --cached third_party/submodules/<name>
rm -rf third_party/submodules/<name>

# Commit the changes
git add .gitmodules
git commit -m "Remove <name> submodule"
```

## CMake Integration

### Automatic Configuration

The project automatically detects and configures available submodules:

```cmake
# In third_party/submodules/CMakeLists.txt
check_submodule_available("json" JSON_AVAILABLE)
if(JSON_AVAILABLE)
    # Configure nlohmann/json
    add_library(submodule_json INTERFACE)
    target_include_directories(submodule_json INTERFACE 
        "${CMAKE_CURRENT_SOURCE_DIR}/json/single_include"
    )
    add_library(third_party::submodule_json ALIAS submodule_json)
endif()
```

### Using Submodule Dependencies

In your CMakeLists.txt files:

```cmake
# Link against submodule dependencies
target_link_libraries(your_target 
    PRIVATE 
        third_party::submodule_json
        third_party::catch2
        third_party::cxxopts
)
```

### Custom Targets

The project provides custom targets for submodule management:

```bash
# Initialize all submodules
cmake --build build --target init-submodules

# Update all submodules to latest versions
cmake --build build --target update-submodules
```

## Available Submodules

### nlohmann/json
- **Purpose**: JSON parsing and serialization
- **Type**: Header-only library
- **Usage**: `#include <nlohmann/json.hpp>`
- **Target**: `third_party::submodule_json`

### Catch2
- **Purpose**: Unit testing framework
- **Type**: Header-only or compiled library
- **Usage**: `#include <catch2/catch.hpp>`
- **Target**: `third_party::catch2`

### cxxopts
- **Purpose**: Command line argument parsing
- **Type**: Header-only library
- **Usage**: `#include <cxxopts.hpp>`
- **Target**: `third_party::cxxopts`

### spdlog (Header-only)
- **Purpose**: Fast logging library
- **Type**: Header-only mode
- **Usage**: `#include <spdlog/spdlog.h>`
- **Target**: `third_party::submodule_spdlog`

## Best Practices

### Version Management

1. **Pin to specific commits**: Don't track moving branches in production
2. **Use tags when available**: Prefer stable releases over development branches
3. **Document versions**: Keep track of which versions you're using
4. **Test updates**: Always test submodule updates before committing

### Repository Hygiene

1. **Regular updates**: Keep submodules reasonably up-to-date
2. **Clean unused submodules**: Remove submodules that are no longer needed
3. **Consistent naming**: Use consistent naming conventions for submodule paths
4. **Documentation**: Document why each submodule is needed

### Build Configuration

1. **Conditional inclusion**: Make submodules optional when possible
2. **Build flags**: Disable unnecessary features in submodule builds
3. **Parallel builds**: Configure submodules to build in parallel
4. **Cache builds**: Use build caching to speed up repeated builds

## Troubleshooting

### Common Issues

1. **Submodule not initialized**
   ```bash
   # Error: submodule directory is empty
   git submodule update --init --recursive
   ```

2. **Submodule commit mismatch**
   ```bash
   # Warning: submodule is not at expected commit
   cd third_party/submodules/<name>
   git checkout <expected-commit>
   cd ../../..
   git add third_party/submodules/<name>
   git commit -m "Fix submodule commit"
   ```

3. **Merge conflicts in submodules**
   ```bash
   # Resolve conflicts in submodule first
   cd third_party/submodules/<name>
   git merge <branch>
   # Resolve conflicts and commit
   cd ../../..
   git add third_party/submodules/<name>
   git commit -m "Resolve submodule conflicts"
   ```

4. **CMake can't find submodule**
   - Verify submodule is initialized: `git submodule status`
   - Check CMakeLists.txt configuration
   - Ensure submodule has expected directory structure

### Debug Information

Enable verbose CMake output to debug submodule issues:

```bash
cmake -B build -S . -DCMAKE_VERBOSE_MAKEFILE=ON
```

Check submodule status:

```bash
# Show status of all submodules
git submodule status

# Show detailed submodule information
git submodule foreach 'echo "Submodule $name at $(git rev-parse HEAD)"'
```

## Example Workflow

Here's a complete example of adding and using a new submodule:

1. **Add the submodule:**
   ```bash
   git submodule add https://github.com/fmtlib/fmt.git third_party/submodules/fmt
   git add .gitmodules third_party/submodules/fmt
   git commit -m "Add fmt submodule"
   ```

2. **Configure in CMake:**
   ```cmake
   # In third_party/submodules/CMakeLists.txt
   check_submodule_available("fmt" FMT_AVAILABLE)
   if(FMT_AVAILABLE)
       add_subdirectory(fmt)
       add_library(third_party::fmt ALIAS fmt::fmt)
   endif()
   ```

3. **Use in code:**
   ```cpp
   #include <fmt/format.h>
   
   int main() {
       std::string message = fmt::format("Hello, {}!", "World");
       std::cout << message << std::endl;
       return 0;
   }
   ```

4. **Link in CMakeLists.txt:**
   ```cmake
   target_link_libraries(your_target PRIVATE third_party::fmt)
   ```

## Integration with Other Dependency Methods

Submodules can coexist with other dependency management approaches:

- **vcpkg fallback**: Use vcpkg if submodule is not available
- **System packages**: Prefer system packages over submodules for common libraries
- **Manual dependencies**: Use submodules for libraries that require manual integration

The project's CMake configuration automatically handles these scenarios and provides appropriate fallbacks.

For more information about Git submodules, see the [official Git documentation](https://git-scm.com/book/en/v2/Git-Tools-Submodules).