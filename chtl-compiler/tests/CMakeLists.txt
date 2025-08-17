# 测试配置
enable_testing()

# 创建测试可执行文件
add_executable(test_scanner test_scanner.cpp)
target_link_libraries(test_scanner chtl_scanner)

add_executable(test_compiler test_compiler.cpp)
target_link_libraries(test_compiler 
    chtl_scanner
    chtl_dispatcher
    chtl_merger
    chtl_compiler_lib
    chtl_js_compiler_lib
)

# 添加测试
add_test(NAME ScannerTest COMMAND test_scanner)
add_test(NAME CompilerTest COMMAND test_compiler)