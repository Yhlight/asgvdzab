# CHTL JS编译器库
add_library(chtl_js_compiler_lib STATIC
    chtl_js_compiler.cpp
)

target_include_directories(chtl_js_compiler_lib PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(chtl_js_compiler_lib PUBLIC
    ${CMAKE_THREAD_LIBS_INIT}
)