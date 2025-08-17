cmake_minimum_required(VERSION 3.16)
project(CHTL_Compiler VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置编译选项
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
endif()

# 查找ANTLR4运行时
find_package(PkgConfig REQUIRED)
find_path(ANTLR4_INCLUDE_DIRS 
    NAMES antlr4-runtime.h
    PATHS ${CMAKE_SOURCE_DIR}/antlr-prebuilt/include
)

find_library(ANTLR4_LIBRARIES
    NAMES antlr4-runtime
    PATHS ${CMAKE_SOURCE_DIR}/antlr-prebuilt/lib
)

if(NOT ANTLR4_INCLUDE_DIRS OR NOT ANTLR4_LIBRARIES)
    message(STATUS "ANTLR4 not found, will use prebuilt version")
    set(ANTLR4_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/antlr-prebuilt/include)
    set(ANTLR4_LIBRARIES ${CMAKE_SOURCE_DIR}/antlr-prebuilt/lib/libantlr4-runtime.a)
endif()

# 包含目录
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${ANTLR4_INCLUDE_DIRS})

# 添加子目录
add_subdirectory(src)

# 主执行文件
add_executable(chtl_compiler src/main.cpp)
target_link_libraries(chtl_compiler 
    chtl_unified_scanner
    chtl_compiler_dispatcher
    chtl_compiler_core
    chtl_js_compiler
    css_compiler
    js_compiler
    result_merger
    ${ANTLR4_LIBRARIES}
)

# 设置输出目录
set_target_properties(chtl_compiler PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)