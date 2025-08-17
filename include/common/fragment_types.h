#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace chtl {

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
    size_t line = 0;
    size_t column = 0;
    size_t offset = 0;
    std::string filename;
    
    SourceLocation() = default;
    SourceLocation(size_t l, size_t c, size_t o, const std::string& f)
        : line(l), column(c), offset(o), filename(f) {}
};

/**
 * 基础代码片段类
 */
class CodeFragment {
public:
    CodeFragment(FragmentType type, const std::string& content, 
                const SourceLocation& location = SourceLocation{})
        : type_(type), content_(content), location_(location) {}
    
    virtual ~CodeFragment() = default;
    
    FragmentType getType() const { return type_; }
    const std::string& getContent() const { return content_; }
    const SourceLocation& getLocation() const { return location_; }
    
    void setContent(const std::string& content) { content_ = content; }
    void setLocation(const SourceLocation& location) { location_ = location; }

protected:
    FragmentType type_;
    std::string content_;
    SourceLocation location_;
};

/**
 * CHTL代码片段
 */
class CHTLFragment : public CodeFragment {
public:
    CHTLFragment(const std::string& content, const SourceLocation& location = SourceLocation{})
        : CodeFragment(FragmentType::CHTL, content, location) {}
        
    // CHTL特有的属性
    bool hasLocalStyle() const { return has_local_style_; }
    bool hasLocalScript() const { return has_local_script_; }
    bool hasTemplates() const { return has_templates_; }
    
    void setHasLocalStyle(bool value) { has_local_style_ = value; }
    void setHasLocalScript(bool value) { has_local_script_ = value; }
    void setHasTemplates(bool value) { has_templates_ = value; }

private:
    bool has_local_style_ = false;
    bool has_local_script_ = false;
    bool has_templates_ = false;
};

/**
 * CHTL JS代码片段
 */
class CHTLJSFragment : public CodeFragment {
public:
    CHTLJSFragment(const std::string& content, const SourceLocation& location = SourceLocation{})
        : CodeFragment(FragmentType::CHTL_JS, content, location) {}
        
    // CHTL JS特有的属性
    bool hasEnhancedSelectors() const { return has_enhanced_selectors_; }
    bool hasVirtualObjects() const { return has_virtual_objects_; }
    bool hasEventDelegation() const { return has_event_delegation_; }
    bool hasAnimations() const { return has_animations_; }
    
    void setHasEnhancedSelectors(bool value) { has_enhanced_selectors_ = value; }
    void setHasVirtualObjects(bool value) { has_virtual_objects_ = value; }
    void setHasEventDelegation(bool value) { has_event_delegation_ = value; }
    void setHasAnimations(bool value) { has_animations_ = value; }

private:
    bool has_enhanced_selectors_ = false;
    bool has_virtual_objects_ = false;
    bool has_event_delegation_ = false;
    bool has_animations_ = false;
};

/**
 * CSS代码片段
 */
class CSSFragment : public CodeFragment {
public:
    CSSFragment(const std::string& content, const SourceLocation& location = SourceLocation{})
        : CodeFragment(FragmentType::CSS, content, location) {}
        
    // CSS特有的属性
    bool isGlobalStyle() const { return is_global_style_; }
    void setIsGlobalStyle(bool value) { is_global_style_ = value; }

private:
    bool is_global_style_ = true;
};

/**
 * JavaScript代码片段
 */
class JSFragment : public CodeFragment {
public:
    JSFragment(const std::string& content, const SourceLocation& location = SourceLocation{})
        : CodeFragment(FragmentType::JAVASCRIPT, content, location) {}
        
    // JavaScript特有的属性
    bool isModule() const { return is_module_; }
    void setIsModule(bool value) { is_module_ = value; }

private:
    bool is_module_ = false;
};

/**
 * 代码片段容器
 */
class FragmentContainer {
public:
    void addFragment(std::unique_ptr<CodeFragment> fragment);
    
    const std::vector<std::unique_ptr<CodeFragment>>& getFragments() const { return fragments_; }
    std::vector<std::unique_ptr<CodeFragment>>& getFragments() { return fragments_; }
    
    std::vector<CHTLFragment*> getCHTLFragments() const;
    std::vector<CHTLJSFragment*> getCHTLJSFragments() const;
    std::vector<CSSFragment*> getCSSFragments() const;
    std::vector<JSFragment*> getJSFragments() const;
    
    size_t getFragmentCount() const { return fragments_.size(); }
    bool isEmpty() const { return fragments_.empty(); }
    
    void clear() { fragments_.clear(); }

private:
    std::vector<std::unique_ptr<CodeFragment>> fragments_;
};

/**
 * 片段类型工具函数
 */
class FragmentUtils {
public:
    static std::string fragmentTypeToString(FragmentType type);
    static FragmentType stringToFragmentType(const std::string& str);
    static bool isScriptFragment(FragmentType type);
    static bool isStyleFragment(FragmentType type);
};

} // namespace chtl