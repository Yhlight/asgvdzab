# OutputMerger库
add_library(output_merger STATIC
    output_merger.cpp
)

target_include_directories(output_merger PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(output_merger PUBLIC
    ${CMAKE_THREAD_LIBS_INIT}
)