#pragma once

#include "Common/FragmentTypes.h"
#include "Common/GlobalMap.h"
#include "Scanner/LexerState.h"
#include <string>
#include <vector>
#include <stack>

namespace Chtl {

/**
 * 解析上下文类型
 */
enum class ContextType {
    ROOT,                     // 根上下文
    HTML_ELEMENT,            // HTML元素上下文
    TEXT_NODE,               // 文本节点上下文
    STYLE_BLOCK,             // 样式块上下文
    SCRIPT_BLOCK,            // 脚本块上下文
    TEMPLATE_DEFINITION,     // 模板定义上下文
    CUSTOM_DEFINITION,       // 自定义定义上下文
    ORIGIN_BLOCK,            // 原始嵌入上下文
    CONFIGURATION_BLOCK,     // 配置块上下文
    NAMESPACE_BLOCK,         // 命名空间上下文
    IMPORT_BLOCK,            // 导入块上下文
    ATTRIBUTE_LIST,          // 属性列表上下文
    VARIABLE_GROUP           // 变量组上下文
};

/**
 * 解析上下文信息
 */
struct ContextInfo {
    ContextType Type;
    std::string Name;                    // 上下文名称（如元素名、模板名等）
    SourceLocation Location;             // 上下文开始位置
    size_t Depth;                       // 嵌套深度
    std::string CurrentElementTag;       // 当前HTML元素标签
    std::vector<std::string> Attributes; // 当前元素的属性列表
    bool HasLocalStyle;                 // 是否包含局部样式
    bool HasLocalScript;                // 是否包含局部脚本
};

/**
 * 解析上下文管理器
 */
class ParseContextManager {
public:
    ParseContextManager();
    ~ParseContextManager() = default;
    
    // === 上下文栈管理 ===
    void PushContext(ContextType type, const std::string& name = "", 
                    const SourceLocation& location = SourceLocation{});
    void PopContext();
    bool HasContext() const { return !ContextStack_.empty(); }
    size_t GetContextDepth() const { return ContextStack_.size(); }
    
    // === 当前上下文访问 ===
    const ContextInfo& GetCurrentContext() const;
    ContextType GetCurrentContextType() const;
    std::string GetCurrentContextName() const;
    size_t GetCurrentDepth() const;
    
    // === 上下文类型检测 ===
    bool IsInRootContext() const;
    bool IsInHtmlElement() const;
    bool IsInTextNode() const;
    bool IsInStyleBlock() const;
    bool IsInScriptBlock() const;
    bool IsInTemplateDefinition() const;
    bool IsInCustomDefinition() const;
    bool IsInOriginBlock() const;
    bool IsInConfigurationBlock() const;
    bool IsInNamespaceBlock() const;
    bool IsInImportBlock() const;
    bool IsInAttributeList() const;
    bool IsInVariableGroup() const;
    
    // === 元素上下文管理 ===
    void SetCurrentElementTag(const std::string& tag);
    std::string GetCurrentElementTag() const;
    void AddAttribute(const std::string& attribute);
    const std::vector<std::string>& GetCurrentAttributes() const;
    void ClearAttributes();
    
    // === 局部块标记 ===
    void MarkHasLocalStyle() { if (!ContextStack_.empty()) ContextStack_.back().HasLocalStyle = true; }
    void MarkHasLocalScript() { if (!ContextStack_.empty()) ContextStack_.back().HasLocalScript = true; }
    bool CurrentElementHasLocalStyle() const;
    bool CurrentElementHasLocalScript() const;
    
    // === 路径和命名空间 ===
    std::string GetContextPath() const;  // 获取完整的上下文路径
    std::string GetFullyQualifiedName(const std::string& name) const;
    
    // === 作用域检查 ===
    bool IsValidInCurrentContext(TokenType tokenType) const;
    bool CanDefineTemplate() const;
    bool CanDefineCustom() const;
    bool CanUseEnhancedSelector() const;
    bool CanUseVirtualObject() const;
    
    // === 错误检查 ===
    std::vector<std::string> ValidateContextStack() const;
    bool HasUnclosedContexts() const { return !ContextStack_.empty(); }
    
    // === 工具方法 ===
    void Reset();
    std::string GetDebugInfo() const;
    std::string GetContextDescription() const;

private:
    std::vector<ContextInfo> ContextStack_;
    
    // 工具方法
    std::string ContextTypeToString(ContextType type) const;
    bool IsValidTokenInContext(TokenType tokenType, ContextType contextType) const;
    ContextInfo CreateContextInfo(ContextType type, const std::string& name, 
                                 const SourceLocation& location) const;
};

/**
 * 解析上下文作用域守护类 - RAII管理上下文
 */
class ContextScope {
public:
    ContextScope(ParseContextManager& manager, ContextType type, 
                const std::string& name = "", const SourceLocation& location = SourceLocation{});
    ~ContextScope();
    
    // 禁止拷贝和赋值
    ContextScope(const ContextScope&) = delete;
    ContextScope& operator=(const ContextScope&) = delete;
    
    // 允许移动
    ContextScope(ContextScope&& other) noexcept;
    ContextScope& operator=(ContextScope&& other) noexcept;

private:
    ParseContextManager* Manager_;
    bool ShouldPop_;
};

} // namespace Chtl