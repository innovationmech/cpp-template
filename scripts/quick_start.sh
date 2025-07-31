#!/bin/bash

# ==============================================================================
# cpp-template Quick Start Script
# ==============================================================================
#
# This script helps users quickly set up a new C++ project from the cpp-template.
# It automates the common setup tasks including project customization, dependency
# initialization, and initial build verification.
#
# Usage:
#   ./scripts/quick_start.sh [PROJECT_NAME] [OPTIONS]
#
# Examples:
#   ./scripts/quick_start.sh my-awesome-project
#   ./scripts/quick_start.sh my-project --skip-build
#   ./scripts/quick_start.sh game-engine --dependency-method vcpkg
#
# Features:
# - Interactive project name input
# - Automatic placeholder replacement
# - Dependency method selection
# - Initial build verification
# - Git repository initialization
# - Development environment setup
#
# Author: cpp-template project
# Version: 1.0.0
# ==============================================================================

set -e  # Exit on any error

# Script configuration
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
TIMESTAMP=$(date +"%Y%m%d_%H%M%S")

# Default values
PROJECT_NAME=""
DEPENDENCY_METHOD="auto"
SKIP_BUILD=false
SKIP_TESTS=false
SKIP_GIT_INIT=false
SKIP_PREREQ_CHECK=false
VERBOSE=false
DRY_RUN=false

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Logging functions
log_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

log_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1" >&2
}

log_step() {
    echo -e "${PURPLE}[STEP]${NC} $1"
}

log_verbose() {
    if [[ "$VERBOSE" == "true" ]]; then
        echo -e "${CYAN}[VERBOSE]${NC} $1"
    fi
}

# Help function
show_help() {
    cat << EOF
cpp-template Quick Start Script

Usage: $0 [PROJECT_NAME] [OPTIONS]

Arguments:
  PROJECT_NAME              Name for the new project (optional, will prompt if not provided)

Options:
  -d, --dependency-method   Dependency management method (auto|vcpkg|submodules|manual|system)
  -s, --skip-build         Skip the initial build verification
  -t, --skip-tests         Skip running tests after build
  -g, --skip-git-init      Skip Git repository initialization
      --skip-prereq-check  Skip prerequisite tool checking
  -v, --verbose            Enable verbose output
  -n, --dry-run            Show what would be done without making changes
  -h, --help               Show this help message

Dependency Methods:
  auto        Automatically detect and use the best available method
  vcpkg       Use vcpkg package manager (requires vcpkg installation)
  submodules  Use Git submodules for dependencies
  manual      Use manually managed dependencies in third_party/manual
  system      Use system package manager (brew, apt, etc.)

Examples:
  $0 my-awesome-project
  $0 my-project --dependency-method vcpkg --skip-tests
  $0 game-engine --verbose
  $0 --dry-run my-project

EOF
}

# Parse command line arguments
parse_arguments() {
    while [[ $# -gt 0 ]]; do
        case $1 in
            -d|--dependency-method)
                DEPENDENCY_METHOD="$2"
                shift 2
                ;;
            -s|--skip-build)
                SKIP_BUILD=true
                shift
                ;;
            -t|--skip-tests)
                SKIP_TESTS=true
                shift
                ;;
            -g|--skip-git-init)
                SKIP_GIT_INIT=true
                shift
                ;;
            --skip-prereq-check)
                SKIP_PREREQ_CHECK=true
                shift
                ;;
            -v|--verbose)
                VERBOSE=true
                shift
                ;;
            -n|--dry-run)
                DRY_RUN=true
                shift
                ;;
            -h|--help)
                show_help
                exit 0
                ;;
            -*)
                log_error "Unknown option: $1"
                show_help
                exit 1
                ;;
            *)
                if [[ -z "$PROJECT_NAME" ]]; then
                    PROJECT_NAME="$1"
                else
                    log_error "Multiple project names provided: '$PROJECT_NAME' and '$1'"
                    exit 1
                fi
                shift
                ;;
        esac
    done
}

# Validate project name
validate_project_name() {
    local name="$1"
    
    if [[ -z "$name" ]]; then
        return 1
    fi
    
    if [[ ! "$name" =~ ^[a-zA-Z][a-zA-Z0-9_-]*$ ]]; then
        log_error "Invalid project name: '$name'"
        log_error "Project name must start with a letter and contain only letters, numbers, hyphens, and underscores"
        return 1
    fi
    
    if [[ ${#name} -gt 50 ]]; then
        log_error "Project name is too long (max 50 characters): '$name'"
        return 1
    fi
    
    return 0
}

# Interactive project name input
get_project_name() {
    if [[ -n "$PROJECT_NAME" ]]; then
        if validate_project_name "$PROJECT_NAME"; then
            return 0
        else
            exit 1
        fi
    fi
    
    echo -e "${BLUE}Welcome to cpp-template Quick Start!${NC}"
    echo
    echo "This script will help you set up a new C++ project from the template."
    echo
    
    while true; do
        read -p "Enter your project name: " PROJECT_NAME
        
        if validate_project_name "$PROJECT_NAME"; then
            break
        fi
        
        echo "Please try again with a valid project name."
    done
}

# Check prerequisites
check_prerequisites() {
    log_step "Checking prerequisites..."
    
    local missing_tools=()
    local warnings=()
    
    # Check for required tools
    if ! command -v cmake &> /dev/null; then
        missing_tools+=("cmake")
    fi
    
    if ! command -v git &> /dev/null; then
        missing_tools+=("git")
    fi
    
    if ! command -v python3 &> /dev/null && ! command -v python &> /dev/null; then
        missing_tools+=("python")
    fi
    
    # Check for C++ compiler
    if ! command -v g++ &> /dev/null && ! command -v clang++ &> /dev/null; then
        missing_tools+=("C++ compiler (g++ or clang++)")
    fi
    
    # Check for optional tools
    if ! command -v vcpkg &> /dev/null; then
        warnings+=("vcpkg (optional, for vcpkg dependency management)")
    fi
    
    if ! command -v brew &> /dev/null && ! command -v apt-get &> /dev/null && ! command -v yum &> /dev/null; then
        warnings+=("system package manager (brew/apt/yum - optional, for system dependency management)")
    fi
    
    # Show warnings for missing optional tools
    if [[ ${#warnings[@]} -gt 0 ]]; then
        log_warning "Optional tools not found (some features may be limited):"
        for tool in "${warnings[@]}"; do
            log_warning "  - $tool"
        done
        echo
    fi
    
    # Handle missing required tools
    if [[ ${#missing_tools[@]} -gt 0 ]]; then
        log_error "Missing required tools:"
        for tool in "${missing_tools[@]}"; do
            log_error "  - $tool"
        done
        echo
        log_info "Installation suggestions:"
        log_info "  macOS: Install Xcode Command Line Tools: xcode-select --install"
        log_info "  macOS: Install Homebrew: /bin/bash -c \"\$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)\""
        log_info "  macOS: Then run: brew install cmake git python3"
        log_info "  Ubuntu/Debian: sudo apt-get install cmake git python3 build-essential"
        log_info "  RHEL/CentOS: sudo yum install cmake git python3 gcc-c++"
        echo
        
        if [[ "$DRY_RUN" == "true" ]]; then
            log_warning "DRY RUN: Continuing despite missing tools"
            return 0
        fi
        
        read -p "Continue anyway? Some features may not work (y/N): " -n 1 -r
        echo
        if [[ ! $REPLY =~ ^[Yy]$ ]]; then
            log_info "Setup cancelled. Please install the required tools and try again."
            exit 1
        fi
        
        log_warning "Continuing with missing tools - some features may not work"
        return 0
    fi
    
    log_success "All required prerequisites are available"
}

# Detect dependency management method
detect_dependency_method() {
    if [[ "$DEPENDENCY_METHOD" != "auto" ]]; then
        return 0
    fi
    
    log_step "Auto-detecting dependency management method..."
    
    # Check for vcpkg
    if command -v vcpkg &> /dev/null; then
        DEPENDENCY_METHOD="vcpkg"
        log_info "Detected vcpkg installation"
        return 0
    fi
    
    # Check for system package managers
    if command -v brew &> /dev/null; then
        DEPENDENCY_METHOD="system"
        log_info "Detected Homebrew (macOS)"
        return 0
    elif command -v apt-get &> /dev/null; then
        DEPENDENCY_METHOD="system"
        log_info "Detected APT (Ubuntu/Debian)"
        return 0
    elif command -v yum &> /dev/null; then
        DEPENDENCY_METHOD="system"
        log_info "Detected YUM (RHEL/CentOS)"
        return 0
    fi
    
    # Default to submodules
    DEPENDENCY_METHOD="submodules"
    log_info "Using Git submodules as fallback method"
}

# Replace placeholders
replace_placeholders() {
    log_step "Replacing project placeholders..."
    
    local python_cmd="python3"
    if ! command -v python3 &> /dev/null; then
        python_cmd="python"
    fi
    
    local replace_script="$SCRIPT_DIR/replace_placeholders.py"
    
    if [[ ! -f "$replace_script" ]]; then
        log_error "Placeholder replacement script not found: $replace_script"
        exit 1
    fi
    
    if [[ "$DRY_RUN" == "true" ]]; then
        log_info "DRY RUN: Would replace placeholders with project name '$PROJECT_NAME'"
        "$python_cmd" "$replace_script" "$PROJECT_NAME" --project-root "$PROJECT_ROOT" --dry-run
    else
        "$python_cmd" "$replace_script" "$PROJECT_NAME" --project-root "$PROJECT_ROOT"
    fi
    
    log_success "Placeholders replaced successfully"
}

# Initialize dependencies
init_dependencies() {
    log_step "Initializing dependencies using method: $DEPENDENCY_METHOD"
    
    case "$DEPENDENCY_METHOD" in
        vcpkg)
            init_vcpkg_dependencies
            ;;
        submodules)
            init_submodule_dependencies
            ;;
        manual)
            init_manual_dependencies
            ;;
        system)
            init_system_dependencies
            ;;
        *)
            log_error "Unknown dependency method: $DEPENDENCY_METHOD"
            exit 1
            ;;
    esac
}

# Initialize vcpkg dependencies
init_vcpkg_dependencies() {
    log_info "Setting up vcpkg dependencies..."
    
    if [[ "$DRY_RUN" == "true" ]]; then
        log_info "DRY RUN: Would install vcpkg dependencies"
        return 0
    fi
    
    # Check if vcpkg is available
    if ! command -v vcpkg &> /dev/null; then
        log_warning "vcpkg not found in PATH. Please install vcpkg first."
        log_info "See: https://github.com/Microsoft/vcpkg"
        return 1
    fi
    
    # Install dependencies
    log_info "Installing vcpkg packages..."
    vcpkg install --triplet=x64-linux || vcpkg install --triplet=x64-osx || vcpkg install
    
    log_success "vcpkg dependencies initialized"
}

# Initialize Git submodule dependencies
init_submodule_dependencies() {
    log_info "Setting up Git submodule dependencies..."
    
    if [[ "$DRY_RUN" == "true" ]]; then
        log_info "DRY RUN: Would initialize Git submodules"
        return 0
    fi
    
    cd "$PROJECT_ROOT"
    git submodule update --init --recursive
    
    log_success "Git submodule dependencies initialized"
}

# Initialize manual dependencies
init_manual_dependencies() {
    log_info "Manual dependencies are already included in third_party/manual"
    log_success "Manual dependencies ready"
}

# Initialize system dependencies
init_system_dependencies() {
    log_info "Setting up system dependencies..."
    
    if [[ "$DRY_RUN" == "true" ]]; then
        log_info "DRY RUN: Would install system dependencies"
        return 0
    fi
    
    if command -v brew &> /dev/null; then
        log_info "Installing dependencies with Homebrew..."
        brew install fmt nlohmann-json googletest
    elif command -v apt-get &> /dev/null; then
        log_info "Installing dependencies with APT..."
        sudo apt-get update
        sudo apt-get install -y libfmt-dev nlohmann-json3-dev libgtest-dev
    elif command -v yum &> /dev/null; then
        log_info "Installing dependencies with YUM..."
        sudo yum install -y fmt-devel json-devel gtest-devel
    else
        log_warning "No supported system package manager found"
        log_info "Please install dependencies manually:"
        log_info "  - fmt"
        log_info "  - nlohmann-json"
        log_info "  - googletest"
        return 1
    fi
    
    log_success "System dependencies initialized"
}

# Build project
build_project() {
    if [[ "$SKIP_BUILD" == "true" ]]; then
        log_info "Skipping build as requested"
        return 0
    fi
    
    log_step "Building project..."
    
    if [[ "$DRY_RUN" == "true" ]]; then
        log_info "DRY RUN: Would build project"
        return 0
    fi
    
    cd "$PROJECT_ROOT"
    
    # Clean previous build
    if [[ -d "build" ]]; then
        log_verbose "Removing existing build directory"
        rm -rf build
    fi
    
    # Configure build
    log_info "Configuring CMake..."
    mkdir build
    cd build
    
    local cmake_args=()
    
    case "$DEPENDENCY_METHOD" in
        vcpkg)
            local vcpkg_root
            vcpkg_root=$(vcpkg integrate install 2>/dev/null | grep -o '/[^"]*vcpkg' | head -1) || true
            if [[ -n "$vcpkg_root" ]]; then
                cmake_args+=("-DCMAKE_TOOLCHAIN_FILE=$vcpkg_root/scripts/buildsystems/vcpkg.cmake")
            fi
            ;;
    esac
    
    cmake .. "${cmake_args[@]}"
    
    # Build
    log_info "Building project..."
    cmake --build . --parallel $(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
    
    log_success "Project built successfully"
}

# Run tests
run_tests() {
    if [[ "$SKIP_TESTS" == "true" ]] || [[ "$SKIP_BUILD" == "true" ]]; then
        log_info "Skipping tests"
        return 0
    fi
    
    log_step "Running tests..."
    
    if [[ "$DRY_RUN" == "true" ]]; then
        log_info "DRY RUN: Would run tests"
        return 0
    fi
    
    cd "$PROJECT_ROOT/build"
    
    if ! ctest --output-on-failure; then
        log_warning "Some tests failed, but continuing..."
    else
        log_success "All tests passed"
    fi
}

# Initialize Git repository
init_git_repo() {
    if [[ "$SKIP_GIT_INIT" == "true" ]]; then
        log_info "Skipping Git initialization"
        return 0
    fi
    
    log_step "Initializing Git repository..."
    
    if [[ "$DRY_RUN" == "true" ]]; then
        log_info "DRY RUN: Would initialize Git repository"
        return 0
    fi
    
    cd "$PROJECT_ROOT"
    
    # Check if already a Git repository
    if [[ -d ".git" ]]; then
        log_info "Git repository already exists"
        
        # Ask if user wants to reset
        read -p "Do you want to reset the Git history? (y/N): " -n 1 -r
        echo
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            log_info "Resetting Git history..."
            rm -rf .git
            git init
            git add .
            git commit -m "Initial commit from cpp-template"
        fi
    else
        git init
        git add .
        git commit -m "Initial commit from cpp-template"
    fi
    
    log_success "Git repository initialized"
}

# Generate project summary
generate_summary() {
    log_step "Generating project summary..."
    
    echo
    echo "=" * 60
    echo -e "${GREEN}PROJECT SETUP COMPLETE!${NC}"
    echo "=" * 60
    echo -e "Project Name: ${BLUE}$PROJECT_NAME${NC}"
    echo -e "Location: ${BLUE}$PROJECT_ROOT${NC}"
    echo -e "Dependency Method: ${BLUE}$DEPENDENCY_METHOD${NC}"
    echo
    
    if [[ "$DRY_RUN" != "true" ]]; then
        echo "Next steps:"
        echo "1. Review the generated project structure"
        echo "2. Customize the code for your specific needs"
        echo "3. Update the README.md with your project details"
        echo "4. Set up your remote Git repository"
        echo
        
        echo "Quick commands:"
        echo -e "  ${CYAN}cd $PROJECT_ROOT${NC}"
        
        if [[ "$SKIP_BUILD" != "true" ]]; then
            echo -e "  ${CYAN}./build/src/$PROJECT_NAME${NC}  # Run the main application"
            echo -e "  ${CYAN}./build/examples/usage_scenarios${NC}  # Run usage examples"
        else
            echo -e "  ${CYAN}mkdir build && cd build${NC}"
            echo -e "  ${CYAN}cmake ..${NC}"
            echo -e "  ${CYAN}cmake --build .${NC}"
        fi
        
        echo
        echo "Documentation:"
        echo -e "  ${CYAN}docs/TEMPLATE_USAGE_GUIDE.md${NC}  # Detailed usage guide"
        echo -e "  ${CYAN}docs/CUSTOMIZATION_GUIDE.md${NC}  # Customization options"
        echo -e "  ${CYAN}README.md${NC}  # Project overview"
    fi
    
    echo
    log_success "Setup completed successfully!"
}

# Main function
main() {
    # Parse arguments
    parse_arguments "$@"
    
    # Show banner
    echo -e "${PURPLE}"
    echo "  ██████╗██████╗ ██████╗       ████████╗███████╗███╗   ███╗██████╗ ██╗      █████╗ ████████╗███████╗"
    echo " ██╔════╝██╔══██╗██╔══██╗      ╚══██╔══╝██╔════╝████╗ ████║██╔══██╗██║     ██╔══██╗╚══██╔══╝██╔════╝"
    echo " ██║     ██████╔╝██████╔╝         ██║   █████╗  ██╔████╔██║██████╔╝██║     ███████║   ██║   █████╗  "
    echo " ██║     ██╔═══╝ ██╔═══╝          ██║   ██╔══╝  ██║╚██╔╝██║██╔═══╝ ██║     ██╔══██║   ██║   ██╔══╝  "
    echo " ╚██████╗██║     ██║              ██║   ███████╗██║ ╚═╝ ██║██║     ███████╗██║  ██║   ██║   ███████╗"
    echo "  ╚═════╝╚═╝     ╚═╝              ╚═╝   ╚══════╝╚═╝     ╚═╝╚═╝     ╚══════╝╚═╝  ╚═╝   ╚═╝   ╚══════╝"
    echo -e "${NC}"
    echo -e "                                    ${BLUE}Quick Start Script v1.0.0${NC}"
    echo
    
    # Get project name
    get_project_name
    
    # Check prerequisites
    if [[ "$SKIP_PREREQ_CHECK" != "true" ]]; then
        check_prerequisites
    else
        log_info "Skipping prerequisite check as requested"
    fi
    
    # Detect dependency method
    detect_dependency_method
    
    # Show configuration
    if [[ "$DRY_RUN" == "true" ]]; then
        echo -e "${YELLOW}DRY RUN MODE - No changes will be made${NC}"
    fi
    
    echo "Configuration:"
    echo "  Project Name: $PROJECT_NAME"
    echo "  Dependency Method: $DEPENDENCY_METHOD"
    echo "  Skip Build: $SKIP_BUILD"
    echo "  Skip Tests: $SKIP_TESTS"
    echo "  Skip Git Init: $SKIP_GIT_INIT"
    echo
    
    if [[ "$DRY_RUN" != "true" ]]; then
        read -p "Continue with setup? (Y/n): " -n 1 -r
        echo
        if [[ $REPLY =~ ^[Nn]$ ]]; then
            log_info "Setup cancelled by user"
            exit 0
        fi
    fi
    
    # Execute setup steps
    replace_placeholders
    init_dependencies
    build_project
    run_tests
    init_git_repo
    generate_summary
}

# Error handling
trap 'log_error "Script failed at line $LINENO"' ERR

# Run main function
main "$@"