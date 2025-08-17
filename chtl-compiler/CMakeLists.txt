cmake_minimum_required(VERSION 3.10)
project(CHTLCompiler VERSION 1.0.0 LANGUAGES CXX)

# 设置C++17标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 添加包含目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# 查找ANTLR运行时库
set(ANTLR4_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../antlr-prebuilt)
find_package(antlr4-runtime REQUIRED PATHS ${ANTLR4_ROOT}/lib/cmake)

# 收集源文件
file(GLOB_RECURSE SCANNER_SOURCES "src/scanner/*.cpp")
file(GLOB_RECURSE DISPATCHER_SOURCES "src/dispatcher/*.cpp")
file(GLOB_RECURSE MERGER_SOURCES "src/merger/*.cpp")
file(GLOB_RECURSE CHTL_COMPILER_SOURCES "src/compilers/chtl/*.cpp")
file(GLOB_RECURSE CHTL_JS_COMPILER_SOURCES "src/compilers/chtl_js/*.cpp")
file(GLOB_RECURSE CSS_COMPILER_SOURCES "src/compilers/css/*.cpp")
file(GLOB_RECURSE JS_COMPILER_SOURCES "src/compilers/javascript/*.cpp")

# 创建静态库
add_library(chtl_scanner STATIC ${SCANNER_SOURCES})
add_library(chtl_dispatcher STATIC ${DISPATCHER_SOURCES})
add_library(chtl_merger STATIC ${MERGER_SOURCES})
add_library(chtl_compiler_lib STATIC ${CHTL_COMPILER_SOURCES})
add_library(chtl_js_compiler_lib STATIC ${CHTL_JS_COMPILER_SOURCES})
add_library(css_compiler_lib STATIC ${CSS_COMPILER_SOURCES})
add_library(js_compiler_lib STATIC ${JS_COMPILER_SOURCES})

# 链接ANTLR到CSS和JS编译器
target_link_libraries(css_compiler_lib PUBLIC antlr4_static)
target_link_libraries(js_compiler_lib PUBLIC antlr4_static)

# 创建主可执行文件
add_executable(chtlc src/main.cpp)

# 链接所有库到主程序
target_link_libraries(chtlc 
    chtl_scanner
    chtl_dispatcher
    chtl_merger
    chtl_compiler_lib
    chtl_js_compiler_lib
    css_compiler_lib
    js_compiler_lib
)

# 添加测试
enable_testing()
add_subdirectory(tests)

# 安装规则
install(TARGETS chtlc DESTINATION bin)
install(DIRECTORY include/ DESTINATION include/chtl)