#pragma once

#include "Types.h"
#include "Token.h"
#include <vector>
#include <map>
#include <memory>

namespace CHTL {

// AST节点类型枚举
enum class ASTNodeType {
    // 基础节点
    ROOT,               // 根节点
    ELEMENT,            // HTML元素节点
    TEXT,               // 文本节点
    ATTRIBUTE,          // 属性节点
    
    // 样式相关
    STYLE_BLOCK,        // 局部样式块
    CSS_RULE,           // CSS规则
    CSS_PROPERTY,       // CSS属性
    CSS_SELECTOR,       // CSS选择器
    
    // 脚本相关
    SCRIPT_BLOCK,       // 局部脚本块
    JS_STATEMENT,       // JS语句
    JS_EXPRESSION,      // JS表达式
    
    // 模板相关
    TEMPLATE_STYLE,     // 样式组模板
    TEMPLATE_ELEMENT,   // 元素模板
    TEMPLATE_VAR,       // 变量组模板
    
    // 自定义相关
    CUSTOM_STYLE,       // 自定义样式组
    CUSTOM_ELEMENT,     // 自定义元素
    CUSTOM_VAR,         // 自定义变量组
    
    // 原始嵌入
    ORIGIN_HTML,        // 原始HTML
    ORIGIN_CSS,         // 原始CSS
    ORIGIN_JS,          // 原始JS
    ORIGIN_CUSTOM,      // 自定义原始类型
    
    // 导入和命名空间
    IMPORT,             // 导入声明
    NAMESPACE,          // 命名空间
    CONFIGURATION,      // 配置块
    
    // 特殊操作
    INHERIT,            // 继承
    DELETE,             // 删除
    INSERT,             // 插入
    
    // CHTL JS特有
    VIR_OBJECT,         // 虚对象
    ENHANCED_SELECTOR,  // 增强选择器
    LISTEN_BLOCK,       // 监听块
    DELEGATE_BLOCK,     // 事件委托块
    ANIMATE_BLOCK       // 动画块
};

class ASTNode : public std::enable_shared_from_this<ASTNode> {
public:
    ASTNodeType type;
    std::string name;
    std::string value;
    std::map<std::string, std::string> attributes;
    std::vector<ASTNodePtr> children;
    std::weak_ptr<ASTNode> parent;
    
    // 位置信息
    size_t start_line;
    size_t end_line;
    size_t start_column;
    size_t end_column;
    
    ASTNode(ASTNodeType t, const std::string& n = "", const std::string& v = "")
        : type(t), name(n), value(v),
          start_line(0), end_line(0), start_column(0), end_column(0) {}
    
    // 添加子节点
    void addChild(ASTNodePtr child);
    
    // 添加属性
    void addAttribute(const std::string& key, const std::string& value);
    
    // 查找子节点
    ASTNodePtr findChild(ASTNodeType type, const std::string& name = "") const;
    
    // 查找所有指定类型的子节点
    std::vector<ASTNodePtr> findChildren(ASTNodeType type) const;
    
    // 获取属性值
    std::string getAttribute(const std::string& key) const;
    
    // 判断是否有属性
    bool hasAttribute(const std::string& key) const;
    
    // 设置位置信息
    void setPosition(size_t sl, size_t el, size_t sc, size_t ec);
    
    // 转换为字符串(用于调试)
    std::string toString(int indent = 0) const;
    
    // 获取节点类型的字符串表示
    std::string getTypeString() const;
};

// 特殊的AST节点类型

// 模板节点
class TemplateNode : public ASTNode {
public:
    TemplateType template_type;
    std::string template_name;
    
    TemplateNode(TemplateType tt, const std::string& name)
        : ASTNode(ASTNodeType::TEMPLATE_STYLE), template_type(tt), template_name(name) {
        // 根据模板类型设置正确的AST节点类型
        switch(tt) {
            case TemplateType::STYLE:
                type = ASTNodeType::TEMPLATE_STYLE;
                break;
            case TemplateType::ELEMENT:
                type = ASTNodeType::TEMPLATE_ELEMENT;
                break;
            case TemplateType::VAR:
                type = ASTNodeType::TEMPLATE_VAR;
                break;
        }
    }
};

// 自定义节点
class CustomNode : public ASTNode {
public:
    CustomType custom_type;
    std::string custom_name;
    
    CustomNode(CustomType ct, const std::string& name)
        : ASTNode(ASTNodeType::CUSTOM_STYLE), custom_type(ct), custom_name(name) {
        // 根据自定义类型设置正确的AST节点类型
        switch(ct) {
            case CustomType::STYLE:
                type = ASTNodeType::CUSTOM_STYLE;
                break;
            case CustomType::ELEMENT:
                type = ASTNodeType::CUSTOM_ELEMENT;
                break;
            case CustomType::VAR:
                type = ASTNodeType::CUSTOM_VAR;
                break;
        }
    }
};

// 原始嵌入节点
class OriginNode : public ASTNode {
public:
    OriginType origin_type;
    std::string origin_name;
    
    OriginNode(OriginType ot, const std::string& name = "")
        : ASTNode(ASTNodeType::ORIGIN_HTML), origin_type(ot), origin_name(name) {
        // 根据原始类型设置正确的AST节点类型
        switch(ot) {
            case OriginType::HTML:
                type = ASTNodeType::ORIGIN_HTML;
                break;
            case OriginType::STYLE:
                type = ASTNodeType::ORIGIN_CSS;
                break;
            case OriginType::JAVASCRIPT:
                type = ASTNodeType::ORIGIN_JS;
                break;
            case OriginType::CUSTOM:
                type = ASTNodeType::ORIGIN_CUSTOM;
                break;
        }
    }
};

} // namespace CHTL