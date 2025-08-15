#ifndef CHTL_COMPILER_H
#define CHTL_COMPILER_H

#include "base_compiler.h"
#include <unordered_map>
#include <unordered_set>
#include <stack>

namespace chtl {

// CHTL编译器 - 处理CHTL主体语法和局部style
class CHTLCompiler : public BaseCompiler {
public:
    CHTLCompiler();
    ~CHTLCompiler() override;
    
    // 实现基类接口
    bool compile(const std::string& input, 
                std::string& output,
                std::vector<CompileError>& errors) override;
    
    void setOptions(const CompileOptions& options) override;
    std::string getName() const override { return "CHTL Compiler"; }
    std::string getVersion() const override { return "1.0.0"; }
    bool validate(const std::string& input) const override;
    
    // CHTL特有的配置
    struct CHTLConfig {
        // 索引起始计数
        int indexInitialCount = 0;
        // 是否禁用名称配置组
        bool disableNameGroup = false;
        // 调试模式
        bool debugMode = false;
        // 自动类名前缀
        std::string autoClassPrefix = "chtl-auto-";
        // 自动ID前缀
        std::string autoIdPrefix = "chtl-id-";
    };
    
    void setCHTLConfig(const CHTLConfig& config);
    
    // 获取生成的全局样式
    std::string getGeneratedGlobalStyles() const;
    
    // 获取模板定义
    struct TemplateInfo {
        std::string name;
        std::string type; // @Style, @Element, @Var
        std::string content;
        std::vector<std::string> inherits;
    };
    std::vector<TemplateInfo> getTemplates() const;
    
private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace chtl

#endif // CHTL_COMPILER_H