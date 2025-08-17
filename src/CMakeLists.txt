# CHTLUnifiedScanner - 精准代码切割器
add_library(chtl_unified_scanner STATIC
    scanner/CHTLUnifiedScanner.cpp
    scanner/CHTLUnifiedScanner.h
    scanner/CodeFragment.h
)

# CompilerDispatcher - 编译器调度器
add_library(chtl_compiler_dispatcher STATIC
    dispatcher/CompilerDispatcher.cpp
    dispatcher/CompilerDispatcher.h
)

# CHTL Compiler - CHTL编译器 (手写)
add_library(chtl_compiler_core STATIC
    compilers/chtl/CHTLCompiler.cpp
    compilers/chtl/CHTLCompiler.h
    compilers/chtl/CHTLLexer.cpp
    compilers/chtl/CHTLLexer.h
    compilers/chtl/CHTLASTNodes.h
)

# CHTL JS Compiler - CHTL JS编译器 (手写)
add_library(chtl_js_compiler STATIC
    compilers/chtljs/CHTLJSCompiler.cpp
    compilers/chtljs/CHTLJSCompiler.h
)

# CSS Compiler - CSS编译器 (基于ANTLR)
add_library(css_compiler STATIC
    compilers/css/CSSCompiler.cpp
    compilers/css/CSSCompiler.h
    # ANTLR生成的文件将在这里添加
)

# JavaScript Compiler - JavaScript编译器 (基于ANTLR)
add_library(js_compiler STATIC
    compilers/js/JSCompiler.cpp
    compilers/js/JSCompiler.h
    # ANTLR生成的文件将在这里添加
)

# Result Merger - 编译结果合并器
add_library(result_merger STATIC
    merger/ResultMerger.cpp
    merger/ResultMerger.h
    merger/HTMLOutput.cpp
    merger/HTMLOutput.h
)

# 链接依赖
target_link_libraries(chtl_compiler_dispatcher
    chtl_unified_scanner
    chtl_compiler_core
    chtl_js_compiler
    css_compiler
    js_compiler
    result_merger
)