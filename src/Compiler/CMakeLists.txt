# Compiler模块CMakeLists.txt

# 收集源文件
file(GLOB_RECURSE COMPILER_SOURCES
    "*.cpp"
    "*.h"
)

# 创建Compiler库
add_library(chtl_compiler STATIC ${COMPILER_SOURCES})

# 设置包含目录
target_include_directories(chtl_compiler
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
)

# 链接依赖
target_link_libraries(chtl_compiler
    PUBLIC
        chtl_core
        chtl_scanner
        chtl_utils
)

# 设置编译选项
target_compile_features(chtl_compiler PUBLIC cxx_std_17)

# 安装目标
install(TARGETS chtl_compiler
    ARCHIVE DESTINATION lib
)