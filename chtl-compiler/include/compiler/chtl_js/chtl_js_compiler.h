#ifndef CHTL_COMPILER_CHTL_JS_COMPILER_H
#define CHTL_COMPILER_CHTL_JS_COMPILER_H

#include "compiler/base_compiler.h"
#include <unordered_map>
#include <regex>

namespace chtl {
namespace compiler {

// CHTL JS编译器上下文
struct CHTLJSContext {
    // DOM选择器映射
    std::unordered_map<std::string, std::string> selectorMap;
    
    // 事件委托注册表
    struct DelegateInfo {
        std::string parentSelector;
        std::vector<std::string> targetSelectors;
        std::map<std::string, std::string> eventHandlers;
    };
    std::vector<DelegateInfo> delegates;
    
    // 唯一ID计数器
    size_t idCounter = 0;
    
    // 当前元素上下文
    std::string currentElement;
    std::string currentClass;
    std::string currentId;
};

class CHTLJSCompiler : public BaseCompiler {
public:
    CHTLJSCompiler();
    ~CHTLJSCompiler() override;
    
    // 实现基类接口
    CompilationResult compile(const CodeSegment& segment,
                            const CompilerOptions& options = CompilerOptions()) override;
    
    std::string getName() const override { return "CHTL JS Compiler"; }
    
    bool supports(CodeSegmentType type) const override {
        return type == CodeSegmentType::CHTL_JS;
    }
    
    void reset() override;

private:
    CHTLJSContext context_;
    
    // 处理增强选择器 {{selector}}
    std::string processEnhancedSelectors(const std::string& code);
    
    // 处理增强监听器 ->listen()
    std::string processEnhancedListeners(const std::string& code);
    
    // 处理事件委托 ->delegate()
    std::string processEventDelegation(const std::string& code);
    
    // 处理动画语法 animate()
    std::string processAnimations(const std::string& code);
    
    // 转换选择器为JavaScript代码
    std::string convertSelector(const std::string& selector);
    
    // 生成事件委托代码
    std::string generateDelegateCode();
    
    // 辅助方法
    std::string generateVariableName(const std::string& prefix);
    std::string escapeJsString(const std::string& str);
    
    // 正则表达式模式
    static const std::regex enhancedSelectorPattern_;
    static const std::regex listenPattern_;
    static const std::regex delegatePattern_;
    static const std::regex animatePattern_;
};

} // namespace compiler
} // namespace chtl

#endif // CHTL_COMPILER_CHTL_JS_COMPILER_H