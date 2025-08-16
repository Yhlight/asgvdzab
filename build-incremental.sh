#!/bin/bash

echo "==================================="
echo "CHTL Incremental Build Script"
echo "==================================="
echo

cd "$(dirname "$0")"

# Create directories if needed
mkdir -p lib target/classes dist/lib

# Count existing classes
EXISTING_CLASSES=$(find target/classes -name "*.class" 2>/dev/null | wc -l)
echo "Existing compiled classes: $EXISTING_CLASSES"

# Apply only the new fixes
echo
echo "Running additional fixes..."
for fix in "FixMoreCompilationIssues.java" "FixSpecificImportIssues.java"; do
    if [ -f "$fix" ]; then
        echo "  Running ${fix%.java}..."
        javac "$fix" 2>/dev/null && java "${fix%.java}" >/dev/null 2>&1
    fi
done

# Compile only files that haven't been compiled yet
echo
echo "Attempting to compile uncompiled files..."

TOTAL_FILES=$(find src/main/java -name "*.java" | wc -l)
COMPILED=0
ALREADY_COMPILED=0
FAILED=0

find src/main/java -name "*.java" | while read -r file; do
    # Check if class file already exists
    CLASS_FILE=$(echo "$file" | sed 's|src/main/java/||' | sed 's|\.java$|.class|')
    
    if [ -f "target/classes/$CLASS_FILE" ]; then
        ALREADY_COMPILED=$((ALREADY_COMPILED + 1))
        printf "+"
    else
        # Try to compile
        if javac -cp "lib/*:target/classes" -d target/classes -encoding UTF-8 \
            -source 17 -target 17 -Xlint:none -nowarn -XDignore.symbol.file "$file" 2>/dev/null; then
            COMPILED=$((COMPILED + 1))
            printf "."
        else
            FAILED=$((FAILED + 1))
            printf "x"
        fi
    fi
    
    TOTAL=$((ALREADY_COMPILED + COMPILED + FAILED))
    if [ $((TOTAL % 20)) -eq 0 ]; then
        printf " (%d new/%d total)" "$COMPILED" "$TOTAL"
    fi
done

echo
echo

# Final count
FINAL_CLASSES=$(find target/classes -name "*.class" 2>/dev/null | wc -l)
echo "Results:"
echo "  Previously compiled: $EXISTING_CLASSES"
echo "  Newly compiled: $COMPILED"
echo "  Total compiled: $FINAL_CLASSES"
echo "  Failed: $FAILED"
echo "  Success rate: $(( FINAL_CLASSES * 100 / TOTAL_FILES ))%"

# Create updated JAR if we have more classes
if [ $FINAL_CLASSES -gt $EXISTING_CLASSES ]; then
    echo
    echo "Creating updated JAR..."
    cd target/classes
    jar cf ../chtl-compiler.jar $(find . -name "*.class" | sed 's/^\.\///') 2>/dev/null
    cd ../..
    
    if [ -f "target/chtl-compiler.jar" ]; then
        cp target/chtl-compiler.jar dist/
        echo "Updated JAR created with $FINAL_CLASSES classes"
    fi
fi

echo
echo "Done!"
exit 0