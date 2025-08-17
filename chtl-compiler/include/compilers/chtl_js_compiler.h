#ifndef CHTL_CHTL_JS_COMPILER_H
#define CHTL_CHTL_JS_COMPILER_H

#include "compilers/base_compiler.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <regex>
#include <set>

namespace chtl {

// CHTL JS特殊语法元素
struct CHTLJSElement {
    enum Type {
        SELECTOR,       // {{选择器}}
        ARROW_ACCESS,   // ->操作符
        LISTEN,         // listen方法
        DELEGATE,       // delegate方法
        ANIMATE         // animate方法
    };
    
    Type type;
    std::string content;
    size_t start_pos;
    size_t end_pos;
};

class CHTLJSCompiler : public ICompiler {
public:
    CHTLJSCompiler();
    ~CHTLJSCompiler() override;
    
    // 实现ICompiler接口
    CompileResult compile(const CodeFragment& fragment,
                         const CompileOptions& options) override;
    
    std::string getName() const override { return "CHTL JS Compiler"; }
    
    CodeFragmentType getSupportedType() const override { 
        return CodeFragmentType::CHTL_JS; 
    }
    
    void setDebugMode(bool debug) override { debug_mode_ = debug; }

private:
    // 解析CHTL JS特殊语法
    std::vector<CHTLJSElement> parseSpecialSyntax(const std::string& source);
    
    // 转换方法
    std::string transformSelector(const std::string& selector);
    std::string transformArrowAccess(const std::string& code);
    std::string transformListen(const std::string& code);
    std::string transformDelegate(const std::string& code);
    std::string transformAnimate(const std::string& code);
    
    // 工具方法
    std::string extractSelectorContent(const std::string& selector);
    std::string generateQuerySelector(const std::string& selector);
    std::string generateDelegateFunction(const std::string& parent, 
                                       const std::string& targets,
                                       const std::string& events);
    std::string generateAnimateFunction(const std::string& config);
    
    // 辅助函数
    bool isValidSelector(const std::string& selector);
    std::string escapeJsString(const std::string& str);
    std::string trim(const std::string& str);
    
    // 事件委托管理
    struct DelegateInfo {
        std::string parent_selector;
        std::vector<std::string> target_selectors;
        std::unordered_map<std::string, std::string> event_handlers;
    };
    
    std::string generateDelegateRegistry();
    
private:
    bool debug_mode_;
    
    // 编译状态
    std::string current_output_;
    std::vector<DelegateInfo> delegate_registry_;
    int unique_id_counter_;
    
    // 正则表达式
    std::regex selector_regex_;
    std::regex arrow_regex_;
    std::regex listen_regex_;
    std::regex delegate_regex_;
    std::regex animate_regex_;
};

} // namespace chtl

#endif // CHTL_CHTL_JS_COMPILER_H