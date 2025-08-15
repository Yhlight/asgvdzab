#!/bin/bash

# CMOD Module Packaging Script - Linux Platform

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
    echo "  --no-export             Don't generate [Export] block"
    echo "  --sign                  Sign the module"
    echo "  -h, --help              Show this help message"
    echo
    echo "Examples:"
    echo "  $0 src/main/java/com/chtl/module/Chtholly/CMOD -o dist/"
    echo "  $0 mymodule/CMOD --sign"
}

# Parse command line arguments
MODULE_PATH=""
OUTPUT_DIR="."
MODULE_NAME=""
MODULE_VERSION=""
GENERATE_EXPORT=true
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
        --no-export)
            GENERATE_EXPORT=false
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
    
    # Check for CMOD structure
    if [[ -d "$module_dir/CMOD" ]] || [[ -d "$module_dir/cmod" ]]; then
        # Combined CMOD+CJMOD structure
        local cmod_dir=""
        if [[ -d "$module_dir/CMOD" ]]; then
            cmod_dir="$module_dir/CMOD"
        else
            cmod_dir="$module_dir/cmod"
        fi
        
        if [[ ! -d "$cmod_dir/src" ]]; then
            error "CMOD src directory not found"
        fi
        
        if [[ ! -d "$cmod_dir/info" ]]; then
            error "CMOD info directory not found"
        fi
        
        echo "$cmod_dir"
    else
        # Pure CMOD structure
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

# Process CHTL files
process_chtl_files() {
    local src_dir="$1"
    local output_dir="$2"
    
    info "Processing CHTL files..."
    
    # Find all CHTL files
    local chtl_files=()
    while IFS= read -r -d '' file; do
        chtl_files+=("$file")
    done < <(find "$src_dir" -name "*.chtl" -print0)
    
    if [[ ${#chtl_files[@]} -eq 0 ]]; then
        warning "No CHTL files found"
        return 0
    fi
    
    # Create output directory
    mkdir -p "$output_dir"
    
    # Copy CHTL files maintaining structure
    for file in "${chtl_files[@]}"; do
        local rel_path="${file#$src_dir/}"
        local dest_file="$output_dir/$rel_path"
        local dest_dir=$(dirname "$dest_file")
        
        mkdir -p "$dest_dir"
        cp "$file" "$dest_file"
    done
    
    info "Processed ${#chtl_files[@]} CHTL files"
}

# Generate Export block
generate_export_block() {
    local src_dir="$1"
    local info_file="$2"
    
    if [[ "$GENERATE_EXPORT" != true ]]; then
        return 0
    fi
    
    # Check if Export block already exists
    if grep -q "^\[Export\]" "$info_file"; then
        info "[Export] block already exists"
        return 0
    fi
    
    info "Generating [Export] block..."
    
    # Analyze CHTL files to extract exportable items
    local styles=""
    local elements=""
    local vars=""
    
    # Find all CHTL files
    while IFS= read -r -d '' file; do
        # Extract @Style declarations
        while IFS= read -r style; do
            styles="$styles$style, "
        done < <(grep -oP '@Style\s+\K\w+' "$file")
        
        # Extract @Element declarations
        while IFS= read -r element; do
            elements="$elements$element, "
        done < <(grep -oP '@Element\s+\K\w+' "$file")
        
        # Extract @Var declarations
        while IFS= read -r var; do
            vars="$vars$var, "
        done < <(grep -oP '@Var\s+\K\w+' "$file")
    done < <(find "$src_dir" -name "*.chtl" -print0)
    
    # Remove trailing comma and space
    styles="${styles%, }"
    elements="${elements%, }"
    vars="${vars%, }"
    
    # Append Export block to info file
    {
        echo ""
        echo "[Export]"
        echo "{"
        [[ -n "$styles" ]] && echo "    @Style $styles;"
        [[ -n "$elements" ]] && echo "    @Element $elements;"
        [[ -n "$vars" ]] && echo "    @Var $vars;"
        echo "}"
    } >> "$info_file"
    
    info "[Export] block generated"
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
    
    # Process CHTL files
    process_chtl_files "$module_dir/src" "$work_dir/src"
    
    # Generate Export block if needed
    local info_file=""
    for file in "$work_dir/info"/*.chtl; do
        if [[ -f "$file" ]]; then
            info_file="$file"
            break
        fi
    done
    
    if [[ -n "$info_file" ]]; then
        generate_export_block "$work_dir/src" "$info_file"
    fi
    
    # Create manifest
    cat > "$work_dir/MANIFEST.MF" << EOF
Module-Name: $MODULE_NAME
Module-Version: $MODULE_VERSION
Module-Type: CMOD
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
    info "CMOD Module Packaging Script"
    info "============================"
    
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
    PACKAGE_FILE="$OUTPUT_DIR/${MODULE_NAME}-${MODULE_VERSION}.cmod"
    
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