#!/bin/bash
# =============================================================================
# format-code.sh - Code Formatting Script
# Comprehensive script to format all code in the cpp-template project
#
# This script formats C++, CMake, and other project files using various
# formatting tools to maintain consistent code style.
#
# Usage:
#   ./scripts/format-code.sh [options]
#
# Options:
#   -h, --help     Show this help message
#   -c, --check    Check formatting without making changes
#   -v, --verbose  Enable verbose output
#   --cpp-only     Format only C++ files
#   --cmake-only   Format only CMake files
# =============================================================================

set -euo pipefail

# Script directory and project root
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

# Default options
CHECK_ONLY=false
VERBOSE=false
CPP_ONLY=false
CMAKE_ONLY=false

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

Format all code in the cpp-template project.

OPTIONS:
    -h, --help       Show this help message
    -c, --check      Check formatting without making changes
    -v, --verbose    Enable verbose output
    --cpp-only       Format only C++ files
    --cmake-only     Format only CMake files

EXAMPLES:
    $0                    # Format all files
    $0 --check            # Check formatting compliance
    $0 --cpp-only         # Format only C++ files
    $0 --cmake-only       # Format only CMake files
    $0 --check --verbose  # Check with verbose output

EOF
}

log_info() {
    if [[ "$VERBOSE" == true ]]; then
        echo -e "${BLUE}[INFO]${NC} $1"
    fi
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

check_tool() {
    local tool=$1
    local install_hint=$2

    if ! command -v "$tool" &>/dev/null; then
        log_error "$tool is not installed"
        log_info "Install with: $install_hint"
        return 1
    fi

    log_info "Found $tool: $(command -v "$tool")"
    return 0
}

find_files() {
    local pattern=$1
    local exclude_dirs=("third_party" "build" ".git")

    local exclude_args=()
    for dir in "${exclude_dirs[@]}"; do
        exclude_args+=("-path" "./$dir" "-prune" "-o")
    done

    find "$PROJECT_ROOT" "${exclude_args[@]}" -name "$pattern" -type f -print
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
        -c | --check)
            CHECK_ONLY=true
            shift
            ;;
        -v | --verbose)
            VERBOSE=true
            shift
            ;;
        --cpp-only)
            CPP_ONLY=true
            shift
            ;;
        --cmake-only)
            CMAKE_ONLY=true
            shift
            ;;
        *)
            log_error "Unknown option: $1"
            print_usage
            exit 1
            ;;
    esac
done

# Validate options
if [[ "$CPP_ONLY" == true && "$CMAKE_ONLY" == true ]]; then
    log_error "Cannot specify both --cpp-only and --cmake-only"
    exit 1
fi

# =============================================================================
# Main Execution
# =============================================================================

cd "$PROJECT_ROOT"

echo "🎨 Code Formatting Script"
echo "========================="
echo "Project: $(basename "$PROJECT_ROOT")"
echo "Mode: $(if [[ "$CHECK_ONLY" == true ]]; then echo "CHECK"; else echo "FORMAT"; fi)"
echo ""

# Track overall success
overall_success=true

# =============================================================================
# C++ File Formatting
# =============================================================================

if [[ "$CMAKE_ONLY" != true ]]; then
    echo "🔧 C++ Code Formatting"
    echo "-----------------------"

    # Check for clang-format
    if check_tool "clang-format" "apt-get install clang-format (Ubuntu) or brew install clang-format (macOS)"; then
        # Find C++ files
        cpp_files=()
        for ext in "*.cpp" "*.hpp" "*.h" "*.cxx" "*.hxx" "*.cc" "*.c++" "*.h++"; do
            while IFS= read -r -d '' file; do
                cpp_files+=("$file")
            done < <(find_files "$ext" -print0)
        done

        if [[ ${#cpp_files[@]} -eq 0 ]]; then
            log_warning "No C++ files found"
        else
            log_info "Found ${#cpp_files[@]} C++ files"

            if [[ "$CHECK_ONLY" == true ]]; then
                log_info "Checking C++ formatting..."
                failed_files=()

                for file in "${cpp_files[@]}"; do
                    if [[ "$VERBOSE" == true ]]; then
                        echo "  Checking: $file"
                    fi

                    if ! clang-format --style=file --dry-run --Werror "$file" &>/dev/null; then
                        failed_files+=("$file")
                    fi
                done

                if [[ ${#failed_files[@]} -eq 0 ]]; then
                    log_success "All C++ files are properly formatted"
                else
                    log_error "${#failed_files[@]} C++ files have formatting issues:"
                    for file in "${failed_files[@]}"; do
                        echo "  ❌ $file"
                    done
                    overall_success=false
                fi
            else
                log_info "Formatting C++ files..."
                formatted_count=0

                for file in "${cpp_files[@]}"; do
                    if [[ "$VERBOSE" == true ]]; then
                        echo "  Formatting: $file"
                    fi

                    # Create backup and format
                    cp "$file" "$file.backup"
                    if clang-format --style=file -i "$file"; then
                        # Check if file changed
                        if ! cmp -s "$file" "$file.backup"; then
                            formatted_count=$((formatted_count + 1))
                            if [[ "$VERBOSE" == true ]]; then
                                echo "    ✅ Formatted"
                            fi
                        elif [[ "$VERBOSE" == true ]]; then
                            echo "    ✅ Already formatted"
                        fi
                    else
                        log_error "Failed to format $file"
                        # Restore backup
                        mv "$file.backup" "$file"
                        overall_success=false
                        continue
                    fi

                    # Remove backup
                    rm "$file.backup"
                done

                if [[ $formatted_count -gt 0 ]]; then
                    log_success "Formatted $formatted_count C++ files"
                else
                    log_success "All C++ files were already formatted"
                fi
            fi
        fi
    else
        log_warning "Skipping C++ formatting (clang-format not available)"
        overall_success=false
    fi

    echo ""
fi

# =============================================================================
# CMake File Formatting
# =============================================================================

if [[ "$CPP_ONLY" != true ]]; then
    echo "🏗️  CMake File Formatting"
    echo "-------------------------"

    # Check for cmake-format
    if check_tool "cmake-format" "pip install cmake-format"; then
        # Find CMake files
        cmake_files=()
        for pattern in "CMakeLists.txt" "*.cmake"; do
            while IFS= read -r -d '' file; do
                cmake_files+=("$file")
            done < <(find_files "$pattern" -print0)
        done

        if [[ ${#cmake_files[@]} -eq 0 ]]; then
            log_warning "No CMake files found"
        else
            log_info "Found ${#cmake_files[@]} CMake files"

            # Check if config file exists
            config_file="$PROJECT_ROOT/.cmake-format.yaml"
            config_args=()
            if [[ -f "$config_file" ]]; then
                config_args=("--config-file" "$config_file")
                log_info "Using config file: $config_file"
            else
                log_warning "No .cmake-format.yaml config file found"
            fi

            if [[ "$CHECK_ONLY" == true ]]; then
                log_info "Checking CMake formatting..."
                failed_files=()

                for file in "${cmake_files[@]}"; do
                    if [[ "$VERBOSE" == true ]]; then
                        echo "  Checking: $file"
                    fi

                    if ! cmake-format "${config_args[@]}" --check "$file" &>/dev/null; then
                        failed_files+=("$file")
                    fi
                done

                if [[ ${#failed_files[@]} -eq 0 ]]; then
                    log_success "All CMake files are properly formatted"
                else
                    log_error "${#failed_files[@]} CMake files have formatting issues:"
                    for file in "${failed_files[@]}"; do
                        echo "  ❌ $file"
                    done
                    overall_success=false
                fi
            else
                log_info "Formatting CMake files..."
                formatted_count=0

                for file in "${cmake_files[@]}"; do
                    if [[ "$VERBOSE" == true ]]; then
                        echo "  Formatting: $file"
                    fi

                    # Create backup and format
                    cp "$file" "$file.backup"
                    if cmake-format "${config_args[@]}" --in-place "$file"; then
                        # Check if file changed
                        if ! cmp -s "$file" "$file.backup"; then
                            formatted_count=$((formatted_count + 1))
                            if [[ "$VERBOSE" == true ]]; then
                                echo "    ✅ Formatted"
                            fi
                        elif [[ "$VERBOSE" == true ]]; then
                            echo "    ✅ Already formatted"
                        fi
                    else
                        log_error "Failed to format $file"
                        # Restore backup
                        mv "$file.backup" "$file"
                        overall_success=false
                        continue
                    fi

                    # Remove backup
                    rm "$file.backup"
                done

                if [[ $formatted_count -gt 0 ]]; then
                    log_success "Formatted $formatted_count CMake files"
                else
                    log_success "All CMake files were already formatted"
                fi
            fi
        fi
    else
        log_warning "Skipping CMake formatting (cmake-format not available)"
    fi

    echo ""
fi

# =============================================================================
# Summary
# =============================================================================

echo "📋 Summary"
echo "=========="

if [[ "$overall_success" == true ]]; then
    if [[ "$CHECK_ONLY" == true ]]; then
        log_success "All files are properly formatted! ✨"
    else
        log_success "Code formatting completed successfully! ✨"
    fi
    echo ""
    echo "🎉 Your code looks great!"
    exit 0
else
    if [[ "$CHECK_ONLY" == true ]]; then
        log_error "Some files have formatting issues! 🚨"
        echo ""
        echo "💡 To fix formatting issues, run:"
        echo "   $0  # (without --check)"
    else
        log_error "Some formatting operations failed! 🚨"
        echo ""
        echo "💡 Please check the error messages above and try again"
    fi
    echo ""
    echo "📚 For more information:"
    echo "   - clang-format: https://clang.llvm.org/docs/ClangFormat.html"
    echo "   - cmake-format: https://cmake-format.readthedocs.io/"
    exit 1
fi
