#pragma once

#include "compiler/compiler_interface.h"
#include <memory>
#include <string>
#include <vector>

namespace chtl {

class CSSCompiler : public ICompiler {
public:
    CSSCompiler();
    ~CSSCompiler() override;

    CompilationResult compile(const CodeFragment& fragment, 
                            const CompilerOptions& options = {}) override;
    
    std::string getName() const override { return "CSS Compiler"; }
    std::string getVersion() const override { return "1.0.0"; }
    bool supports(CodeFragmentType type) const override {
        return type == CodeFragmentType::CSS;
    }

private:
    class Impl;
    std::unique_ptr<Impl> impl_;

    std::string processCSS(const std::string& css);
    std::string handleCHTLFeatures(const std::string& css);
};

} // namespace chtl