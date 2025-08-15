#!/bin/bash

# CJMOD Module Packaging Script - Linux Platform

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

# Show usage
usage() {
    echo "Usage: $0 <module-path> [options]"
    echo
    echo "Options:"
    echo "  -o, --output <dir>      Output directory (default: .)"
    echo "  -n, --name <name>       Module name (default: read from info file)"
    echo "  -v, --version <ver>     Module version (default: read from info file)"
    echo "  --no-source             Don't include source code"
    echo "  --sign                  Sign the module"
    echo "  -h, --help              Show this help message"
    echo
    echo "Examples:"
    echo "  $0 src/main/java/com/chtl/module/Chtholly/CJMOD -o dist/"
    echo "  $0 mymodule/CJMOD --sign"
}

# Parse command line arguments
MODULE_PATH=""
OUTPUT_DIR="."
MODULE_NAME=""
MODULE_VERSION=""
INCLUDE_SOURCE=true
SIGN_MODULE=false

while [[ $# -gt 0 ]]; do
    case $1 in
        -o|--output)
            OUTPUT_DIR="$2"
            shift 2
            ;;
        -n|--name)
            MODULE_NAME="$2"
            shift 2
            ;;
        -v|--version)
            MODULE_VERSION="$2"
            shift 2
            ;;
        --no-source)
            INCLUDE_SOURCE=false
            shift
            ;;
        --sign)
            SIGN_MODULE=true
            shift
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            if [[ -z "$MODULE_PATH" ]]; then
                MODULE_PATH="$1"
                shift
            else
                error "Unknown argument: $1"
            fi
            ;;
    esac
done

# Validate arguments
if [[ -z "$MODULE_PATH" ]]; then
    error "Module path not specified"
fi

if [[ ! -d "$MODULE_PATH" ]]; then
    error "Module path does not exist: $MODULE_PATH"
fi

# Check module structure
check_module_structure() {
    local module_dir="$1"
    
    # Check for CJMOD structure
    if [[ -d "$module_dir/CJMOD" ]] || [[ -d "$module_dir/cjmod" ]]; then
        # Combined CMOD+CJMOD structure
        local cjmod_dir=""
        if [[ -d "$module_dir/CJMOD" ]]; then
            cjmod_dir="$module_dir/CJMOD"
        else
            cjmod_dir="$module_dir/cjmod"
        fi
        
        if [[ ! -d "$cjmod_dir/src" ]]; then
            error "CJMOD src directory not found"
        fi
        
        if [[ ! -d "$cjmod_dir/info" ]]; then
            error "CJMOD info directory not found"
        fi
        
        echo "$cjmod_dir"
    else
        # Pure CJMOD structure
        if [[ ! -d "$module_dir/src" ]]; then
            error "src directory not found"
        fi
        
        if [[ ! -d "$module_dir/info" ]]; then
            error "info directory not found"
        fi
        
        echo "$module_dir"
    fi
}

# Read module info
read_module_info() {
    local info_dir="$1/info"
    local info_file=""
    
    # Find info.chtl file
    for file in "$info_dir"/*.chtl; do
        if [[ -f "$file" ]]; then
            info_file="$file"
            break
        fi
    done
    
    if [[ -z "$info_file" ]]; then
        error "Module info file not found"
    fi
    
    # Extract name and version if not provided
    if [[ -z "$MODULE_NAME" ]]; then
        MODULE_NAME=$(grep -oP 'name\s*=\s*"\K[^"]+' "$info_file" | head -1)
        if [[ -z "$MODULE_NAME" ]]; then
            MODULE_NAME=$(basename "$MODULE_PATH")
        fi
    fi
    
    if [[ -z "$MODULE_VERSION" ]]; then
        MODULE_VERSION=$(grep -oP 'version\s*=\s*"\K[^"]+' "$info_file" | head -1)
        if [[ -z "$MODULE_VERSION" ]]; then
            MODULE_VERSION="1.0.0"
        fi
    fi
}

# Compile Java source
compile_java_source() {
    local src_dir="$1"
    local build_dir="$2"
    
    info "Compiling Java source code..."
    
    # Find all Java files
    local java_files=()
    while IFS= read -r -d '' file; do
        java_files+=("$file")
    done < <(find "$src_dir" -name "*.java" -print0)
    
    if [[ ${#java_files[@]} -eq 0 ]]; then
        info "No Java files found, skipping compilation"
        return 0
    fi
    
    # Create build directory
    mkdir -p "$build_dir"
    
    # Get project classpath
    local classpath=""
    local project_root="$(cd "$(dirname "$0")/../.." && pwd)"
    
    # Add compiler JAR
    if [[ -f "$project_root/target/chtl-compiler-1.0.jar" ]]; then
        classpath="$project_root/target/chtl-compiler-1.0.jar"
    fi
    
    # Add dependencies
    if [[ -d "$project_root/target/lib" ]]; then
        for jar in "$project_root/target/lib"/*.jar; do
            if [[ -f "$jar" ]]; then
                classpath="$classpath:$jar"
            fi
        done
    fi
    
    # Compile
    if [[ -n "$classpath" ]]; then
        javac -cp "$classpath" -d "$build_dir" "${java_files[@]}"
    else
        javac -d "$build_dir" "${java_files[@]}"
    fi
    
    # Create JAR
    jar cf "$build_dir/module.jar" -C "$build_dir" .
}

# Create module package
create_package() {
    local module_dir="$1"
    local package_file="$2"
    
    info "Creating module package..."
    
    # Create temporary directory
    local temp_dir=$(mktemp -d)
    local work_dir="$temp_dir/module"
    mkdir -p "$work_dir"
    
    # Copy module structure
    cp -r "$module_dir"/* "$work_dir/"
    
    # Compile if needed
    if [[ -d "$work_dir/src" ]] && ls "$work_dir/src"/*.java &>/dev/null 2>&1; then
        local build_dir="$work_dir/build"
        compile_java_source "$work_dir/src" "$build_dir"
        
        # Remove source if not needed
        if [[ "$INCLUDE_SOURCE" != true ]]; then
            rm -rf "$work_dir/src"
        fi
    fi
    
    # Create manifest
    cat > "$work_dir/MANIFEST.MF" << EOF
Module-Name: $MODULE_NAME
Module-Version: $MODULE_VERSION
Module-Type: CJMOD
Build-Date: $(date -u +"%Y-%m-%d %H:%M:%S UTC")
EOF
    
    # Create ZIP archive
    cd "$temp_dir"
    zip -r "$package_file" module/
    
    # Clean up
    rm -rf "$temp_dir"
    
    info "Module package created: $package_file"
}

# Sign module
sign_module() {
    local module_file="$1"
    
    info "Signing module..."
    
    # Check for signing key
    local key_file="$HOME/.chtl/keys/module-signing.key"
    if [[ ! -f "$key_file" ]]; then
        warning "Signing key not found, skipping signature"
        return 0
    fi
    
    # Create signature
    openssl dgst -sha256 -sign "$key_file" -out "${module_file}.sig" "$module_file"
    
    info "Module signed successfully"
}

# Main process
main() {
    info "CJMOD Module Packaging Script"
    info "============================="
    
    # Check module structure
    MODULE_DIR=$(check_module_structure "$MODULE_PATH")
    info "Module directory: $MODULE_DIR"
    
    # Read module info
    read_module_info "$MODULE_DIR"
    info "Module name: $MODULE_NAME"
    info "Module version: $MODULE_VERSION"
    
    # Create output directory
    mkdir -p "$OUTPUT_DIR"
    
    # Create package filename
    PACKAGE_FILE="$OUTPUT_DIR/${MODULE_NAME}-${MODULE_VERSION}.cjmod"
    
    # Create package
    create_package "$MODULE_DIR" "$PACKAGE_FILE"
    
    # Sign if requested
    if [[ "$SIGN_MODULE" == true ]]; then
        sign_module "$PACKAGE_FILE"
    fi
    
    # Show package info
    echo
    info "Package created successfully!"
    echo "  File: $PACKAGE_FILE"
    echo "  Size: $(du -h "$PACKAGE_FILE" | cut -f1)"
    
    if [[ -f "${PACKAGE_FILE}.sig" ]]; then
        echo "  Signature: ${PACKAGE_FILE}.sig"
    fi
}

# Run main
main