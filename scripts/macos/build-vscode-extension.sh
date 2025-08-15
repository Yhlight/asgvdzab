#!/bin/bash

# VSCode CHTL Extension Build Script - macOS Platform

set -e

# Color definitions
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
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
    
    # Check Node.js
    if ! command -v node &> /dev/null; then
        # Try to install with Homebrew
        if command -v brew &> /dev/null; then
            warning "Node.js not found, attempting to install via Homebrew..."
            brew install node
        else
            error "Node.js not found. Please install Node.js 14 or higher"
        fi
    fi
    
    NODE_VERSION=$(node -v | cut -d'v' -f2 | cut -d'.' -f1)
    if [ "$NODE_VERSION" -lt 14 ]; then
        error "Node.js 14 or higher required, current version: $(node -v)"
    fi
    info "Node.js version: $(node -v)"
    
    # Check npm
    if ! command -v npm &> /dev/null; then
        error "npm not found. Please install npm"
    fi
    info "npm version: $(npm -v)"
    
    # Check vsce
    if ! command -v vsce &> /dev/null; then
        info "Installing vsce (Visual Studio Code Extension CLI)..."
        npm install -g vsce
    fi
    info "vsce version: $(vsce --version)"
}

# Build compiler
build_compiler() {
    info "Checking CHTL compiler..."
    
    cd "$PROJECT_ROOT"
    
    if [ ! -f "target/chtl-compiler-1.0.jar" ]; then
        info "Compiler not found, building..."
        
        if [ -f "scripts/macos/build-compiler.sh" ]; then
            bash "scripts/macos/build-compiler.sh" --skip-tests --no-app
        else
            mvn clean package -DskipTests
        fi
    fi
    
    if [ ! -f "target/chtl-compiler-1.0.jar" ]; then
        error "Failed to build compiler"
    fi
    
    info "Compiler ready"
}

# Build extension
build_extension() {
    info "Building VSCode extension..."
    
    cd "$EXTENSION_DIR"
    
    # Clean old build
    if [ -d "out" ]; then
        rm -rf out
    fi
    
    # Install dependencies
    info "Installing dependencies..."
    npm install
    
    # Compile TypeScript
    info "Compiling TypeScript..."
    npm run compile
    
    # Run tests (optional)
    if [ "$SKIP_TESTS" != true ]; then
        info "Running tests..."
        npm test || warning "Some tests failed"
    fi
    
    info "Extension build completed"
}

# Copy resources
copy_resources() {
    info "Copying resources..."
    
    # Create resources directory
    mkdir -p "$EXTENSION_DIR/resources"
    
    # Copy compiler
    if [ -f "$PROJECT_ROOT/target/chtl-compiler-1.0.jar" ]; then
        info "Copying compiler JAR..."
        cp "$PROJECT_ROOT/target/chtl-compiler-1.0.jar" "$EXTENSION_DIR/resources/chtl-compiler.jar"
    else
        error "Compiler JAR not found"
    fi
    
    # Copy official modules
    if [ -d "$PROJECT_ROOT/target/modules" ]; then
        info "Copying official modules..."
        cp -r "$PROJECT_ROOT/target/modules" "$EXTENSION_DIR/resources/"
    elif [ -d "$PROJECT_ROOT/src/main/java/com/chtl/module" ]; then
        info "Packaging official modules..."
        mkdir -p "$EXTENSION_DIR/resources/modules"
        
        # Package Chtholly module if exists
        if [ -d "$PROJECT_ROOT/src/main/java/com/chtl/module/Chtholly" ]; then
            bash "$PROJECT_ROOT/scripts/macos/package-unified.sh" \
                "$PROJECT_ROOT/src/main/java/com/chtl/module/Chtholly" \
                -o "$EXTENSION_DIR/resources/modules"
        fi
    fi
    
    # Create launcher scripts
    create_launcher_scripts
    
    info "Resources copied successfully"
}

# Create launcher scripts
create_launcher_scripts() {
    info "Creating launcher scripts..."
    
    # Create chtl.sh for Unix
    cat > "$EXTENSION_DIR/resources/chtl.sh" << 'EOF'
#!/bin/bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
java -jar "$SCRIPT_DIR/chtl-compiler.jar" "$@"
EOF
    chmod +x "$EXTENSION_DIR/resources/chtl.sh"
    
    # Create chtl.bat for Windows
    cat > "$EXTENSION_DIR/resources/chtl.bat" << 'EOF'
@echo off
set SCRIPT_DIR=%~dp0
java -jar "%SCRIPT_DIR%chtl-compiler.jar" %*
EOF
    
    # Create macOS-specific launcher
    cat > "$EXTENSION_DIR/resources/chtl-macos.command" << 'EOF'
#!/bin/bash
cd "$(dirname "$0")"
java -jar chtl-compiler.jar "$@"
EOF
    chmod +x "$EXTENSION_DIR/resources/chtl-macos.command"
    
    info "Launcher scripts created"
}

# Package extension
package_extension() {
    info "Packaging VSCode extension..."
    
    cd "$EXTENSION_DIR"
    
    # Update version if provided
    if [ -n "$EXTENSION_VERSION" ]; then
        info "Updating version to $EXTENSION_VERSION..."
        npm version "$EXTENSION_VERSION" --no-git-tag-version
    fi
    
    # Package with vsce
    info "Creating VSIX package..."
    vsce package
    
    # Find generated VSIX file
    VSIX_FILE=$(ls -t *.vsix | head -1)
    if [ -z "$VSIX_FILE" ]; then
        error "Failed to create VSIX package"
    fi
    
    info "Extension packaged: $VSIX_FILE"
    
    # Copy to dist directory
    if [ -n "$OUTPUT_DIR" ]; then
        mkdir -p "$OUTPUT_DIR"
        cp "$VSIX_FILE" "$OUTPUT_DIR/"
        info "Package copied to: $OUTPUT_DIR/$VSIX_FILE"
    fi
}

# Create installer app
create_installer_app() {
    info "Creating macOS installer app..."
    
    if [ -z "$OUTPUT_DIR" ]; then
        OUTPUT_DIR="$PROJECT_ROOT/dist"
    fi
    
    INSTALLER_APP="$OUTPUT_DIR/CHTL Extension Installer.app"
    mkdir -p "$INSTALLER_APP/Contents/"{MacOS,Resources}
    
    # Find VSIX file
    VSIX_FILE=$(find "$EXTENSION_DIR" -name "*.vsix" | head -1)
    if [ -z "$VSIX_FILE" ]; then
        warning "No VSIX file found, skipping installer creation"
        return
    fi
    
    # Copy VSIX to Resources
    cp "$VSIX_FILE" "$INSTALLER_APP/Contents/Resources/extension.vsix"
    
    # Create installer script
    cat > "$INSTALLER_APP/Contents/MacOS/install" << 'EOF'
#!/bin/bash

# Get app bundle path
APP_DIR="$(dirname "$0")/.."
VSIX_FILE="$APP_DIR/Resources/extension.vsix"

# Check if VSCode is installed
if ! command -v code &> /dev/null; then
    osascript -e 'display alert "VSCode not found" message "Please install Visual Studio Code first." as critical'
    exit 1
fi

# Install extension
if code --install-extension "$VSIX_FILE"; then
    osascript -e 'display alert "Installation Successful" message "CHTL extension has been installed."'
else
    osascript -e 'display alert "Installation Failed" message "Failed to install CHTL extension." as critical'
fi
EOF
    chmod +x "$INSTALLER_APP/Contents/MacOS/install"
    
    # Create Info.plist
    cat > "$INSTALLER_APP/Contents/Info.plist" << EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleIdentifier</key>
    <string>com.chtl.extension.installer</string>
    <key>CFBundleName</key>
    <string>CHTL Extension Installer</string>
    <key>CFBundleExecutable</key>
    <string>install</string>
    <key>CFBundlePackageType</key>
    <string>APPL</string>
</dict>
</plist>
EOF
    
    info "Installer app created: $INSTALLER_APP"
}

# Show build info
show_build_info() {
    echo
    info "Build summary:"
    
    cd "$EXTENSION_DIR"
    
    # Find VSIX file
    VSIX_FILE=$(ls -t *.vsix | head -1)
    if [ -n "$VSIX_FILE" ]; then
        echo "  Package: $VSIX_FILE"
        echo "  Size: $(du -h "$VSIX_FILE" | cut -f1)"
        echo "  Location: $EXTENSION_DIR"
    fi
    
    echo
    info "Installation:"
    echo "  Command line: code --install-extension $VSIX_FILE"
    
    if [ -d "$OUTPUT_DIR/CHTL Extension Installer.app" ]; then
        echo "  macOS app: Double-click 'CHTL Extension Installer.app'"
    fi
}

# Parse command line arguments
SKIP_TESTS=false
SKIP_COMPILER=false
CREATE_INSTALLER=true
OUTPUT_DIR=""
EXTENSION_VERSION=""

while [[ $# -gt 0 ]]; do
    case $1 in
        --skip-tests)
            SKIP_TESTS=true
            shift
            ;;
        --skip-compiler)
            SKIP_COMPILER=true
            shift
            ;;
        --no-installer)
            CREATE_INSTALLER=false
            shift
            ;;
        -o|--output)
            OUTPUT_DIR="$2"
            shift 2
            ;;
        -v|--version)
            EXTENSION_VERSION="$2"
            shift 2
            ;;
        -h|--help)
            echo "Usage: $0 [options]"
            echo
            echo "Options:"
            echo "  --skip-tests        Skip running tests"
            echo "  --skip-compiler     Skip compiler build check"
            echo "  --no-installer      Don't create macOS installer app"
            echo "  -o, --output <dir>  Output directory for VSIX"
            echo "  -v, --version <ver> Set extension version"
            echo "  -h, --help          Show this help message"
            exit 0
            ;;
        *)
            error "Unknown option: $1"
            ;;
    esac
done

# Main execution
info "VSCode CHTL Extension Build Script - macOS"
info "========================================"

# Set directories
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$( cd "$SCRIPT_DIR/../.." && pwd )"
EXTENSION_DIR="$PROJECT_ROOT/vscode-chtl"

# Check extension directory
if [ ! -d "$EXTENSION_DIR" ]; then
    error "Extension directory not found: $EXTENSION_DIR"
fi

# Build steps
check_environment

if [ "$SKIP_COMPILER" != true ]; then
    build_compiler
fi

build_extension
copy_resources
package_extension

if [ "$CREATE_INSTALLER" = true ]; then
    create_installer_app
fi

show_build_info

info "Build process completed successfully!"