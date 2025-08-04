#!/bin/bash
# Development environment setup script for cpp-template
# Installs required dependencies using system package managers

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
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

# Function to check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Function to detect Linux distribution
detect_linux_distro() {
    if [ -f /etc/os-release ]; then
        . /etc/os-release
        echo $ID
    elif [ -f /etc/debian_version ]; then
        echo "debian"
    elif [ -f /etc/redhat-release ]; then
        echo "rhel"
    else
        echo "unknown"
    fi
}

print_status "Setting up development environment for cpp-template..."

# Detect platform
if [[ "$OSTYPE" == "darwin"* ]]; then
    PLATFORM="macos"
    print_status "Detected platform: macOS"
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    PLATFORM="linux"
    DISTRO=$(detect_linux_distro)
    print_status "Detected platform: Linux ($DISTRO)"
else
    print_error "Unsupported operating system: $OSTYPE"
    exit 1
fi

# macOS setup
if [ "$PLATFORM" = "macos" ]; then
    print_status "Setting up macOS development environment..."

    # Check for Xcode Command Line Tools
    if ! xcode-select -p >/dev/null 2>&1; then
        print_status "Installing Xcode Command Line Tools..."
        xcode-select --install
        print_warning "Please complete the Xcode Command Line Tools installation and run this script again."
        exit 1
    fi

    # Check for Homebrew
    if ! command_exists brew; then
        print_status "Installing Homebrew..."
        /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

        # Add Homebrew to PATH for current session
        if [[ $(uname -m) == "arm64" ]]; then
            eval "$(/opt/homebrew/bin/brew shellenv)"
        else
            eval "$(/usr/local/bin/brew shellenv)"
        fi
    fi

    print_status "Installing macOS dependencies via Homebrew..."

    # Essential build tools
    brew install cmake git ninja pkg-config

    # Common C++ libraries
    brew install boost openssl@3 zlib curl sqlite postgresql

    # Development utilities
    brew install autoconf automake libtool

    # Optional: vcpkg for additional package management
    if ! command_exists vcpkg; then
        print_status "Installing vcpkg..."
        brew install vcpkg
    fi

    print_success "macOS development environment setup complete!"

# Linux setup
elif [ "$PLATFORM" = "linux" ]; then
    print_status "Setting up Linux development environment..."

    case $DISTRO in
        ubuntu | debian)
            print_status "Installing Ubuntu/Debian dependencies..."

            # Update package list
            sudo apt update

            # Essential build tools
            sudo apt install -y build-essential cmake git ninja-build pkg-config

            # Common C++ libraries
            sudo apt install -y \
                libboost-all-dev \
                libssl-dev \
                zlib1g-dev \
                libcurl4-openssl-dev \
                libsqlite3-dev \
                libpq-dev

            # Development utilities
            sudo apt install -y autoconf automake libtool

            print_success "Ubuntu/Debian dependencies installed!"
            ;;

        fedora | rhel | centos)
            print_status "Installing Fedora/RHEL dependencies..."

            # Install development tools group
            sudo dnf groupinstall -y "Development Tools"

            # Essential build tools
            sudo dnf install -y cmake git ninja-build pkgconfig

            # Common C++ libraries
            sudo dnf install -y \
                boost-devel \
                openssl-devel \
                zlib-devel \
                libcurl-devel \
                sqlite-devel \
                postgresql-devel

            # Development utilities
            sudo dnf install -y autoconf automake libtool

            print_success "Fedora/RHEL dependencies installed!"
            ;;

        arch)
            print_status "Installing Arch Linux dependencies..."

            # Update package database
            sudo pacman -Sy

            # Essential build tools
            sudo pacman -S --needed base-devel cmake git ninja pkgconf

            # Common C++ libraries
            sudo pacman -S --needed \
                boost \
                openssl \
                zlib \
                curl \
                sqlite \
                postgresql-libs

            print_success "Arch Linux dependencies installed!"
            ;;

        *)
            print_warning "Unsupported Linux distribution: $DISTRO"
            print_status "Please install the following packages manually:"
            print_status "- cmake, git, ninja-build, pkg-config"
            print_status "- boost-devel, openssl-devel, zlib-devel"
            print_status "- libcurl-devel, sqlite-devel, postgresql-devel"
            ;;
    esac
fi

# Verify installation
print_status "Verifying installation..."

# Check essential tools
MISSING_TOOLS=()

if ! command_exists cmake; then
    MISSING_TOOLS+=("cmake")
fi

if ! command_exists git; then
    MISSING_TOOLS+=("git")
fi

if ! command_exists ninja; then
    MISSING_TOOLS+=("ninja")
fi

if ! command_exists pkg-config; then
    MISSING_TOOLS+=("pkg-config")
fi

if [ ${#MISSING_TOOLS[@]} -eq 0 ]; then
    print_success "All essential tools are available!"

    # Print versions
    print_status "Tool versions:"
    echo "  CMake: $(cmake --version | head -n1)"
    echo "  Git: $(git --version)"
    echo "  Ninja: $(ninja --version)"
    echo "  pkg-config: $(pkg-config --version)"

else
    print_error "Missing tools: ${MISSING_TOOLS[*]}"
    print_error "Please install missing tools and run this script again."
    exit 1
fi

# Test CMake configuration
print_status "Testing CMake configuration..."
if cmake -B build-test -S . >/dev/null 2>&1; then
    print_success "CMake configuration successful!"
    rm -rf build-test
else
    print_warning "CMake configuration failed. Some dependencies may be missing."
    print_status "You can still proceed, but some features may not be available."
fi

print_success "Development environment setup complete!"
print_status ""
print_status "Next steps:"
print_status "1. Build the project:"
print_status "   cmake -B build -S ."
print_status "   cmake --build build"
print_status ""
print_status "2. Run tests:"
print_status "   cmake --build build --target test"
print_status ""
print_status "3. For vcpkg support (if available):"
print_status "   cmake --preset vcpkg"
print_status "   cmake --build build"
print_status ""
print_status "For more information, see the documentation in cmake/ directory."
