#include "compilers/base_compiler.h"
#include "compilers/chtl_compiler.h"
#include "compilers/chtl_js_compiler.h"
#include <memory>

namespace chtl {

std::unique_ptr<ICompiler> CompilerFactory::createCompiler(CodeFragmentType type) {
    switch (type) {
        case CodeFragmentType::CHTL:
            return std::make_unique<CHTLCompiler>();
            
        case CodeFragmentType::CHTL_JS:
            return std::make_unique<CHTLJSCompiler>();
            
        case CodeFragmentType::CSS:
            // TODO: 实现CSS编译器（ANTLR）
            return nullptr;
            
        case CodeFragmentType::JAVASCRIPT:
            // TODO: 实现JavaScript编译器（ANTLR）
            return nullptr;
            
        case CodeFragmentType::HTML_TEXT:
            // HTML文本不需要编译，直接返回
            return nullptr;
            
        default:
            return nullptr;
    }
}

} // namespace chtl