cmake_minimum_required(VERSION 3.10)
project(ChtlCompiler VERSION 1.0.0 LANGUAGES CXX)

# 设置C++17标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Lib)

# 添加包含目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Include)

# 收集源文件
file(GLOB_RECURSE LEXER_SOURCES "Src/Lexer/*.cpp")
file(GLOB_RECURSE COMMON_SOURCES "Src/Common/*.cpp")
file(GLOB_RECURSE AST_SOURCES "Src/AST/*.cpp")
file(GLOB_RECURSE PARSER_SOURCES "Src/Parser/*.cpp")
file(GLOB_RECURSE GENERATOR_SOURCES "Src/Generator/*.cpp")

# 创建静态库
add_library(ChtlLexer STATIC ${LEXER_SOURCES})
add_library(ChtlCommon STATIC ${COMMON_SOURCES})
add_library(ChtlAST STATIC ${AST_SOURCES})
add_library(ChtlParser STATIC ${PARSER_SOURCES})
add_library(ChtlGenerator STATIC ${GENERATOR_SOURCES})

# 链接依赖
target_link_libraries(ChtlLexer PUBLIC ChtlCommon)
target_link_libraries(ChtlAST PUBLIC ChtlCommon)
target_link_libraries(ChtlParser PUBLIC ChtlLexer ChtlAST ChtlCommon)
target_link_libraries(ChtlGenerator PUBLIC ChtlAST ChtlParser ChtlCommon)

# 创建主可执行文件
add_executable(Chtlc Src/Main.cpp)

# 链接所有库到主程序
target_link_libraries(Chtlc 
    ChtlCommon
    ChtlLexer
    ChtlAST
    ChtlParser
    ChtlGenerator
)

# 添加测试
enable_testing()
add_subdirectory(Tests)

# 安装规则
install(TARGETS Chtlc DESTINATION bin)
install(DIRECTORY Include/ DESTINATION include/Chtl)