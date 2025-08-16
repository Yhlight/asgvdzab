cmake_minimum_required(VERSION 3.16)
project(CHTL_Compiler VERSION 1.0.0 LANGUAGES CXX)

# 设置C++17标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置编译选项
if(MSVC)
    add_compile_options(/W4 /utf-8)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 包含目录
include_directories(${CMAKE_SOURCE_DIR}/include)

# 源文件目录
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
    "src/*.hpp"
    "include/*.hpp"
)

# 创建可执行文件
add_executable(chtl_compiler ${SOURCES})

# 创建测试可执行文件
file(GLOB TEST_SOURCES "tests/*.cpp")
# 收集测试需要的源文件（除了main.cpp）
file(GLOB_RECURSE TEST_LIB_SOURCES 
    "src/*.cpp"
)
list(REMOVE_ITEM TEST_LIB_SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")

add_executable(chtl_tests ${TEST_SOURCES} ${TEST_LIB_SOURCES})

# 创建切片扫描器测试
add_executable(slice_scanner_tests tests/slice_scanner_test.cpp ${TEST_LIB_SOURCES})

# 创建Token系统测试
add_executable(token_tests tests/token_test.cpp ${TEST_LIB_SOURCES})

# 创建综合词法分析器测试
add_executable(comprehensive_lexer_tests tests/comprehensive_lexer_test.cpp ${TEST_LIB_SOURCES})

# 创建CHTL语法验证测试
add_executable(chtl_syntax_validation_tests tests/chtl_syntax_validation_test.cpp ${TEST_LIB_SOURCES})

# 创建CHTL语法文档一致性测试
add_executable(chtl_grammar_conformance_tests tests/chtl_grammar_conformance_test.cpp ${TEST_LIB_SOURCES})

# 创建CHTL AST语法文档一致性测试
add_executable(chtl_ast_conformance_tests tests/chtl_ast_conformance_test.cpp src/ast/chtl_ast.cpp src/ast/chtl_js_ast.cpp ${TEST_LIB_SOURCES})

# 如果需要链接其他库（例如ANTLR），可以在这里添加
# find_package(antlr4-cpp-runtime REQUIRED)
# target_link_libraries(chtl_compiler antlr4-cpp-runtime)

# 设置目标属性
set_target_properties(chtl_compiler PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# 添加预处理器定义
target_compile_definitions(chtl_compiler PRIVATE
    CHTL_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    CHTL_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    CHTL_VERSION_PATCH=${PROJECT_VERSION_PATCH}
)

# 为不同的构建类型设置不同的编译选项
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(chtl_compiler PRIVATE DEBUG_MODE)
    if(NOT MSVC)
        target_compile_options(chtl_compiler PRIVATE -g -O0)
    endif()
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(NOT MSVC)
        target_compile_options(chtl_compiler PRIVATE -O3 -DNDEBUG)
    endif()
endif()

# 创建解析器和生成器集成测试
add_executable(parser_generator_integration_tests 
    tests/parser_generator_integration_test.cpp 
    src/parsers/chtl_parser.cpp
    src/generators/chtl_generator.cpp
    src/ast/chtl_ast.cpp 
    src/ast/chtl_js_ast.cpp
    ${TEST_LIB_SOURCES}
)

# 创建RAII状态管理集成测试
add_executable(raii_state_management_integration_tests 
    tests/raii_state_management_integration_test.cpp 
    src/ast/chtl_ast.cpp 
    src/ast/chtl_js_ast.cpp
    ${TEST_LIB_SOURCES}
)

# 创建增强Import和命名空间测试程序（不依赖gtest）
add_executable(enhanced_import_namespace_demo 
    tests/enhanced_import_namespace_test.cpp
    src/import/enhanced_import_manager.cpp
    src/namespace/enhanced_namespace_manager.cpp
)

# 创建快速验证程序
add_executable(quick_import_namespace_test 
    tests/quick_import_namespace_test.cpp
    src/import/enhanced_import_manager.cpp
    src/namespace/enhanced_namespace_manager.cpp
)

# 创建调试程序
add_executable(debug_namespace_test 
    tests/debug_namespace_test.cpp
    src/namespace/enhanced_namespace_manager.cpp
)

# 创建Import调试程序
add_executable(debug_import_test 
    tests/debug_import_test.cpp
    src/import/enhanced_import_manager.cpp
)

# ===== Cmod体系 =====

# 创建Cmod综合测试程序
add_executable(cmod_system_test 
    tests/cmod_system_test.cpp
    src/cmod/cmod_manager.cpp
    src/compiler/cmod_compiler_extension.cpp
)

# 创建独立的Cmod命令行工具
add_executable(chtl-cmod
    src/tools/chtl_cmod_tool.cpp
    src/cmod/cmod_manager.cpp
    src/compiler/cmod_compiler_extension.cpp
)

# CJmod system test
add_executable(cjmod_system_test
    tests/cjmod_system_test.cpp
    src/cjmod/cjmod_manager.cpp
    src/compiler/cjmod_compiler_extension.cpp
    src/cmod/cmod_manager.cpp
    src/import/enhanced_import_manager.cpp
    src/namespace/enhanced_namespace_manager.cpp
)

# CJmod standalone tool
add_executable(chtl-cjmod
    src/tools/chtl_cjmod_tool.cpp
    src/cjmod/cjmod_manager.cpp
    src/compiler/cjmod_compiler_extension.cpp
    src/cmod/cmod_manager.cpp
    src/import/enhanced_import_manager.cpp
    src/namespace/enhanced_namespace_manager.cpp
)

# CHTL Basic Parser test
add_executable(chtl_basic_parser_test
    tests/chtl_basic_parser_test.cpp
    src/parsers/chtl_basic_parser.cpp
    src/lexer/chtl_lexer.cpp
    src/ast/chtl_ast.cpp
)

# Debug parser
add_executable(debug_parser
    tests/debug_parser.cpp
    src/parsers/chtl_basic_parser.cpp
    src/lexer/chtl_lexer.cpp
    src/ast/chtl_ast.cpp
)

# 创建RAII状态管理演示程序（不依赖gtest）
add_executable(raii_state_management_demo 
    tests/raii_state_management_demo.cpp
)

# 安装规则
install(TARGETS chtl_compiler
    RUNTIME DESTINATION bin
)