#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <stack>
#include "lexer/LexerState.h"

namespace chtl {

// 上下文类型枚举
enum class ContextType {
    GLOBAL,                 // 全局上下文
    TEMPLATE,               // 模板上下文
    CUSTOM,                 // 自定义上下文
    NAMESPACE,              // 命名空间上下文
    ELEMENT,                // 元素上下文
    STYLE_BLOCK,            // 样式块上下文
    SCRIPT_BLOCK,           // 脚本块上下文
    ATTRIBUTE,              // 属性上下文
    CSS_RULE,               // CSS规则上下文
    FUNCTION,               // 函数上下文
    ANIMATION,              // 动画上下文
    IMPORT,                 // 导入上下文
    ORIGIN,                 // 原始嵌入上下文
    CONFIGURATION           // 配置组上下文
};

// 上下文信息结构
struct ContextInfo {
    ContextType type;
    std::string name;
    std::string parent_context;
    size_t start_line;
    size_t start_column;
    size_t end_line;
    size_t end_column;
    std::unordered_map<std::string, std::string> metadata;
    
    ContextInfo(ContextType t, const std::string& n, const std::string& parent = "",
                size_t sl = 0, size_t sc = 0, size_t el = 0, size_t ec = 0)
        : type(t), name(n), parent_context(parent), 
          start_line(sl), start_column(sc), end_line(el), end_column(ec) {}
};

// 词法分析器上下文管理器
class LexerContext {
public:
    LexerContext();
    ~LexerContext();
    
    // 上下文管理
    void pushContext(ContextType type, const std::string& name = "", 
                     size_t line = 0, size_t column = 0);
    bool popContext();
    ContextInfo* getCurrentContext() const;
    ContextInfo* getContext(const std::string& name) const;
    
    // 上下文查询
    std::vector<ContextInfo*> getContextStack() const;
    size_t getContextDepth() const;
    bool isInContext(ContextType type) const;
    bool isInContext(const std::string& name) const;
    
    // 上下文类型查询
    ContextType getCurrentContextType() const;
    std::string getCurrentContextName() const;
    std::string getCurrentContextPath() const;
    
    // 上下文层次结构
    std::vector<std::string> getContextHierarchy() const;
    std::string getContextPath(const std::string& context_name) const;
    bool isChildOf(const std::string& child, const std::string& parent) const;
    
    // 上下文元数据
    void setContextMetadata(const std::string& key, const std::string& value);
    std::string getContextMetadata(const std::string& key) const;
    bool hasContextMetadata(const std::string& key) const;
    void removeContextMetadata(const std::string& key);
    
    // 上下文验证
    bool isValidContext(ContextType type) const;
    bool canNestContext(ContextType parent, ContextType child) const;
    bool isContextComplete(const std::string& context_name) const;
    
    // 上下文完成
    void completeContext(const std::string& context_name, size_t end_line, size_t end_column);
    void completeCurrentContext(size_t end_line, size_t end_column);
    
    // 上下文查找
    ContextInfo* findContextByName(const std::string& name) const;
    std::vector<ContextInfo*> findContextsByType(ContextType type) const;
    ContextInfo* findParentContext(ContextType type) const;
    
    // 上下文清理
    void clearContexts();
    void clearContext(const std::string& context_name);
    void clearIncompleteContexts();
    
    // 上下文统计
    size_t getContextCount() const;
    size_t getContextCountByType(ContextType type) const;
    size_t getIncompleteContextCount() const;
    
    // 错误处理
    void setContextError(const std::string& context_name, const std::string& error_message);
    std::string getContextError(const std::string& context_name) const;
    bool hasContextError(const std::string& context_name) const;
    void clearContextError(const std::string& context_name);
    
    // 调试和诊断
    std::string getDebugInfo() const;
    std::string getContextStackInfo() const;
    std::vector<std::string> getContextErrors() const;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
    
    // 内部方法
    bool isValidContextName(const std::string& name) const;
    std::string generateContextPath(const std::string& name) const;
    void updateContextHierarchy();
    bool canCreateContext(ContextType type, const std::string& parent_context) const;
};

} // namespace chtl