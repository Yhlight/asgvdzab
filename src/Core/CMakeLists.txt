# Core模块CMakeLists.txt

# 收集源文件
file(GLOB_RECURSE CORE_SOURCES
    "*.cpp"
    "*.h"
)

# 创建Core库
add_library(chtl_core STATIC ${CORE_SOURCES})

# 设置包含目录
target_include_directories(chtl_core
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
)

# 链接库
target_link_libraries(chtl_core
    PUBLIC
        Threads::Threads
)

# 设置编译选项
target_compile_features(chtl_core PUBLIC cxx_std_17)

# 安装目标
install(TARGETS chtl_core
    ARCHIVE DESTINATION lib
)