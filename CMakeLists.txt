cmake_minimum_required(VERSION 3.10)
project(CHTL VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 配置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 设置ANTLR4预编译包路径
set(ANTLR4_ROOT "${CMAKE_SOURCE_DIR}/antlr-prebuilt")

# 包含ANTLR4配置
if(EXISTS "${ANTLR4_ROOT}/ANTLR4Config.cmake")
    include("${ANTLR4_ROOT}/ANTLR4Config.cmake")
    message(STATUS "✓ 使用ANTLR4预编译包: ${ANTLR4_ROOT}")
else()
    message(FATAL_ERROR "未找到ANTLR4预编译包配置文件: ${ANTLR4_ROOT}/ANTLR4Config.cmake")
endif()

# 设置包含目录
set(CHTL_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/generated/antlr/css
    ${CMAKE_SOURCE_DIR}/generated/antlr/js
    ${ANTLR4_INCLUDE_DIRS}
)

# 收集CHTL核心源文件
file(GLOB_RECURSE CHTL_CORE_SOURCES
    "src/ast/*.cpp"
    "src/common/*.cpp"
    "src/utils/*.cpp"
    "src/lexer/*.cpp"
    "src/generators/*.cpp"
    "src/scanner/*.cpp"
    "src/import/*.cpp"
    "src/namespace/*.cpp"
    "src/cmod/*.cpp"
    "src/cjmod/*.cpp"
    "src/network/*.cpp"
    "src/compiler/*.cpp"
    "src/core/*.cpp"
    "src/constraints/*.cpp"
    "src/compilers/*.cpp"
)

file(GLOB_RECURSE CHTL_COMPILER_SOURCES
    "src/compilers/*.cpp"
)

# 收集ANTLR生成的源文件
file(GLOB_RECURSE ANTLR_GENERATED_SRC
    "generated/antlr/css/*.cpp"
    "generated/antlr/js/*.cpp"
)

# 检查ANTLR生成的文件是否存在
if(NOT ANTLR_GENERATED_SRC)
    message(WARNING "未找到ANTLR生成的源文件，将稍后生成")
endif()

# 创建核心库
add_library(chtl_core ${CHTL_CORE_SOURCES})
target_include_directories(chtl_core PUBLIC ${CHTL_INCLUDE_DIRS})

# 创建编译器库
add_library(chtl_compilers ${CHTL_COMPILER_SOURCES} ${ANTLR_GENERATED_SRC})
target_include_directories(chtl_compilers PUBLIC 
    ${CHTL_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/generated/antlr/css
    ${CMAKE_SOURCE_DIR}/generated/antlr/js
)
target_link_libraries(chtl_compilers chtl_core ANTLR4::antlr4_runtime)

# 创建主可执行文件
add_executable(chtl src/main.cpp)
target_include_directories(chtl PRIVATE ${CHTL_INCLUDE_DIRS})
target_link_libraries(chtl chtl_core chtl_compilers)

# 生成ANTLR解析器的自定义目标
add_custom_target(generate_antlr_parsers
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/generated/antlr/css
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/generated/antlr/js
    COMMAND java -jar ${ANTLR4_JAR_LOCATION} 
        ${CMAKE_SOURCE_DIR}/grammars/CSS3.g4 
        -Dlanguage=Cpp -listener -visitor 
        -o ${CMAKE_SOURCE_DIR}/generated/antlr/css/
    COMMAND java -jar ${ANTLR4_JAR_LOCATION} 
        ${CMAKE_SOURCE_DIR}/grammars/JavaScript.g4 
        -Dlanguage=Cpp -listener -visitor 
        -o ${CMAKE_SOURCE_DIR}/generated/antlr/js/
    COMMENT "生成ANTLR CSS和JavaScript解析器"
    VERBATIM
)

# 创建测试可执行文件
if(EXISTS "${CMAKE_SOURCE_DIR}/tests")
    file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
    
    # 创建测试库
    add_library(chtl_test_lib ${CHTL_CORE_SOURCES} ${CHTL_COMPILER_SOURCES})
    target_include_directories(chtl_test_lib PUBLIC ${CHTL_INCLUDE_DIRS})
    target_link_libraries(chtl_test_lib ANTLR4::antlr4_runtime)
    
    # 现代ANTLR测试
    if(EXISTS "${CMAKE_SOURCE_DIR}/tests/modern_antlr_test.cpp")
        add_executable(modern_antlr_test tests/modern_antlr_test.cpp)
        target_link_libraries(modern_antlr_test chtl_test_lib)
        target_include_directories(modern_antlr_test PRIVATE ${CHTL_INCLUDE_DIRS})
    endif()
    
    # CSS编译器测试
    if(EXISTS "${CMAKE_SOURCE_DIR}/tests/css_compiler_test.cpp")
        add_executable(css_compiler_test tests/css_compiler_test.cpp)
        target_link_libraries(css_compiler_test chtl_test_lib)
        target_include_directories(css_compiler_test PRIVATE ${CHTL_INCLUDE_DIRS})
    endif()
    
    # JS编译器测试
    if(EXISTS "${CMAKE_SOURCE_DIR}/tests/js_compiler_test.cpp")
        add_executable(js_compiler_test tests/js_compiler_test.cpp)
        target_link_libraries(js_compiler_test chtl_test_lib)
        target_include_directories(js_compiler_test PRIVATE ${CHTL_INCLUDE_DIRS})
    endif()
    
    # 统一编译器架构测试
    if(EXISTS "${CMAKE_SOURCE_DIR}/tests/unified_compiler_test.cpp")
        add_executable(unified_compiler_test tests/unified_compiler_test.cpp)
        target_link_libraries(unified_compiler_test chtl_test_lib)
        target_include_directories(unified_compiler_test PRIVATE ${CHTL_INCLUDE_DIRS})
    endif()
    
    # 集成编译器系统测试
    if(EXISTS "${CMAKE_SOURCE_DIR}/tests/integrated_system_test.cpp")
        add_executable(integrated_system_test tests/integrated_system_test.cpp)
        target_link_libraries(integrated_system_test chtl_test_lib)
        target_include_directories(integrated_system_test PRIVATE ${CHTL_INCLUDE_DIRS})
        
        # 添加扫描器最小单元切割测试
        add_executable(scanner_minimal_unit_test tests/scanner_minimal_unit_test.cpp)
        target_link_libraries(scanner_minimal_unit_test chtl_test_lib)
        target_include_directories(scanner_minimal_unit_test PRIVATE ${CHTL_INCLUDE_DIRS})
        
        # 生成器注释系统测试
        add_executable(generator_comment_test tests/generator_comment_test.cpp)
        target_link_libraries(generator_comment_test chtl_test_lib)
        target_include_directories(generator_comment_test PRIVATE ${CHTL_INCLUDE_DIRS})

        # CHTL语法测试（如果有ANTLR）
        if(ANTLR4_FOUND)
            add_executable(chtl_syntax_test tests/chtl_syntax_test.cpp)
            target_link_libraries(chtl_syntax_test chtl_test_lib)
            target_include_directories(chtl_syntax_test PRIVATE ${CHTL_INCLUDE_DIRS})
        endif()
    endif()
endif()

# 安装配置
install(TARGETS chtl DESTINATION bin)
install(TARGETS chtl_core chtl_compilers DESTINATION lib)
install(DIRECTORY include/ DESTINATION include/chtl)

# 显示配置信息
message(STATUS "=== CHTL项目配置 ===")
message(STATUS "C++标准: ${CMAKE_CXX_STANDARD}")
message(STATUS "ANTLR4路径: ${ANTLR4_ROOT}")
message(STATUS "ANTLR4版本: ${ANTLR4_VERSION}")
message(STATUS "包含目录: ${CHTL_INCLUDE_DIRS}")
message(STATUS "构建类型: ${CMAKE_BUILD_TYPE}")
message(STATUS "输出目录: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")