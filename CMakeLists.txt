cmake_minimum_required(VERSION 3.16)
project(ChtlCompiler VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置编译器标志
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
endif()

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Lib)

# 包含目录
include_directories(${CMAKE_SOURCE_DIR}/Include)

# 查找ANTLR4运行时
find_package(PkgConfig REQUIRED)
find_package(antlr4-runtime REQUIRED)

# 添加子目录
add_subdirectory(Src)

# 创建主可执行文件
add_executable(ChtlCompiler
    Src/Main.cpp
)

# 链接库
target_link_libraries(ChtlCompiler
    ChtlCore
    antlr4-runtime
)

# 设置包含目录
target_include_directories(ChtlCompiler PRIVATE
    ${CMAKE_SOURCE_DIR}/Include
    ${ANTLR4_INCLUDE_DIRS}
)