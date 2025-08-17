cmake_minimum_required(VERSION 3.16)
project(CHTL_Compiler VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置编译选项
if(MSVC)
    add_compile_options(/W4 /utf-8)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 包含目录
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

# 查找必要的库
find_package(Threads REQUIRED)

# 设置源文件目录
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# 添加子目录
add_subdirectory(src/Core)
add_subdirectory(src/Scanner)
add_subdirectory(src/Compiler)
add_subdirectory(src/Utils)

# 主执行文件
add_executable(chtl_compiler
    src/main.cpp
)

# 链接库
target_link_libraries(chtl_compiler
    PRIVATE
    chtl_core
    chtl_scanner
    chtl_compiler
    chtl_utils
    Threads::Threads
)

# 安装目标
install(TARGETS chtl_compiler
    RUNTIME DESTINATION bin
)

# 创建模块目录
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/modules)