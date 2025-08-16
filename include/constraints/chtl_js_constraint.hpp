#pragma once

#include "common/types.hpp"
#include <string>
#include <vector>
#include <set>
#include <unordered_set>

namespace chtl {

/**
 * CHTL JavaScript块允许的语法元素类型
 */
enum class CHtlJSAllowedElement {
    TEMPLATE_VARIABLE,              // 模板变量: @Var变量组名(变量名)
    CUSTOM_VARIABLE,                // 自定义变量: @Var变量组名(变量名)
    GENERATOR_COMMENT,              // 生成器注释: --注释内容
    RAW_EMBEDDING,                  // 原始嵌入: [Origin] @JavaScript
    FROM_NAMESPACE,                 // 从命名空间获取: 元素名 from 命名空间
    FULL_QUALIFIED_NAME,            // 全缀名: 命名空间.元素名
    PURE_JAVASCRIPT,                // 纯JavaScript代码
    CHTL_DOM_ACCESS,               // CHTL DOM访问: 访问CHTL生成的元素
    CHTL_STATE_BINDING             // CHTL状态绑定: 绑定到CHTL状态系统
};

/**
 * CHTL JavaScript约束违规类型
 */
enum class CHtlJSConstraintViolationType {
    FORBIDDEN_CHTL_ELEMENT,         // 禁止的CHTL元素(在JS中不允许使用)
    INVALID_VARIABLE_REFERENCE,     // 无效变量引用
    MALFORMED_FROM_CLAUSE,          // 格式错误的from子句
    INVALID_DOM_ACCESS,             // 无效的DOM访问
    FORBIDDEN_API,                  // 禁止的API调用
    MISSING_NAMESPACE               // 缺失命名空间
};

/**
 * CHTL JavaScript约束违规信息
 */
struct CHtlJSConstraintViolation {
    CHtlJSConstraintViolationType type;
    std::string message;
    std::string element;
    size_t line = 0;
    size_t column = 0;
};

/**
 * CHTL JavaScript约束验证结果
 */
struct CHtlJSConstraintResult {
    bool isValid = true;
    std::vector<CHtlJSConstraintViolation> violations;
    std::set<std::string> usedTemplateVariables;
    std::set<std::string> usedCustomVariables;
    std::set<std::string> usedNamespaces;
    std::set<std::string> accessedDOMElements;
    std::set<std::string> boundStates;
};

/**
 * CHTL JavaScript块约束器
 * 
 * 验证CHTL JavaScript块(script块)中允许使用的语法元素：
 * 1. 模板变量和自定义变量的使用
 * 2. 生成器注释
 * 3. 原始JavaScript嵌入
 * 4. 从命名空间获取元素  
 * 5. 全缀名访问
 * 6. 纯JavaScript代码
 * 7. CHTL DOM访问(访问CHTL生成的元素)
 * 8. CHTL状态绑定
 * 
 * 不允许:
 * - CHTL特有的HTML语法(元素定义等)
 * - CHTL样式组定义
 * - 某些危险的JavaScript API
 */
class CHtlJSConstraint {
public:
    CHtlJSConstraint();
    virtual ~CHtlJSConstraint() = default;

    /**
     * 验证CHTL JavaScript块内容
     * @param jsContent JavaScript块的内容
     * @param availableNamespaces 可用的命名空间列表
     * @param availableTemplateVars 可用的模板变量列表
     * @param availableCustomVars 可用的自定义变量列表
     * @param availableDOMElements 可用的CHTL DOM元素列表
     * @return 验证结果
     */
    CHtlJSConstraintResult validateCHtlJSBlock(
        const std::string& jsContent,
        const std::vector<std::string>& availableNamespaces = {},
        const std::vector<std::string>& availableTemplateVars = {},
        const std::vector<std::string>& availableCustomVars = {},
        const std::vector<std::string>& availableDOMElements = {}
    );

    /**
     * 检查是否为允许的CHTL JavaScript元素
     * @param element 要检查的元素文本
     * @return 允许的元素类型，如果不允许则返回空
     */
    std::vector<CHtlJSAllowedElement> checkAllowedElement(const std::string& element);

    /**
     * 验证模板变量引用
     * @param varReference 变量引用文本
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
     * 验证from子句
     * @param fromClause from子句文本
     * @return 是否有效
     */
    bool validateFromClause(const std::string& fromClause);

    /**
     * 验证CHTL DOM访问
     * @param domAccess DOM访问语句
     * @return 是否有效
     */
    bool validateCHtlDOMAccess(const std::string& domAccess);

    /**
     * 检查是否包含禁止的API调用
     * @param jsContent JavaScript内容
     * @return 发现的禁止API列表
     */
    std::vector<std::string> checkForbiddenAPIs(const std::string& jsContent);

    /**
     * 检查是否包含禁止的CHTL元素
     * @param jsContent JavaScript内容
     * @return 发现的禁止CHTL元素列表
     */
    std::vector<std::string> checkForbiddenCHtlElements(const std::string& jsContent);

private:
    // 禁止的API调用模式
    std::unordered_set<std::string> forbiddenAPIs_;
    
    // 禁止的CHTL元素模式(在JS中不应该出现)
    std::unordered_set<std::string> forbiddenCHtlElements_;
    
    /**
     * 解析JavaScript内容为语句列表
     * @param content JavaScript内容
     * @return 语句列表
     */
    std::vector<std::string> parseJSStatements(const std::string& content);
    
    /**
     * 检查是否为纯JavaScript代码
     * @param statement 语句
     * @return 是否为纯JavaScript
     */
    bool isPureJavaScript(const std::string& statement);
    
    /**
     * 检查是否为CHTL变量引用
     * @param statement 语句
     * @return 是否为变量引用
     */
    bool isVariableReference(const std::string& statement);
    
    /**
     * 检查是否为CHTL DOM访问
     * @param statement 语句
     * @return 是否为CHTL DOM访问
     */
    bool isCHtlDOMAccess(const std::string& statement);
    
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
     * 添加约束违规
     * @param result 结果对象
     * @param type 违规类型
     * @param message 错误消息
     * @param element 相关元素
     * @param line 行号
     * @param column 列号
     */
    void addViolation(CHtlJSConstraintResult& result, 
                     CHtlJSConstraintViolationType type,
                     const std::string& message,
                     const std::string& element = "",
                     size_t line = 0, 
                     size_t column = 0);
};

} // namespace chtl