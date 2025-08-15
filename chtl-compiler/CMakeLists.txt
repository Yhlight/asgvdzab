cmake_minimum_required(VERSION 3.16)
project(CHTLCompiler VERSION 1.0.0 LANGUAGES CXX)

# 设置C++17标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置编译选项
if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 查找必要的包
find_package(Threads REQUIRED)

# ANTLR4配置
set(ANTLR4_ROOT ${CMAKE_SOURCE_DIR}/lib/antlr4)
set(ANTLR4_INCLUDE_DIRS ${ANTLR4_ROOT}/include/antlr4-runtime)
set(ANTLR4_LIB_DIR ${ANTLR4_ROOT}/lib)

# 包含目录
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${ANTLR4_INCLUDE_DIRS})

# 查找ANTLR4库
find_library(ANTLR4_RUNTIME antlr4-runtime PATHS ${ANTLR4_LIB_DIR})

# 添加子目录
add_subdirectory(src)

# 创建主可执行文件
add_executable(chtl-compiler src/main.cpp)

# 链接库
target_link_libraries(chtl-compiler
    chtl_compiler_system
    chtl_scanner
    compiler_dispatcher
    chtl_compiler_lib
    chtl_js_compiler_lib
    css_compiler_lib
    javascript_compiler_lib
    output_merger
    ${ANTLR4_RUNTIME}
    Threads::Threads
)

# 安装规则
install(TARGETS chtl-compiler DESTINATION bin)
install(DIRECTORY include/ DESTINATION include/chtl-compiler)

# 测试
enable_testing()
add_subdirectory(tests EXCLUDE_FROM_ALL)