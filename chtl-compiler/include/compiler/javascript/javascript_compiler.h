#ifndef CHTL_COMPILER_JAVASCRIPT_COMPILER_H
#define CHTL_COMPILER_JAVASCRIPT_COMPILER_H

#include "compiler/base_compiler.h"

namespace chtl {
namespace compiler {

class JavaScriptCompiler : public BaseCompiler {
public:
    JavaScriptCompiler();
    ~JavaScriptCompiler() override;
    
    // 实现基类接口
    CompilationResult compile(const CodeSegment& segment,
                            const CompilerOptions& options = CompilerOptions()) override;
    
    std::string getName() const override { return "JavaScript Compiler"; }
    
    bool supports(CodeSegmentType type) const override {
        return type == CodeSegmentType::JAVASCRIPT;
    }
    
    void reset() override;

private:
    // JavaScript处理方法
    std::string processJavaScript(const std::string& js);
    std::string minifyJavaScript(const std::string& js);
    std::string validateJavaScript(const std::string& js);
};

} // namespace compiler
} // namespace chtl

#endif // CHTL_COMPILER_JAVASCRIPT_COMPILER_H