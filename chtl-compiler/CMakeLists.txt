cmake_minimum_required(VERSION 3.16)
project(CHTLCompiler VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置编译选项
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# 编译器选项
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

# 包含目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# 查找必要的包
find_package(Threads REQUIRED)

# ANTLR支持（稍后配置）
option(USE_ANTLR "Use ANTLR for CSS and JavaScript parsers" ON)
if(USE_ANTLR)
    # ANTLR配置将在后续添加
    add_definitions(-DUSE_ANTLR)
endif()

# 收集源文件
file(GLOB_RECURSE SCANNER_SOURCES src/scanner/*.cpp)
file(GLOB_RECURSE DISPATCHER_SOURCES src/dispatcher/*.cpp)
# CHTL编译器源文件
set(CHTL_COMPILER_SOURCES
    src/compiler/chtl/chtl_compiler.cpp
    src/compiler/chtl/chtl_lexer.cpp
    src/compiler/chtl/chtl_parser.cpp
    src/compiler/chtl/chtl_ast.cpp
    src/compiler/chtl/chtl_global_map.cpp
)
file(GLOB_RECURSE CHTL_JS_COMPILER_SOURCES src/compiler/chtl_js/*.cpp)
file(GLOB_RECURSE CSS_COMPILER_SOURCES src/compiler/css/*.cpp)
file(GLOB_RECURSE JS_COMPILER_SOURCES src/compiler/javascript/*.cpp)
file(GLOB_RECURSE MERGER_SOURCES src/merger/*.cpp)

# 添加基础编译器源文件
set(BASE_COMPILER_SOURCES 
    src/compiler/base_compiler.cpp
    src/compiler/compiler_factory.cpp
)

# 创建静态库
add_library(chtl_scanner STATIC ${SCANNER_SOURCES})
add_library(chtl_dispatcher STATIC ${DISPATCHER_SOURCES})
add_library(chtl_compiler_core STATIC ${CHTL_COMPILER_SOURCES} ${BASE_COMPILER_SOURCES})
add_library(chtl_js_compiler_core STATIC ${CHTL_JS_COMPILER_SOURCES})
add_library(css_compiler_core STATIC ${CSS_COMPILER_SOURCES})
add_library(js_compiler_core STATIC ${JS_COMPILER_SOURCES})
add_library(chtl_merger STATIC ${MERGER_SOURCES})

# 主可执行文件
add_executable(chtlc src/main.cpp)

# 链接库
target_link_libraries(chtlc
    chtl_scanner
    chtl_dispatcher
    chtl_compiler_core
    chtl_js_compiler_core
    css_compiler_core
    js_compiler_core
    chtl_merger
    Threads::Threads
)

# 安装规则
install(TARGETS chtlc DESTINATION bin)
install(DIRECTORY include/ DESTINATION include/chtl)

# 测试
enable_testing()
add_subdirectory(tests)