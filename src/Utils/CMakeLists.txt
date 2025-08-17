# Utils模块CMakeLists.txt

# 收集源文件
file(GLOB_RECURSE UTILS_SOURCES
    "*.cpp"
    "*.h"
)

# 创建Utils库
add_library(chtl_utils STATIC ${UTILS_SOURCES})

# 设置包含目录
target_include_directories(chtl_utils
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
)

# 链接依赖
target_link_libraries(chtl_utils
    PUBLIC
        chtl_core
)

# 设置编译选项
target_compile_features(chtl_utils PUBLIC cxx_std_17)

# 安装目标
install(TARGETS chtl_utils
    ARCHIVE DESTINATION lib
)