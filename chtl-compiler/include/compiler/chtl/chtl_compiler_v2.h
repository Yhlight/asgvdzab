#ifndef CHTL_COMPILER_V2_H
#define CHTL_COMPILER_V2_H

#include <string>
#include <memory>
#include "compiler/compiler_interface.h"
#include "compiler/chtl/chtl_state.h"

namespace chtl {

// 新版CHTL编译器
class CHTLCompilerV2 : public ICompiler {
public:
    CHTLCompilerV2();
    ~CHTLCompilerV2() override = default;
    
    // 实现ICompiler接口
    CompilationResult compile(const CodeFragment& fragment,
                            const CompilerOptions& options = {}) override;
    
    std::string getName() const override { return "CHTL Compiler V2"; }
    std::string getVersion() const override { return "2.0.0"; }
    bool supports(CodeFragmentType type) const override {
        return type == CodeFragmentType::CHTL;
    }
    
private:
    std::unique_ptr<GlobalMap> global_map_;
};

} // namespace chtl

#endif // CHTL_COMPILER_V2_H