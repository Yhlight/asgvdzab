#include "compiler/compiler_interface.h"
#include "compiler/chtl/chtl_compiler.h"
#include "compiler/chtljs/chtljs_compiler.h"

namespace chtl {

// 前置声明
class CSSCompiler;
class JavaScriptCompiler;

std::unique_ptr<ICompiler> CompilerFactory::createCompiler(CodeFragmentType type) {
    switch (type) {
        case CodeFragmentType::CHTL:
            return std::make_unique<CHTLCompiler>();
            
        case CodeFragmentType::CHTL_JS:
            return std::make_unique<CHTLJSCompiler>();
            
        case CodeFragmentType::CSS:
            // TODO: 实现CSSCompiler后取消注释
            // return std::make_unique<CSSCompiler>();
            return nullptr;
            
        case CodeFragmentType::JAVASCRIPT:
            // TODO: 实现JavaScriptCompiler后取消注释
            // return std::make_unique<JavaScriptCompiler>();
            return nullptr;
            
        case CodeFragmentType::HTML_RAW:
            // 原始HTML不需要编译，直接返回
            return nullptr;
            
        default:
            return nullptr;
    }
}

} // namespace chtl