cmake_minimum_required(VERSION 3.15)
project(CHTL-Compiler VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置编译选项
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 设置构建类型
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# 包含目录
include_directories(${CMAKE_SOURCE_DIR}/Include)

# ===== CHTL编译器模块 =====

# CHTL Common模块
set(CHTL_COMMON_SOURCES
    # CHTL/Common/Common.cpp        # 待实现
    # CHTL/Common/StringUtils.cpp   # 待实现
    # CHTL/Common/FileUtils.cpp     # 待实现
    # CHTL/Common/HtmlUtils.cpp     # 待实现
    # CHTL/Common/CssUtils.cpp      # 待实现
    # CHTL/Common/DebugUtils.cpp    # 待实现
)

# CHTL Lexer模块
set(CHTL_LEXER_SOURCES
    CHTL/Lexer/lexer.cpp
    # CHTL/Lexer/Token.cpp          # 待实现
    # CHTL/Lexer/LexerFactory.cpp   # 待实现
    # CHTL/Lexer/TokenUtils.cpp     # 待实现
)

# CHTL Parser模块
set(CHTL_PARSER_SOURCES
    CHTL/Parser/ASTNodes.cpp
    CHTL/Parser/chtl_ast.cpp
    CHTL/Parser/chtl_js_ast.cpp
    # CHTL/Parser/Parser.cpp         # 待实现
    # CHTL/Parser/ASTVisitor.cpp     # 待实现
    # CHTL/Parser/ParserFactory.cpp  # 待实现
    # CHTL/Parser/SemanticAnalyzer.cpp # 待实现
)

# CHTL Generator模块
set(CHTL_GENERATOR_SOURCES
    # CHTL/Generator/HtmlGenerator.cpp    # 待实现
    # CHTL/Generator/CssGenerator.cpp     # 待实现
    # CHTL/Generator/JsGenerator.cpp      # 待实现
    # CHTL/Generator/SourceMapGenerator.cpp # 待实现
    # CHTL/Generator/CodeOptimizer.cpp    # 待实现
)

# CHTL Loader模块
set(CHTL_LOADER_SOURCES
    # CHTL/Loader/ModuleLoader.cpp     # 待实现
    # CHTL/Loader/ImportResolver.cpp   # 待实现
    # CHTL/Loader/NamespaceManager.cpp # 待实现
    # CHTL/Loader/TemplateSystem.cpp   # 待实现
    # CHTL/Loader/ConstraintSystem.cpp # 待实现
)

# 合并所有CHTL源文件
set(CHTL_ALL_SOURCES
    ${CHTL_COMMON_SOURCES}
    ${CHTL_LEXER_SOURCES}
    ${CHTL_PARSER_SOURCES}
    ${CHTL_GENERATOR_SOURCES}
    ${CHTL_LOADER_SOURCES}
)

# 过滤掉注释掉的文件（不存在的文件）
set(CHTL_EXISTING_SOURCES)
foreach(source ${CHTL_ALL_SOURCES})
    if(EXISTS ${CMAKE_SOURCE_DIR}/${source})
        list(APPEND CHTL_EXISTING_SOURCES ${source})
    endif()
endforeach()

# 创建CHTL静态库
if(CHTL_EXISTING_SOURCES)
    add_library(chtl-core STATIC ${CHTL_EXISTING_SOURCES})
    target_include_directories(chtl-core PUBLIC ${CMAKE_SOURCE_DIR}/Include)
    target_compile_features(chtl-core PUBLIC cxx_std_17)
    
    # 输出创建的库信息
    list(LENGTH CHTL_EXISTING_SOURCES CHTL_SOURCE_COUNT)
    message(STATUS "CHTL Core Library created with ${CHTL_SOURCE_COUNT} source files")
else()
    message(WARNING "No existing CHTL source files found")
endif()

# ===== CHTL JS编译器模块 =====
# TODO: 实现CHTL JS模块

# ===== 可执行程序 =====

# 编译器主程序
if(EXISTS ${CMAKE_SOURCE_DIR}/Tools/Compiler/Main.cpp)
    add_executable(chtlc Tools/Compiler/Main.cpp)
    if(TARGET chtl-core)
        target_link_libraries(chtlc chtl-core)
    endif()
    target_include_directories(chtlc PRIVATE ${CMAKE_SOURCE_DIR}/Include)
endif()

# ===== 测试 =====

# 启用测试
enable_testing()

# 查找测试文件并创建测试可执行程序
file(GLOB_RECURSE TEST_SOURCES "Tests/*.cpp")
foreach(test_source ${TEST_SOURCES})
    get_filename_component(test_name ${test_source} NAME_WE)
    get_filename_component(test_dir ${test_source} DIRECTORY)
    get_filename_component(test_dir_name ${test_dir} NAME)
    
    set(test_executable test_${test_dir_name}_${test_name})
    add_executable(${test_executable} ${test_source})
    
    if(TARGET chtl-core)
        target_link_libraries(${test_executable} chtl-core)
    endif()
    target_include_directories(${test_executable} PRIVATE ${CMAKE_SOURCE_DIR}/Include)
    
    # 添加到测试
    add_test(NAME ${test_executable} COMMAND ${test_executable})
endforeach()

# ===== 安装规则 =====

# 安装头文件
install(DIRECTORY ${CMAKE_SOURCE_DIR}/Include/
        DESTINATION include
        FILES_MATCHING PATTERN "*.hpp")

# 安装库
if(TARGET chtl-core)
    install(TARGETS chtl-core
            ARCHIVE DESTINATION lib
            LIBRARY DESTINATION lib)
endif()

# 安装可执行文件
if(TARGET chtlc)
    install(TARGETS chtlc
            RUNTIME DESTINATION bin)
endif()

# ===== 开发信息 =====

message(STATUS "====================================")
message(STATUS "CHTL Compiler Build Configuration")
message(STATUS "====================================")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Source Directory: ${CMAKE_SOURCE_DIR}")
message(STATUS "Binary Directory: ${CMAKE_BINARY_DIR}")
message(STATUS "====================================")

# 显示找到的源文件
if(CHTL_EXISTING_SOURCES)
    message(STATUS "Found CHTL source files:")
    foreach(source ${CHTL_EXISTING_SOURCES})
        message(STATUS "  - ${source}")
    endforeach()
else()
    message(STATUS "No CHTL source files found yet")
endif()

# 显示找到的测试文件
if(TEST_SOURCES)
    message(STATUS "Found test files:")
    foreach(test ${TEST_SOURCES})
        message(STATUS "  - ${test}")
    endforeach()
else()
    message(STATUS "No test files found yet")
endif()

message(STATUS "====================================")
message(STATUS "Ready to build CHTL Compiler!")
message(STATUS "====================================")