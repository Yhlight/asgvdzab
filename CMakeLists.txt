cmake_minimum_required(VERSION 3.16)
project(CHTL_Compiler)

# 设置C++17标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")

# 包含目录
include_directories(include)

# 源文件
set(SOURCES
    src/main.cpp
    src/scanner/CHTLUnifiedScanner.cpp
    src/dispatcher/CompilerDispatcher.cpp
    src/compilers/CHTLCompiler.cpp
    src/compilers/CHTLJSCompiler.cpp
    src/compilers/CSSCompiler.cpp
    src/compilers/JavaScriptCompiler.cpp
    src/merger/ResultMerger.cpp
    src/common/Token.cpp
    src/common/AST.cpp
    src/common/Utils.cpp
)

# 头文件
set(HEADERS
    include/scanner/CHTLUnifiedScanner.h
    include/dispatcher/CompilerDispatcher.h
    include/compilers/CHTLCompiler.h
    include/compilers/CHTLJSCompiler.h
    include/compilers/CSSCompiler.h
    include/compilers/JavaScriptCompiler.h
    include/merger/ResultMerger.h
    include/common/Token.h
    include/common/AST.h
    include/common/Utils.h
    include/common/Types.h
)

# 创建可执行文件
add_executable(chtl_compiler ${SOURCES} ${HEADERS})

# 如果需要链接外部库(比如ANTLR)，可以在这里添加
# find_package(antlr4-runtime REQUIRED)
# target_link_libraries(chtl_compiler antlr4-runtime)