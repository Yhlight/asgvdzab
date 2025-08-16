#pragma once

#include "ast/chtl_ast.hpp"
#include "lexer/chtl_lexer.hpp"
#include "common/types.hpp"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace chtl {

/**
 * 模板类型枚举
 */
enum class TemplateType {
    STYLE_TEMPLATE,     // @Style 样式组模板
    ELEMENT_TEMPLATE,   // @Element 元素模板
    VAR_TEMPLATE        // @Var 变量组模板
};

/**
 * 模板继承类型
 */
enum class InheritanceType {
    COMPOSITION,        // 组合式继承 (@Style TemplateName;)
    EXPLICIT           // 显性继承 (inherit @Style TemplateName;)
};

/**
 * 模板变量定义
 */
struct TemplateVariable {
    std::string name;           // 变量名
    std::string value;          // 变量值
    Position position;          // 位置信息
    
    TemplateVariable(const std::string& n, const std::string& v, const Position& pos = Position{})
        : name(n), value(v), position(pos) {}
};

/**
 * 模板使用引用
 */
struct TemplateReference {
    TemplateType type;          // 模板类型
    std::string templateName;   // 模板名称
    std::vector<std::string> arguments; // 参数列表 (用于变量模板)
    Position position;          // 位置信息
    InheritanceType inheritanceType; // 继承类型
    
    TemplateReference(TemplateType t, const std::string& name, const Position& pos = Position{})
        : type(t), templateName(name), position(pos), inheritanceType(InheritanceType::COMPOSITION) {}
};

/**
 * 模板定义
 */
struct TemplateDefinition {
    TemplateType type;                          // 模板类型
    std::string name;                           // 模板名称
    CHTLASTNodePtr content;                     // 模板内容
    std::vector<TemplateVariable> variables;    // 变量列表 (用于变量模板)
    std::vector<TemplateReference> inheritance; // 继承列表
    Position position;                          // 位置信息
    
    TemplateDefinition() : type(TemplateType::STYLE_TEMPLATE), name(""), position(Position{}) {}
    
    TemplateDefinition(TemplateType t, const std::string& n, const Position& pos = Position{})
        : type(t), name(n), position(pos) {}
};

/**
 * 模板注册表
 * 管理所有已定义的模板
 */
class TemplateRegistry {
public:
    TemplateRegistry();
    ~TemplateRegistry();

    /**
     * 注册模板定义
     */
    bool registerTemplate(const TemplateDefinition& templateDef);

    /**
     * 查找模板定义
     */
    const TemplateDefinition* findTemplate(TemplateType type, const std::string& name) const;

    /**
     * 获取所有模板
     */
    const std::unordered_map<std::string, TemplateDefinition>& getAllTemplates() const;

    /**
     * 清空注册表
     */
    void clear();

    /**
     * 验证模板引用
     */
    bool validateTemplateReference(const TemplateReference& ref) const;

private:
    std::unordered_map<std::string, TemplateDefinition> templates_;
    
    // 生成模板键名
    std::string generateTemplateKey(TemplateType type, const std::string& name) const;
};

/**
 * 模板解析状态
 */
enum class TemplateParseState {
    INITIAL,                    // 初始状态
    READING_TEMPLATE_HEADER,    // 读取模板头 ([Template] @Style)
    READING_TEMPLATE_NAME,      // 读取模板名称
    READING_TEMPLATE_CONTENT,   // 读取模板内容
    READING_INHERITANCE,        // 读取继承信息
    READING_VARIABLE_DEF,       // 读取变量定义
    READING_USAGE              // 读取模板使用
};

/**
 * CHTL模板系统解析器
 * 严格按照CHTL语法文档实现模板解析
 */
class CHTLTemplateParser {
public:
    CHTLTemplateParser();
    ~CHTLTemplateParser();

    /**
     * 解析模板声明 [Template] @Type Name { ... }
     * @param source 源代码
     * @param filename 文件名
     * @return 解析结果
     */
    ParseResult parseTemplateDeclaration(const std::string& source, const std::string& filename = "");

    /**
     * 解析模板使用 @Style TemplateName;
     * @param usage 模板使用代码
     * @param context 当前上下文
     * @return 模板引用
     */
    TemplateReference parseTemplateUsage(const std::string& usage, const std::string& context = "");

    /**
     * 解析变量模板使用 ThemeColor(variableName)
     * @param expression 变量表达式
     * @return 模板引用
     */
    TemplateReference parseVariableTemplateUsage(const std::string& expression);

    /**
     * 展开模板引用
     * @param ref 模板引用
     * @return 展开后的AST节点
     */
    CHTLASTNodePtr expandTemplateReference(const TemplateReference& ref);

    /**
     * 设置模板注册表
     */
    void setTemplateRegistry(std::shared_ptr<TemplateRegistry> registry);

    /**
     * 获取模板注册表
     */
    std::shared_ptr<TemplateRegistry> getTemplateRegistry() const;

    /**
     * 设置严格模式
     */
    void setStrictMode(bool strict);

    /**
     * 获取解析错误
     */
    const std::vector<ParseError>& getErrors() const;

    /**
     * 获取解析警告
     */
    const std::vector<ParseWarning>& getWarnings() const;

private:
    std::shared_ptr<TemplateRegistry> registry_;
    CHTLLexer lexer_;
    TemplateParseState currentState_;
    
    bool strictMode_;
    std::vector<ParseError> errors_;
    std::vector<ParseWarning> warnings_;

    // 核心解析方法
    TemplateDefinition parseTemplateDefinition(const std::vector<Token>& tokens);
    TemplateType parseTemplateType(const std::string& typeStr);
    std::string parseTemplateName(const std::vector<Token>& tokens, size_t& index);
    CHTLASTNodePtr parseTemplateContent(const std::vector<Token>& tokens, size_t& index, TemplateType type);
    CHTLASTNodePtr parseVariableTemplateContent(const std::string& content);
    std::vector<TemplateVariable> parseVariableDefinitions(const std::vector<Token>& tokens, size_t& index);
    std::vector<TemplateReference> parseInheritanceList(const std::vector<Token>& tokens, size_t& index);

    // 模板展开方法
    CHTLASTNodePtr expandStyleTemplate(const TemplateDefinition& templateDef, const TemplateReference& ref);
    CHTLASTNodePtr expandElementTemplate(const TemplateDefinition& templateDef, const TemplateReference& ref);
    CHTLASTNodePtr expandVariableTemplate(const TemplateDefinition& templateDef, const TemplateReference& ref);

    // 继承处理方法
    void applyInheritance(TemplateDefinition& templateDef);
    void mergeStyleTemplate(TemplateDefinition& target, const TemplateDefinition& source);
    void mergeElementTemplate(TemplateDefinition& target, const TemplateDefinition& source);
    void mergeVariableTemplate(TemplateDefinition& target, const TemplateDefinition& source);

    // 工具方法
    void reportError(const std::string& message, const Position& position = Position{});
    void reportWarning(const std::string& message, const Position& position = Position{});
    bool isValidTemplateName(const std::string& name) const;
    bool isValidVariableName(const std::string& name) const;
    std::string resolveVariableValue(const std::string& varName, const std::vector<TemplateVariable>& variables) const;
};

/**
 * 模板工具类
 * 提供模板相关的工具方法
 */
class TemplateUtils {
public:
    /**
     * 检查是否为模板声明
     */
    static bool isTemplateDeclaration(const std::string& source);

    /**
     * 检查是否为模板使用
     */
    static bool isTemplateUsage(const std::string& source);

    /**
     * 检查是否为变量模板使用
     */
    static bool isVariableTemplateUsage(const std::string& source);

    /**
     * 提取模板类型字符串
     */
    static std::string extractTemplateTypeString(const std::string& source);

    /**
     * 提取模板名称
     */
    static std::string extractTemplateName(const std::string& source);

    /**
     * 验证模板语法
     */
    static bool validateTemplateSyntax(const std::string& source);
};

} // namespace chtl