#!/bin/bash

# Unified Module Packaging Script - Linux Platform
# Packages modules containing both CMOD and CJMOD

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
    echo "  $0 src/main/java/com/chtl/module/Chtholly -o dist/"
    echo "  $0 mymodule --sign"
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
    
    info "Checking module structure..."
    
    # Check for unified structure (CMOD + CJMOD)
    local has_cmod=false
    local has_cjmod=false
    
    if [[ -d "$module_dir/CMOD" ]] || [[ -d "$module_dir/cmod" ]]; then
        has_cmod=true
    fi
    
    if [[ -d "$module_dir/CJMOD" ]] || [[ -d "$module_dir/cjmod" ]]; then
        has_cjmod=true
    fi
    
    if [[ "$has_cmod" == false && "$has_cjmod" == false ]]; then
        error "No CMOD or CJMOD directories found. This script is for unified modules."
    fi
    
    if [[ "$has_cmod" == true ]]; then
        info "Found CMOD component"
    fi
    
    if [[ "$has_cjmod" == true ]]; then
        info "Found CJMOD component"
    fi
}

# Read module info
read_module_info() {
    local module_dir="$1"
    
    info "Reading module info..."
    
    # Try to find info file in module root first
    local info_file=""
    local module_basename=$(basename "$module_dir")
    
    # Check for root info file
    if [[ -f "$module_dir/info.chtl" ]]; then
        info_file="$module_dir/info.chtl"
    elif [[ -f "$module_dir/$module_basename.chtl" ]]; then
        info_file="$module_dir/$module_basename.chtl"
    else
        # Check in CMOD directory
        if [[ -d "$module_dir/CMOD/info" ]]; then
            for file in "$module_dir/CMOD/info"/*.chtl; do
                if [[ -f "$file" ]]; then
                    info_file="$file"
                    break
                fi
            done
        elif [[ -d "$module_dir/cmod/info" ]]; then
            for file in "$module_dir/cmod/info"/*.chtl; do
                if [[ -f "$file" ]]; then
                    info_file="$file"
                    break
                fi
            done
        fi
    fi
    
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

# Package CMOD component
package_cmod() {
    local src_dir="$1"
    local dest_dir="$2"
    
    info "Packaging CMOD component..."
    
    # Create destination directory
    mkdir -p "$dest_dir/CMOD"
    
    # Copy CMOD structure
    if [[ -d "$src_dir/src" ]]; then
        cp -r "$src_dir/src" "$dest_dir/CMOD/"
    fi
    
    if [[ -d "$src_dir/info" ]]; then
        cp -r "$src_dir/info" "$dest_dir/CMOD/"
    fi
    
    info "CMOD component packaged"
}

# Package CJMOD component
package_cjmod() {
    local src_dir="$1"
    local dest_dir="$2"
    
    info "Packaging CJMOD component..."
    
    # Create destination directory
    mkdir -p "$dest_dir/CJMOD"
    
    # Copy source
    if [[ -d "$src_dir/src" ]]; then
        mkdir -p "$dest_dir/CJMOD/src"
        
        # Compile Java files if present
        local java_files=()
        while IFS= read -r -d '' file; do
            java_files+=("$file")
        done < <(find "$src_dir/src" -name "*.java" -print0)
        
        if [[ ${#java_files[@]} -gt 0 ]]; then
            info "Compiling Java source..."
            
            # Create build directory
            local build_dir="$dest_dir/CJMOD/build"
            mkdir -p "$build_dir"
            
            # Get classpath
            local classpath=""
            local project_root="$(cd "$(dirname "$0")/../.." && pwd)"
            
            if [[ -f "$project_root/target/chtl-compiler-1.0.jar" ]]; then
                classpath="$project_root/target/chtl-compiler-1.0.jar"
            fi
            
            # Compile
            if [[ -n "$classpath" ]]; then
                javac -cp "$classpath" -d "$build_dir" "${java_files[@]}"
            else
                javac -d "$build_dir" "${java_files[@]}"
            fi
            
            # Create JAR
            jar cf "$dest_dir/CJMOD/module.jar" -C "$build_dir" .
        fi
        
        # Copy source if requested
        if [[ "$INCLUDE_SOURCE" == true ]]; then
            cp -r "$src_dir/src" "$dest_dir/CJMOD/"
        fi
    fi
    
    # Copy info
    if [[ -d "$src_dir/info" ]]; then
        cp -r "$src_dir/info" "$dest_dir/CJMOD/"
    fi
    
    info "CJMOD component packaged"
}

# Create unified package
create_package() {
    local module_dir="$1"
    local package_file="$2"
    
    info "Creating unified module package..."
    
    # Create temporary directory
    local temp_dir=$(mktemp -d)
    local work_dir="$temp_dir/module"
    mkdir -p "$work_dir"
    
    # Package CMOD component if exists
    if [[ -d "$module_dir/CMOD" ]]; then
        package_cmod "$module_dir/CMOD" "$work_dir"
    elif [[ -d "$module_dir/cmod" ]]; then
        package_cmod "$module_dir/cmod" "$work_dir"
    fi
    
    # Package CJMOD component if exists
    if [[ -d "$module_dir/CJMOD" ]]; then
        package_cjmod "$module_dir/CJMOD" "$work_dir"
    elif [[ -d "$module_dir/cjmod" ]]; then
        package_cjmod "$module_dir/cjmod" "$work_dir"
    fi
    
    # Copy root info file if exists
    if [[ -f "$module_dir/info.chtl" ]]; then
        cp "$module_dir/info.chtl" "$work_dir/"
    fi
    
    # Create manifest
    cat > "$work_dir/MANIFEST.MF" << EOF
Module-Name: $MODULE_NAME
Module-Version: $MODULE_VERSION
Module-Type: Unified
Build-Date: $(date -u +"%Y-%m-%d %H:%M:%S UTC")
Include-Source: $INCLUDE_SOURCE
EOF
    
    # Create ZIP archive
    cd "$temp_dir"
    zip -r "$package_file" module/
    
    # Clean up
    rm -rf "$temp_dir"
    
    info "Unified module package created: $package_file"
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
    info "Unified Module Packaging Script"
    info "==============================="
    
    # Check module structure
    check_module_structure "$MODULE_PATH"
    
    # Read module info
    read_module_info "$MODULE_PATH"
    info "Module name: $MODULE_NAME"
    info "Module version: $MODULE_VERSION"
    
    # Create output directory
    mkdir -p "$OUTPUT_DIR"
    
    # Create package filename
    PACKAGE_FILE="$OUTPUT_DIR/${MODULE_NAME}-${MODULE_VERSION}-unified.zip"
    
    # Create package
    create_package "$MODULE_PATH" "$PACKAGE_FILE"
    
    # Sign if requested
    if [[ "$SIGN_MODULE" == true ]]; then
        sign_module "$PACKAGE_FILE"
    fi
    
    # Show package info
    echo
    info "Package created successfully!"
    echo "  File: $PACKAGE_FILE"
    echo "  Size: $(du -h "$PACKAGE_FILE" | cut -f1)"
    echo "  Type: Unified (CMOD + CJMOD)"
    
    if [[ -f "${PACKAGE_FILE}.sig" ]]; then
        echo "  Signature: ${PACKAGE_FILE}.sig"
    fi
}

# Run main
main