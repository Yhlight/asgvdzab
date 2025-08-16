# 测试配置
add_executable(test_scanner test_scanner.cpp)
target_link_libraries(test_scanner chtl_scanner)
add_test(NAME scanner_test COMMAND test_scanner)

add_executable(test_chtl_compiler test_chtl_compiler.cpp)
target_link_libraries(test_chtl_compiler 
    chtl_compiler_core
    chtl_scanner  # 添加scanner依赖
    ${CMAKE_CURRENT_BINARY_DIR}/../libchtl_compiler_core.a  # 确保链接编译器核心库
)
add_test(NAME chtl_compiler_test COMMAND test_chtl_compiler)