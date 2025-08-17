cmake_minimum_required(VERSION 3.16)
project(CHTLCompiler VERSION 1.0.0 LANGUAGES CXX)

# 设置C++17标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ANTLR4运行时路径
set(ANTLR4_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/antlr4-runtime")
set(ANTLR4_INCLUDE_DIRS "${ANTLR4_ROOT}/runtime/src")
set(ANTLR4_LIB_DIR "${ANTLR4_ROOT}/build")

# 设置编译选项
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g -O0)
    else()
        add_compile_options(-O3)
    endif()
elseif(MSVC)
    add_compile_options(/W4)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(/Od)
    else()
        add_compile_options(/O2)
    endif()
endif()

# 添加include目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# ANTLR4运行时已在上面定义
if(EXISTS ${ANTLR4_ROOT})
    include_directories(${ANTLR4_INCLUDE_DIRS})
    link_directories(${ANTLR4_LIB_DIR})
endif()

# 收集源文件
file(GLOB_RECURSE SCANNER_SOURCES "src/scanner/*.cpp")
file(GLOB_RECURSE DISPATCHER_SOURCES "src/dispatcher/*.cpp")
file(GLOB_RECURSE CHTL_COMPILER_SOURCES "src/compiler/chtl/*.cpp")
file(GLOB_RECURSE CHTLJS_COMPILER_SOURCES "src/compiler/chtljs/*.cpp")
file(GLOB_RECURSE CSS_COMPILER_SOURCES "src/compiler/css/*.cpp")
file(GLOB_RECURSE JS_COMPILER_SOURCES "src/compiler/javascript/*.cpp")
file(GLOB_RECURSE MERGER_SOURCES "src/merger/*.cpp")

# 编译器工厂
set(COMPILER_FACTORY_SOURCES "src/compiler/compiler_factory.cpp")

# 创建静态库
add_library(chtl_scanner STATIC ${SCANNER_SOURCES})
add_library(chtl_dispatcher STATIC ${DISPATCHER_SOURCES})
add_library(chtl_compiler STATIC ${CHTL_COMPILER_SOURCES})
add_library(chtljs_compiler STATIC ${CHTLJS_COMPILER_SOURCES})
add_library(css_compiler STATIC ${CSS_COMPILER_SOURCES})
add_library(js_compiler STATIC ${JS_COMPILER_SOURCES})
add_library(chtl_merger STATIC ${MERGER_SOURCES})

# 主执行文件
add_executable(chtlc src/main.cpp ${COMPILER_FACTORY_SOURCES})

# 链接库
target_link_libraries(chtlc 
    chtl_scanner
    chtl_dispatcher
    chtl_compiler
    chtljs_compiler
    css_compiler
    js_compiler
    chtl_merger
)

# 如果ANTLR4存在，链接ANTLR4运行时（目前暂不使用）
# if(EXISTS ${ANTLR4_ROOT})
#     target_link_libraries(css_compiler ${ANTLR4_LIB_DIR}/libantlr4-runtime.a)
#     target_link_libraries(js_compiler ${ANTLR4_LIB_DIR}/libantlr4-runtime.a)
#     target_link_libraries(chtlc ${ANTLR4_LIB_DIR}/libantlr4-runtime.a)
# endif()

# 测试
enable_testing()
add_subdirectory(tests)

# 安装规则
install(TARGETS chtlc DESTINATION bin)
install(DIRECTORY include/ DESTINATION include/chtl-compiler)