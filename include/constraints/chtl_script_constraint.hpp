#pragma once

#include "common/types.hpp"
#include <string>
#include <vector>
#include <set>
#include <unordered_set>

namespace chtl {

/**
 * CHTL Script块禁止的语法元素类型
 */
enum class CHtlScriptForbiddenElement {
    TEMPLATE_VARIABLE,              // 模板变量: VariableGroupName(variableName) 或 @Var VariableGroupName(variableName)
    CUSTOM_VARIABLE,                // 自定义变量: VariableGroupName(variableName) 或 @Var VariableGroupName(variableName)
    VARIABLE_SPECIALIZATION,        // 变量组特例化: VariableGroupName(variableName = value)
    NAMESPACE_FROM,                 // 命名空间from: ElementName from namespace.path
    TEMPLATE_STYLE_GROUP,           // 模板样式组: @Style StyleGroupName
    CUSTOM_STYLE_GROUP,             // 自定义样式组: @Style StyleGroupName
    TEMPLATE_ELEMENT,               // 模板元素: @Element ElementName
    CUSTOM_ELEMENT,                 // 自定义元素: @Element ElementName
    FULL_QUALIFIED_NAME,            // 全缀名: [Custom] @Var/@Style/@Element Name
    STYLE_BLOCKS,                   // 样式块: style { ... }
    TEXT_BLOCKS,                    // 文本块: text { ... }
    INHERITANCE,                    // 继承: inherit @Style StyleGroupName
    DELETE_OPERATIONS               // Delete操作: delete properties, delete @Style
};

/**
 * CHTL Script块允许的语法元素类型
 */
enum class CHtlScriptAllowedElement {
    GENERATOR_COMMENT,              // 生成器注释: --注释内容
    RAW_EMBEDDING_HTML,             // 原始HTML嵌入: [Origin] @Html { HTML代码 }
    RAW_EMBEDDING_CSS,              // 原始CSS嵌入: [Origin] @Style { CSS代码 }
    RAW_EMBEDDING_JS,               // 原始JavaScript嵌入: [Origin] @JavaScript { JS代码 }
    RAW_EMBEDDING_CHTL,             // 原始CHTL嵌入: [Origin] @Chtl { CHTL代码 }
    PURE_JAVASCRIPT                 // 纯JavaScript代码
};

/**
 * CHTL Script约束违规类型
 */
enum class CHtlScriptConstraintViolationType {
    FORBIDDEN_CHTL_SYNTAX,          // 禁止的CHTL语法
    INVALID_RAW_EMBEDDING,          // 无效的原始嵌入
    MALFORMED_GENERATOR_COMMENT     // 格式错误的生成器注释
};

/**
 * CHTL Script约束违规信息
 */
struct CHtlScriptConstraintViolation {
    CHtlScriptConstraintViolationType type;
    CHtlScriptForbiddenElement forbiddenElement;
    std::string message;
    std::string element;
    size_t line = 0;
    size_t column = 0;
};

/**
 * CHTL Script约束验证结果
 */
struct CHtlScriptConstraintResult {
    bool isValid = true;
    std::vector<CHtlScriptConstraintViolation> violations;
    std::set<std::string> foundGeneratorComments;
    std::set<std::string> foundRawEmbeddings;
    size_t pureJavaScriptLines = 0;
};

/**
 * CHTL Script块约束器
 * 
 * 验证CHTL Script块(非局部script)中的语法约束：
 * 
 * ❌ 禁止使用的CHTL语法：
 * - 模板变量: VariableGroupName(variableName), @Var VariableGroupName(variableName)
 * - 自定义变量: 同上
 * - 变量组特例化: VariableGroupName(variableName = value)
 * - 命名空间from: ElementName from namespace.path
 * - 任何样式组、元素、继承、delete操作等CHTL特有语法
 * 
 * ✅ 允许使用的语法：
 * - 生成器注释: --注释内容
 * - 原始嵌入(任意类型): [Origin] @Html/@Style/@JavaScript/@Chtl { 代码 }
 * - 纯JavaScript代码: 标准JavaScript语法
 */
class CHtlScriptConstraint {
public:
    CHtlScriptConstraint();
    virtual ~CHtlScriptConstraint() = default;

    /**
     * 验证CHTL Script块内容
     * @param scriptContent Script块的内容
     * @return 验证结果
     */
    CHtlScriptConstraintResult validateCHtlScriptBlock(const std::string& scriptContent);

    /**
     * 检查是否为允许的Script元素
     * @param element 要检查的元素文本
     * @return 允许的元素类型，如果不允许则返回空
     */
    std::vector<CHtlScriptAllowedElement> checkAllowedElement(const std::string& element);

    /**
     * 检查是否包含禁止的CHTL语法
     * @param scriptContent Script内容
     * @return 发现的禁止语法列表
     */
    std::vector<CHtlScriptForbiddenElement> checkForbiddenCHtlSyntax(const std::string& scriptContent);

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
    // 禁止的CHTL语法模式
    std::unordered_set<std::string> forbiddenCHtlPatterns_;
    
    /**
     * 解析Script内容为语句列表
     * @param content Script内容
     * @return 语句列表
     */
    std::vector<std::string> parseScriptStatements(const std::string& content);
    
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
     * 检查是否为CHTL样式组、元素等语法
     * @param statement 语句
     * @return 是否为CHTL特有语法
     */
    bool isCHtlSpecificSyntax(const std::string& statement);
    
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
    void addViolation(CHtlScriptConstraintResult& result, 
                     CHtlScriptConstraintViolationType type,
                     CHtlScriptForbiddenElement forbiddenElement,
                     const std::string& message,
                     const std::string& element = "",
                     size_t line = 0, 
                     size_t column = 0);
};

} // namespace chtl