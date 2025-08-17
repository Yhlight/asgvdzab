# 测试配置
enable_testing()

# 创建测试可执行文件
add_executable(TestLexer TestLexer.cpp)
target_link_libraries(TestLexer ChtlLexer)

# 添加测试
add_test(NAME LexerTest COMMAND TestLexer)