# Scanner模块CMakeLists.txt

# 收集源文件
file(GLOB_RECURSE SCANNER_SOURCES
    "*.cpp"
    "*.h"
)

# 创建Scanner库
add_library(chtl_scanner STATIC ${SCANNER_SOURCES})

# 设置包含目录
target_include_directories(chtl_scanner
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
)

# 链接依赖
target_link_libraries(chtl_scanner
    PUBLIC
        chtl_core
)

# 设置编译选项
target_compile_features(chtl_scanner PUBLIC cxx_std_17)

# 安装目标
install(TARGETS chtl_scanner
    ARCHIVE DESTINATION lib
)