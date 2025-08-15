# CHTLUnifiedScanner库
add_library(chtl_scanner STATIC
    chtl_unified_scanner.cpp
)

target_include_directories(chtl_scanner PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# 需要iostream支持
target_link_libraries(chtl_scanner PUBLIC
    ${CMAKE_THREAD_LIBS_INIT}
)