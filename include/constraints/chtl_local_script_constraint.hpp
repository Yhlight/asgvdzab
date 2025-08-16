#pragma once

#include "common/types.hpp"
#include <string>
#include <vector>
#include <set>
#include <unordered_set>

namespace chtl {

/**
 * CHTL 局部script块允许的语法元素类型
 */
enum class LocalScriptAllowedElement {
    TEMPLATE_VARIABLE,              // 模板变量: VariableGroupName(variableName) 或 @Var VariableGroupName(variableName)
    CUSTOM_VARIABLE,                // 自定义变量: VariableGroupName(variableName) 或 @Var VariableGroupName(variableName)
    VARIABLE_SPECIALIZATION,        // 变量组特例化: VariableGroupName(variableName = value)
    NAMESPACE_FROM,                 // 命名空间from: ElementName from namespace.path
    GENERATOR_COMMENT,              // 生成器注释: --注释内容
    RAW_EMBEDDING,                  // 原始嵌入: [Origin] @Type [name] { 内容 } (支持基本类型和自定义类型)
    PURE_JAVASCRIPT,                // 纯JavaScript代码（包括{{&}}等模板语法）
    JAVASCRIPT_TEMPLATE_SYNTAX      // JavaScript模板语法: {{variable}}, {{{html}}}, {{&unescaped}} 等
};

/**
 * CHTL 局部script块禁止的语法元素类型
 */
enum class LocalScriptForbiddenElement {
    TEMPLATE_STYLE_GROUP,           // 模板样式组: @Style StyleGroupName (script中不应使用样式)
    CUSTOM_STYLE_GROUP,             // 自定义样式组: @Style StyleGroupName
    TEMPLATE_ELEMENT,               // 模板元素: @Element ElementName (script中不应使用元素定义)
    CUSTOM_ELEMENT,                 // 自定义元素: @Element ElementName
    STYLE_BLOCKS,                   // 样式块: style { ... } (script中不应定义样式)
    TEXT_BLOCKS,                    // 文本块: text { ... } (script中不应定义文本)
    INHERITANCE,                    // 继承: inherit @Style StyleGroupName (script中不应使用样式继承)
    DELETE_OPERATIONS,              // Delete操作: delete properties, delete @Style (script中不应使用样式操作)
    FULL_QUALIFIED_NAME             // 全缀名: [Custom] @Var/@Style/@Element Name (除变量外的全缀名)
};

/**
 * CHTL 局部script约束违规类型
 */
enum class LocalScriptConstraintViolationType {
    FORBIDDEN_CHTL_SYNTAX,          // 禁止的CHTL语法（样式、元素相关）
    INVALID_VARIABLE_REFERENCE,     // 无效的变量引用
    MALFORMED_FROM_CLAUSE,          // 格式错误的from子句
    MALFORMED_GENERATOR_COMMENT,    // 格式错误的生成器注释
    INVALID_RAW_EMBEDDING,          // 无效的原始嵌入
    UNKNOWN_SYNTAX_ELEMENT          // 未知的语法元素
};

/**
 * CHTL 局部script约束违规信息
 */
struct LocalScriptConstraintViolation {
    LocalScriptConstraintViolationType type;
    LocalScriptForbiddenElement forbiddenElement;
    std::string message;
    std::string element;
    size_t line = 0;
    size_t column = 0;
};

/**
 * CHTL 局部script约束验证结果
 */
struct LocalScriptConstraintResult {
    bool isValid = true;
    std::vector<LocalScriptConstraintViolation> violations;
    std::set<std::string> templateVariables;
    std::set<std::string> customVariables;
    std::set<std::string> foundGeneratorComments;
    std::set<std::string> foundRawEmbeddings;
    std::set<std::string> fromClauses;
    std::set<std::string> jsTemplateSyntax;
    size_t pureJavaScriptLines = 0;
};

/**
 * CHTL 局部script块约束器
 * 
 * 验证CHTL局部script块(script { ... })中的语法约束：
 * 
 * ✅ 允许使用的CHTL语法：
 * - 模板变量: VariableGroupName(variableName), @Var VariableGroupName(variableName)
 * - 自定义变量组: 同模板变量格式
 * - 变量组特例化: VariableGroupName(variableName = value)
 * - 命名空间from: ElementName from namespace.path
 * - 生成器注释: --注释内容
 * - 原始嵌入: [Origin] @Type [name] { 内容 } (支持基本类型和自定义类型)
 * - 纯JavaScript代码: 标准JavaScript语法
 * - JavaScript模板语法: {{variable}}, {{{html}}}, {{&unescaped}} 等
 * 
 * ❌ 禁止使用的CHTL语法：
 * - 样式相关: @Style, style块, inherit, delete样式操作
 * - 元素相关: @Element, text块
 * - 样式全缀名: [Custom] @Style, [Template] @Style, [Custom] @Element等
 * 
 * 注意：--注释和原始嵌入是特殊存在，任意地方都可以写
 *       {{&}} 等JavaScript模板语法是本身具有的功能，不应被误禁
 */
class CHtlLocalScriptConstraint {
public:
    CHtlLocalScriptConstraint();
    virtual ~CHtlLocalScriptConstraint() = default;

    /**
     * 验证CHTL局部script块内容
     * @param scriptContent 局部script块的内容
     * @return 验证结果
     */
    LocalScriptConstraintResult validateCHtlLocalScriptBlock(const std::string& scriptContent);

    /**
     * 检查是否为允许的局部script元素
     * @param element 要检查的元素文本
     * @return 允许的元素类型，如果不允许则返回空
     */
    std::vector<LocalScriptAllowedElement> checkAllowedElement(const std::string& element);

    /**
     * 检查是否包含禁止的CHTL语法
     * @param scriptContent Script内容
     * @return 发现的禁止语法列表
     */
    std::vector<LocalScriptForbiddenElement> checkForbiddenCHtlSyntax(const std::string& scriptContent);

    /**
     * 验证模板变量引用
     * @param varReference 变量引用文本
     * @return 是否有效
     */
    bool validateTemplateVariableReference(const std::string& varReference);

    /**
     * 验证自定义变量引用
     * @param varReference 变量引用文本
     * @return 是否有效
     */
    bool validateCustomVariableReference(const std::string& varReference);

    /**
     * 验证变量组特例化
     * @param specialization 特例化文本
     * @return 是否有效
     */
    bool validateVariableSpecialization(const std::string& specialization);

    /**
     * 验证from子句
     * @param fromClause from子句文本
     * @return 是否有效
     */
    bool validateFromClause(const std::string& fromClause);

    /**
     * 验证生成器注释
     * @param comment 注释文本
     * @return 是否有效
     */
    bool validateGeneratorComment(const std::string& comment);

    /**
     * 验证原始嵌入
     * @param embedding 原始嵌入文本
     * @return 是否有效
     */
    bool validateRawEmbedding(const std::string& embedding);

    /**
     * 检查是否为JavaScript模板语法
     * @param text 文本
     * @return 是否为模板语法
     */
    bool isJavaScriptTemplateSyntax(const std::string& text);

private:
    // 禁止的CHTL语法模式（样式和元素相关）
    std::unordered_set<std::string> forbiddenCHtlPatterns_;
    
    /**
     * 解析局部script内容为语句列表
     * @param content 局部script内容
     * @return 语句列表
     */
    std::vector<std::string> parseLocalScriptStatements(const std::string& content);
    
    /**
     * 检查是否为纯JavaScript代码
     * @param statement 语句
     * @return 是否为纯JavaScript
     */
    bool isPureJavaScript(const std::string& statement);
    
    /**
     * 检查是否为生成器注释
     * @param statement 语句
     * @return 是否为生成器注释
     */
    bool isGeneratorComment(const std::string& statement);
    
    /**
     * 检查是否为原始嵌入
     * @param statement 语句
     * @return 是否为原始嵌入
     */
    bool isRawEmbedding(const std::string& statement);
    
    /**
     * 检查是否为模板变量引用
     * @param statement 语句
     * @return 是否为模板变量引用
     */
    bool isTemplateVariableReference(const std::string& statement);
    
    /**
     * 检查是否为自定义变量引用
     * @param statement 语句
     * @return 是否为自定义变量引用
     */
    bool isCustomVariableReference(const std::string& statement);
    
    /**
     * 检查是否为变量特例化
     * @param statement 语句
     * @return 是否为变量特例化
     */
    bool isVariableSpecialization(const std::string& statement);
    
    /**
     * 检查是否为命名空间from子句
     * @param statement 语句
     * @return 是否为from子句
     */
    bool isNamespaceFromClause(const std::string& statement);
    
    /**
     * 检查是否为禁止的样式相关CHTL语法
     * @param statement 语句
     * @return 是否为禁止的样式语法
     */
    bool isForbiddenStyleSyntax(const std::string& statement);
    
    /**
     * 检查是否为禁止的元素相关CHTL语法
     * @param statement 语句
     * @return 是否为禁止的元素语法
     */
    bool isForbiddenElementSyntax(const std::string& statement);
    
    /**
     * 添加约束违规
     * @param result 结果对象
     * @param type 违规类型
     * @param forbiddenElement 禁止的元素类型
     * @param message 错误消息
     * @param element 相关元素
     * @param line 行号
     * @param column 列号
     */
    void addViolation(LocalScriptConstraintResult& result, 
                    LocalScriptConstraintViolationType type,
                    LocalScriptForbiddenElement forbiddenElement,
                    const std::string& message,
                    const std::string& element = "",
                    size_t line = 0, 
                    size_t column = 0);
};

} // namespace chtl