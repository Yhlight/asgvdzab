#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <regex>
#include <variant>

namespace chtl {

/**
 * 模板类型枚举
 */
enum class TemplateType {
    STYLE,      // @Style - 样式组模板
    ELEMENT,    // @Element - 元素模板
    VAR         // @Var - 变量组模板
};

/**
 * 变量定义
 */
struct VariableDefinition {
    std::string name;
    std::string value;
    bool hasDefaultValue = true;
};

/**
 * 样式属性定义
 */
struct StyleProperty {
    std::string property;
    std::string value;
    bool isVariable = false;        // 是否引用变量组
    std::string variableGroup;      // 变量组名
    std::string variableName;      // 变量名
};

/**
 * 元素节点定义
 */
struct ElementNode {
    std::string tagName;
    std::unordered_map<std::string, std::string> attributes;
    std::vector<std::shared_ptr<ElementNode>> children;
    std::string textContent;
    std::vector<StyleProperty> localStyles;
    bool isTextNode = false;
};

/**
 * 模板定义基类
 */
class TemplateDefinition {
public:
    TemplateDefinition(const std::string& name, TemplateType type)
        : name_(name), type_(type) {}
    virtual ~TemplateDefinition() = default;
    
    const std::string& getName() const { return name_; }
    TemplateType getType() const { return type_; }
    
    // 继承相关
    void addInheritance(const std::string& parentName) {
        inheritedTemplates_.push_back(parentName);
    }
    
    const std::vector<std::string>& getInheritedTemplates() const {
        return inheritedTemplates_;
    }
    
    virtual std::string compile(const std::unordered_map<std::string, std::string>& parameters = {}) = 0;

protected:
    std::string name_;
    TemplateType type_;
    std::vector<std::string> inheritedTemplates_;
};

/**
 * 样式组模板 [Template] @Style
 */
class StyleTemplate : public TemplateDefinition {
public:
    StyleTemplate(const std::string& name) 
        : TemplateDefinition(name, TemplateType::STYLE) {}
    
    void addProperty(const StyleProperty& property) {
        properties_.push_back(property);
    }
    
    const std::vector<StyleProperty>& getProperties() const {
        return properties_;
    }
    
    std::string compile(const std::unordered_map<std::string, std::string>& parameters = {}) override;

private:
    std::vector<StyleProperty> properties_;
};

/**
 * 元素模板 [Template] @Element
 */
class ElementTemplate : public TemplateDefinition {
public:
    ElementTemplate(const std::string& name)
        : TemplateDefinition(name, TemplateType::ELEMENT) {}
    
    void setRootElement(std::shared_ptr<ElementNode> root) {
        rootElement_ = root;
    }
    
    std::shared_ptr<ElementNode> getRootElement() const {
        return rootElement_;
    }
    
    std::string compile(const std::unordered_map<std::string, std::string>& parameters = {}) override;

private:
    std::shared_ptr<ElementNode> rootElement_;
    
    std::string compileElement(std::shared_ptr<ElementNode> element, 
                              const std::unordered_map<std::string, std::string>& parameters);
};

/**
 * 变量组模板 [Template] @Var
 */
class VariableTemplate : public TemplateDefinition {
public:
    VariableTemplate(const std::string& name)
        : TemplateDefinition(name, TemplateType::VAR) {}
    
    void addVariable(const VariableDefinition& variable) {
        variables_[variable.name] = variable;
    }
    
    const std::unordered_map<std::string, VariableDefinition>& getVariables() const {
        return variables_;
    }
    
    std::string getVariableValue(const std::string& variableName) const;
    
    std::string compile(const std::unordered_map<std::string, std::string>& parameters = {}) override;

private:
    std::unordered_map<std::string, VariableDefinition> variables_;
};

/**
 * 模板解析器
 */
class TemplateParser {
public:
    /**
     * 解析模板定义
     * @param templateCode 模板代码
     * @return 解析的模板定义
     */
    std::shared_ptr<TemplateDefinition> parseTemplate(const std::string& templateCode);
    
    /**
     * 解析模板头部
     * @param headerLine 头部行 (如: [Template] @Style ThemeName)
     * @return {templateType, templateName}
     */
    std::pair<TemplateType, std::string> parseTemplateHeader(const std::string& headerLine);
    
    /**
     * 解析样式模板内容
     */
    std::shared_ptr<StyleTemplate> parseStyleTemplate(const std::string& name, const std::string& content);
    
    /**
     * 解析元素模板内容
     */
    std::shared_ptr<ElementTemplate> parseElementTemplate(const std::string& name, const std::string& content);
    
    /**
     * 解析变量组模板内容
     */
    std::shared_ptr<VariableTemplate> parseVariableTemplate(const std::string& name, const std::string& content);

private:
    /**
     * 解析样式属性
     */
    StyleProperty parseStyleProperty(const std::string& propertyLine);
    
    /**
     * 解析元素节点
     */
    std::shared_ptr<ElementNode> parseElementNode(const std::string& elementCode);
    
    /**
     * 解析变量定义
     */
    VariableDefinition parseVariableDefinition(const std::string& variableLine);
    
    /**
     * 提取代码块内容 (处理嵌套大括号)
     */
    std::string extractBlockContent(const std::string& code, size_t startPos);
    
    /**
     * 分割代码块
     */
    std::vector<std::string> splitCodeBlocks(const std::string& code);
};

/**
 * 模板使用解析器
 */
class TemplateUsageParser {
public:
    /**
     * 解析模板使用 (如: @Style ThemeName; 或 @Element Box;)
     */
    struct TemplateUsage {
        TemplateType type;
        std::string templateName;
        std::unordered_map<std::string, std::string> parameters;
        size_t position;
        size_t length;
    };
    
    /**
     * 在代码中查找所有模板使用
     */
    std::vector<TemplateUsage> findTemplateUsages(const std::string& code);
    
    /**
     * 解析变量组使用 (如: ThemeColor(tableColor))
     */
    struct VariableUsage {
        std::string variableGroup;
        std::string variableName;
        size_t position;
        size_t length;
    };
    
    /**
     * 在代码中查找所有变量组使用
     */
    std::vector<VariableUsage> findVariableUsages(const std::string& code);

private:
    std::regex templateUsagePattern_;
    std::regex variableUsagePattern_;
    
    void initializePatterns();
};

/**
 * 模板管理器
 */
class TemplateManager {
public:
    TemplateManager();
    
    /**
     * 注册模板定义
     */
    void registerTemplate(std::shared_ptr<TemplateDefinition> templateDef);
    
    /**
     * 获取模板定义
     */
    std::shared_ptr<TemplateDefinition> getTemplate(const std::string& name, TemplateType type);
    
    /**
     * 处理模板继承
     */
    void resolveInheritance();
    
    /**
     * 编译模板使用
     */
    std::string compileTemplateUsage(const TemplateUsageParser::TemplateUsage& usage);
    
    /**
     * 编译变量组使用
     */
    std::string compileVariableUsage(const TemplateUsageParser::VariableUsage& usage);
    
    /**
     * 处理代码中的所有模板和变量使用
     */
    std::string processTemplatesInCode(const std::string& code);
    
    /**
     * 清空所有模板
     */
    void clear();
    
    /**
     * 获取所有模板统计信息
     */
    struct TemplateStatistics {
        size_t styleTemplates = 0;
        size_t elementTemplates = 0;
        size_t variableTemplates = 0;
        size_t totalUsages = 0;
    };
    
    TemplateStatistics getStatistics() const;

private:
    std::unordered_map<std::string, std::shared_ptr<TemplateDefinition>> styleTemplates_;
    std::unordered_map<std::string, std::shared_ptr<TemplateDefinition>> elementTemplates_;
    std::unordered_map<std::string, std::shared_ptr<TemplateDefinition>> variableTemplates_;
    
    std::unique_ptr<TemplateParser> parser_;
    std::unique_ptr<TemplateUsageParser> usageParser_;
    
    /**
     * 解析继承语法
     */
    void parseInheritanceInTemplate(std::shared_ptr<TemplateDefinition> templateDef, const std::string& content);
    
    /**
     * 应用继承
     */
    void applyInheritance(std::shared_ptr<TemplateDefinition> templateDef);
};

/**
 * 模板系统集成接口
 */
class TemplateSystem {
public:
    TemplateSystem();
    
    /**
     * 从源代码中扫描并注册所有模板定义
     */
    void scanAndRegisterTemplates(const std::string& sourceCode);
    
    /**
     * 处理源代码中的模板使用
     */
    std::string processSourceCode(const std::string& sourceCode);
    
    /**
     * 获取模板管理器
     */
    TemplateManager& getManager() { return *manager_; }
    
    /**
     * 重置系统
     */
    void reset();

private:
    std::unique_ptr<TemplateManager> manager_;
    
    /**
     * 扫描模板定义
     */
    std::vector<std::string> scanTemplateDefinitions(const std::string& sourceCode);
    
    /**
     * 移除已处理的模板定义
     */
    std::string removeTemplateDefinitions(const std::string& sourceCode);
};

} // namespace chtl