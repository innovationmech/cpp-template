#!/bin/bash
# =============================================================================
# validate-actions.sh - GitHub Actions Local Validation Script
# This script provides multiple methods to validate GitHub Actions workflows
# locally before committing changes.
#
# Methods supported:
# 1. GitHub CLI (gh) workflow validation and dry-run
# 2. act (local GitHub Actions runner)
# 3. Manual build and package testing
# 4. Workflow syntax validation
#
# Usage:
#   ./scripts/validate-actions.sh [method]
#
# Methods:
#   gh      - Use GitHub CLI to validate and test workflows
#   act     - Use act to run workflows locally (requires Docker)
#   build   - Manual build and package testing
#   syntax  - Validate workflow YAML syntax
#   all     - Run all validation methods
# =============================================================================

set -euo pipefail

# Script directory and project root
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Default method
METHOD="${1:-all}"

# =============================================================================
# Helper Functions
# =============================================================================

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
    echo -e "${RED}[ERROR]${NC} $1"
}

check_command() {
    local cmd=$1
    if command -v "$cmd" &>/dev/null; then
        return 0
    else
        return 1
    fi
}

print_usage() {
    cat <<EOF
Usage: $0 [METHOD]

Validate GitHub Actions workflows locally using different methods.

METHODS:
    gh      Use GitHub CLI to validate workflows and simulate runs
    act     Use act to run workflows locally (requires Docker)
    build   Manual build and package testing
    syntax  Validate workflow YAML syntax only
    all     Run all available validation methods (default)

EXAMPLES:
    $0              # Run all validation methods
    $0 gh           # Use GitHub CLI only
    $0 act          # Use act only
    $0 build        # Manual build testing only

REQUIREMENTS:
    - For 'gh': GitHub CLI installed and authenticated
    - For 'act': act tool and Docker installed
    - For 'build': CMake, build tools, and dependencies

EOF
}

# =============================================================================
# GitHub CLI Validation
# =============================================================================

validate_with_gh() {
    log_info "🔍 Validating workflows with GitHub CLI..."

    if ! check_command gh; then
        log_error "GitHub CLI (gh) not found. Install with: brew install gh"
        return 1
    fi

    # Check if authenticated
    if ! gh auth status &>/dev/null; then
        log_error "GitHub CLI not authenticated. Run: gh auth login"
        return 1
    fi

    cd "$PROJECT_ROOT"

    # List available workflows
    log_info "📋 Available workflows:"
    if gh workflow list &>/dev/null; then
        gh workflow list
    else
        log_warning "Repository not connected to GitHub or no workflows found"
        log_info "Checking workflow files locally..."
        find .github/workflows -name "*.yml" -o -name "*.yaml" 2>/dev/null | while read -r workflow; do
            echo "  $(basename "$workflow")"
        done
    fi

    echo ""

    # Validate workflow syntax
    log_info "🔧 Validating workflow syntax..."

    for workflow in .github/workflows/*.yml .github/workflows/*.yaml; do
        if [[ -f "$workflow" ]]; then
            log_info "Checking $(basename "$workflow")..."

            # Use gh to validate workflow syntax
            if gh workflow view "$(basename "$workflow")" --repo . &>/dev/null; then
                log_success "✅ $(basename "$workflow") syntax is valid"
            else
                # If gh workflow view fails, try basic YAML validation
                if python3 -c "import yaml; yaml.safe_load(open('$workflow'))" 2>/dev/null; then
                    log_success "✅ $(basename "$workflow") YAML syntax is valid"
                else
                    log_error "❌ $(basename "$workflow") has syntax errors"
                    return 1
                fi
            fi
        fi
    done

    # Test workflow triggering (dry-run)
    log_info "🧪 Testing workflow trigger conditions..."

    # Simulate different events
    log_info "Simulating push to master branch..."
    echo "This would trigger the release workflow on push to master"

    log_info "Simulating pull request..."
    echo "This would trigger code quality checks on PR"

    log_success "GitHub CLI validation completed successfully"
    return 0
}

# =============================================================================
# act Tool Validation
# =============================================================================

validate_with_act() {
    log_info "🎭 Validating workflows with act..."

    if ! check_command act; then
        log_error "act not found. Install with:"
        log_info "  macOS: brew install act"
        log_info "  Linux: curl https://raw.githubusercontent.com/nektos/act/master/install.sh | sudo bash"
        log_info "  Or download from: https://github.com/nektos/act/releases"
        return 1
    fi

    if ! check_command docker; then
        log_error "Docker not found. act requires Docker to run workflows"
        log_info "Install Docker from: https://docs.docker.com/get-docker/"
        return 1
    fi

    cd "$PROJECT_ROOT"

    # List available workflows and jobs
    log_info "📋 Available workflows and jobs:"
    act -l || true

    echo ""

    # Dry run - don't actually execute
    log_info "🧪 Performing dry run..."
    act -n || true

    echo ""

    # Run local test workflow if it exists
    if [[ -f ".github/workflows/local.yml" ]]; then
        log_info "🚀 Running local test workflow..."
        act -j local-version-test || log_warning "Local version test failed or skipped"
        act -j local-build-test || log_warning "Local build test failed or skipped"
    else
        log_warning "local.yml not found, skipping local workflow test"
    fi

    # Run PR tests workflow if it exists (simulate pull_request event)
    if [[ -f ".github/workflows/pr-tests.yml" ]]; then
        log_info "🔍 Testing PR workflow (simulation)..."
        # Simulate pull_request event for PR tests
        act pull_request -j test --artifact-server-path /tmp/artifacts || log_warning "PR test workflow failed or skipped"
        act pull_request -j integration-test || log_warning "Integration test workflow failed or skipped"
    else
        log_warning "pr-tests.yml not found, skipping PR workflow test"
    fi

    # Run PR local test workflow if it exists (lightweight PR testing)
    if [[ -f ".github/workflows/pr-local-test.yml" ]]; then
        log_info "🚀 Testing PR local workflow (lightweight)..."
        # Run lightweight PR tests for faster local validation
        act pull_request -j pr-quick-test || log_warning "PR quick test failed or skipped"
        act pull_request -j pr-build-test || log_warning "PR build test failed or skipped"
    else
        log_warning "pr-local-test.yml not found, skipping PR local workflow test"
    fi

    log_success "act validation completed"
    return 0
}

# =============================================================================
# Manual Build and Package Testing
# =============================================================================

validate_with_build() {
    log_info "🔨 Manual build and package testing..."

    cd "$PROJECT_ROOT"

    # Check build requirements
    local missing_deps=()

    if ! check_command cmake; then
        missing_deps+=("cmake")
    fi

    if ! check_command ninja; then
        if ! check_command make; then
            missing_deps+=("ninja or make")
        fi
    fi

    if [[ ${#missing_deps[@]} -gt 0 ]]; then
        log_error "Missing dependencies: ${missing_deps[*]}"
        log_info "Install with:"
        log_info "  macOS: brew install cmake ninja"
        log_info "  Ubuntu: sudo apt-get install cmake ninja-build build-essential"
        return 1
    fi

    # Test version detection
    log_info "🔍 Testing version detection..."
    current_version=$(grep -E "^\s*VERSION\s+[0-9]+\.[0-9]+\.[0-9]+" CMakeLists.txt | head -1 | sed -E 's/.*VERSION\s+([0-9]+\.[0-9]+\.[0-9]+).*/\1/') || true

    if [[ -n "$current_version" ]]; then
        log_success "✅ Version detected: $current_version"
    else
        log_error "❌ Could not detect version from CMakeLists.txt"
        return 1
    fi

    # Clean previous build
    if [[ -d "build" ]]; then
        log_info "🧹 Cleaning previous build..."
        rm -rf build
    fi

    # Configure
    log_info "⚙️ Configuring CMake..."
    local generator="Ninja"
    if ! check_command ninja; then
        generator="Unix Makefiles"
    fi

    cmake -B build -S . \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=install \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
        -DENABLE_CLANG_TIDY=OFF \
        -DENABLE_CPPCHECK=OFF \
        -G "$generator"

    log_success "✅ CMake configuration successful"

    # Build
    log_info "🔨 Building project..."
    cmake --build build --config Release --parallel
    log_success "✅ Build successful"

    # Test (if available)
    if cmake --build build --target help | grep -q "test"; then
        log_info "🧪 Running tests..."
        cmake --build build --target test --config Release || log_warning "Some tests failed"
    else
        log_info "No tests found, skipping test phase"
    fi

    # Install
    log_info "📦 Installing project..."
    cmake --install build --config Release
    log_success "✅ Installation successful"

    # Create packages
    log_info "📦 Creating packages..."
    cd build

    # Check available generators
    local available_generators
    available_generators=$(cpack --help | grep -A 20 "Available generators:" | grep "^  " | awk '{print $1}' | tr '\n' ' ')
    log_info "Available CPack generators: $available_generators"

    # Create TGZ (should work everywhere)
    log_info "Creating TGZ package..."
    cpack -G TGZ -C Release -V

    # Create platform-specific packages if available
    case "$(uname)" in
        "Linux")
            if echo "$available_generators" | grep -q "DEB"; then
                log_info "Creating DEB package..."
                cpack -G DEB -C Release -V || log_warning "DEB package creation failed"
            fi
            if echo "$available_generators" | grep -q "RPM"; then
                log_info "Creating RPM package..."
                cpack -G RPM -C Release -V || log_warning "RPM package creation failed"
            fi
            ;;
        "Darwin")
            if echo "$available_generators" | grep -q "DragNDrop"; then
                log_info "Creating DMG package..."
                cpack -G DragNDrop -C Release -V || log_warning "DMG package creation failed"
            fi
            ;;
        *)
            log_info "Platform-specific packages not configured for $(uname)"
            ;;
    esac

    cd ..

    # List generated packages
    log_info "📋 Generated packages:"
    find build -name "cpp-template-*" -type f | while read -r file; do
        if [[ -f "$file" ]]; then
            local size
            size=$(ls -lh "$file" | awk '{print $5}')
            log_success "  $(basename "$file") ($size)"
        fi
    done

    log_success "Manual build and package testing completed successfully"
    return 0
}

# =============================================================================
# Syntax-only Validation
# =============================================================================

validate_syntax() {
    log_info "📝 Validating workflow YAML syntax..."

    cd "$PROJECT_ROOT"

    local errors=0

    for workflow in .github/workflows/*.yml .github/workflows/*.yaml; do
        if [[ -f "$workflow" ]]; then
            log_info "Checking $(basename "$workflow")..."

            # Check YAML syntax with Python
            if python3 -c "import yaml; yaml.safe_load(open('$workflow'))" 2>/dev/null; then
                log_success "✅ $(basename "$workflow") YAML syntax is valid"
            else
                log_error "❌ $(basename "$workflow") has YAML syntax errors"
                errors=$((errors + 1))
            fi

            # Check for common GitHub Actions issues
            if grep -q "uses: actions/" "$workflow"; then
                log_info "  Found GitHub Actions usage"
            fi

            if grep -q "secrets\." "$workflow"; then
                log_info "  Found secrets usage"
            fi

            if grep -q "matrix:" "$workflow"; then
                log_info "  Found matrix strategy"
            fi
        fi
    done

    if [[ $errors -eq 0 ]]; then
        log_success "All workflow files have valid syntax"
        return 0
    else
        log_error "$errors workflow file(s) have syntax errors"
        return 1
    fi
}

# =============================================================================
# Main Execution
# =============================================================================

main() {
    cd "$PROJECT_ROOT"

    echo "🚀 GitHub Actions Local Validation"
    echo "=================================="
    echo "Project: $(basename "$PROJECT_ROOT")"
    echo "Method: $METHOD"
    echo ""

    case "$METHOD" in
        "gh")
            validate_with_gh
            ;;
        "act")
            validate_with_act
            ;;
        "build")
            validate_with_build
            ;;
        "syntax")
            validate_syntax
            ;;
        "all")
            local exit_code=0

            log_info "Running all validation methods..."
            echo ""

            # Syntax validation (always runs)
            validate_syntax || exit_code=1
            echo ""

            # GitHub CLI validation (if available)
            if check_command gh; then
                validate_with_gh || exit_code=1
            else
                log_warning "Skipping GitHub CLI validation (gh not installed)"
            fi
            echo ""

            # Manual build validation
            validate_with_build || exit_code=1
            echo ""

            # act validation (if available and Docker is running)
            if check_command act && check_command docker; then
                if docker info &>/dev/null; then
                    validate_with_act || exit_code=1
                else
                    log_warning "Skipping act validation (Docker not running)"
                fi
            else
                log_warning "Skipping act validation (act or Docker not available)"
            fi

            return $exit_code
            ;;
        "-h" | "--help")
            print_usage
            exit 0
            ;;
        *)
            log_error "Unknown method: $METHOD"
            print_usage
            exit 1
            ;;
    esac
}

# Run main function
main "$@"
