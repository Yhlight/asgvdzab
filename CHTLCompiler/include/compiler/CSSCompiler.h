#pragma once

#include "dispatcher/CompilerDispatcher.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace chtl {

// CSS编译器类（简化实现，未来可集成ANTLR）
class CSSCompiler : public ICompiler {
public:
    CSSCompiler();
    ~CSSCompiler() override;
    
    // 实现ICompiler接口
    CompilationResult compile(const CodeFragment& fragment) override;
    std::string getName() const override { return "CSS编译器"; }
    
    // 设置编译选项
    void setOptions(const std::unordered_map<std::string, std::string>& options);
    
    // 获取编译统计信息
    struct CompileStats {
        size_t totalRules;
        size_t selectors;
        size_t properties;
        size_t mediaQueries;
        size_t keyframes;
        size_t imports;
    };
    
    CompileStats getCompileStats() const;
    
    // 获取错误信息
    std::vector<std::string> getErrors() const;
    
    // 清除错误信息
    void clearErrors();
    
    // 验证CSS语法
    bool validateCSS(const std::string& css);
    
    // 压缩CSS
    std::string minifyCSS(const std::string& css);
    
    // 格式化CSS
    std::string formatCSS(const std::string& css);

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
    
    // 私有方法
    std::string parseCSS(const std::string& source);
    std::string generateCSS(const std::string& parsed);
    void addError(const std::string& error);
    
    // CSS处理辅助方法
    std::string processSelectors(const std::string& css);
    std::string processProperties(const std::string& css);
    std::string processMediaQueries(const std::string& css);
    std::string processKeyframes(const std::string& css);
    std::string processImports(const std::string& css);
    
    // 验证辅助方法
    bool validateSelector(const std::string& selector);
    bool validateProperty(const std::string& property);
    bool validateValue(const std::string& value);
};

} // namespace chtl