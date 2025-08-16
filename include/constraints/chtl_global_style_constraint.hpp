#pragma once

#include "common/types.hpp"
#include <string>
#include <vector>
#include <set>
#include <unordered_set>

namespace chtl {

/**
 * CHTL全局样式块允许的语法元素类型
 */
enum class GlobalStyleAllowedElement {
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
    RAW_EMBEDDING,                  // 原始嵌入: [Origin] @Html/@Style/@JavaScript { 内容 }
    FROM_NAMESPACE,                 // 从命名空间获取: ElementName from namespace.path
    CSS_SELECTOR,                   // CSS选择器 (基本CSS语法)
    CSS_PROPERTY                    // CSS属性 (基本CSS语法)
};

/**
 * 约束违规类型
 */
enum class ConstraintViolationType {
    FORBIDDEN_ELEMENT,              // 禁止的元素
    INVALID_SYNTAX,                 // 无效语法
    MISSING_NAMESPACE,              // 缺失命名空间
    INVALID_VARIABLE_REFERENCE,     // 无效变量引用
    INVALID_STYLE_GROUP_REFERENCE,  // 无效样式组引用
    MALFORMED_FROM_CLAUSE,          // 格式错误的from子句
    NESTED_STYLE_BLOCK,             // 嵌套样式块(在全局样式块中不允许)
    LOCAL_STYLE_FEATURE             // 局部样式块特性(在全局样式块中不允许)
};

/**
 * 约束违规信息
 */
struct ConstraintViolation {
    ConstraintViolationType type;
    std::string message;
    std::string element;
    size_t line = 0;
    size_t column = 0;
};

/**
 * CHTL全局样式块约束验证结果
 */
struct GlobalStyleConstraintResult {
    bool isValid = true;
    std::vector<ConstraintViolation> violations;
    std::set<std::string> usedTemplateVariables;
    std::set<std::string> usedCustomVariables;
    std::set<std::string> usedStyleGroups;
    std::set<std::string> usedNamespaces;
    std::set<std::string> deletedProperties;
    std::set<std::string> deletedInheritances;
    std::set<std::string> inheritances;
};

/**
 * CHTL全局样式块约束器
 * 
 * 验证全局样式块中只允许使用的语法元素：
 * 1. 模板变量的使用
 * 2. 自定义变量的使用和特例化
 * 3. 模板样式组和自定义样式组
 * 4. 无值样式组
 * 5. 自定义样式组的特例化
 * 6. delete属性和delete继承
 * 7. 继承(样式组之间的继承)
 * 8. 生成器注释
 * 9. 全缀名
 * 10. 原始嵌入
 * 11. 从命名空间中获取元素
 * 12. 基本CSS语法(选择器和属性)
 */
class CHtlGlobalStyleConstraint {
public:
    CHtlGlobalStyleConstraint();
    virtual ~CHtlGlobalStyleConstraint() = default;

    /**
     * 验证全局样式块内容
     * @param styleContent 全局样式块的内容
     * @param availableNamespaces 可用的命名空间列表
     * @param availableTemplateVars 可用的模板变量列表
     * @param availableCustomVars 可用的自定义变量列表
     * @param availableStyleGroups 可用的样式组列表
     * @return 验证结果
     */
    GlobalStyleConstraintResult validateGlobalStyleBlock(
        const std::string& styleContent,
        const std::vector<std::string>& availableNamespaces = {},
        const std::vector<std::string>& availableTemplateVars = {},
        const std::vector<std::string>& availableCustomVars = {},
        const std::vector<std::string>& availableStyleGroups = {}
    );

    /**
     * 检查是否为允许的全局样式元素
     * @param element 要检查的元素文本
     * @return 允许的元素类型，如果不允许则返回空
     */
    std::vector<GlobalStyleAllowedElement> checkAllowedElement(const std::string& element);

    /**
     * 验证模板变量引用
     * @param varReference 变量引用文本，如 @Var ThemeColor(primaryColor)
     * @return 是否有效
     */
    bool validateTemplateVariableReference(const std::string& varReference);

    /**
     * 验证自定义变量引用
     * @param varReference 自定义变量引用文本
     * @return 是否有效
     */
    bool validateCustomVariableReference(const std::string& varReference);

    /**
     * 验证样式组引用
     * @param styleGroupRef 样式组引用文本，如 @Style DefaultButton
     * @return 是否有效
     */
    bool validateStyleGroupReference(const std::string& styleGroupRef);

    /**
     * 验证delete语句
     * @param deleteStatement delete语句文本，如 delete color, background
     * @return 是否有效
     */
    bool validateDeleteStatement(const std::string& deleteStatement);

    /**
     * 验证继承语句
     * @param inheritStatement 继承语句文本，如 inherit @Style BaseTheme
     * @return 是否有效
     */
    bool validateInheritStatement(const std::string& inheritStatement);

    /**
     * 验证from子句
     * @param fromClause from子句文本，如 ThemeColor from ui.theme
     * @return 是否有效
     */
    bool validateFromClause(const std::string& fromClause);

    /**
     * 验证全缀名
     * @param fullQualifiedName 全缀名文本，如 ui.theme.PrimaryButton
     * @return 是否有效
     */
    bool validateFullQualifiedName(const std::string& fullQualifiedName);

    /**
     * 检查是否包含禁止的局部样式块特性
     * @param styleContent 样式内容
     * @return 发现的禁止特性列表
     */
    std::vector<std::string> checkForbiddenLocalStyleFeatures(const std::string& styleContent);

private:
    // 允许的全局样式元素模式
    std::unordered_set<std::string> allowedElementPatterns_;
    
    // 禁止的局部样式特性模式
    std::unordered_set<std::string> forbiddenLocalPatterns_;
    
    /**
     * 解析样式内容为语句列表
     * @param content 样式内容
     * @return 语句列表
     */
    std::vector<std::string> parseStyleStatements(const std::string& content);
    
    /**
     * 检查是否为CSS选择器
     * @param statement 语句
     * @return 是否为CSS选择器
     */
    bool isCSSSelector(const std::string& statement);
    
    /**
     * 检查是否为CSS属性
     * @param statement 语句  
     * @return 是否为CSS属性
     */
    bool isCSSProperty(const std::string& statement);
    
    /**
     * 检查是否为CHTL变量引用
     * @param statement 语句
     * @return 是否为变量引用
     */
    bool isVariableReference(const std::string& statement);
    
    /**
     * 检查是否为CHTL样式组引用
     * @param statement 语句
     * @return 是否为样式组引用
     */
    bool isStyleGroupReference(const std::string& statement);
    
    /**
     * 检查是否为delete语句
     * @param statement 语句
     * @return 是否为delete语句
     */
    bool isDeleteStatement(const std::string& statement);
    
    /**
     * 检查是否为inherit语句
     * @param statement 语句
     * @return 是否为inherit语句
     */
    bool isInheritStatement(const std::string& statement);
    
    /**
     * 检查是否为from子句
     * @param statement 语句
     * @return 是否为from子句
     */
    bool isFromClause(const std::string& statement);
    
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
     * 添加约束违规
     * @param result 结果对象
     * @param type 违规类型
     * @param message 错误消息
     * @param element 相关元素
     * @param line 行号
     * @param column 列号
     */
    void addViolation(GlobalStyleConstraintResult& result, 
                     ConstraintViolationType type,
                     const std::string& message,
                     const std::string& element = "",
                     size_t line = 0, 
                     size_t column = 0);
};

} // namespace chtl