#ifndef CHTL_JS_COMPILER_H
#define CHTL_JS_COMPILER_H

#include "base_compiler.h"
#include <unordered_map>
#include <unordered_set>

namespace chtl {

// CHTL JS编译器 - 处理CHTL增强的JavaScript语法
class CHTLJSCompiler : public BaseCompiler {
public:
    CHTLJSCompiler();
    ~CHTLJSCompiler() override;
    
    // 实现基类接口
    bool compile(const std::string& input, 
                std::string& output,
                std::vector<CompileError>& errors) override;
    
    void setOptions(const CompileOptions& options) override;
    std::string getName() const override { return "CHTL JS Compiler"; }
    std::string getVersion() const override { return "1.0.0"; }
    bool validate(const std::string& input) const override;
    
    // CHTL JS特有功能
    struct CHTLJSConfig {
        // 是否启用增强选择器
        bool enableEnhancedSelector = true;
        // 是否启用增强监听器
        bool enableEnhancedListener = true;
        // 是否启用事件委托
        bool enableEventDelegation = true;
        // 是否启用动画API
        bool enableAnimationAPI = true;
        // 选择器缓存大小
        size_t selectorCacheSize = 100;
        // 是否使用->语法
        bool useArrowSyntax = true;
    };
    
    void setCHTLJSConfig(const CHTLJSConfig& config);
    
    // 注册的事件委托信息
    struct EventDelegation {
        std::string parentSelector;
        std::string targetSelector;
        std::string eventType;
        std::string handlerName;
    };
    std::vector<EventDelegation> getEventDelegations() const;
    
    // 获取生成的辅助函数
    std::string getHelperFunctions() const;
    
private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace chtl

#endif // CHTL_JS_COMPILER_H