# 核心库源文件
set(CHTL_CORE_SOURCES
    # 基础类型和接口
    Common/FragmentTypes.cpp
    Common/CompilerInterface.cpp
    
    # 统一扫描器
    Scanner/UnifiedScanner.cpp
    Scanner/Token.cpp
    Scanner/Lexer.cpp
    
    # 编译器调度器
    Dispatcher/CompilerDispatcher.cpp
    
    # CHTL编译器
    Compilers/Chtl/ChtlCompiler.cpp
    Compilers/Chtl/ChtlParser.cpp
    Compilers/Chtl/ChtlAst.cpp
    Compilers/Chtl/TemplateManager.cpp
    Compilers/Chtl/NamespaceManager.cpp
    
    # CHTL JS编译器
    Compilers/ChtlJs/ChtlJsCompiler.cpp
    Compilers/ChtlJs/ChtlJsParser.cpp
    Compilers/ChtlJs/ChtlJsAst.cpp
    Compilers/ChtlJs/VirtualObjectManager.cpp
    
    # CSS编译器 (基于ANTLR)
    Compilers/Css/CssCompiler.cpp
    Compilers/Css/CssAntlrWrapper.cpp
    
    # JavaScript编译器 (基于ANTLR)
    Compilers/Js/JsCompiler.cpp
    Compilers/Js/JsAntlrWrapper.cpp
    
    # 结果合并器
    Merger/ResultMerger.cpp
    Merger/HtmlGenerator.cpp
    
    # 工具类
    Utils/StringUtils.cpp
    Utils/FileUtils.cpp
    Utils/ErrorHandler.cpp
)

# 创建核心库
add_library(ChtlCore STATIC ${CHTL_CORE_SOURCES})

# 设置包含目录
target_include_directories(ChtlCore PUBLIC
    ${CMAKE_SOURCE_DIR}/Include
    ${ANTLR4_INCLUDE_DIRS}
)

# 链接ANTLR4运行时
target_link_libraries(ChtlCore
    antlr4-runtime
)

# 设置编译器特定的标志
target_compile_features(ChtlCore PUBLIC cxx_std_17)