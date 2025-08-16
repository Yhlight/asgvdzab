# CHTL Compiler Build Instructions

## Current Build Status

The CHTL compiler project has significant compilation issues due to circular dependencies, duplicate class definitions, and missing imports. However, we have created build scripts that can compile a subset of the classes successfully.

## Build Scripts Available

### 1. Best-Effort Build (Recommended)
- **Linux/Mac**: `./build-best-effort.sh`
- **Windows**: `build-best-effort.bat`

This script attempts to compile each Java file individually and creates a JAR with whatever compiles successfully. Currently, it can compile approximately 49 out of 224 classes.

### 2. Standard Build Scripts
- **Linux/Mac**: `./build.sh`
- **Windows**: `build-simple.bat`, `build-final.bat`, `build-universal.bat`

These scripts attempt a full compilation but currently fail due to compilation errors.

### 3. Diagnostic Script
- **Windows**: `diagnose-build.bat`

This script helps diagnose build environment issues on Windows.

## Quick Start

### On Linux/Mac:
```bash
chmod +x build-best-effort.sh
./build-best-effort.sh
```

### On Windows:
```cmd
build-best-effort.bat
```

## Build Output

Successful builds create:
- `dist/chtl-compiler.jar` - The compiled classes in a JAR file
- `dist/lib/` - Required dependencies
- `dist/chtl.sh` (Linux/Mac) or `dist/chtl.bat` (Windows) - Launcher scripts

## Known Issues

1. **Compilation Errors**: Many source files have compilation errors including:
   - Duplicate class definitions
   - Circular dependencies
   - Missing imports
   - Private access violations

2. **Partial Functionality**: Due to compilation issues, only basic functionality may be available.

3. **Fix Scripts**: Several fix scripts are included that attempt to resolve common issues:
   - `FixCompilationErrors.java`
   - `FixAllCompilationIssues.java`
   - `FixFileNaming.java`
   - `FixRemainingCompilationErrors.java`

## Dependencies

The build scripts automatically download required dependencies:
- ANTLR Runtime 4.13.1
- SLF4J API 2.0.9
- SLF4J Simple 2.0.9

## Troubleshooting

1. **No Java files found**: Ensure you're running the build script from the project root directory.

2. **Java version issues**: The project requires Java 17 or later.

3. **Download failures**: If dependency downloads fail, you may need to manually download the JAR files and place them in the `lib/` directory.

4. **Windows path issues**: The Windows scripts use improved file listing methods to avoid common path issues.

## Next Steps

To fully fix the compilation issues, manual intervention is required to:
1. Resolve circular dependencies between classes
2. Fix duplicate class definitions
3. Add missing imports
4. Refactor code with private access violations

The best-effort build provides a starting point with a partially functional compiler that can be used for basic testing and development.