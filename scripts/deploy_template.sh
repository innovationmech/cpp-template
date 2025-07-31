#!/bin/bash

# ==============================================================================
# cpp-template Deployment Script
# ==============================================================================
#
# This script helps deploy the cpp-template to various platforms and repositories.
# It supports GitHub releases, template repositories, package registries, and
# documentation sites.
#
# Usage:
#   ./scripts/deploy_template.sh [OPTIONS]
#
# Examples:
#   ./scripts/deploy_template.sh --github-release v1.0.0
#   ./scripts/deploy_template.sh --update-docs
#   ./scripts/deploy_template.sh --package-registry
#   ./scripts/deploy_template.sh --all
#
# Features:
# - GitHub release creation with assets
# - Template repository updates
# - Package registry publishing
# - Documentation deployment
# - Version management
# - Automated testing before deployment
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
VERSION=""
GITHUB_RELEASE=false
UPDATE_DOCS=false
PACKAGE_REGISTRY=false
TEMPLATE_REPO=false
DRY_RUN=false
VERBOSE=false
SKIP_TESTS=false
SKIP_BUILD=false
FORCE=false

# GitHub configuration
GITHUB_OWNER="your-username"
GITHUB_REPO="cpp-template"
GITHUB_TOKEN="${GITHUB_TOKEN:-}"

# Documentation configuration
DOCS_BRANCH="gh-pages"
DOCS_DIR="docs"

# Package registry configuration
REGISTRY_URL="https://registry.example.com"
REGISTRY_TOKEN="${REGISTRY_TOKEN:-}"

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
cpp-template Deployment Script

Usage: $0 [OPTIONS]

Options:
  -r, --github-release VERSION    Create GitHub release with specified version
  -d, --update-docs               Update and deploy documentation
  -p, --package-registry          Publish to package registry
  -t, --template-repo             Update template repository
  -a, --all                       Deploy to all platforms
      --dry-run                   Show what would be done without making changes
      --skip-tests                Skip running tests before deployment
      --skip-build                Skip building project before deployment
      --force                     Force deployment even if checks fail
  -v, --verbose                   Enable verbose output
  -h, --help                      Show this help message

Environment Variables:
  GITHUB_TOKEN                    GitHub personal access token
  REGISTRY_TOKEN                  Package registry authentication token
  GITHUB_OWNER                    GitHub repository owner (default: your-username)
  GITHUB_REPO                     GitHub repository name (default: cpp-template)

Examples:
  $0 --github-release v1.0.0
  $0 --update-docs --verbose
  $0 --all --dry-run
  $0 --package-registry --force

EOF
}

# Parse command line arguments
parse_arguments() {
    while [[ $# -gt 0 ]]; do
        case $1 in
            -r|--github-release)
                GITHUB_RELEASE=true
                VERSION="$2"
                shift 2
                ;;
            -d|--update-docs)
                UPDATE_DOCS=true
                shift
                ;;
            -p|--package-registry)
                PACKAGE_REGISTRY=true
                shift
                ;;
            -t|--template-repo)
                TEMPLATE_REPO=true
                shift
                ;;
            -a|--all)
                GITHUB_RELEASE=true
                UPDATE_DOCS=true
                PACKAGE_REGISTRY=true
                TEMPLATE_REPO=true
                shift
                ;;
            --dry-run)
                DRY_RUN=true
                shift
                ;;
            --skip-tests)
                SKIP_TESTS=true
                shift
                ;;
            --skip-build)
                SKIP_BUILD=true
                shift
                ;;
            --force)
                FORCE=true
                shift
                ;;
            -v|--verbose)
                VERBOSE=true
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
                log_error "Unexpected argument: $1"
                show_help
                exit 1
                ;;
        esac
    done
}

# Validate configuration
validate_config() {
    log_step "Validating configuration..."
    
    # Check if any deployment option is selected
    if [[ "$GITHUB_RELEASE" != "true" ]] && [[ "$UPDATE_DOCS" != "true" ]] && \
       [[ "$PACKAGE_REGISTRY" != "true" ]] && [[ "$TEMPLATE_REPO" != "true" ]]; then
        log_error "No deployment option selected. Use --help for available options."
        exit 1
    fi
    
    # Validate version for GitHub release
    if [[ "$GITHUB_RELEASE" == "true" ]] && [[ -z "$VERSION" ]]; then
        log_error "Version is required for GitHub release. Use --github-release VERSION"
        exit 1
    fi
    
    # Validate version format
    if [[ -n "$VERSION" ]] && [[ ! "$VERSION" =~ ^v?[0-9]+\.[0-9]+\.[0-9]+(-[a-zA-Z0-9.-]+)?$ ]]; then
        log_error "Invalid version format: $VERSION"
        log_error "Expected format: v1.0.0 or 1.0.0 (with optional pre-release suffix)"
        exit 1
    fi
    
    # Check GitHub token for GitHub operations
    if [[ "$GITHUB_RELEASE" == "true" ]] || [[ "$TEMPLATE_REPO" == "true" ]]; then
        if [[ -z "$GITHUB_TOKEN" ]] && [[ "$DRY_RUN" != "true" ]]; then
            log_error "GITHUB_TOKEN environment variable is required for GitHub operations"
            log_error "Create a personal access token at: https://github.com/settings/tokens"
            exit 1
        fi
    fi
    
    # Check registry token for package registry
    if [[ "$PACKAGE_REGISTRY" == "true" ]]; then
        if [[ -z "$REGISTRY_TOKEN" ]] && [[ "$DRY_RUN" != "true" ]]; then
            log_warning "REGISTRY_TOKEN not set. Package registry deployment may fail."
        fi
    fi
    
    # Override from environment variables
    GITHUB_OWNER="${GITHUB_OWNER:-your-username}"
    GITHUB_REPO="${GITHUB_REPO:-cpp-template}"
    
    log_success "Configuration validated"
}

# Check prerequisites
check_prerequisites() {
    log_step "Checking prerequisites..."
    
    local missing_tools=()
    
    # Check for required tools
    if ! command -v git &> /dev/null; then
        missing_tools+=("git")
    fi
    
    if ! command -v curl &> /dev/null; then
        missing_tools+=("curl")
    fi
    
    # Check for GitHub CLI if doing GitHub operations
    if [[ "$GITHUB_RELEASE" == "true" ]] || [[ "$TEMPLATE_REPO" == "true" ]]; then
        if ! command -v gh &> /dev/null; then
            missing_tools+=("gh (GitHub CLI)")
        fi
    fi
    
    # Check for documentation tools if updating docs
    if [[ "$UPDATE_DOCS" == "true" ]]; then
        if ! command -v python3 &> /dev/null && ! command -v python &> /dev/null; then
            missing_tools+=("python (for documentation generation)")
        fi
    fi
    
    if [[ ${#missing_tools[@]} -gt 0 ]]; then
        log_error "Missing required tools:"
        for tool in "${missing_tools[@]}"; do
            log_error "  - $tool"
        done
        
        log_info "Installation suggestions:"
        log_info "  GitHub CLI: https://cli.github.com/"
        log_info "  macOS: brew install gh"
        log_info "  Ubuntu: sudo apt install gh"
        
        if [[ "$FORCE" != "true" ]]; then
            exit 1
        else
            log_warning "Continuing with missing tools due to --force flag"
        fi
    fi
    
    log_success "Prerequisites check completed"
}

# Run tests
run_tests() {
    if [[ "$SKIP_TESTS" == "true" ]]; then
        log_info "Skipping tests as requested"
        return 0
    fi
    
    log_step "Running tests..."
    
    if [[ "$DRY_RUN" == "true" ]]; then
        log_info "DRY RUN: Would run test suite"
        return 0
    fi
    
    cd "$PROJECT_ROOT"
    
    # Check if build directory exists
    if [[ ! -d "build" ]] || [[ "$SKIP_BUILD" != "true" ]]; then
        log_info "Building project for testing..."
        
        # Clean and rebuild
        rm -rf build
        mkdir build
        cd build
        
        # Configure with vcpkg if available
        local cmake_args=()
        if command -v vcpkg &> /dev/null; then
            local vcpkg_root
            vcpkg_root=$(vcpkg integrate install 2>/dev/null | grep -o '/[^"]*vcpkg' | head -1) || true
            if [[ -n "$vcpkg_root" ]]; then
                cmake_args+=("-DCMAKE_TOOLCHAIN_FILE=$vcpkg_root/scripts/buildsystems/vcpkg.cmake")
            fi
        fi
        
        cmake .. "${cmake_args[@]}"
        cmake --build . --parallel $(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
        
        cd ..
    fi
    
    # Run tests
    cd build
    if ! ctest --output-on-failure; then
        log_error "Tests failed"
        if [[ "$FORCE" != "true" ]]; then
            exit 1
        else
            log_warning "Continuing despite test failures due to --force flag"
        fi
    fi
    
    cd ..
    log_success "All tests passed"
}

# Create GitHub release
create_github_release() {
    if [[ "$GITHUB_RELEASE" != "true" ]]; then
        return 0
    fi
    
    log_step "Creating GitHub release: $VERSION"
    
    if [[ "$DRY_RUN" == "true" ]]; then
        log_info "DRY RUN: Would create GitHub release $VERSION"
        log_info "DRY RUN: Would upload release assets"
        return 0
    fi
    
    cd "$PROJECT_ROOT"
    
    # Check if release already exists
    if gh release view "$VERSION" &> /dev/null; then
        if [[ "$FORCE" == "true" ]]; then
            log_warning "Release $VERSION already exists. Deleting due to --force flag."
            gh release delete "$VERSION" --yes
        else
            log_error "Release $VERSION already exists. Use --force to overwrite."
            exit 1
        fi
    fi
    
    # Create release archive
    local archive_name="cpp-template-$VERSION.tar.gz"
    log_info "Creating release archive: $archive_name"
    
    # Create a clean copy for the archive
    local temp_dir="/tmp/cpp-template-release-$TIMESTAMP"
    mkdir -p "$temp_dir"
    
    # Copy project files, excluding build artifacts and git
    rsync -av --exclude='.git' --exclude='build' --exclude='vcpkg_installed' \
          --exclude='*.log' --exclude='.DS_Store' \
          "$PROJECT_ROOT/" "$temp_dir/cpp-template-$VERSION/"
    
    # Create archive
    cd "$temp_dir"
    tar -czf "$archive_name" "cpp-template-$VERSION"
    mv "$archive_name" "$PROJECT_ROOT/"
    
    # Clean up temp directory
    rm -rf "$temp_dir"
    
    cd "$PROJECT_ROOT"
    
    # Generate release notes
    local release_notes_file="release-notes-$VERSION.md"
    generate_release_notes "$VERSION" > "$release_notes_file"
    
    # Create GitHub release
    log_info "Creating GitHub release..."
    gh release create "$VERSION" \
        --title "cpp-template $VERSION" \
        --notes-file "$release_notes_file" \
        "$archive_name"
    
    # Clean up
    rm -f "$release_notes_file" "$archive_name"
    
    log_success "GitHub release $VERSION created successfully"
}

# Generate release notes
generate_release_notes() {
    local version="$1"
    
    cat << EOF
# cpp-template $version

A modern, feature-rich C++ project template with comprehensive tooling and best practices.

## Features

- **Multiple Dependency Management**: Support for vcpkg, Git submodules, manual dependencies, and system packages
- **Modern CMake**: Clean, modular CMake configuration with best practices
- **Comprehensive Testing**: Unit tests, integration tests, and performance benchmarks using Google Test
- **Modular Architecture**: Well-structured codebase with clear separation of concerns
- **Documentation**: Complete documentation with examples and customization guides
- **CI/CD Ready**: GitHub Actions workflows for continuous integration
- **Cross-Platform**: Support for Windows, macOS, and Linux

## Quick Start

1. Download and extract the template
2. Run the quick start script: \`./scripts/quick_start.sh your-project-name\`
3. Follow the customization guide in \`docs/CUSTOMIZATION_GUIDE.md\`

## What's Included

- Core library with modular architecture
- Example modules and applications
- Comprehensive test suite
- Documentation and guides
- Build scripts and automation tools
- Multiple dependency management examples

## Requirements

- C++17 compatible compiler
- CMake 3.15+
- Git
- Python 3.6+ (for scripts)

## Documentation

- [Template Usage Guide](docs/TEMPLATE_USAGE_GUIDE.md)
- [Customization Guide](docs/CUSTOMIZATION_GUIDE.md)
- [API Reference](docs/api/)
- [Examples](examples/)

## Support

For questions, issues, or contributions, please visit the [GitHub repository](https://github.com/$GITHUB_OWNER/$GITHUB_REPO).
EOF
}

# Update documentation
update_documentation() {
    if [[ "$UPDATE_DOCS" != "true" ]]; then
        return 0
    fi
    
    log_step "Updating documentation..."
    
    if [[ "$DRY_RUN" == "true" ]]; then
        log_info "DRY RUN: Would update and deploy documentation"
        return 0
    fi
    
    cd "$PROJECT_ROOT"
    
    # Check if docs branch exists
    if ! git show-ref --verify --quiet "refs/heads/$DOCS_BRANCH"; then
        log_info "Creating documentation branch: $DOCS_BRANCH"
        git checkout --orphan "$DOCS_BRANCH"
        git rm -rf .
        echo "# Documentation" > README.md
        git add README.md
        git commit -m "Initialize documentation branch"
        git checkout main
    fi
    
    # Generate documentation
    log_info "Generating documentation..."
    
    # Create temporary docs directory
    local temp_docs="/tmp/cpp-template-docs-$TIMESTAMP"
    mkdir -p "$temp_docs"
    
    # Copy documentation files
    cp -r "$DOCS_DIR"/* "$temp_docs/" 2>/dev/null || true
    cp README.md "$temp_docs/index.md"
    
    # Generate API documentation if Doxygen is available
    if command -v doxygen &> /dev/null; then
        log_info "Generating API documentation with Doxygen..."
        # Add Doxygen configuration and generation here
    fi
    
    # Switch to docs branch and update
    git checkout "$DOCS_BRANCH"
    
    # Clear existing docs
    find . -maxdepth 1 -not -name '.git' -not -name '.' -exec rm -rf {} +
    
    # Copy new docs
    cp -r "$temp_docs"/* .
    
    # Commit changes
    git add .
    if git diff --staged --quiet; then
        log_info "No documentation changes to commit"
    else
        git commit -m "Update documentation - $(date)"
        git push origin "$DOCS_BRANCH"
        log_success "Documentation updated and pushed"
    fi
    
    # Switch back to main branch
    git checkout main
    
    # Clean up
    rm -rf "$temp_docs"
    
    log_success "Documentation deployment completed"
}

# Publish to package registry
publish_package() {
    if [[ "$PACKAGE_REGISTRY" != "true" ]]; then
        return 0
    fi
    
    log_step "Publishing to package registry..."
    
    if [[ "$DRY_RUN" == "true" ]]; then
        log_info "DRY RUN: Would publish package to registry"
        return 0
    fi
    
    # This is a placeholder for package registry publishing
    # Implementation depends on the specific registry being used
    
    log_info "Package registry publishing is not yet implemented"
    log_info "This would typically involve:"
    log_info "  1. Creating a package manifest"
    log_info "  2. Building distributable artifacts"
    log_info "  3. Uploading to the registry"
    log_info "  4. Updating package metadata"
    
    log_warning "Package registry publishing skipped"
}

# Update template repository
update_template_repo() {
    if [[ "$TEMPLATE_REPO" != "true" ]]; then
        return 0
    fi
    
    log_step "Updating template repository..."
    
    if [[ "$DRY_RUN" == "true" ]]; then
        log_info "DRY RUN: Would update template repository"
        return 0
    fi
    
    cd "$PROJECT_ROOT"
    
    # Check if we're in a clean state
    if ! git diff --quiet || ! git diff --staged --quiet; then
        if [[ "$FORCE" != "true" ]]; then
            log_error "Working directory is not clean. Commit or stash changes first."
            exit 1
        else
            log_warning "Working directory is not clean, but continuing due to --force flag"
        fi
    fi
    
    # Push to main branch
    log_info "Pushing changes to main branch..."
    git push origin main
    
    # Create and push tag if version is specified
    if [[ -n "$VERSION" ]]; then
        log_info "Creating and pushing tag: $VERSION"
        
        # Check if tag already exists
        if git tag -l | grep -q "^$VERSION$"; then
            if [[ "$FORCE" == "true" ]]; then
                log_warning "Tag $VERSION already exists. Deleting due to --force flag."
                git tag -d "$VERSION"
                git push origin ":refs/tags/$VERSION" || true
            else
                log_error "Tag $VERSION already exists. Use --force to overwrite."
                exit 1
            fi
        fi
        
        git tag -a "$VERSION" -m "Release $VERSION"
        git push origin "$VERSION"
    fi
    
    log_success "Template repository updated"
}

# Generate deployment summary
generate_summary() {
    log_step "Generating deployment summary..."
    
    echo
    echo "=" * 60
    echo -e "${GREEN}DEPLOYMENT COMPLETED!${NC}"
    echo "=" * 60
    
    if [[ "$GITHUB_RELEASE" == "true" ]]; then
        echo -e "${GREEN}✓${NC} GitHub Release: $VERSION"
        echo -e "  URL: https://github.com/$GITHUB_OWNER/$GITHUB_REPO/releases/tag/$VERSION"
    fi
    
    if [[ "$UPDATE_DOCS" == "true" ]]; then
        echo -e "${GREEN}✓${NC} Documentation Updated"
        echo -e "  URL: https://$GITHUB_OWNER.github.io/$GITHUB_REPO/"
    fi
    
    if [[ "$PACKAGE_REGISTRY" == "true" ]]; then
        echo -e "${YELLOW}⚠${NC} Package Registry: Not implemented"
    fi
    
    if [[ "$TEMPLATE_REPO" == "true" ]]; then
        echo -e "${GREEN}✓${NC} Template Repository Updated"
        echo -e "  URL: https://github.com/$GITHUB_OWNER/$GITHUB_REPO"
    fi
    
    echo
    echo "Next steps:"
    echo "1. Verify the deployments are working correctly"
    echo "2. Update any external references to the new version"
    echo "3. Announce the release to users"
    echo "4. Monitor for any issues or feedback"
    
    echo
    log_success "Deployment completed successfully!"
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
    echo -e "                                    ${BLUE}Deployment Script v1.0.0${NC}"
    echo
    
    # Validate configuration
    validate_config
    
    # Check prerequisites
    check_prerequisites
    
    # Show configuration
    if [[ "$DRY_RUN" == "true" ]]; then
        echo -e "${YELLOW}DRY RUN MODE - No changes will be made${NC}"
    fi
    
    echo "Deployment Configuration:"
    [[ "$GITHUB_RELEASE" == "true" ]] && echo "  GitHub Release: $VERSION"
    [[ "$UPDATE_DOCS" == "true" ]] && echo "  Update Documentation: Yes"
    [[ "$PACKAGE_REGISTRY" == "true" ]] && echo "  Package Registry: Yes"
    [[ "$TEMPLATE_REPO" == "true" ]] && echo "  Template Repository: Yes"
    echo "  Skip Tests: $SKIP_TESTS"
    echo "  Skip Build: $SKIP_BUILD"
    echo "  Force: $FORCE"
    echo
    
    if [[ "$DRY_RUN" != "true" ]]; then
        read -p "Continue with deployment? (Y/n): " -n 1 -r
        echo
        if [[ $REPLY =~ ^[Nn]$ ]]; then
            log_info "Deployment cancelled by user"
            exit 0
        fi
    fi
    
    # Execute deployment steps
    run_tests
    create_github_release
    update_documentation
    publish_package
    update_template_repo
    generate_summary
}

# Error handling
trap 'log_error "Deployment failed at line $LINENO"' ERR

# Run main function
main "$@"