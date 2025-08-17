#include "compiler/compiler_interface.h"
#include "compiler/chtl/chtl_compiler_v2.h"
#include "compiler/chtljs/chtljs_preprocessor.h"
#include "compiler/css/css_compiler.h"
#include "compiler/javascript/js_compiler.h"

namespace chtl {

std::unique_ptr<ICompiler> CompilerFactory::createCompiler(CodeFragmentType type) {
    switch (type) {
        case CodeFragmentType::CHTL:
            return std::make_unique<CHTLCompilerV2>();
            
        case CodeFragmentType::CHTL_JS:
            return std::make_unique<CHTLJSPreprocessor>();
            
        case CodeFragmentType::CSS:
            return std::make_unique<CSSCompiler>();
            
        case CodeFragmentType::JAVASCRIPT:
            return std::make_unique<JavaScriptCompiler>();
            
        case CodeFragmentType::HTML_RAW:
            // 原始HTML不需要编译，直接返回
            return nullptr;
            
        default:
            return nullptr;
    }
}

} // namespace chtl