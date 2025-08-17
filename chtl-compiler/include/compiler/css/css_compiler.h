#ifndef CHTL_CSS_COMPILER_H
#define CHTL_CSS_COMPILER_H

#include <string>
#include <memory>
#include "compiler/compiler_interface.h"

namespace chtl {

// CSS编译器 - 简单的CSS处理器
class CSSCompiler : public ICompiler {
public:
    CSSCompiler();
    ~CSSCompiler() override;
    
    // 实现ICompiler接口
    CompilationResult compile(const CodeFragment& fragment, 
                            const CompilerOptions& options = {}) override;
    
    std::string getName() const override { return "CSS Compiler"; }
    std::string getVersion() const override { return "1.0.0"; }
    bool supports(CodeFragmentType type) const override {
        return type == CodeFragmentType::CSS;
    }
    
private:
    // CSS访问器 - 用于遍历和处理CSS AST
    class CSSVisitor;
    
    // 处理CSS并返回优化后的结果
    std::string processCSS(const std::string& input, const CompilerOptions& options);
    
    // CSS优化选项
    struct OptimizationOptions {
        bool minify = false;
        bool merge_selectors = false;
        bool remove_unused = false;
        bool autoprefixer = false;
    };
    
    // 执行CSS优化
    std::string optimizeCSS(const std::string& css, const OptimizationOptions& opts);
};

} // namespace chtl

#endif // CHTL_CSS_COMPILER_H