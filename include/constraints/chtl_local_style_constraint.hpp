#pragma once

#include "common/types.hpp"
#include <string>
#include <vector>
#include <set>
#include <unordered_set>

namespace chtl {

/**
 * CHTL 局部样式块允许的语法元素类型
 * 注意：局部样式块与全局样式块允许相同的CHTL语法
 */
enum class LocalStyleAllowedElement {
    TEMPLATE_VARIABLE,              // 模板变量: VariableGroupName(variableName) 或 @Var VariableGroupName(variableName)
    CUSTOM_VARIABLE,                // 自定义变量: VariableGroupName(variableName) 或 @Var VariableGroupName(variableName)
    CUSTOM_VARIABLE_SPECIALIZATION, // 自定义变量特例化: VariableGroupName(variableName = value)
    TEMPLATE_STYLE_GROUP,           // 模板样式组: @Style StyleGroupName
    CUSTOM_STYLE_GROUP,             // 自定义样式组: @Style StyleGroupName
    VALUELESS_STYLE_GROUP,          // 无值样式组: @Style StyleGroupName { properties }
    CUSTOM_STYLE_SPECIALIZATION,    // 自定义样式组特例化: @Style StyleGroupName { delete properties }
    DELETE_PROPERTY,                // delete属性: delete property1, property2
    DELETE_INHERITANCE,             // delete继承: delete @Style StyleGroupName
    INHERITANCE,                    // 继承: inherit @Style StyleGroupName 或 @Style BaseStyle
    GENERATOR_COMMENT,              // 生成器注释: --注释内容
    FULL_QUALIFIED_NAME,            // 全缀名: [Custom] @Var/@Style/@Element Name 或 [Template] @Var/@Style/@Element Name
    RAW_EMBEDDING,                  // 原始嵌入: [Origin] @Type [name] { 内容 } (支持基本类型和自定义类型)
    FROM_NAMESPACE,                 // 从命名空间获取: ElementName from namespace.path
    CSS_SELECTOR,                   // CSS选择器 (基本CSS语法)
    CSS_PROPERTY                    // CSS属性 (基本CSS语法)
};

/**
 * CHTL 局部样式约束违规类型
 */
enum class LocalStyleConstraintViolationType {
    FORBIDDEN_LOCAL_FEATURE,        // 禁止的局部样式特性 (如 :hover, :focus, &)
    INVALID_SYNTAX,                 // 无效的语法
    MALFORMED_VARIABLE_REFERENCE,   // 格式错误的变量引用
    MALFORMED_STYLE_GROUP_REFERENCE,// 格式错误的样式组引用
    MALFORMED_FROM_CLAUSE,          // 格式错误的from子句
    MALFORMED_GENERATOR_COMMENT,    // 格式错误的生成器注释
    INVALID_RAW_EMBEDDING,          // 无效的原始嵌入
    UNKNOWN_SYNTAX_ELEMENT          // 未知的语法元素
};

/**
 * CHTL 局部样式约束违规信息
 */
struct LocalStyleConstraintViolation {
    LocalStyleConstraintViolationType type;
    std::string message;
    std::string element;
    size_t line = 0;
    size_t column = 0;
};

/**
 * CHTL 局部样式约束验证结果
 */
struct LocalStyleConstraintResult {
    bool isValid = true;
    std::vector<LocalStyleConstraintViolation> violations;
    std::set<std::string> templateVariables;
    std::set<std::string> customVariables;
    std::set<std::string> styleGroups;
    std::set<std::string> inheritances;
    std::set<std::string> foundGeneratorComments;
    std::set<std::string> foundRawEmbeddings;
    std::set<std::string> fromClauses;
    std::set<std::string> fullQualifiedNames;
    size_t cssStatements = 0;
};

/**
 * CHTL 局部样式块约束器
 * 
 * 验证CHTL局部样式块(style { ... })中的语法约束：
 * 
 * ✅ 允许使用的CHTL语法（与全局样式块相同）：
 * - 模板变量: VariableGroupName(variableName), @Var VariableGroupName(variableName)
 * - 自定义变量: 同上，包括特例化
 * - 样式组: @Style StyleGroupName，包括无值样式组和特例化
 * - delete操作: delete property1, property2; delete @Style StyleGroupName
 * - 继承: inherit @Style StyleGroupName, @Style BaseStyle
 * - 生成器注释: --注释内容
 * - 全缀名: [Custom/Template] @Var/@Style/@Element Name
 * - 原始嵌入: [Origin] @Html/@Style/@JavaScript { 内容 }
 * - from命名空间: ElementName from namespace.path
 * - 基本CSS语法: 选择器和属性
 * 
 * 注意：局部样式块与全局样式块的语法约束完全相同
 */
class CHtlLocalStyleConstraint {
public:
    CHtlLocalStyleConstraint();
    virtual ~CHtlLocalStyleConstraint() = default;

    /**
     * 验证CHTL局部样式块内容
     * @param styleContent 局部样式块的内容
     * @return 验证结果
     */
    LocalStyleConstraintResult validateCHtlLocalStyleBlock(const std::string& styleContent);

    /**
     * 检查是否为允许的局部样式元素
     * @param element 要检查的元素文本
     * @return 允许的元素类型，如果不允许则返回空
     */
    std::vector<LocalStyleAllowedElement> checkAllowedElement(const std::string& element);

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
     * 验证样式组引用
     * @param styleGroupRef 样式组引用文本
     * @return 是否有效
     */
    bool validateStyleGroupReference(const std::string& styleGroupRef);

    /**
     * 验证继承语句
     * @param inheritStatement 继承语句文本
     * @return 是否有效
     */
    bool validateInheritStatement(const std::string& inheritStatement);

    /**
     * 验证delete语句
     * @param deleteStatement delete语句文本
     * @return 是否有效
     */
    bool validateDeleteStatement(const std::string& deleteStatement);

    /**
     * 验证from子句
     * @param fromClause from子句文本
     * @return 是否有效
     */
    bool validateFromClause(const std::string& fromClause);

    /**
     * 验证全缀名
     * @param fullQualifiedName 全缀名文本
     * @return 是否有效
     */
    bool validateFullQualifiedName(const std::string& fullQualifiedName);

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

private:
    /**
     * 解析局部样式内容为语句列表
     * @param content 局部样式内容
     * @return 语句列表
     */
    std::vector<std::string> parseLocalStyleStatements(const std::string& content);
    
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
     * 检查是否为变量引用
     * @param statement 语句
     * @return 是否为变量引用
     */
    bool isVariableReference(const std::string& statement);
    
    /**
     * 检查是否为样式组引用
     * @param statement 语句
     * @return 是否为样式组引用
     */
    bool isStyleGroupReference(const std::string& statement);
    
    /**
     * 检查是否为继承语句
     * @param statement 语句
     * @return 是否为继承语句
     */
    bool isInheritStatement(const std::string& statement);
    
    /**
     * 检查是否为delete语句
     * @param statement 语句
     * @return 是否为delete语句
     */
    bool isDeleteStatement(const std::string& statement);
    
    /**
     * 检查是否为from子句
     * @param statement 语句
     * @return 是否为from子句
     */
    bool isFromClause(const std::string& statement);
    
    /**
     * 检查是否为全缀名
     * @param statement 语句
     * @return 是否为全缀名
     */
    bool isFullQualifiedName(const std::string& statement);
    
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
     * 检查是否为基本CSS语法
     * @param statement 语句
     * @return 是否为基本CSS语法
     */
    bool isBasicCSS(const std::string& statement);
    
    /**
     * 添加约束违规
     * @param result 结果对象
     * @param type 违规类型
     * @param message 错误消息
     * @param element 相关元素
     * @param line 行号
     * @param column 列号
     */
    void addViolation(LocalStyleConstraintResult& result, 
                    LocalStyleConstraintViolationType type,
                    const std::string& message,
                    const std::string& element = "",
                    size_t line = 0, 
                    size_t column = 0);
};

} // namespace chtl