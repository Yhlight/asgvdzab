#pragma once

#include "compiler/compiler_interface.h"
#include <memory>
#include <string>

namespace chtl {

class JavaScriptCompiler : public ICompiler {
public:
    JavaScriptCompiler();
    ~JavaScriptCompiler() override;

    CompilationResult compile(const CodeFragment& fragment, 
                            const CompilerOptions& options = {}) override;
    
    std::string getName() const override { return "JavaScript Compiler"; }
    std::string getVersion() const override { return "1.0.0"; }
    bool supports(CodeFragmentType type) const override {
        return type == CodeFragmentType::JAVASCRIPT;
    }

private:
    class Impl;
    std::unique_ptr<Impl> impl_;

    std::string processJavaScript(const std::string& js);
    std::string handleCHTLFeatures(const std::string& js);
};

} // namespace chtl