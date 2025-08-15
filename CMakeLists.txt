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

# 安装规则
install(TARGETS chtl_compiler
    RUNTIME DESTINATION bin
)