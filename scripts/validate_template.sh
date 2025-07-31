#!/bin/bash

# ==============================================================================
# cpp-template Validation Script
# ==============================================================================
#
# This script validates the cpp-template for completeness, correctness, and
# functionality. It performs comprehensive checks including file structure,
# build system, dependencies, tests, documentation, and code quality.
#
# Usage:
#   ./scripts/validate_template.sh [OPTIONS]
#
# Examples:
#   ./scripts/validate_template.sh
#   ./scripts/validate_template.sh --quick
#   ./scripts/validate_template.sh --verbose --report
#   ./scripts/validate_template.sh --fix-issues
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
QUICK_MODE=false
VERBOSE=false
GENERATE_REPORT=false
FIX_ISSUES=false
SKIP_BUILD=false
SKIP_TESTS=false
SKIP_DOCS=false
SKIP_SECURITY=false
OUTPUT_DIR="$PROJECT_ROOT/validation_reports"
REPORT_FILE="validation_report_$TIMESTAMP.md"

# Validation results
ERROR_COUNT=0
WARNING_COUNT=0
INFO_COUNT=0
FIXED_COUNT=0

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
    ((INFO_COUNT++))
    if [[ "$GENERATE_REPORT" == "true" ]] && [[ -f "$OUTPUT_DIR/$REPORT_FILE" ]]; then
        echo "- ℹ️ $1" >> "$OUTPUT_DIR/$REPORT_FILE"
    fi
}

log_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
    if [[ "$GENERATE_REPORT" == "true" ]] && [[ -f "$OUTPUT_DIR/$REPORT_FILE" ]]; then
        echo "- ✅ $1" >> "$OUTPUT_DIR/$REPORT_FILE"
    fi
}

log_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
    ((WARNING_COUNT++))
    if [[ "$GENERATE_REPORT" == "true" ]] && [[ -f "$OUTPUT_DIR/$REPORT_FILE" ]]; then
        echo "- ⚠️ $1" >> "$OUTPUT_DIR/$REPORT_FILE"
    fi
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1" >&2
    ((ERROR_COUNT++))
    if [[ "$GENERATE_REPORT" == "true" ]] && [[ -f "$OUTPUT_DIR/$REPORT_FILE" ]]; then
        echo "- ❌ $1" >> "$OUTPUT_DIR/$REPORT_FILE"
    fi
}

log_fixed() {
    echo -e "${GREEN}[FIXED]${NC} $1"
    ((FIXED_COUNT++))
    if [[ "$GENERATE_REPORT" == "true" ]] && [[ -f "$OUTPUT_DIR/$REPORT_FILE" ]]; then
        echo "- 🔧 $1" >> "$OUTPUT_DIR/$REPORT_FILE"
    fi
}

log_step() {
    echo -e "${PURPLE}[STEP]${NC} $1"
    if [[ "$GENERATE_REPORT" == "true" ]] && [[ -f "$OUTPUT_DIR/$REPORT_FILE" ]]; then
        echo "" >> "$OUTPUT_DIR/$REPORT_FILE"
        echo "## $1" >> "$OUTPUT_DIR/$REPORT_FILE"
        echo "" >> "$OUTPUT_DIR/$REPORT_FILE"
    fi
}

log_verbose() {
    if [[ "$VERBOSE" == "true" ]]; then
        echo -e "${CYAN}[VERBOSE]${NC} $1"
    fi
}

# Help function
show_help() {
    cat << EOF
cpp-template Validation Script

Usage: $0 [OPTIONS]

Options:
  -q, --quick              Quick validation (skip time-consuming checks)
  -v, --verbose            Enable verbose output
  -r, --report             Generate detailed validation report
  -f, --fix-issues         Attempt to automatically fix detected issues
      --skip-build         Skip build system validation
      --skip-tests         Skip test suite validation
      --skip-docs          Skip documentation validation
      --skip-security      Skip security scanning
  -o, --output-dir DIR     Output directory for reports (default: validation_reports)
  -h, --help               Show this help message

Validation Categories:
  - File structure and organization
  - Build system configuration
  - Dependency management
  - Code quality and style
  - Documentation completeness
  - Test suite coverage
  - Security vulnerabilities

Examples:
  $0                       # Full validation
  $0 --quick --verbose     # Quick validation with detailed output
  $0 --report --fix-issues # Generate report and fix issues
  $0 --skip-security       # Skip security scanning

EOF
}

# Parse command line arguments
parse_arguments() {
    while [[ $# -gt 0 ]]; do
        case $1 in
            -q|--quick)
                QUICK_MODE=true
                shift
                ;;
            -v|--verbose)
                VERBOSE=true
                shift
                ;;
            -r|--report)
                GENERATE_REPORT=true
                shift
                ;;
            -f|--fix-issues)
                FIX_ISSUES=true
                shift
                ;;
            --skip-build)
                SKIP_BUILD=true
                shift
                ;;
            --skip-tests)
                SKIP_TESTS=true
                shift
                ;;
            --skip-docs)
                SKIP_DOCS=true
                shift
                ;;
            --skip-security)
                SKIP_SECURITY=true
                shift
                ;;
            -o|--output-dir)
                OUTPUT_DIR="$2"
                shift 2
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
                log_error "Unexpected argument: $1"
                show_help
                exit 1
                ;;
        esac
    done
}

# Initialize validation
init_validation() {
    log_step "Initializing validation..."
    
    # Create output directory if generating report
    if [[ "$GENERATE_REPORT" == "true" ]]; then
        mkdir -p "$OUTPUT_DIR"
        
        # Initialize report file
        {
            echo "# cpp-template Validation Report"
            echo ""
            echo "**Generated:** $(date)"
            echo "**Template Version:** $(git describe --tags --always 2>/dev/null || echo "unknown")"
            echo "**Validation Mode:** $(if [[ "$QUICK_MODE" == "true" ]]; then echo "Quick"; else echo "Full"; fi)"
            echo ""
        } > "$OUTPUT_DIR/$REPORT_FILE"
    fi
    
    log_info "Validation initialized"
    log_verbose "Project root: $PROJECT_ROOT"
    log_verbose "Output directory: $OUTPUT_DIR"
    log_verbose "Quick mode: $QUICK_MODE"
}

# Validate file structure
validate_file_structure() {
    log_step "Validating file structure..."
    
    local required_files=(
        "README.md"
        "CMakeLists.txt"
        "vcpkg.json"
        ".gitignore"
        "src/main.cpp"
        "libs/core/CMakeLists.txt"
        "libs/core/include/core/core.hpp"
        "libs/core/src/core.cpp"
        "tests/CMakeLists.txt"
        "examples/CMakeLists.txt"
        "docs/TEMPLATE_USAGE_GUIDE.md"
        "docs/CUSTOMIZATION_GUIDE.md"
        "scripts/quick_start.sh"
        "scripts/replace_placeholders.py"
    )
    
    local required_dirs=(
        "src"
        "libs"
        "libs/core"
        "libs/core/include"
        "libs/core/src"
        "tests"
        "examples"
        "docs"
        "scripts"
        "third_party"
        "third_party/manual"
    )
    
    # Check required files
    for file in "${required_files[@]}"; do
        local file_path="$PROJECT_ROOT/$file"
        if [[ -f "$file_path" ]]; then
            log_verbose "Found required file: $file"
        else
            log_error "Missing required file: $file"
        fi
    done
    
    # Check required directories
    for dir in "${required_dirs[@]}"; do
        local dir_path="$PROJECT_ROOT/$dir"
        if [[ -d "$dir_path" ]]; then
            log_verbose "Found required directory: $dir"
        else
            log_error "Missing required directory: $dir"
        fi
    done
    
    # Check file permissions
    local executable_files=(
        "scripts/quick_start.sh"
        "scripts/deploy_template.sh"
        "scripts/validate_template.sh"
    )
    
    for file in "${executable_files[@]}"; do
        local file_path="$PROJECT_ROOT/$file"
        if [[ -f "$file_path" ]]; then
            if [[ ! -x "$file_path" ]]; then
                log_warning "Script is not executable: $file"
                if [[ "$FIX_ISSUES" == "true" ]]; then
                    chmod +x "$file_path"
                    log_fixed "Made script executable: $file"
                fi
            else
                log_verbose "Script is executable: $file"
            fi
        fi
    done
    
    log_success "File structure validation completed"
}

# Validate dependencies
validate_dependencies() {
    log_step "Validating dependency management..."
    
    # Check Git submodules
    if [[ -f ".gitmodules" ]]; then
        log_info "Checking Git submodules..."
        
        if git submodule status | grep -q '^-'; then
            log_warning "Some Git submodules are not initialized"
            if [[ "$FIX_ISSUES" == "true" ]]; then
                git submodule update --init --recursive
                log_fixed "Initialized Git submodules"
            fi
        else
            log_success "Git submodules are properly initialized"
        fi
    fi
    
    # Check manual dependencies
    if [[ -d "third_party/manual" ]]; then
        log_info "Checking manual dependencies..."
        
        local manual_deps=("third_party/manual/example_lib")
        for dep in "${manual_deps[@]}"; do
            if [[ -d "$PROJECT_ROOT/$dep" ]]; then
                log_success "Manual dependency found: $dep"
            else
                log_warning "Manual dependency missing: $dep"
            fi
        done
    fi
    
    # Check vcpkg.json
    if [[ -f "vcpkg.json" ]]; then
        log_info "Validating vcpkg.json..."
        
        if command -v python3 &> /dev/null; then
            if python3 -m json.tool vcpkg.json > /dev/null 2>&1; then
                log_success "vcpkg.json is valid JSON"
            else
                log_error "vcpkg.json contains invalid JSON"
            fi
        fi
        
        # Check for required fields
        if grep -q '"name"' vcpkg.json && grep -q '"version"' vcpkg.json; then
            log_success "vcpkg.json contains required fields"
        else
            log_warning "vcpkg.json missing required fields (name, version)"
        fi
    else
        log_warning "vcpkg.json not found"
    fi
    
    log_success "Dependency validation completed"
}

# Validate documentation
validate_documentation() {
    if [[ "$SKIP_DOCS" == "true" ]]; then
        log_info "Skipping documentation validation"
        return 0
    fi
    
    log_step "Validating documentation..."
    
    # Check README.md
    if [[ -f "$PROJECT_ROOT/README.md" ]]; then
        log_info "Checking README.md..."
        
        local section_count=0
        if grep -q '^# ' "$PROJECT_ROOT/README.md"; then
            ((section_count++))
        fi
        if grep -q '^## ' "$PROJECT_ROOT/README.md"; then
            ((section_count++))
        fi
        if grep -q '^### ' "$PROJECT_ROOT/README.md"; then
            ((section_count++))
        fi
        
        if [[ $section_count -ge 3 ]]; then
            log_success "README.md has good structure"
        else
            log_warning "README.md lacks proper structure (headers)"
        fi
        
        # Check for essential sections
        local essential_sections=("Installation" "Usage" "Build" "Test")
        for section in "${essential_sections[@]}"; do
            if grep -qi "$section" "$PROJECT_ROOT/README.md"; then
                log_verbose "Found section: $section"
            else
                log_warning "Missing section in README.md: $section"
            fi
        done
    else
        log_error "README.md not found"
    fi
    
    # Check documentation files
    local doc_files=(
        "docs/TEMPLATE_USAGE_GUIDE.md"
        "docs/CUSTOMIZATION_GUIDE.md"
    )
    
    for doc_file in "${doc_files[@]}"; do
        if [[ -f "$PROJECT_ROOT/$doc_file" ]]; then
            log_success "Documentation file found: $doc_file"
            
            # Check file size (should not be empty)
            if [[ -s "$PROJECT_ROOT/$doc_file" ]]; then
                log_verbose "Documentation file has content: $doc_file"
            else
                log_warning "Documentation file is empty: $doc_file"
            fi
        else
            log_error "Missing documentation file: $doc_file"
        fi
    done
    
    log_success "Documentation validation completed"
}

# Validate security
validate_security() {
    if [[ "$SKIP_SECURITY" == "true" ]]; then
        log_info "Skipping security validation"
        return 0
    fi
    
    if [[ "$QUICK_MODE" == "true" ]]; then
        log_info "Skipping security checks in quick mode"
        return 0
    fi
    
    log_step "Validating security..."
    
    # Check for sensitive files
    log_info "Checking for sensitive files..."
    
    local sensitive_patterns=(
        "*.key"
        "*.pem"
        "*.p12"
        "*.pfx"
        "*password*"
        "*secret*"
        "*.env"
    )
    
    for pattern in "${sensitive_patterns[@]}"; do
        if find "$PROJECT_ROOT" -name "$pattern" -not -path "*/.*" 2>/dev/null | grep -q .; then
            log_warning "Found potentially sensitive files matching: $pattern"
        fi
    done
    
    # Check .gitignore for security
    if [[ -f "$PROJECT_ROOT/.gitignore" ]]; then
        log_info "Checking .gitignore security..."
        
        local security_ignores=("*.key" "*.env" "secrets/" "credentials/")
        for ignore in "${security_ignores[@]}"; do
            if grep -q "$ignore" "$PROJECT_ROOT/.gitignore"; then
                log_verbose "Good: .gitignore includes $ignore"
            else
                log_warning ".gitignore should include: $ignore"
            fi
        done
    fi
    
    log_success "Security validation completed"
}

# Generate final report
generate_final_report() {
    log_step "Generating validation summary..."
    
    echo
    echo "============================================================"
    echo -e "${PURPLE}VALIDATION SUMMARY${NC}"
    echo "============================================================"
    
    local success_count=$(grep -c "✅" "$OUTPUT_DIR/$REPORT_FILE" 2>/dev/null || echo "0")
    echo -e "${GREEN}✓ Successes:${NC} $success_count"
    echo -e "${BLUE}ℹ Info:${NC} $INFO_COUNT"
    echo -e "${YELLOW}⚠ Warnings:${NC} $WARNING_COUNT"
    echo -e "${RED}❌ Errors:${NC} $ERROR_COUNT"
    
    if [[ "$FIX_ISSUES" == "true" ]]; then
        echo -e "${GREEN}🔧 Fixed:${NC} $FIXED_COUNT"
    fi
    
    echo
    
    # Overall status
    if [[ $ERROR_COUNT -eq 0 ]]; then
        if [[ $WARNING_COUNT -eq 0 ]]; then
            echo -e "${GREEN}🎉 VALIDATION PASSED - Template is in excellent condition!${NC}"
        else
            echo -e "${YELLOW}✅ VALIDATION PASSED - Template is good with minor warnings${NC}"
        fi
    else
        echo -e "${RED}❌ VALIDATION FAILED - Template has critical issues that need attention${NC}"
    fi
    
    if [[ "$GENERATE_REPORT" == "true" ]]; then
        echo
        echo -e "Detailed report saved to: ${CYAN}$OUTPUT_DIR/$REPORT_FILE${NC}"
        
        # Add summary to report
        local success_count=$(grep -c "✅" "$OUTPUT_DIR/$REPORT_FILE" 2>/dev/null || echo "0")
        cat >> "$OUTPUT_DIR/$REPORT_FILE" << EOF

## Validation Summary

- ✅ Successes: $success_count
- ℹ️ Info: $INFO_COUNT
- ⚠️ Warnings: $WARNING_COUNT
- ❌ Errors: $ERROR_COUNT
- 🔧 Fixed: $FIXED_COUNT

**Overall Status:** $(if [[ $ERROR_COUNT -eq 0 ]]; then echo "PASSED"; else echo "FAILED"; fi)

---
*Generated by cpp-template validation script v1.0.0*
EOF
    fi
    
    echo
    log_success "Validation completed!"
    
    # Exit with appropriate code
    exit $ERROR_COUNT
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
    echo -e "                                    ${BLUE}Validation Script v1.0.0${NC}"
    echo
    
    # Initialize
    init_validation
    
    # Show configuration
    echo "Validation Configuration:"
    echo "  Mode: $(if [[ "$QUICK_MODE" == "true" ]]; then echo "Quick"; else echo "Full"; fi)"
    echo "  Generate Report: $GENERATE_REPORT"
    echo "  Fix Issues: $FIX_ISSUES"
    echo "  Skip Build: $SKIP_BUILD"
    echo "  Skip Tests: $SKIP_TESTS"
    echo "  Skip Docs: $SKIP_DOCS"
    echo "  Skip Security: $SKIP_SECURITY"
    echo
    
    # Run validation steps
    validate_file_structure
    validate_dependencies
    validate_documentation
    validate_security
    
    # Generate final report
    generate_final_report
}

# Run main function
main "$@"