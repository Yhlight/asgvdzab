#!/bin/bash

# CHTL Compiler Build Script - Linux Platform
# Builds the compiler and packages official modules

set -e

# Color definitions
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Print functions
info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

error() {
    echo -e "${RED}[ERROR]${NC} $1" >&2
    exit 1
}

warning() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

# Check environment
check_environment() {
    info "Checking build environment..."
    
    # Check Java
    if ! command -v java &> /dev/null; then
        error "Java not found. Please install JDK 17 or higher"
    fi
    
    JAVA_VERSION=$(java -version 2>&1 | awk -F '"' '/version/ {print $2}' | cut -d'.' -f1)
    if [ "$JAVA_VERSION" -lt 17 ]; then
        error "Java 17 or higher required, current version: $JAVA_VERSION"
    fi
    info "Java version: $(java -version 2>&1 | head -n 1)"
    
    # Check Maven
    if ! command -v mvn &> /dev/null; then
        error "Maven not found. Please install Maven 3.6.0 or higher"
    fi
    info "Maven version: $(mvn -version | head -n 1)"
}

# Main build function
build() {
    info "Starting CHTL compiler build..."
    
    # Navigate to project root
    cd "$(dirname "$0")/../.."
    PROJECT_ROOT=$(pwd)
    
    # Clean old build
    if [ -d "target" ]; then
        info "Cleaning old build..."
        rm -rf target
    fi
    
    # Build with Maven
    info "Running Maven build..."
    if [ "$SKIP_TESTS" = true ]; then
        mvn clean package -DskipTests
    else
        mvn clean package
    fi
    
    # Check build result
    if [ ! -f "target/chtl-compiler-1.0.jar" ]; then
        error "Build failed: JAR file not found"
    fi
    
    info "Build completed successfully"
}

# Package modules
package_modules() {
    info "Packaging official modules..."
    
    # Create module directory
    MODULE_DIR="$PROJECT_ROOT/target/modules"
    mkdir -p "$MODULE_DIR"
    
    # Package Chtholly module
    CHTHOLLY_PATH="$PROJECT_ROOT/src/main/java/com/chtl/module/Chtholly"
    if [ -d "$CHTHOLLY_PATH" ]; then
        info "Packaging Chtholly module..."
        
        # Use packaging script
        if [ -f "$PROJECT_ROOT/scripts/linux/package-unified.sh" ]; then
            bash "$PROJECT_ROOT/scripts/linux/package-unified.sh" "$CHTHOLLY_PATH" "$MODULE_DIR"
        else
            warning "Module packaging script not found, copying source files..."
            cp -r "$CHTHOLLY_PATH" "$MODULE_DIR/"
        fi
    fi
    
    info "Module packaging completed"
}

# Create distribution package
create_distribution() {
    info "Creating distribution package..."
    
    # Create dist directory
    DIST_DIR="$PROJECT_ROOT/dist"
    mkdir -p "$DIST_DIR"
    
    # Create temporary directory
    TEMP_DIR=$(mktemp -d)
    PACKAGE_NAME="chtl-compiler-linux"
    PACKAGE_DIR="$TEMP_DIR/$PACKAGE_NAME"
    mkdir -p "$PACKAGE_DIR"
    
    # Copy files
    cp "target/chtl-compiler-1.0.jar" "$PACKAGE_DIR/chtl-compiler.jar"
    
    # Copy modules
    if [ -d "target/modules" ]; then
        cp -r "target/modules" "$PACKAGE_DIR/"
    fi
    
    # Create launcher script
    cat > "$PACKAGE_DIR/chtl" << 'EOF'
#!/bin/bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
java -jar "$SCRIPT_DIR/chtl-compiler.jar" "$@"
EOF
    chmod +x "$PACKAGE_DIR/chtl"
    
    # Create installation script
    cat > "$PACKAGE_DIR/install.sh" << 'EOF'
#!/bin/bash
set -e

echo "Installing CHTL compiler..."

# Check permissions
if [ "$EUID" -ne 0 ]; then 
    echo "Please run with sudo"
    exit 1
fi

# Install directory
INSTALL_DIR="/opt/chtl"
BIN_LINK="/usr/local/bin/chtl"

# Create installation directory
mkdir -p "$INSTALL_DIR"

# Copy files
cp -r * "$INSTALL_DIR/"

# Create symbolic link
ln -sf "$INSTALL_DIR/chtl" "$BIN_LINK"

# Set permissions
chmod -R 755 "$INSTALL_DIR"
chmod +x "$INSTALL_DIR/chtl"

echo "CHTL compiler installed successfully!"
echo "You can now use 'chtl' command"
EOF
    chmod +x "$PACKAGE_DIR/install.sh"
    
    # Create uninstall script
    cat > "$PACKAGE_DIR/uninstall.sh" << 'EOF'
#!/bin/bash
set -e

echo "Uninstalling CHTL compiler..."

# Check permissions
if [ "$EUID" -ne 0 ]; then 
    echo "Please run with sudo"
    exit 1
fi

# Remove files
rm -rf /opt/chtl
rm -f /usr/local/bin/chtl

echo "CHTL compiler uninstalled successfully!"
EOF
    chmod +x "$PACKAGE_DIR/uninstall.sh"
    
    # Create README
    cat > "$PACKAGE_DIR/README.md" << 'EOF'
# CHTL Compiler - Linux

## Installation

Run with administrator privileges:
```bash
sudo bash install.sh
```

## Usage

After installation, you can use the `chtl` command:
```bash
chtl input.chtl -o output.html
```

## Uninstallation

Run with administrator privileges:
```bash
sudo bash uninstall.sh
```
EOF
    
    # Package
    info "Creating tarball..."
    cd "$TEMP_DIR"
    tar -czf "$DIST_DIR/$PACKAGE_NAME.tar.gz" "$PACKAGE_NAME"
    
    # Clean up
    rm -rf "$TEMP_DIR"
    
    info "Distribution package created: $DIST_DIR/$PACKAGE_NAME.tar.gz"
}

# Parse command line arguments
SKIP_TESTS=false
PACKAGE_ONLY=false

while [[ $# -gt 0 ]]; do
    case $1 in
        --skip-tests)
            SKIP_TESTS=true
            shift
            ;;
        --package-only)
            PACKAGE_ONLY=true
            shift
            ;;
        -h|--help)
            echo "Usage: $0 [options]"
            echo "Options:"
            echo "  --skip-tests    Skip running tests"
            echo "  --package-only  Only create distribution package (skip build)"
            echo "  -h, --help      Show this help message"
            exit 0
            ;;
        *)
            error "Unknown option: $1"
            ;;
    esac
done

# Main execution
info "CHTL Compiler Build Script - Linux"
info "=================================="

if [ "$PACKAGE_ONLY" != true ]; then
    check_environment
    build
    package_modules
fi

create_distribution

info "Build process completed successfully!"