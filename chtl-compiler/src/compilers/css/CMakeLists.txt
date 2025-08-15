# CSS编译器库
add_library(css_compiler_lib STATIC
    css_compiler.cpp
)

target_include_directories(css_compiler_lib PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(css_compiler_lib PUBLIC
    ${CMAKE_THREAD_LIBS_INIT}
)