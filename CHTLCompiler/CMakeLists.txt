cmake_minimum_required(VERSION 3.16)
project(CHTLCompiler VERSION 1.0.0 LANGUAGES CXX)

# 设置C++17标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")

# 包含目录
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/third_party)

# 源文件
file(GLOB_RECURSE SOURCES "src/*.cpp")

# 确保词法分析器源文件被包含
set(LEXER_SOURCES
    src/lexer/CHTLToken.cpp
    src/lexer/GlobalMap.cpp
    src/lexer/LexerState.cpp
    src/lexer/LexerContext.cpp
    src/lexer/CHTLLexer.cpp
)

# 确保AST源文件被包含
set(AST_SOURCES
    src/ast/ASTNode.cpp
)

# 确保解析器源文件被包含
set(PARSER_SOURCES
    src/parser/CHTLParser.cpp
)

# 确保状态机源文件被包含
set(STATEMACHINE_SOURCES
    src/statemachine/CHTLStateMachine.cpp
)

# 创建可执行文件
add_executable(chtl_compiler ${SOURCES} ${LEXER_SOURCES} ${AST_SOURCES} ${PARSER_SOURCES} ${STATEMACHINE_SOURCES})

# 安装规则
install(TARGETS chtl_compiler DESTINATION bin)