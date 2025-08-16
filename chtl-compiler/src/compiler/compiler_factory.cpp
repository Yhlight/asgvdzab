#include "compiler/base_compiler.h"
#include "compiler/chtl/chtl_compiler.h"
#include "compiler/chtl_js/chtl_js_compiler.h"
#include "compiler/css/css_compiler.h"
#include "compiler/javascript/javascript_compiler.h"
#include <unordered_map>
#include <memory>
#include <functional>

namespace chtl {
namespace compiler {

// 简单的HTML原始内容编译器
class HTMLRawCompiler : public BaseCompiler {
public:
    CompilationResult compile(const CodeSegment& segment,
                            const CompilerOptions& options = CompilerOptions()) override {
        CompilationResult result;
        result.output = segment.content;
        result.success = true;
        return result;
    }
    
    std::string getName() const override { return "HTML Raw Compiler"; }
    
    bool supports(CodeSegmentType type) const override {
        return type == CodeSegmentType::HTML_RAW;
    }
};

// 全局编译器注册表
static std::unordered_map<CodeSegmentType, std::function<std::unique_ptr<BaseCompiler>()>> compilerRegistry;

// 注册默认编译器
static void registerDefaultCompilers() {
    static bool registered = false;
    if (registered) return;
    
    // 注册CHTL编译器
    compilerRegistry[CodeSegmentType::CHTL] = []() {
        return std::make_unique<CHTLCompiler>();
    };
    
    // 注册CHTL JS编译器
    compilerRegistry[CodeSegmentType::CHTL_JS] = []() {
        return std::make_unique<CHTLJSCompiler>();
    };
    
    // 注册CSS编译器
    compilerRegistry[CodeSegmentType::CSS] = []() {
        return std::make_unique<CSSCompiler>();
    };
    
    // 注册JavaScript编译器
    compilerRegistry[CodeSegmentType::JAVASCRIPT] = []() {
        return std::make_unique<JavaScriptCompiler>();
    };
    
    // 注册HTML原始内容编译器
    compilerRegistry[CodeSegmentType::HTML_RAW] = []() {
        return std::make_unique<HTMLRawCompiler>();
    };
    
    registered = true;
}

std::unique_ptr<BaseCompiler> CompilerFactory::createCompiler(CodeSegmentType type) {
    registerDefaultCompilers();
    
    auto it = compilerRegistry.find(type);
    if (it != compilerRegistry.end()) {
        return it->second();
    }
    
    return nullptr;
}

void CompilerFactory::registerCompiler(CodeSegmentType type, 
                                     std::unique_ptr<BaseCompiler> compiler) {
    // 这个版本不太合适，因为我们需要工厂函数而不是实例
    // 保留接口但暂不实现
}

} // namespace compiler
} // namespace chtl