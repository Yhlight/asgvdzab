# 核心库源文件
set(CHTL_CORE_SOURCES
    # 基础类型和接口
    common/fragment_types.cpp
    common/compiler_interface.cpp
    
    # 统一扫描器
    scanner/unified_scanner.cpp
    scanner/token.cpp
    scanner/lexer.cpp
    
    # 编译器调度器
    dispatcher/compiler_dispatcher.cpp
    
    # CHTL编译器
    compilers/chtl/chtl_compiler.cpp
    compilers/chtl/chtl_parser.cpp
    compilers/chtl/chtl_ast.cpp
    compilers/chtl/template_manager.cpp
    compilers/chtl/namespace_manager.cpp
    
    # CHTL JS编译器
    compilers/chtljs/chtljs_compiler.cpp
    compilers/chtljs/chtljs_parser.cpp
    compilers/chtljs/chtljs_ast.cpp
    compilers/chtljs/virtual_object_manager.cpp
    
    # CSS编译器 (基于ANTLR)
    compilers/css/css_compiler.cpp
    compilers/css/css_antlr_wrapper.cpp
    
    # JavaScript编译器 (基于ANTLR)
    compilers/js/js_compiler.cpp
    compilers/js/js_antlr_wrapper.cpp
    
    # 结果合并器
    merger/result_merger.cpp
    merger/html_generator.cpp
    
    # 工具类
    utils/string_utils.cpp
    utils/file_utils.cpp
    utils/error_handler.cpp
)

# 创建核心库
add_library(chtl_core STATIC ${CHTL_CORE_SOURCES})

# 设置包含目录
target_include_directories(chtl_core PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${ANTLR4_INCLUDE_DIRS}
)

# 链接ANTLR4运行时
target_link_libraries(chtl_core
    antlr4-runtime
)

# 设置编译器特定的标志
target_compile_features(chtl_core PUBLIC cxx_std_17)