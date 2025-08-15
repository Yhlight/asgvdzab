#!/bin/bash

# CHTL Compiler Build Script - macOS Platform
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
    
    # Check Xcode Command Line Tools
    if ! xcode-select -p &> /dev/null; then
        warning "Xcode Command Line Tools not installed"
        info "Installing Xcode Command Line Tools..."
        xcode-select --install
    fi
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
        if [ -f "$PROJECT_ROOT/scripts/macos/package-unified.sh" ]; then
            bash "$PROJECT_ROOT/scripts/macos/package-unified.sh" "$CHTHOLLY_PATH" "$MODULE_DIR"
        else
            warning "Module packaging script not found, copying source files..."
            cp -r "$CHTHOLLY_PATH" "$MODULE_DIR/"
        fi
    fi
    
    info "Module packaging completed"
}

# Create macOS app bundle
create_app_bundle() {
    info "Creating macOS app bundle..."
    
    APP_NAME="CHTL Compiler"
    APP_DIR="$PROJECT_ROOT/dist/CHTL Compiler.app"
    
    # Create app structure
    mkdir -p "$APP_DIR/Contents/"{MacOS,Resources,Java}
    
    # Copy JAR file
    cp "target/chtl-compiler-1.0.jar" "$APP_DIR/Contents/Java/chtl-compiler.jar"
    
    # Copy modules
    if [ -d "target/modules" ]; then
        cp -r "target/modules" "$APP_DIR/Contents/Resources/"
    fi
    
    # Create launcher script
    cat > "$APP_DIR/Contents/MacOS/chtl" << 'EOF'
#!/bin/bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
APP_DIR="$(dirname "$SCRIPT_DIR")"
java -jar "$APP_DIR/Java/chtl-compiler.jar" "$@"
EOF
    chmod +x "$APP_DIR/Contents/MacOS/chtl"
    
    # Create Info.plist
    cat > "$APP_DIR/Contents/Info.plist" << EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleIdentifier</key>
    <string>com.chtl.compiler</string>
    <key>CFBundleName</key>
    <string>CHTL Compiler</string>
    <key>CFBundleDisplayName</key>
    <string>CHTL Compiler</string>
    <key>CFBundleVersion</key>
    <string>1.0.0</string>
    <key>CFBundleShortVersionString</key>
    <string>1.0</string>
    <key>CFBundleExecutable</key>
    <string>chtl</string>
    <key>CFBundlePackageType</key>
    <string>APPL</string>
    <key>NSHighResolutionCapable</key>
    <true/>
</dict>
</plist>
EOF
    
    info "App bundle created: $APP_DIR"
}

# Create distribution package
create_distribution() {
    info "Creating distribution package..."
    
    # Create dist directory
    DIST_DIR="$PROJECT_ROOT/dist"
    mkdir -p "$DIST_DIR"
    
    # Create temporary directory
    TEMP_DIR=$(mktemp -d)
    PACKAGE_NAME="chtl-compiler-macos"
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

# Install directory
INSTALL_DIR="/usr/local/chtl"
BIN_LINK="/usr/local/bin/chtl"

# Create installation directory
sudo mkdir -p "$INSTALL_DIR"

# Copy files
sudo cp -r * "$INSTALL_DIR/"

# Create symbolic link
sudo ln -sf "$INSTALL_DIR/chtl" "$BIN_LINK"

# Set permissions
sudo chmod -R 755 "$INSTALL_DIR"
sudo chmod +x "$INSTALL_DIR/chtl"

echo "CHTL compiler installed successfully!"
echo "You can now use 'chtl' command"
EOF
    chmod +x "$PACKAGE_DIR/install.sh"
    
    # Create uninstall script
    cat > "$PACKAGE_DIR/uninstall.sh" << 'EOF'
#!/bin/bash
set -e

echo "Uninstalling CHTL compiler..."

# Remove files
sudo rm -rf /usr/local/chtl
sudo rm -f /usr/local/bin/chtl

echo "CHTL compiler uninstalled successfully!"
EOF
    chmod +x "$PACKAGE_DIR/uninstall.sh"
    
    # Create README
    cat > "$PACKAGE_DIR/README.md" << 'EOF'
# CHTL Compiler - macOS

## Installation

### Method 1: Command Line
```bash
bash install.sh
```

### Method 2: App Bundle
Use the `.app` bundle in the DMG file.

## Usage

After installation, you can use the `chtl` command:
```bash
chtl input.chtl -o output.html
```

## Uninstallation

```bash
bash uninstall.sh
```
EOF
    
    # Package as tarball
    info "Creating tarball..."
    cd "$TEMP_DIR"
    tar -czf "$DIST_DIR/$PACKAGE_NAME.tar.gz" "$PACKAGE_NAME"
    
    # Create DMG if app bundle exists
    if [ -d "$DIST_DIR/CHTL Compiler.app" ]; then
        info "Creating DMG..."
        
        DMG_TEMP=$(mktemp -d)
        cp -r "$DIST_DIR/CHTL Compiler.app" "$DMG_TEMP/"
        cp -r "$PACKAGE_DIR"/* "$DMG_TEMP/"
        
        hdiutil create -volname "CHTL Compiler" \
            -srcfolder "$DMG_TEMP" \
            -ov -format UDZO \
            "$DIST_DIR/$PACKAGE_NAME.dmg"
        
        rm -rf "$DMG_TEMP"
        info "DMG created: $DIST_DIR/$PACKAGE_NAME.dmg"
    fi
    
    # Clean up
    rm -rf "$TEMP_DIR"
    
    info "Distribution packages created"
}

# Parse command line arguments
SKIP_TESTS=false
CREATE_APP=true
PACKAGE_ONLY=false

while [[ $# -gt 0 ]]; do
    case $1 in
        --skip-tests)
            SKIP_TESTS=true
            shift
            ;;
        --no-app)
            CREATE_APP=false
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
            echo "  --no-app        Don't create macOS app bundle"
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
info "CHTL Compiler Build Script - macOS"
info "=================================="

if [ "$PACKAGE_ONLY" != true ]; then
    check_environment
    build
    package_modules
    
    if [ "$CREATE_APP" = true ]; then
        create_app_bundle
    fi
fi

create_distribution

info "Build process completed successfully!"

# Show results
echo
info "Build artifacts:"
if [ -f "dist/chtl-compiler-macos.tar.gz" ]; then
    echo "  Tarball: dist/chtl-compiler-macos.tar.gz"
fi
if [ -f "dist/chtl-compiler-macos.dmg" ]; then
    echo "  DMG: dist/chtl-compiler-macos.dmg"
fi
if [ -d "dist/CHTL Compiler.app" ]; then
    echo "  App Bundle: dist/CHTL Compiler.app"
fi