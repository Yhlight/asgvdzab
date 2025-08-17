#ifndef CHTLJS_PREPROCESSOR_H
#define CHTLJS_PREPROCESSOR_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "compiler/compiler_interface.h"
#include "compiler/chtljs/chtljs_tokens.h"

namespace chtl {

// CHTL JS预处理器 - 只处理CHTL JS特有语法，将其转换为标准JavaScript
class CHTLJSPreprocessor : public ICompiler {
public:
    CHTLJSPreprocessor();
    ~CHTLJSPreprocessor() override;
    
    // 实现ICompiler接口
    CompilationResult compile(const CodeFragment& fragment, 
                            const CompilerOptions& options = {}) override;
    
    std::string getName() const override { return "CHTL JS Preprocessor"; }
    std::string getVersion() const override { return "1.0.0"; }
    bool supports(CodeFragmentType type) const override {
        return type == CodeFragmentType::CHTL_JS;
    }
    
private:
    // 预处理上下文
    struct PreprocessContext {
        std::string input;
        std::string output;
        size_t position = 0;
        size_t line = 1;
        size_t column = 1;
        
        // 虚对象管理
        std::unordered_map<std::string, std::vector<std::string>> vir_objects;
        
        // 生成的辅助函数
        std::vector<std::string> helper_functions;
        
        // 错误和警告
        std::vector<std::string> errors;
        std::vector<std::string> warnings;
    };
    
    // 预处理主函数
    std::string preprocess(const std::string& input, PreprocessContext& ctx);
    
    // 处理特定的CHTL JS语法
    void processSelector(PreprocessContext& ctx);
    void processArrowOperator(PreprocessContext& ctx);
    void processVirDeclaration(PreprocessContext& ctx);
    void processListenFunction(PreprocessContext& ctx);
    void processDelegateFunction(PreprocessContext& ctx);
    void processAnimateFunction(PreprocessContext& ctx);
    
    // 辅助函数
    bool lookAhead(const PreprocessContext& ctx, const std::string& text) const;
    void advance(PreprocessContext& ctx, size_t count = 1);
    void skipWhitespace(PreprocessContext& ctx);
    std::string readUntil(PreprocessContext& ctx, char delimiter);
    std::string readUntilAny(PreprocessContext& ctx, const std::string& delimiters);
    std::string readIdentifier(PreprocessContext& ctx);
    std::string readSelector(PreprocessContext& ctx);
    
    // 生成JavaScript代码
    std::string generateSelectorCode(const std::string& selector);
    std::string generateListenHelper();
    std::string generateDelegateHelper();
    std::string generateAnimateHelper();
    
    // 虚对象相关
    std::string generateVirFunction(const std::string& virName, 
                                   const std::string& functionName);
    
    // 错误处理
    void reportError(PreprocessContext& ctx, const std::string& message);
    void reportWarning(PreprocessContext& ctx, const std::string& message);
};

} // namespace chtl

#endif // CHTLJS_PREPROCESSOR_H