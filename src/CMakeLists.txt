# 收集所有源文件
file(GLOB_RECURSE CHTL_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/scanner/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/chtl/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/chtljs/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/css/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/javascript/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/dispatcher/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/common/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/utils/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/cmod/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/cjmod/*.cpp"
)

# 创建静态库
add_library(chtl_core STATIC ${CHTL_SOURCES})

# 创建可执行文件
add_executable(chtl main.cpp)
target_link_libraries(chtl chtl_core)

# 如果需要，链接文件系统库
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.1)
    target_link_libraries(chtl stdc++fs)
endif()