# CHTL Compiler Build Instructions

## Current Build Status

The CHTL compiler project has significant compilation issues due to circular dependencies, duplicate class definitions, and missing imports. However, through a series of automated fixes and improved build scripts, we can now successfully compile 58 out of 224 classes (26% of the codebase).

## Build Scripts Available

### 1. Ultimate Build Scripts (Recommended)
- **Linux/Mac**: `./build-ultimate.sh`
- **Windows**: `build-ultimate.bat`

These are the most comprehensive build scripts with all fixes applied. They successfully compile approximately 58 out of 224 classes.

### 2. Windows Fixed Build
- **Windows**: `build-windows-fixed.bat`

This is a reliable build script for Windows with improved JAR creation and error handling.

### 3. Best-Effort Build
- **Linux/Mac**: `./build-best-effort.sh`
- **Windows**: `build-best-effort.bat`

These scripts attempt to compile each Java file individually and create a JAR with whatever compiles successfully.

### 4. Minimal Build
- **Windows**: `build-minimal.bat`

A simplified build script that focuses on essential classes and uses alternative JAR creation methods.

### 5. Standard Build Scripts
- **Linux/Mac**: `./build.sh`
- **Windows**: `build-simple.bat`, `build-final.bat`, `build-universal.bat`

These scripts attempt a full compilation but currently fail due to compilation errors.

### 6. Diagnostic Scripts
- **Windows**: `diagnose-build.bat` - Diagnoses build environment issues
- **Windows**: `check-compiled-classes.bat` - Checks what classes were compiled

These scripts help diagnose build issues and verify compilation results.

## Quick Start

### On Linux/Mac:
```bash
chmod +x build-ultimate.sh
./build-ultimate.sh
```

### On Windows:
```cmd
build-ultimate.bat
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
   - `FixCompilationErrors.java` - Fixes basic compilation errors
   - `FixAllCompilationIssues.java` - Comprehensive fixes for duplicate classes and imports
   - `FixFileNaming.java` - Fixes file naming issues
   - `FixRemainingCompilationErrors.java` - Fixes remaining symbol and import issues
   - `FixAdvancedCompilationIssues.java` - Fixes advanced issues including missing imports and annotations
   - `FixANTLRAndParserIssues.java` - Fixes ANTLR-related parser and lexer issues
   - `FixCHTLJSContextFinal.java` - Specific fixes for CHTLJSContext.java

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