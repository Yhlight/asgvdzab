#!/bin/bash

# CHTL Compiler Build Script for IDE
# This script compiles the project without Maven

echo "CHTL Compiler Build for IDE"
echo "==========================="
echo

# Set project root
PROJECT_ROOT="$(cd "$(dirname "$0")" && pwd)"
cd "$PROJECT_ROOT"

# Create necessary directories
echo "Creating directories..."
mkdir -p target/classes
mkdir -p lib

# Download dependencies manually if needed
echo "Checking dependencies..."
DEPS_NEEDED=false

# Check for ANTLR runtime
if [ ! -f "lib/antlr4-runtime-4.13.1.jar" ]; then
    echo "WARNING: ANTLR runtime not found. Some features may not work."
    echo "To fix: Download antlr4-runtime-4.13.1.jar from https://www.antlr.org/"
    DEPS_NEEDED=true
fi

# Check for SLF4J
if [ ! -f "lib/slf4j-api-2.0.9.jar" ]; then
    echo "WARNING: SLF4J API not found. Logging may not work."
    echo "To fix: Download slf4j-api-2.0.9.jar from https://www.slf4j.org/"
    DEPS_NEEDED=true
fi

if [ "$DEPS_NEEDED" = true ]; then
    echo
    echo "Some dependencies are missing. The compilation will continue but some features may not work."
    echo "Press Enter to continue..."
    read
fi

# Set classpath
CLASSPATH="target/classes"
for jar in lib/*.jar; do
    if [ -f "$jar" ]; then
        CLASSPATH="$CLASSPATH:$jar"
    fi
done

# Find all Java files
echo
echo "Finding Java source files..."
find src/main/java -name "*.java" > java_files.txt
TOTAL_FILES=$(wc -l < java_files.txt)
echo "Found $TOTAL_FILES Java files"

# Compile in stages to handle dependencies
echo
echo "Stage 1: Compiling model classes..."
javac -cp "$CLASSPATH" -d target/classes \
    src/main/java/com/chtl/model/*.java \
    src/main/java/com/chtl/exception/*.java \
    2>compile_errors_stage1.txt

if [ $? -ne 0 ]; then
    echo "Errors in stage 1:"
    cat compile_errors_stage1.txt
else
    echo "Stage 1 completed successfully"
fi

echo
echo "Stage 2: Compiling scanner classes..."
javac -cp "$CLASSPATH" -d target/classes \
    src/main/java/com/chtl/scanner/*.java \
    2>compile_errors_stage2.txt

if [ $? -ne 0 ]; then
    echo "Errors in stage 2:"
    head -20 compile_errors_stage2.txt
else
    echo "Stage 2 completed successfully"
fi

echo
echo "Stage 3: Compiling parser classes..."
javac -cp "$CLASSPATH" -d target/classes \
    src/main/java/com/chtl/parser/*.java \
    src/main/java/com/chtl/ast/*.java \
    2>compile_errors_stage3.txt

if [ $? -ne 0 ]; then
    echo "Errors in stage 3:"
    head -20 compile_errors_stage3.txt
else
    echo "Stage 3 completed successfully"
fi

echo
echo "Stage 4: Compiling remaining classes..."
javac -cp "$CLASSPATH" -d target/classes -encoding UTF-8 @java_files.txt 2>compile_errors_final.txt

if [ $? -eq 0 ]; then
    echo "Compilation completed successfully!"
    
    # Create JAR file
    echo
    echo "Creating JAR file..."
    cd target/classes
    jar cf ../chtl-compiler-1.0.jar com/
    cd ../..
    
    if [ -f "target/chtl-compiler-1.0.jar" ]; then
        echo "JAR created: target/chtl-compiler-1.0.jar"
        echo "Size: $(du -h target/chtl-compiler-1.0.jar | cut -f1)"
    fi
else
    echo "Compilation failed. See compile_errors_final.txt for details"
    echo "First 50 errors:"
    head -50 compile_errors_final.txt
fi

# Cleanup
rm -f java_files.txt

echo
echo "Build process completed"
echo
echo "To run in IDE:"
echo "1. Add target/classes to your IDE's classpath"
echo "2. Add any JAR files from lib/ directory to classpath"
echo "3. Main class: com.chtl.cli.CHTLCLI"