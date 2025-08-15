# CompilerDispatcher库
add_library(compiler_dispatcher STATIC
    compiler_dispatcher.cpp
)

target_include_directories(compiler_dispatcher PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# 依赖各个编译器库
target_link_libraries(compiler_dispatcher PUBLIC
    chtl_compiler_lib
    chtl_js_compiler_lib
    css_compiler_lib
    javascript_compiler_lib
    ${CMAKE_THREAD_LIBS_INIT}
)