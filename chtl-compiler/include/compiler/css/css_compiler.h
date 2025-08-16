#ifndef CHTL_COMPILER_CSS_COMPILER_H
#define CHTL_COMPILER_CSS_COMPILER_H

#include "compiler/base_compiler.h"

namespace chtl {
namespace compiler {

class CSSCompiler : public BaseCompiler {
public:
    CSSCompiler();
    ~CSSCompiler() override;
    
    // 实现基类接口
    CompilationResult compile(const CodeSegment& segment,
                            const CompilerOptions& options = CompilerOptions()) override;
    
    std::string getName() const override { return "CSS Compiler"; }
    
    bool supports(CodeSegmentType type) const override {
        return type == CodeSegmentType::CSS;
    }
    
    void reset() override;

private:
    // CSS处理方法
    std::string processCSS(const std::string& css);
    std::string minifyCSS(const std::string& css);
    std::string validateCSS(const std::string& css);
};

} // namespace compiler
} // namespace chtl

#endif // CHTL_COMPILER_CSS_COMPILER_H