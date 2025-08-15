#!/bin/bash

# CHTL Compiler Automated Test Runner
# 这个脚本用于运行所有测试并生成测试报告

echo "========================================"
echo "CHTL Compiler Automated Test Runner"
echo "========================================"
echo ""

# 设置颜色输出
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m' # No Color

# 设置变量
TEST_DIR="src/test/java"
BUILD_DIR="build"
REPORT_DIR="test-reports"
TIMESTAMP=$(date +"%Y%m%d_%H%M%S")
REPORT_FILE="$REPORT_DIR/test-report-$TIMESTAMP.txt"

# 创建必要的目录
mkdir -p $BUILD_DIR
mkdir -p $REPORT_DIR

# 清理旧的构建
echo "Cleaning old builds..."
rm -rf $BUILD_DIR/*

# 编译源代码
echo "Compiling source code..."
echo ""

# 查找所有Java文件
JAVA_FILES=$(find src/main/java -name "*.java" -type f)
TEST_FILES=$(find src/test/java -name "*.java" -type f 2>/dev/null || true)

# 编译主代码
echo "Compiling main sources..."
javac -d $BUILD_DIR -cp ".:src/main/java" $JAVA_FILES
if [ $? -ne 0 ]; then
    echo -e "${RED}✗ Main compilation failed${NC}"
    exit 1
fi
echo -e "${GREEN}✓ Main compilation successful${NC}"
echo ""

# 编译测试代码
if [ -n "$TEST_FILES" ]; then
    echo "Compiling test sources..."
    javac -d $BUILD_DIR -cp "$BUILD_DIR:src/test/java" $TEST_FILES
    if [ $? -ne 0 ]; then
        echo -e "${RED}✗ Test compilation failed${NC}"
        exit 1
    fi
    echo -e "${GREEN}✓ Test compilation successful${NC}"
    echo ""
fi

# 开始测试报告
echo "Running tests..." | tee $REPORT_FILE
echo "==================" | tee -a $REPORT_FILE
echo "" | tee -a $REPORT_FILE

# 运行各个测试
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# 函数：运行单个测试
run_test() {
    local test_name=$1
    local test_class=$2
    
    echo -n "Running $test_name... " | tee -a $REPORT_FILE
    
    # 运行测试并捕获输出
    TEST_OUTPUT=$(cd $BUILD_DIR && java -cp "." $test_class 2>&1)
    TEST_EXIT_CODE=$?
    
    if [ $TEST_EXIT_CODE -eq 0 ]; then
        echo -e "${GREEN}PASSED${NC}" | tee -a $REPORT_FILE
        ((PASSED_TESTS++))
    else
        echo -e "${RED}FAILED${NC}" | tee -a $REPORT_FILE
        echo "$TEST_OUTPUT" | tee -a $REPORT_FILE
        ((FAILED_TESTS++))
    fi
    ((TOTAL_TESTS++))
    
    echo "" | tee -a $REPORT_FILE
}

# 运行集成测试套件
if [ -f "$BUILD_DIR/com/chtl/test/IntegrationTestSuite.class" ]; then
    run_test "Integration Test Suite" "com.chtl.test.IntegrationTestSuite"
fi

# 运行PrecisionScanner测试
if [ -f "$BUILD_DIR/TestPrecisionScanner.class" ]; then
    run_test "Precision Scanner Tests" "TestPrecisionScanner"
fi

# 运行其他独立测试
for test_class in $(find $BUILD_DIR -name "Test*.class" -type f | grep -v "IntegrationTestSuite" | grep -v "TestPrecisionScanner"); do
    # 提取类名
    class_path=$(echo $test_class | sed "s|$BUILD_DIR/||" | sed 's|\.class$||' | tr '/' '.')
    test_name=$(basename $test_class .class)
    run_test "$test_name" "$class_path"
done

# 测试摘要
echo "" | tee -a $REPORT_FILE
echo "======================================" | tee -a $REPORT_FILE
echo "Test Summary" | tee -a $REPORT_FILE
echo "======================================" | tee -a $REPORT_FILE
echo "Total Tests: $TOTAL_TESTS" | tee -a $REPORT_FILE
echo -e "Passed: ${GREEN}$PASSED_TESTS${NC}" | tee -a $REPORT_FILE
echo -e "Failed: ${RED}$FAILED_TESTS${NC}" | tee -a $REPORT_FILE
echo "" | tee -a $REPORT_FILE

# 计算通过率
if [ $TOTAL_TESTS -gt 0 ]; then
    PASS_RATE=$(echo "scale=2; $PASSED_TESTS * 100 / $TOTAL_TESTS" | bc)
    echo "Pass Rate: $PASS_RATE%" | tee -a $REPORT_FILE
fi

echo "" | tee -a $REPORT_FILE
echo "Full report saved to: $REPORT_FILE" | tee -a $REPORT_FILE

# 设置退出代码
if [ $FAILED_TESTS -eq 0 ]; then
    echo -e "${GREEN}✓ All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}✗ Some tests failed!${NC}"
    exit 1
fi