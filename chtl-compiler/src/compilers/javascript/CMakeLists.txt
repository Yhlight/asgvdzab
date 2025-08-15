# JavaScript编译器库
add_library(javascript_compiler_lib STATIC
    javascript_compiler.cpp
)

target_include_directories(javascript_compiler_lib PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(javascript_compiler_lib PUBLIC
    ${CMAKE_THREAD_LIBS_INIT}
)