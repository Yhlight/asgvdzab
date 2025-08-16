#!/bin/bash

echo "==================================="
echo "CHTL Best-Effort Build Script"
echo "==================================="
echo

cd "$(dirname "$0")"

# Create directories
mkdir -p lib target/classes dist/lib

# Check dependencies
echo "Checking dependencies..."
if [ ! -f "lib/antlr4-runtime-4.13.1.jar" ] || [ ! -f "lib/slf4j-api-2.0.9.jar" ] || [ ! -f "lib/slf4j-simple-2.0.9.jar" ]; then
    echo "Downloading missing dependencies..."
    
    [ ! -f "lib/antlr4-runtime-4.13.1.jar" ] && \
        curl -sL -o lib/antlr4-runtime-4.13.1.jar https://repo1.maven.org/maven2/org/antlr/antlr4-runtime/4.13.1/antlr4-runtime-4.13.1.jar
    
    [ ! -f "lib/slf4j-api-2.0.9.jar" ] && \
        curl -sL -o lib/slf4j-api-2.0.9.jar https://repo1.maven.org/maven2/org/slf4j/slf4j-api/2.0.9/slf4j-api-2.0.9.jar
    
    [ ! -f "lib/slf4j-simple-2.0.9.jar" ] && \
        curl -sL -o lib/slf4j-simple-2.0.9.jar https://repo1.maven.org/maven2/org/slf4j/slf4j-simple/2.0.9/slf4j-simple-2.0.9.jar
fi

# Clean
echo
echo "Cleaning previous build..."
rm -rf target/classes/* 2>/dev/null

# Compile each file individually
echo
echo "Compiling source files (best effort)..."
TOTAL_FILES=0
COMPILED_FILES=0
FAILED_FILES=0

# Create a temporary directory for compilation
mkdir -p target/temp

# Find and compile each Java file
find src/main/java -name "*.java" | while read -r file; do
    TOTAL_FILES=$((TOTAL_FILES + 1))
    
    # Try to compile the file
    if javac -cp "lib/*:target/classes" -d target/classes -encoding UTF-8 \
        -source 17 -target 17 -Xlint:none -nowarn "$file" 2>/dev/null; then
        COMPILED_FILES=$((COMPILED_FILES + 1))
        echo -n "."
    else
        FAILED_FILES=$((FAILED_FILES + 1))
        echo -n "x"
    fi
    
    # Print progress every 10 files
    if [ $((TOTAL_FILES % 10)) -eq 0 ]; then
        echo -n " ($TOTAL_FILES)"
    fi
done

echo
echo

# Count actual compiled classes
CLASS_COUNT=$(find target/classes -name "*.class" 2>/dev/null | wc -l)

echo "Compilation Summary:"
echo "  Total source files: $(find src/main/java -name "*.java" | wc -l)"
echo "  Classes compiled: $CLASS_COUNT"

if [ $CLASS_COUNT -eq 0 ]; then
    echo
    echo "[ERROR] No classes compiled successfully!"
    echo
    echo "Trying minimal compilation of essential classes..."
    
    # Try to compile just the essential classes
    ESSENTIAL_FILES=(
        "src/main/java/com/chtl/model/CHTLNode.java"
        "src/main/java/com/chtl/model/CompilationResult.java"
        "src/main/java/com/chtl/model/SourceLocation.java"
        "src/main/java/com/chtl/exception/CHTLException.java"
        "src/main/java/com/chtl/cli/CHTLCLI.java"
    )
    
    for file in "${ESSENTIAL_FILES[@]}"; do
        if [ -f "$file" ]; then
            echo "  Compiling: $(basename "$file")"
            javac -cp "lib/*:target/classes" -d target/classes -encoding UTF-8 \
                -source 17 -target 17 -Xlint:none -nowarn -XDignore.symbol.file "$file" 2>/dev/null || true
        fi
    done
    
    CLASS_COUNT=$(find target/classes -name "*.class" 2>/dev/null | wc -l)
    echo "  Essential classes compiled: $CLASS_COUNT"
fi

if [ $CLASS_COUNT -gt 0 ]; then
    # Create JAR with whatever compiled
    echo
    echo "Creating JAR file..."
    cd target/classes
    jar cf ../chtl-compiler.jar $(find . -name "*.class" | sed 's/^\.\///') 2>/dev/null
    cd ../..
    
    if [ -f "target/chtl-compiler.jar" ]; then
        # Create distribution
        echo "Creating distribution..."
        cp target/chtl-compiler.jar dist/
        cp lib/*.jar dist/lib/ 2>/dev/null
        
        # Create launcher scripts
        cat > dist/chtl.sh << 'EOF'
#!/bin/bash
java -cp "$(dirname "$0")/chtl-compiler.jar:$(dirname "$0")/lib/*" com.chtl.cli.CHTLCLI "$@" 2>/dev/null || \
echo "Warning: Some features may not be available due to compilation issues."
EOF
        chmod +x dist/chtl.sh
        
        cat > dist/chtl.bat << 'EOF'
@echo off
java -cp "%~dp0chtl-compiler.jar;%~dp0lib\*" com.chtl.cli.CHTLCLI %* 2>nul || echo Warning: Some features may not be available.
EOF
        
        echo
        echo "==================================="
        echo "BUILD PARTIALLY SUCCESSFUL!"
        echo "==================================="
        echo
        echo "Output: dist/chtl-compiler.jar"
        echo "Classes compiled: $CLASS_COUNT (partial build)"
        echo
        echo "Note: This is a partial build. Some features may not work."
        echo
        echo "To test what's available:"
        echo "  cd dist"
        echo "  ./chtl.sh --help"
    else
        echo "[ERROR] Failed to create JAR!"
        exit 1
    fi
else
    echo
    echo "==================================="
    echo "BUILD FAILED"
    echo "==================================="
    echo
    echo "No classes could be compiled. The source code has too many errors."
    echo "Manual intervention is required to fix the compilation issues."
    exit 1
fi

echo
exit 0