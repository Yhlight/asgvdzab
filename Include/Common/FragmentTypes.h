#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace Chtl {

/**
 * 代码片段类型枚举
 */
enum class FragmentType {
    CHTL,           // CHTL片段
    CHTL_JS,        // CHTL JS片段  
    CSS,            // CSS片段
    JAVASCRIPT,     // JavaScript片段
    HTML,           // HTML片段
    UNKNOWN         // 未知类型
};

/**
 * 代码片段位置信息
 */
struct SourceLocation {
    size_t Line = 0;
    size_t Column = 0;
    size_t Offset = 0;
    std::string FileName;
    
    SourceLocation() = default;
    SourceLocation(size_t l, size_t c, size_t o, const std::string& f)
        : Line(l), Column(c), Offset(o), FileName(f) {}
};

/**
 * 基础代码片段类
 */
class CodeFragment {
public:
    CodeFragment(FragmentType type, const std::string& content, 
                const SourceLocation& location = SourceLocation{})
        : Type_(type), Content_(content), Location_(location) {}
    
    virtual ~CodeFragment() = default;
    
    FragmentType GetType() const { return Type_; }
    const std::string& GetContent() const { return Content_; }
    const SourceLocation& GetLocation() const { return Location_; }
    
    void SetContent(const std::string& content) { Content_ = content; }
    void SetLocation(const SourceLocation& location) { Location_ = location; }

protected:
    FragmentType Type_;
    std::string Content_;
    SourceLocation Location_;
};

/**
 * CHTL代码片段
 */
class ChtlFragment : public CodeFragment {
public:
    ChtlFragment(const std::string& content, const SourceLocation& location = SourceLocation{})
        : CodeFragment(FragmentType::CHTL, content, location) {}
        
    // CHTL特有的属性
    bool HasLocalStyle() const { return HasLocalStyle_; }
    bool HasLocalScript() const { return HasLocalScript_; }
    bool HasTemplates() const { return HasTemplates_; }
    
    void SetHasLocalStyle(bool value) { HasLocalStyle_ = value; }
    void SetHasLocalScript(bool value) { HasLocalScript_ = value; }
    void SetHasTemplates(bool value) { HasTemplates_ = value; }

private:
    bool HasLocalStyle_ = false;
    bool HasLocalScript_ = false;
    bool HasTemplates_ = false;
};

/**
 * CHTL JS代码片段
 */
class ChtlJsFragment : public CodeFragment {
public:
    ChtlJsFragment(const std::string& content, const SourceLocation& location = SourceLocation{})
        : CodeFragment(FragmentType::CHTL_JS, content, location) {}
        
    // CHTL JS特有的属性
    bool HasEnhancedSelectors() const { return HasEnhancedSelectors_; }
    bool HasVirtualObjects() const { return HasVirtualObjects_; }
    bool HasEventDelegation() const { return HasEventDelegation_; }
    bool HasAnimations() const { return HasAnimations_; }
    
    void SetHasEnhancedSelectors(bool value) { HasEnhancedSelectors_ = value; }
    void SetHasVirtualObjects(bool value) { HasVirtualObjects_ = value; }
    void SetHasEventDelegation(bool value) { HasEventDelegation_ = value; }
    void SetHasAnimations(bool value) { HasAnimations_ = value; }

private:
    bool HasEnhancedSelectors_ = false;
    bool HasVirtualObjects_ = false;
    bool HasEventDelegation_ = false;
    bool HasAnimations_ = false;
};

/**
 * CSS代码片段
 */
class CssFragment : public CodeFragment {
public:
    CssFragment(const std::string& content, const SourceLocation& location = SourceLocation{})
        : CodeFragment(FragmentType::CSS, content, location) {}
        
    // CSS特有的属性
    bool IsGlobalStyle() const { return IsGlobalStyle_; }
    void SetIsGlobalStyle(bool value) { IsGlobalStyle_ = value; }

private:
    bool IsGlobalStyle_ = true;
};

/**
 * JavaScript代码片段
 */
class JsFragment : public CodeFragment {
public:
    JsFragment(const std::string& content, const SourceLocation& location = SourceLocation{})
        : CodeFragment(FragmentType::JAVASCRIPT, content, location) {}
        
    // JavaScript特有的属性
    bool IsModule() const { return IsModule_; }
    void SetIsModule(bool value) { IsModule_ = value; }

private:
    bool IsModule_ = false;
};

/**
 * 代码片段容器
 */
class FragmentContainer {
public:
    void AddFragment(std::unique_ptr<CodeFragment> fragment);
    
    const std::vector<std::unique_ptr<CodeFragment>>& GetFragments() const { return Fragments_; }
    std::vector<std::unique_ptr<CodeFragment>>& GetFragments() { return Fragments_; }
    
    std::vector<ChtlFragment*> GetChtlFragments() const;
    std::vector<ChtlJsFragment*> GetChtlJsFragments() const;
    std::vector<CssFragment*> GetCssFragments() const;
    std::vector<JsFragment*> GetJsFragments() const;
    
    size_t GetFragmentCount() const { return Fragments_.size(); }
    bool IsEmpty() const { return Fragments_.empty(); }
    
    void Clear() { Fragments_.clear(); }

private:
    std::vector<std::unique_ptr<CodeFragment>> Fragments_;
};

/**
 * 片段类型工具函数
 */
class FragmentUtils {
public:
    static std::string FragmentTypeToString(FragmentType type);
    static FragmentType StringToFragmentType(const std::string& str);
    static bool IsScriptFragment(FragmentType type);
    static bool IsStyleFragment(FragmentType type);
};

} // namespace Chtl