#!/bin/bash
# =============================================================================
# setup-pre-commit.sh - Pre-commit Setup Script
# Sets up pre-commit hooks for the cpp-template project
#
# This script installs pre-commit and configures git hooks to run
# code quality checks before each commit.
#
# Usage:
#   ./scripts/setup-pre-commit.sh [options]
#
# Options:
#   -h, --help     Show this help message
#   -f, --force    Force reinstall even if already installed
#   --skip-install Skip pre-commit installation (assume already installed)
# =============================================================================

set -euo pipefail

# Script directory and project root
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

# Default options
FORCE_INSTALL=false
SKIP_INSTALL=false

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# =============================================================================
# Helper Functions
# =============================================================================

print_usage() {
    cat <<EOF
Usage: $0 [OPTIONS]

Set up pre-commit hooks for the cpp-template project.

OPTIONS:
    -h, --help       Show this help message
    -f, --force      Force reinstall even if already installed
    --skip-install   Skip pre-commit installation (assume already installed)

EXAMPLES:
    $0                    # Install and set up pre-commit
    $0 --force            # Force reinstall
    $0 --skip-install     # Only set up hooks (assume pre-commit is installed)

EOF
}

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

# =============================================================================
# Argument Parsing
# =============================================================================

while [[ $# -gt 0 ]]; do
    case $1 in
        -h | --help)
            print_usage
            exit 0
            ;;
        -f | --force)
            FORCE_INSTALL=true
            shift
            ;;
        --skip-install)
            SKIP_INSTALL=true
            shift
            ;;
        *)
            log_error "Unknown option: $1"
            print_usage
            exit 1
            ;;
    esac
done

# =============================================================================
# Main Execution
# =============================================================================

cd "$PROJECT_ROOT"

echo "🪝 Pre-commit Setup"
echo "==================="
echo "Project: $(basename "$PROJECT_ROOT")"
echo ""

# =============================================================================
# Check Prerequisites
# =============================================================================

log_info "Checking prerequisites..."

# Check if we're in a git repository
if ! git rev-parse --git-dir &>/dev/null; then
    log_error "Not in a git repository!"
    exit 1
fi

# Check Python
if ! check_command python3; then
    log_error "Python 3 is required but not installed"
    log_info "Install Python 3: https://www.python.org/downloads/"
    exit 1
fi

python_version=$(python3 --version 2>&1 | cut -d' ' -f2)
log_info "Found Python: $python_version"

# Check pip
if ! check_command pip3 && ! python3 -m pip --version &>/dev/null; then
    log_error "pip is required but not installed"
    log_info "Install pip: python3 -m ensurepip --upgrade"
    exit 1
fi

log_success "Prerequisites check passed"
echo ""

# =============================================================================
# Install pre-commit
# =============================================================================

if [[ "$SKIP_INSTALL" != true ]]; then
    log_info "Checking pre-commit installation..."

    if check_command pre-commit && [[ "$FORCE_INSTALL" != true ]]; then
        pre_commit_version=$(pre-commit --version)
        log_info "Found pre-commit: $pre_commit_version"
    else
        if [[ "$FORCE_INSTALL" == true ]]; then
            log_info "Force installing pre-commit..."
        else
            log_info "Installing pre-commit..."
        fi

        # Try different installation methods
        if python3 -m pip install --user pre-commit; then
            log_success "pre-commit installed successfully with pip"
        elif check_command brew && brew install pre-commit; then
            log_success "pre-commit installed successfully with Homebrew"
        elif check_command apt-get && sudo apt-get update && sudo apt-get install -y pre-commit; then
            log_success "pre-commit installed successfully with apt"
        else
            log_error "Failed to install pre-commit"
            log_info "Please install manually: pip install pre-commit"
            exit 1
        fi

        # Verify installation
        if check_command pre-commit; then
            pre_commit_version=$(pre-commit --version)
            log_success "pre-commit installed: $pre_commit_version"
        else
            log_error "pre-commit installation verification failed"
            log_info "You may need to add ~/.local/bin to your PATH"
            log_info "Or restart your terminal session"
            exit 1
        fi
    fi

    echo ""
fi

# =============================================================================
# Install Git Hooks
# =============================================================================

log_info "Installing pre-commit git hooks..."

if pre-commit install; then
    log_success "Pre-commit git hooks installed successfully"
else
    log_error "Failed to install pre-commit git hooks"
    exit 1
fi

# Install commit-msg hook for commitizen
if pre-commit install --hook-type commit-msg; then
    log_success "Commit message hook installed successfully"
else
    log_warning "Failed to install commit message hook (optional)"
fi

# Install pre-push hook
if pre-commit install --hook-type pre-push; then
    log_success "Pre-push hook installed successfully"
else
    log_warning "Failed to install pre-push hook (optional)"
fi

echo ""

# =============================================================================
# Update Hook Dependencies
# =============================================================================

log_info "Installing/updating hook dependencies..."

if pre-commit install-hooks; then
    log_success "Hook dependencies installed successfully"
else
    log_warning "Some hook dependencies may not have installed correctly"
fi

echo ""

# =============================================================================
# Test Installation
# =============================================================================

log_info "Testing pre-commit installation..."

if pre-commit run --all-files --show-diff-on-failure; then
    log_success "Pre-commit test run completed successfully"
else
    log_warning "Pre-commit test run found issues"
    log_info "This is normal for the first run - hooks may have fixed formatting issues"
    log_info "Run 'pre-commit run --all-files' again to verify fixes"
fi

echo ""

# =============================================================================
# Configuration Summary
# =============================================================================

echo "📋 Configuration Summary"
echo "========================"

# Show enabled hooks
log_info "Enabled hooks:"
pre-commit run --all-files --verbose --dry-run 2>/dev/null | grep -E "^\[INFO\].*\.\.\.$" | sed 's/\[INFO\] */  - /' || true

echo ""

# Show configuration file
if [[ -f ".pre-commit-config.yaml" ]]; then
    log_success "Configuration file: .pre-commit-config.yaml"
else
    log_error "Configuration file missing: .pre-commit-config.yaml"
fi

echo ""

# =============================================================================
# Usage Instructions
# =============================================================================

echo "🎯 Usage Instructions"
echo "====================="

echo "Pre-commit hooks are now installed and will run automatically before each commit."
echo ""
echo "Available commands:"
echo "  pre-commit run                    # Run hooks on staged files"
echo "  pre-commit run --all-files        # Run hooks on all files"
echo "  pre-commit run <hook-id>          # Run specific hook"
echo "  pre-commit autoupdate             # Update hook versions"
echo ""
echo "To skip hooks for emergency commits:"
echo "  git commit --no-verify"
echo ""
echo "Individual hook examples:"
echo "  pre-commit run clang-format       # Format C++ code"
echo "  pre-commit run cmake-format       # Format CMake files"
echo "  pre-commit run clang-tidy-check   # Run static analysis"
echo "  pre-commit run build-test         # Build and test"
echo ""

# Show available hooks
log_info "Available hooks:"
if [[ -f ".pre-commit-config.yaml" ]]; then
    grep -E "^\s*-\s*id:" .pre-commit-config.yaml | sed 's/.*id: */  - /' | sort | uniq
fi

echo ""

log_success "Pre-commit setup completed successfully! 🎉"
echo ""
echo "💡 Tips:"
echo "  - Hooks will run automatically on 'git commit'"
echo "  - Fix any issues and commit again"
echo "  - Use 'git add' to stage fixes from hooks"
echo "  - Update hooks regularly with 'pre-commit autoupdate'"
