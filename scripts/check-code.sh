#!/bin/bash
# =============================================================================
# check-code.sh - Code Quality Analysis Script
# Comprehensive script to run static analysis on the cpp-template project
#
# This script runs various code quality tools including clang-tidy, cppcheck,
# and formatting checks to ensure code quality and consistency.
#
# Usage:
#   ./scripts/check-code.sh [options]
#
# Options:
#   -h, --help        Show this help message
#   -v, --verbose     Enable verbose output
#   --format-only     Run only formatting checks
#   --tidy-only       Run only clang-tidy
#   --cppcheck-only   Run only cppcheck
#   --no-format       Skip formatting checks
#   --no-tidy         Skip clang-tidy
#   --no-cppcheck     Skip cppcheck
# =============================================================================

set -euo pipefail

# Script directory and project root
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

# Default options
VERBOSE=false
FORMAT_ONLY=false
TIDY_ONLY=false
CPPCHECK_ONLY=false
NO_FORMAT=false
NO_TIDY=false
NO_CPPCHECK=false

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
NC='\033[0m' # No Color

# =============================================================================
# Helper Functions
# =============================================================================

print_usage() {
    cat <<EOF
Usage: $0 [OPTIONS]

Run code quality analysis on the cpp-template project.

OPTIONS:
    -h, --help        Show this help message
    -v, --verbose     Enable verbose output
    --format-only     Run only formatting checks
    --tidy-only       Run only clang-tidy
    --cppcheck-only   Run only cppcheck
    --no-format       Skip formatting checks
    --no-tidy         Skip clang-tidy
    --no-cppcheck     Skip cppcheck

EXAMPLES:
    $0                    # Run all checks
    $0 --verbose          # Run all checks with verbose output
    $0 --format-only      # Check only formatting
    $0 --tidy-only        # Run only clang-tidy
    $0 --no-cppcheck      # Run all except cppcheck

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

log_check() {
    echo -e "${PURPLE}[CHECK]${NC} $1"
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
    if [[ "$VERBOSE" == true ]]; then
        $tool --version 2>/dev/null | head -1 || echo "Version info not available"
    fi
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
        -v | --verbose)
            VERBOSE=true
            shift
            ;;
        --format-only)
            FORMAT_ONLY=true
            shift
            ;;
        --tidy-only)
            TIDY_ONLY=true
            shift
            ;;
        --cppcheck-only)
            CPPCHECK_ONLY=true
            shift
            ;;
        --no-format)
            NO_FORMAT=true
            shift
            ;;
        --no-tidy)
            NO_TIDY=true
            shift
            ;;
        --no-cppcheck)
            NO_CPPCHECK=true
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
exclusive_count=0
[[ "$FORMAT_ONLY" == true ]] && ((exclusive_count++))
[[ "$TIDY_ONLY" == true ]] && ((exclusive_count++))
[[ "$CPPCHECK_ONLY" == true ]] && ((exclusive_count++))

if [[ $exclusive_count -gt 1 ]]; then
    log_error "Cannot specify multiple --*-only options"
    exit 1
fi

# Set flags based on exclusive options
if [[ "$FORMAT_ONLY" == true ]]; then
    NO_TIDY=true
    NO_CPPCHECK=true
elif [[ "$TIDY_ONLY" == true ]]; then
    NO_FORMAT=true
    NO_CPPCHECK=true
elif [[ "$CPPCHECK_ONLY" == true ]]; then
    NO_FORMAT=true
    NO_TIDY=true
fi

# =============================================================================
# Main Execution
# =============================================================================

cd "$PROJECT_ROOT"

echo "🔍 Code Quality Analysis"
echo "========================"
echo "Project: $(basename "$PROJECT_ROOT")"
echo ""

# Track overall success
overall_success=true
checks_run=0
checks_passed=0

# =============================================================================
# Build Project (for compile_commands.json)
# =============================================================================

if [[ "$NO_TIDY" != true || "$NO_CPPCHECK" != true ]]; then
    echo "🏗️  Project Build"
    echo "-----------------"

    log_info "Checking if project needs to be built..."

    build_dir="$PROJECT_ROOT/build"
    compile_commands="$build_dir/compile_commands.json"

    need_build=false
    if [[ ! -d "$build_dir" ]]; then
        log_info "Build directory doesn't exist, need to configure"
        need_build=true
    elif [[ ! -f "$compile_commands" ]]; then
        log_info "compile_commands.json doesn't exist, need to build"
        need_build=true
    else
        log_info "Found existing build directory and compile_commands.json"
    fi

    if [[ "$need_build" == true ]]; then
        log_info "Configuring and building project..."

        if ! cmake -B "$build_dir" -S . \
            -DCMAKE_BUILD_TYPE=Debug \
            -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
            -DENABLE_CLANG_TIDY=OFF \
            -DENABLE_CPPCHECK=OFF; then
            log_error "CMake configuration failed"
            overall_success=false
        elif ! cmake --build "$build_dir" --target all; then
            log_warning "Build failed, but will continue with analysis"
        else
            log_success "Project built successfully"
        fi
    else
        log_success "Using existing build"
    fi

    echo ""
fi

# =============================================================================
# Formatting Checks
# =============================================================================

if [[ "$NO_FORMAT" != true ]]; then
    echo "🎨 Code Formatting Check"
    echo "------------------------"

    checks_run=$((checks_run + 1))

    # Use the format script in check mode
    format_script="$SCRIPT_DIR/format-code.sh"
    if [[ -x "$format_script" ]]; then
        log_check "Running formatting check..."

        format_args=("--check")
        if [[ "$VERBOSE" == true ]]; then
            format_args+=("--verbose")
        fi

        if "$format_script" "${format_args[@]}"; then
            log_success "Formatting check passed"
            checks_passed=$((checks_passed + 1))
        else
            log_error "Formatting check failed"
            overall_success=false
        fi
    else
        log_error "Format script not found or not executable: $format_script"
        overall_success=false
    fi

    echo ""
fi

# =============================================================================
# clang-tidy Analysis
# =============================================================================

if [[ "$NO_TIDY" != true ]]; then
    echo "🔧 clang-tidy Analysis"
    echo "----------------------"

    checks_run=$((checks_run + 1))

    if check_tool "clang-tidy" "apt-get install clang-tidy (Ubuntu) or brew install llvm (macOS)"; then
        # Find C++ source files
        cpp_source_files=()
        for ext in "*.cpp" "*.cxx" "*.cc" "*.c++"; do
            while IFS= read -r -d '' file; do
                cpp_source_files+=("$file")
            done < <(find_files "$ext" -print0)
        done

        if [[ ${#cpp_source_files[@]} -eq 0 ]]; then
            log_warning "No C++ source files found for clang-tidy analysis"
        else
            log_info "Found ${#cpp_source_files[@]} C++ source files"
            log_check "Running clang-tidy analysis..."

            # Check for config file
            tidy_config="$PROJECT_ROOT/.clang-tidy"
            tidy_args=()
            if [[ -f "$tidy_config" ]]; then
                tidy_args+=("--config-file=$tidy_config")
                log_info "Using config file: $tidy_config"
            else
                log_warning "No .clang-tidy config file found, using defaults"
            fi

            # Set up compile database
            compile_commands="$PROJECT_ROOT/build/compile_commands.json"
            if [[ -f "$compile_commands" ]]; then
                tidy_args+=("-p" "$PROJECT_ROOT/build")
                log_info "Using compile database: $compile_commands"
            else
                log_warning "No compile_commands.json found, analysis may be limited"
            fi

            # Add header filter
            tidy_args+=("--header-filter=$PROJECT_ROOT/(src|libs|include)/.*\\.(h|hpp|hxx)")

            # Run clang-tidy
            tidy_failed=false
            error_count=0
            warning_count=0

            for file in "${cpp_source_files[@]}"; do
                if [[ "$VERBOSE" == true ]]; then
                    echo "  Analyzing: $file"
                fi

                output_file=$(mktemp)
                if clang-tidy "${tidy_args[@]}" "$file" 2>&1 | tee "$output_file"; then
                    # Count warnings and errors
                    file_warnings=$(grep -c "warning:" "$output_file" || true)
                    file_errors=$(grep -c "error:" "$output_file" || true)
                    warning_count=$((warning_count + file_warnings))
                    error_count=$((error_count + file_errors))

                    if [[ $file_errors -gt 0 ]]; then
                        tidy_failed=true
                    fi

                    if [[ "$VERBOSE" == true && ($file_warnings -gt 0 || $file_errors -gt 0) ]]; then
                        echo "    Warnings: $file_warnings, Errors: $file_errors"
                    fi
                else
                    log_error "clang-tidy failed on $file"
                    tidy_failed=true
                fi

                rm -f "$output_file"
            done

            # Summary
            if [[ "$tidy_failed" == true ]]; then
                log_error "clang-tidy analysis failed (Errors: $error_count, Warnings: $warning_count)"
                overall_success=false
            elif [[ $warning_count -gt 0 ]]; then
                log_warning "clang-tidy analysis completed with $warning_count warnings"
                log_success "clang-tidy check passed (no errors)"
                checks_passed=$((checks_passed + 1))
            else
                log_success "clang-tidy analysis passed with no issues"
                checks_passed=$((checks_passed + 1))
            fi
        fi
    else
        log_warning "Skipping clang-tidy analysis (tool not available)"
        overall_success=false
    fi

    echo ""
fi

# =============================================================================
# cppcheck Analysis
# =============================================================================

if [[ "$NO_CPPCHECK" != true ]]; then
    echo "🔍 cppcheck Analysis"
    echo "--------------------"

    checks_run=$((checks_run + 1))

    if check_tool "cppcheck" "apt-get install cppcheck (Ubuntu) or brew install cppcheck (macOS)"; then
        log_check "Running cppcheck analysis..."

        # cppcheck arguments
        cppcheck_args=(
            "--enable=warning,style,performance,portability,unusedFunction"
            "--std=c++17"
            "--verbose"
            "--quiet"
            "--error-exitcode=1"
            "--inline-suppr"
        )

        # Use compile database if available
        compile_commands="$PROJECT_ROOT/build/compile_commands.json"
        if [[ -f "$compile_commands" ]]; then
            cppcheck_args+=("--project=$compile_commands")
            log_info "Using compile database: $compile_commands"
        else
            # Manually specify directories
            cppcheck_args+=(
                "$PROJECT_ROOT/src"
                "$PROJECT_ROOT/libs"
                "$PROJECT_ROOT/include"
            )
            log_warning "No compile database found, analyzing directories manually"
        fi

        # Add suppressions file if it exists
        suppressions_file="$PROJECT_ROOT/.cppcheck-suppressions"
        if [[ -f "$suppressions_file" ]]; then
            cppcheck_args+=("--suppressions-list=$suppressions_file")
            log_info "Using suppressions file: $suppressions_file"
        fi

        # Include paths
        if [[ -d "$PROJECT_ROOT/include" ]]; then
            cppcheck_args+=("-I$PROJECT_ROOT/include")
        fi
        if [[ -d "$PROJECT_ROOT/libs" ]]; then
            cppcheck_args+=("-I$PROJECT_ROOT/libs")
        fi

        # Run cppcheck
        output_file=$(mktemp)
        if cppcheck "${cppcheck_args[@]}" 2>&1 | tee "$output_file"; then
            # Count issues
            error_count=$(grep -c "error:" "$output_file" || true)
            warning_count=$(grep -c "warning:" "$output_file" || true)
            style_count=$(grep -c "style:" "$output_file" || true)
            performance_count=$(grep -c "performance:" "$output_file" || true)

            total_issues=$((error_count + warning_count + style_count + performance_count))

            if [[ $total_issues -eq 0 ]]; then
                log_success "cppcheck analysis passed with no issues"
                checks_passed=$((checks_passed + 1))
            else
                log_warning "cppcheck found $total_issues issues:"
                [[ $error_count -gt 0 ]] && echo "  - Errors: $error_count"
                [[ $warning_count -gt 0 ]] && echo "  - Warnings: $warning_count"
                [[ $style_count -gt 0 ]] && echo "  - Style: $style_count"
                [[ $performance_count -gt 0 ]] && echo "  - Performance: $performance_count"

                if [[ $error_count -gt 0 ]]; then
                    log_error "cppcheck analysis failed due to errors"
                    overall_success=false
                else
                    log_success "cppcheck check passed (no errors, only warnings/suggestions)"
                    checks_passed=$((checks_passed + 1))
                fi
            fi
        else
            log_error "cppcheck analysis failed"
            overall_success=false
        fi

        rm -f "$output_file"
    else
        log_warning "Skipping cppcheck analysis (tool not available)"
    fi

    echo ""
fi

# =============================================================================
# Summary
# =============================================================================

echo "📊 Analysis Summary"
echo "==================="

echo "Checks run: $checks_run"
echo "Checks passed: $checks_passed"
echo "Checks failed: $((checks_run - checks_passed))"
echo ""

if [[ "$overall_success" == true ]]; then
    log_success "All code quality checks passed! ✨"
    echo ""
    echo "🎉 Your code quality looks excellent!"
    echo ""
    echo "💡 To maintain code quality:"
    echo "   - Run this script regularly: $0"
    echo "   - Format code before committing: ./scripts/format-code.sh"
    echo "   - Consider enabling pre-commit hooks"
    exit 0
else
    log_error "Some code quality checks failed! 🚨"
    echo ""
    echo "💡 To fix issues:"
    if [[ "$NO_FORMAT" != true ]]; then
        echo "   - Fix formatting: ./scripts/format-code.sh"
    fi
    if [[ "$NO_TIDY" != true ]]; then
        echo "   - Review clang-tidy suggestions and fix issues"
        echo "   - Run: clang-tidy --fix-errors <file> (be careful!)"
    fi
    if [[ "$NO_CPPCHECK" != true ]]; then
        echo "   - Review cppcheck warnings and address issues"
        echo "   - Add suppressions to .cppcheck-suppressions if needed"
    fi
    echo ""
    echo "📚 Documentation:"
    echo "   - clang-tidy: https://clang.llvm.org/extra/clang-tidy/"
    echo "   - cppcheck: https://cppcheck.sourceforge.io/"
    exit 1
fi
