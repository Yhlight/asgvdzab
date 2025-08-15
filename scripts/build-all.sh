#!/bin/bash

# CHTL Project Unified Build Script - Unix Version

# Color definitions
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Default values
BUILD_TARGET="all"
PLATFORM=$(uname -s | tr '[:upper:]' '[:lower:]')
SKIP_TESTS=false
MODULE_PATH=""
MODULE_TYPE="unified"

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

# Usage function
show_usage() {
    echo "Usage: $0 [target] [options]"
    echo ""
    echo "Targets:"
    echo "  all          Build all components (default)"
    echo "  compiler     Build CHTL compiler only"
    echo "  vscode       Build VSCode extension only"
    echo "  module       Build specific module"
    echo "  clean        Clean all build outputs"
    echo ""
    echo "Options:"
    echo "  --skip-tests     Skip running tests"
    echo "  --type TYPE      Module type (cmod/cjmod/unified), for module target"
    echo ""
    echo "Examples:"
    echo "  $0                           # Build everything"
    echo "  $0 compiler                  # Build compiler only"
    echo "  $0 module Chtholly           # Build Chtholly module"
    echo "  $0 module path --type cjmod  # Build CJMOD module"
    echo "  $0 --skip-tests              # Build without tests"
    exit 1
}

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        all|compiler|vscode|clean)
            BUILD_TARGET="$1"
            shift
            ;;
        module)
            BUILD_TARGET="module"
            shift
            if [[ $# -gt 0 ]]; then
                MODULE_PATH="$1"
                shift
            else
                error "Module path not specified"
            fi
            ;;
        --type)
            shift
            if [[ $# -gt 0 ]]; then
                case $1 in
                    cmod|cjmod|unified)
                        MODULE_TYPE="$1"
                        ;;
                    *)
                        error "Invalid module type: $1"
                        ;;
                esac
                shift
            else
                error "Module type not specified"
            fi
            ;;
        --skip-tests)
            SKIP_TESTS=true
            shift
            ;;
        -h|--help)
            show_usage
            ;;
        *)
            error "Unknown parameter: $1"
            ;;
    esac
done

# Determine script directory
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$( cd "${SCRIPT_DIR}/.." && pwd )"

# Set platform-specific directory
case "$PLATFORM" in
    linux)
        PLATFORM_DIR="linux"
        ;;
    darwin)
        PLATFORM_DIR="macos"
        ;;
    *)
        error "Unsupported platform: $PLATFORM"
        ;;
esac

# Build functions
build_compiler() {
    info "Starting compiler build..."
    cd "${SCRIPT_DIR}/${PLATFORM_DIR}"
    if ./build-compiler.sh; then
        info "Compiler build completed"
        cd "${PROJECT_ROOT}"
        return 0
    else
        error "Compiler build failed"
    fi
}

build_modules() {
    info "Building official modules..."
    
    # Build Chtholly module
    local module_path="${PROJECT_ROOT}/src/main/java/com/chtl/module/Chtholly"
    if [[ -d "$module_path" ]]; then
        info "Building Chtholly module..."
        cd "${SCRIPT_DIR}/${PLATFORM_DIR}"
        if ./package-unified.sh "$module_path"; then
            info "Chtholly module build completed"
        else
            warning "Chtholly module build failed, continuing..."
        fi
        cd "${PROJECT_ROOT}"
    fi
    
    return 0
}

build_vscode() {
    info "Starting VSCode extension build..."
    cd "${SCRIPT_DIR}/${PLATFORM_DIR}"
    if ./build-vscode-extension.sh; then
        info "VSCode extension build completed"
        cd "${PROJECT_ROOT}"
        return 0
    else
        error "VSCode extension build failed"
    fi
}

build_module() {
    if [[ -z "$MODULE_PATH" ]]; then
        error "Module path not specified"
    fi
    
    info "Building module: $MODULE_PATH"
    info "Module type: $MODULE_TYPE"
    
    cd "${SCRIPT_DIR}/${PLATFORM_DIR}"
    
    case "$MODULE_TYPE" in
        cmod)
            if ./package-cmod.sh "$MODULE_PATH"; then
                info "Module build completed"
            else
                error "Module build failed"
            fi
            ;;
        cjmod)
            if ./package-cjmod.sh "$MODULE_PATH"; then
                info "Module build completed"
            else
                error "Module build failed"
            fi
            ;;
        unified)
            if ./package-unified.sh "$MODULE_PATH"; then
                info "Module build completed"
            else
                error "Module build failed"
            fi
            ;;
    esac
    
    cd "${PROJECT_ROOT}"
}

clean_all() {
    info "Cleaning build outputs..."
    
    cd "${PROJECT_ROOT}"
    
    # Clean compiler build
    if [[ -d "target" ]]; then
        info "Cleaning compiler build..."
        rm -rf target
    fi
    
    # Clean test outputs
    if [[ -d "test-output" ]]; then
        info "Cleaning test outputs..."
        rm -rf test-output
    fi
    
    # Clean module builds
    if [[ -d "build/modules" ]]; then
        info "Cleaning module builds..."
        rm -rf build/modules
    fi
    
    # Clean VSCode extension build
    if [[ -d "vscode-chtl/out" ]]; then
        info "Cleaning VSCode extension build..."
        rm -rf vscode-chtl/out
    fi
    
    # Remove VSCode extension packages
    rm -f vscode-chtl/*.vsix 2>/dev/null
    
    # Clean dist directory
    if [[ -d "dist" ]]; then
        info "Cleaning distribution files..."
        rm -rf dist
    fi
    
    info "Clean completed"
}

# Main execution
echo -e "${GREEN}======================================${NC}"
echo -e "${GREEN} CHTL Unified Build Script${NC}"
echo -e "${GREEN} Platform: $PLATFORM${NC}"
echo -e "${GREEN} Target: $BUILD_TARGET${NC}"
echo -e "${GREEN}======================================${NC}"
echo ""

case "$BUILD_TARGET" in
    all)
        echo -e "${BLUE}[1/3] Building CHTL compiler...${NC}"
        build_compiler
        
        echo ""
        echo -e "${BLUE}[2/3] Building official modules...${NC}"
        build_modules
        
        echo ""
        echo -e "${BLUE}[3/3] Building VSCode extension...${NC}"
        build_vscode
        
        echo ""
        echo -e "${GREEN}======================================${NC}"
        echo -e "${GREEN} Build completed successfully!${NC}"
        echo -e "${GREEN}======================================${NC}"
        ;;
    compiler)
        build_compiler
        ;;
    vscode)
        build_vscode
        ;;
    module)
        build_module
        ;;
    clean)
        clean_all
        ;;
    *)
        error "Invalid build target: $BUILD_TARGET"
        ;;
esac

# Check results for all or specific builds
if [[ "$BUILD_TARGET" == "all" || "$BUILD_TARGET" == "compiler" ]]; then
    echo ""
    echo -e "${BLUE}Compiler package:${NC}"
    if [[ -f "${PROJECT_ROOT}/dist/chtl-compiler-${PLATFORM}.tar.gz" ]]; then
        ls -lh "${PROJECT_ROOT}/dist/chtl-compiler-${PLATFORM}.tar.gz"
    else
        echo "  Not found"
    fi
fi

if [[ "$BUILD_TARGET" == "all" || "$BUILD_TARGET" == "vscode" ]]; then
    echo ""
    echo -e "${BLUE}VSCode extension:${NC}"
    if ls "${PROJECT_ROOT}"/vscode-chtl/*.vsix 1> /dev/null 2>&1; then
        ls -lh "${PROJECT_ROOT}"/vscode-chtl/*.vsix
    else
        echo "  Not found"
    fi
fi

if [[ "$BUILD_TARGET" == "module" ]]; then
    echo ""
    echo -e "${BLUE}Module package:${NC}"
    # Check in current directory and dist
    if ls *.cmod *.cjmod 2>/dev/null || ls "${PROJECT_ROOT}"/dist/*.cmod "${PROJECT_ROOT}"/dist/*.cjmod 2>/dev/null; then
        ls -lh *.cmod *.cjmod 2>/dev/null || ls -lh "${PROJECT_ROOT}"/dist/*.cmod "${PROJECT_ROOT}"/dist/*.cjmod 2>/dev/null
    else
        echo "  Not found"
    fi
fi

echo ""
info "Build process completed!"