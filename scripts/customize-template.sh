#!/bin/bash

# =============================================================================
# Template Customization Script
# 
# This script helps customize the cpp-template for your own project by:
# - Renaming the project and updating all references
# - Updating namespaces and include paths
# - Modifying CMake configuration
# - Updating documentation
# =============================================================================

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Function to show usage
show_usage() {
    echo "Usage: $0 <project-name> <namespace> [description]"
    echo ""
    echo "Arguments:"
    echo "  project-name    New project name (e.g., 'my-awesome-project')"
    echo "  namespace       C++ namespace (e.g., 'my_awesome_project')"
    echo "  description     Project description (optional)"
    echo ""
    echo "Examples:"
    echo "  $0 my-project my_project 'My awesome C++ project'"
    echo "  $0 data-processor data_processor"
    echo ""
    echo "Note: This script should be run from the project root directory."
}

# Function to validate inputs
validate_inputs() {
    local project_name="$1"
    local namespace="$2"
    
    # Validate project name
    if [[ ! "$project_name" =~ ^[a-z0-9][a-z0-9-]*[a-z0-9]$ ]] && [[ ! "$project_name" =~ ^[a-z0-9]$ ]]; then
        print_error "Invalid project name. Use lowercase letters, numbers, and hyphens only."
        print_error "Must start and end with alphanumeric characters."
        return 1
    fi
    
    # Validate namespace
    if [[ ! "$namespace" =~ ^[a-z][a-z0-9_]*$ ]]; then
        print_error "Invalid namespace. Use lowercase letters, numbers, and underscores only."
        print_error "Must start with a letter."
        return 1
    fi
    
    return 0
}

# Function to backup original files
create_backup() {
    local backup_dir="backup_$(date +%Y%m%d_%H%M%S)"
    print_info "Creating backup in $backup_dir..."
    
    mkdir -p "$backup_dir"
    
    # Backup key files that will be modified
    cp CMakeLists.txt "$backup_dir/" 2>/dev/null || true
    cp vcpkg.json "$backup_dir/" 2>/dev/null || true
    cp README.md "$backup_dir/" 2>/dev/null || true
    cp -r include/ "$backup_dir/" 2>/dev/null || true
    cp -r src/ "$backup_dir/" 2>/dev/null || true
    
    print_success "Backup created in $backup_dir"
}

# Function to update CMakeLists.txt
update_cmake_config() {
    local project_name="$1"
    local description="$2"
    
    print_info "Updating CMake configuration..."
    
    # Update main CMakeLists.txt
    sed -i.bak "s/project(cpp-template/project($project_name/g" CMakeLists.txt
    sed -i.bak "s/Modern C++ Project Template demonstrating best practices/$description/g" CMakeLists.txt
    sed -i.bak "s/cpp-template/$project_name/g" CMakeLists.txt
    
    # Update src/CMakeLists.txt
    if [ -f "src/CMakeLists.txt" ]; then
        sed -i.bak "s/cpp-template-app/$project_name-app/g" src/CMakeLists.txt
        sed -i.bak "s/cpp-template-impl/$project_name-impl/g" src/CMakeLists.txt
        sed -i.bak "s/cpp-template/$project_name/g" src/CMakeLists.txt
    fi
    
    # Update tests/CMakeLists.txt
    if [ -f "tests/unit/CMakeLists.txt" ]; then
        sed -i.bak "s/cpp_template_test/$project_name_test/g" tests/unit/CMakeLists.txt
        sed -i.bak "s/cpp-template/$project_name/g" tests/unit/CMakeLists.txt
    fi
    
    # Clean up backup files
    find . -name "*.bak" -delete
    
    print_success "CMake configuration updated"
}

# Function to update vcpkg.json
update_vcpkg_config() {
    local project_name="$1"
    local description="$2"
    
    if [ -f "vcpkg.json" ]; then
        print_info "Updating vcpkg configuration..."
        
        # Use a more robust approach for JSON modification
        python3 -c "
import json
import sys

try:
    with open('vcpkg.json', 'r') as f:
        data = json.load(f)
    
    data['name'] = '$project_name'
    data['description'] = '$description'
    
    with open('vcpkg.json', 'w') as f:
        json.dump(data, f, indent=2)
    
    print('vcpkg.json updated successfully')
except Exception as e:
    print(f'Error updating vcpkg.json: {e}', file=sys.stderr)
    sys.exit(1)
" || {
            # Fallback to sed if Python is not available
            print_warning "Python not available, using sed for vcpkg.json update"
            sed -i.bak "s/\"cpp-template\"/\"$project_name\"/g" vcpkg.json
            sed -i.bak "s/\"A comprehensive C++ project template with modern practices\"/\"$description\"/g" vcpkg.json
            rm -f vcpkg.json.bak
        }
        
        print_success "vcpkg configuration updated"
    fi
}

# Function to update include directory and headers
update_headers() {
    local project_name="$1"
    local namespace="$2"
    
    print_info "Updating header files and include directory..."
    
    # Rename include directory
    if [ -d "include/cpp-template" ]; then
        mv "include/cpp-template" "include/$project_name"
        print_success "Renamed include directory to include/$project_name"
    fi
    
    # Rename main header file
    if [ -f "include/$project_name/cpp-template.h" ]; then
        mv "include/$project_name/cpp-template.h" "include/$project_name/$project_name.h"
        print_success "Renamed main header file"
    fi
    
    # Update header content
    find include/ -name "*.h" -type f -exec sed -i.bak "s/cpp_template/$namespace/g" {} \;
    find include/ -name "*.h" -type f -exec sed -i.bak "s/cpp-template/$project_name/g" {} \;
    
    # Update include guards and comments
    find include/ -name "*.h" -type f -exec sed -i.bak "s/cpp-template library/$project_name library/g" {} \;
    find include/ -name "*.h" -type f -exec sed -i.bak "s/cpp-template project/$project_name project/g" {} \;
    
    # Clean up backup files
    find include/ -name "*.bak" -delete
    
    print_success "Header files updated"
}

# Function to update source files
update_source_files() {
    local project_name="$1"
    local namespace="$2"
    
    print_info "Updating source files..."
    
    # Update all C++ source files
    find src/ -name "*.cpp" -type f -exec sed -i.bak "s/cpp_template/$namespace/g" {} \;
    find src/ -name "*.h" -type f -exec sed -i.bak "s/cpp_template/$namespace/g" {} \;
    find src/ -name "*.cpp" -type f -exec sed -i.bak "s/cpp-template/$project_name/g" {} \;
    find src/ -name "*.h" -type f -exec sed -i.bak "s/cpp-template/$project_name/g" {} \;
    
    # Update library source files
    find libs/ -name "*.cpp" -type f -exec sed -i.bak "s/cpp_template/$namespace/g" {} \; 2>/dev/null || true
    find libs/ -name "*.h" -type f -exec sed -i.bak "s/cpp_template/$namespace/g" {} \; 2>/dev/null || true
    
    # Update test files
    find tests/ -name "*.cpp" -type f -exec sed -i.bak "s/cpp_template/$namespace/g" {} \; 2>/dev/null || true
    find tests/ -name "*.cpp" -type f -exec sed -i.bak "s/cpp-template/$project_name/g" {} \; 2>/dev/null || true
    
    # Update example files
    find examples/ -name "*.cpp" -type f -exec sed -i.bak "s/cpp_template/$namespace/g" {} \; 2>/dev/null || true
    find examples/ -name "*.cpp" -type f -exec sed -i.bak "s/cpp-template/$project_name/g" {} \; 2>/dev/null || true
    
    # Clean up backup files
    find . -name "*.bak" -delete
    
    print_success "Source files updated"
}

# Function to update documentation
update_documentation() {
    local project_name="$1"
    local namespace="$2"
    local description="$3"
    
    print_info "Updating documentation..."
    
    # Update README.md
    if [ -f "README.md" ]; then
        sed -i.bak "s/# cpp-template/# $project_name/g" README.md
        sed -i.bak "s/cpp-template/$project_name/g" README.md
        sed -i.bak "s/cpp_template/$namespace/g" README.md
        sed -i.bak "1,10s/A comprehensive C++ project template.*/$description/" README.md
        rm -f README.md.bak
    fi
    
    # Update DEVELOPER_GUIDE.md
    if [ -f "DEVELOPER_GUIDE.md" ]; then
        sed -i.bak "s/cpp-template/$project_name/g" DEVELOPER_GUIDE.md
        sed -i.bak "s/cpp_template/$namespace/g" DEVELOPER_GUIDE.md
        rm -f DEVELOPER_GUIDE.md.bak
    fi
    
    # Update example documentation
    if [ -f "examples/README.md" ]; then
        sed -i.bak "s/cpp-template/$project_name/g" examples/README.md
        sed -i.bak "s/cpp_template/$namespace/g" examples/README.md
        rm -f examples/README.md.bak
    fi
    
    # Update customization guide
    if [ -f "examples/customization_guide.md" ]; then
        sed -i.bak "s/cpp-template/$project_name/g" examples/customization_guide.md
        sed -i.bak "s/cpp_template/$namespace/g" examples/customization_guide.md
        rm -f examples/customization_guide.md.bak
    fi
    
    print_success "Documentation updated"
}

# Function to clean up template-specific files
cleanup_template_files() {
    print_info "Cleaning up template-specific files..."
    
    # Remove this customization script
    if [ -f "scripts/customize-template.sh" ]; then
        print_warning "Removing customization script (this file will be deleted)"
        # Don't remove it immediately as we're running it
        echo "rm -f scripts/customize-template.sh" > cleanup_script.sh
        chmod +x cleanup_script.sh
    fi
    
    print_success "Template cleanup prepared"
}

# Function to verify the customization
verify_customization() {
    local project_name="$1"
    local namespace="$2"
    
    print_info "Verifying customization..."
    
    local errors=0
    
    # Check if main header exists
    if [ ! -f "include/$project_name/$project_name.h" ]; then
        print_error "Main header file not found: include/$project_name/$project_name.h"
        ((errors++))
    fi
    
    # Check if CMakeLists.txt was updated
    if ! grep -q "project($project_name" CMakeLists.txt; then
        print_error "CMakeLists.txt not properly updated"
        ((errors++))
    fi
    
    # Check if vcpkg.json was updated (if it exists)
    if [ -f "vcpkg.json" ] && ! grep -q "\"$project_name\"" vcpkg.json; then
        print_error "vcpkg.json not properly updated"
        ((errors++))
    fi
    
    if [ $errors -eq 0 ]; then
        print_success "Customization verification passed"
        return 0
    else
        print_error "Customization verification failed with $errors errors"
        return 1
    fi
}

# Main function
main() {
    # Check if we're in the right directory
    if [ ! -f "CMakeLists.txt" ] || [ ! -d "include" ] || [ ! -d "src" ]; then
        print_error "This script must be run from the project root directory"
        print_error "Make sure you're in the directory containing CMakeLists.txt"
        exit 1
    fi
    
    # Check arguments
    if [ $# -lt 2 ] || [ $# -gt 3 ]; then
        show_usage
        exit 1
    fi
    
    local project_name="$1"
    local namespace="$2"
    local description="${3:-A C++ project based on cpp-template}"
    
    # Validate inputs
    if ! validate_inputs "$project_name" "$namespace"; then
        exit 1
    fi
    
    # Show what we're going to do
    echo "=== Template Customization ==="
    echo "Project name: $project_name"
    echo "Namespace: $namespace"
    echo "Description: $description"
    echo ""
    
    # Ask for confirmation
    read -p "Proceed with customization? (y/N): " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        print_info "Customization cancelled"
        exit 0
    fi
    
    # Perform customization steps
    create_backup
    update_cmake_config "$project_name" "$description"
    update_vcpkg_config "$project_name" "$description"
    update_headers "$project_name" "$namespace"
    update_source_files "$project_name" "$namespace"
    update_documentation "$project_name" "$namespace" "$description"
    cleanup_template_files
    
    # Verify the result
    if verify_customization "$project_name" "$namespace"; then
        echo ""
        print_success "=== Customization Complete ==="
        print_info "Your project has been customized successfully!"
        print_info ""
        print_info "Next steps:"
        print_info "1. Review the changes and test the build:"
        print_info "   cmake -B build -S ."
        print_info "   cmake --build build"
        print_info ""
        print_info "2. Update the README.md with project-specific information"
        print_info "3. Modify the source code to implement your functionality"
        print_info "4. Add your project-specific dependencies"
        print_info "5. Update tests to match your new functionality"
        print_info ""
        print_info "6. Initialize git repository (if not already done):"
        print_info "   git add ."
        print_info "   git commit -m 'Initial commit from cpp-template'"
        
        # Execute cleanup if script exists
        if [ -f "cleanup_script.sh" ]; then
            ./cleanup_script.sh
            rm -f cleanup_script.sh
        fi
    else
        print_error "Customization completed with errors. Please review the output above."
        exit 1
    fi
}

# Run main function with all arguments
main "$@"