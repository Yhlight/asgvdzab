# 基础组件源文件 (只包含已实现的部分)
set(CHTL_BASIC_SOURCES
    # 基础类型和接口
    Common/FragmentTypes.cpp
    Common/CompilerInterface.cpp
    Common/GlobalMap.cpp
    
    # Token系统
    Scanner/Token.cpp
)

# 创建基础组件库
add_library(ChtlBasic STATIC ${CHTL_BASIC_SOURCES})

# 设置包含目录
target_include_directories(ChtlBasic PUBLIC
    ${CMAKE_SOURCE_DIR}/Include
)

# 设置编译器特定的标志
target_compile_features(ChtlBasic PUBLIC cxx_std_17)

# 创建Token测试程序
add_executable(TokenTest
    Test/TokenTest.cpp
)

# 创建Token演示程序
add_executable(TokenDemo
    Test/TokenDemo.cpp
)

# 链接基础组件库
target_link_libraries(TokenTest ChtlBasic)
target_link_libraries(TokenDemo ChtlBasic)

target_include_directories(TokenTest PRIVATE
    ${CMAKE_SOURCE_DIR}/Include
)

target_include_directories(TokenDemo PRIVATE
    ${CMAKE_SOURCE_DIR}/Include
)

# 添加测试目标
enable_testing()
add_test(NAME TokenComponentTest COMMAND TokenTest)