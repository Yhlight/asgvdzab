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

# 源文件 - 暂时只包含基础文件，专注于第一部分实现
set(SOURCES
    src/main.cpp
    src/compiler/CHTLCompiler.cpp
    src/compiler/CHTLJSCompiler.cpp
    src/compiler/CSSCompiler.cpp
    src/compiler/JavaScriptCompiler.cpp
    src/dispatcher/CompilerDispatcher.cpp
    src/scanner/CHTLUnifiedScanner.cpp
)

# 确保词法分析器源文件被包含
set(LEXER_SOURCES
    src/lexer/CHTLToken.cpp
    src/lexer/GlobalMap.cpp
)

# 确保AST源文件被包含
set(AST_SOURCES
    src/ast/ASTNode.cpp
    src/ast/CommentNode.cpp
    src/ast/TextNode.cpp
    src/ast/LiteralNode.cpp
    src/ast/HTMLElementNode.cpp
    src/ast/AttributeNode.cpp
    src/ast/DocumentNode.cpp
)

# 确保解析器源文件被包含
set(PARSER_SOURCES
    # src/parser/CHTLParser.cpp  # 暂时注释掉，专注于基础语法解析器
)

# 确保状态机源文件被包含
set(STATEMACHINE_SOURCES
    # src/statemachine/CHTLStateMachine.cpp  # 暂时注释掉，专注于基础语法解析器
)

# 确保Import和Namespace源文件被包含
set(IMPORT_NAMESPACE_SOURCES
    # src/import/ImportManager.cpp  # 暂时注释掉，专注于基础语法解析器
)

# 确保模块源文件被包含
set(MODULE_SOURCES
    # src/module/ModuleManager.cpp  # 暂时注释掉，专注于基础语法解析器
)

# 确保基础语法解析器源文件被包含
set(BASIC_SYNTAX_SOURCES
    src/parser/BasicSyntaxParser.cpp
    src/parser/AttributeSystem.cpp
    src/parser/StyleBlockParser.cpp
    src/parser/TemplateSystem.cpp
    src/parser/CustomSystem.cpp
    src/parser/OriginSystem.cpp
    src/parser/ConfigurationSystem.cpp
)

# 创建可执行文件
add_executable(chtl_compiler ${SOURCES} ${LEXER_SOURCES} ${AST_SOURCES} ${PARSER_SOURCES} ${STATEMACHINE_SOURCES} ${IMPORT_NAMESPACE_SOURCES} ${MODULE_SOURCES} ${BASIC_SYNTAX_SOURCES})

# 安装规则
install(TARGETS chtl_compiler DESTINATION bin)