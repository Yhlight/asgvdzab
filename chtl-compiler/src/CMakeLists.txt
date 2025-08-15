# 添加各个组件的子目录
add_subdirectory(scanner)
add_subdirectory(dispatcher)
add_subdirectory(compilers)
add_subdirectory(merger)

# 添加编译系统实现
add_library(chtl_compiler_system STATIC
    chtl_compiler_system.cpp
)

target_include_directories(chtl_compiler_system PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(chtl_compiler_system PUBLIC
    chtl_scanner
    compiler_dispatcher
    output_merger
    ${CMAKE_THREAD_LIBS_INIT}
)