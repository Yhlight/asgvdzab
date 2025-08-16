cmake_minimum_required(VERSION 3.10)
project(antlr4-runtime)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 收集所有源文件
file(GLOB_RECURSE ANTLR4_SRC
    "*.cpp"
)

# 排除某些可能有问题的文件（如果有的话）
list(FILTER ANTLR4_SRC EXCLUDE REGEX ".*test.*")
list(FILTER ANTLR4_SRC EXCLUDE REGEX ".*Test.*")

# 创建静态库
add_library(antlr4-runtime-static STATIC ${ANTLR4_SRC})

# 创建动态库
add_library(antlr4-runtime-shared SHARED ${ANTLR4_SRC})

# 设置包含目录
target_include_directories(antlr4-runtime-static PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/atn
    ${CMAKE_CURRENT_SOURCE_DIR}/dfa
    ${CMAKE_CURRENT_SOURCE_DIR}/misc
    ${CMAKE_CURRENT_SOURCE_DIR}/support
    ${CMAKE_CURRENT_SOURCE_DIR}/tree
    ${CMAKE_CURRENT_SOURCE_DIR}/tree/pattern
    ${CMAKE_CURRENT_SOURCE_DIR}/tree/xpath
)

target_include_directories(antlr4-runtime-shared PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/atn
    ${CMAKE_CURRENT_SOURCE_DIR}/dfa
    ${CMAKE_CURRENT_SOURCE_DIR}/misc
    ${CMAKE_CURRENT_SOURCE_DIR}/support
    ${CMAKE_CURRENT_SOURCE_DIR}/tree
    ${CMAKE_CURRENT_SOURCE_DIR}/tree/pattern
    ${CMAKE_CURRENT_SOURCE_DIR}/tree/xpath
)

# 查找并链接UUID库（Linux需要）
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(UUID uuid)
    if(UUID_FOUND)
        target_link_libraries(antlr4-runtime-static ${UUID_LIBRARIES})
        target_link_libraries(antlr4-runtime-shared ${UUID_LIBRARIES})
        target_include_directories(antlr4-runtime-static PRIVATE ${UUID_INCLUDE_DIRS})
        target_include_directories(antlr4-runtime-shared PRIVATE ${UUID_INCLUDE_DIRS})
    endif()
endif()

# 设置输出名称
set_target_properties(antlr4-runtime-static PROPERTIES
    OUTPUT_NAME "antlr4-runtime"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

set_target_properties(antlr4-runtime-shared PROPERTIES
    OUTPUT_NAME "antlr4-runtime"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# 安装规则
install(TARGETS antlr4-runtime-static antlr4-runtime-shared
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

# 安装头文件
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION include/antlr4-runtime
    FILES_MATCHING PATTERN "*.h"
)