#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <regex>

namespace chtl {

/**
 * 约束类型枚举
 */
enum class ConstraintType {
    PRECISE,        // 精确约束：针对特定HTML元素、自定义或模板对象
    TYPE_BASED,     // 类型约束：针对@Html、[Custom]、[Template]类型
    GLOBAL          // 全局约束：在命名空间级别生效
};

/**
 * 约束范围枚举
 */
enum class ConstraintScope {
    ELEMENT,        // 元素级约束
    NAMESPACE,      // 命名空间级约束
    GLOBAL          // 全局约束
};

/**
 * 单个约束规则
 */
struct ConstraintRule {
    ConstraintType type;
    std::string target;             // 约束目标（元素名、类型名等）
    std::string context;            // 约束上下文（在哪个元素/命名空间中）
    ConstraintScope scope;          // 约束范围
    std::string description;        // 约束描述
    
    ConstraintRule() : type(ConstraintType::PRECISE), scope(ConstraintScope::ELEMENT) {}
};

/**
 * 约束违反结果
 */
struct ConstraintViolation {
    std::string message;            // 违反消息
    std::string context;            // 违反上下文
    std::string target;             // 违反的目标
    ConstraintRule rule;            // 相关的约束规则
    int line = -1;                  // 代码行号
    int column = -1;                // 代码列号
};

/**
 * 约束解析器
 * 解析except语句并生成约束规则
 */
class ConstraintParser {
public:
    /**
     * 解析except语句
     * @param statement except语句
     * @param context 当前上下文（元素或命名空间）
     * @return 约束规则列表
     */
    static std::vector<ConstraintRule> parseExceptStatement(const std::string& statement, 
                                                           const std::string& context);

    /**
     * 解析精确约束
     * except span, [Custom] @Element Box;
     */
    static std::vector<ConstraintRule> parsePreciseConstraints(const std::string& targets, 
                                                              const std::string& context);

    /**
     * 解析类型约束
     * except @Html; except [Template] @Var;
     */
    static std::vector<ConstraintRule> parseTypeConstraints(const std::string& targets, 
                                                           const std::string& context);

    /**
     * 解析全局约束
     * except [Template]; (在命名空间中)
     */
    static std::vector<ConstraintRule> parseGlobalConstraints(const std::string& targets, 
                                                             const std::string& context);

private:
    /**
     * 拆分约束目标列表
     * @param targets 目标字符串 "span, [Custom] @Element Box, @Html"
     * @return 目标列表
     */
    static std::vector<std::string> splitTargets(const std::string& targets);

    /**
     * 确定约束类型
     * @param target 单个目标字符串
     * @return 约束类型
     */
    static ConstraintType determineConstraintType(const std::string& target);

    /**
     * 清理目标字符串
     * @param target 原始目标
     * @return 清理后的目标
     */
    static std::string cleanTarget(const std::string& target);
};

/**
 * 约束验证器
 * 验证代码是否违反约束规则
 */
class ConstraintValidator {
public:
    ConstraintValidator();

    /**
     * 添加约束规则
     * @param rule 约束规则
     */
    void addConstraintRule(const ConstraintRule& rule);

    /**
     * 添加多个约束规则
     * @param rules 约束规则列表
     */
    void addConstraintRules(const std::vector<ConstraintRule>& rules);

    /**
     * 验证代码片段
     * @param code 代码片段
     * @param context 当前上下文
     * @return 约束违反列表
     */
    std::vector<ConstraintViolation> validateCode(const std::string& code, 
                                                  const std::string& context);

    /**
     * 验证HTML元素使用
     * @param elementName 元素名称
     * @param context 上下文
     * @return 是否违反约束
     */
    bool isElementAllowed(const std::string& elementName, const std::string& context);

    /**
     * 验证模板/自定义对象使用
     * @param category 类别（Custom/Template）
     * @param type 类型（Element/Style/Var）
     * @param name 对象名称
     * @param context 上下文
     * @return 是否违反约束
     */
    bool isObjectAllowed(const std::string& category, const std::string& type, 
                        const std::string& name, const std::string& context);

    /**
     * 验证原始嵌入使用
     * @param type 嵌入类型（@Html/@Style/@JavaScript）
     * @param context 上下文
     * @return 是否违反约束
     */
    bool isOriginEmbeddingAllowed(const std::string& type, const std::string& context);

    /**
     * 清除所有约束规则
     */
    void clearConstraints();

    /**
     * 获取当前约束规则数量
     * @return 规则数量
     */
    size_t getConstraintCount() const { return constraints_.size(); }

    /**
     * 获取所有约束规则
     * @return 约束规则列表
     */
    const std::vector<ConstraintRule>& getConstraints() const { return constraints_; }

private:
    std::vector<ConstraintRule> constraints_;

    /**
     * 检查精确约束违反
     * @param target 目标
     * @param context 上下文
     * @return 是否违反
     */
    bool checkPreciseConstraintViolation(const std::string& target, const std::string& context);

    /**
     * 检查类型约束违反
     * @param type 类型
     * @param context 上下文
     * @return 是否违反
     */
    bool checkTypeConstraintViolation(const std::string& type, const std::string& context);

    /**
     * 检查全局约束违反
     * @param category 类别
     * @param context 上下文
     * @return 是否违反
     */
    bool checkGlobalConstraintViolation(const std::string& category, const std::string& context);

    /**
     * 上下文匹配检查
     * @param ruleContext 规则上下文
     * @param currentContext 当前上下文
     * @return 是否匹配
     */
    bool contextMatches(const std::string& ruleContext, const std::string& currentContext);
};

/**
 * 约束处理器
 * 处理和编译CHTL代码中的约束语句
 */
class ConstraintProcessor {
public:
    ConstraintProcessor();

    /**
     * 处理代码中的约束语句
     * @param code 原始代码
     * @param context 上下文
     * @return 处理后的代码（移除except语句）和约束规则
     */
    std::pair<std::string, std::vector<ConstraintRule>> processConstraints(const std::string& code, 
                                                                          const std::string& context);

    /**
     * 验证代码是否符合约束
     * @param code 要验证的代码
     * @param context 上下文
     * @param rules 应用的约束规则
     * @return 违反信息列表
     */
    std::vector<ConstraintViolation> validateConstraints(const std::string& code, 
                                                        const std::string& context,
                                                        const std::vector<ConstraintRule>& rules);

    /**
     * 获取处理统计信息
     * @return 统计信息字符串
     */
    std::string getStatistics() const;

private:
    ConstraintValidator validator_;
    int processedStatements_ = 0;
    int totalRules_ = 0;

    /**
     * 扫描代码查找except语句
     * @param code 代码
     * @return except语句列表
     */
    std::vector<std::string> scanExceptStatements(const std::string& code);

    /**
     * 从代码中移除except语句
     * @param code 代码
     * @param statements except语句列表
     * @return 清理后的代码
     */
    std::string removeExceptStatements(const std::string& code, 
                                     const std::vector<std::string>& statements);
};

/**
 * 约束系统管理器
 * 整合约束解析、验证和处理功能的高级接口
 */
class ConstraintSystemManager {
public:
    ConstraintSystemManager();

    /**
     * 启用约束检查
     * @param enabled 是否启用
     */
    void setConstraintCheckingEnabled(bool enabled) { enabled_ = enabled; }

    /**
     * 检查约束是否启用
     * @return 是否启用
     */
    bool isConstraintCheckingEnabled() const { return enabled_; }

    /**
     * 设置严格模式
     * @param strict 是否严格模式（违反约束时报错而非警告）
     */
    void setStrictMode(bool strict) { strictMode_ = strict; }

    /**
     * 处理带约束的代码
     * @param code 原始代码
     * @param context 上下文
     * @return {处理后代码, 违反信息列表}
     */
    std::pair<std::string, std::vector<ConstraintViolation>> processCodeWithConstraints(
        const std::string& code, const std::string& context);

    /**
     * 验证代码片段
     * @param code 代码片段
     * @param context 上下文
     * @return 违反信息列表
     */
    std::vector<ConstraintViolation> validateCodeFragment(const std::string& code, 
                                                         const std::string& context);

    /**
     * 添加全局约束规则
     * @param rule 约束规则
     */
    void addGlobalConstraint(const ConstraintRule& rule);

    /**
     * 清除所有约束
     */
    void clearAllConstraints();

    /**
     * 获取约束统计信息
     * @return 统计信息
     */
    std::string getConstraintStatistics() const;

private:
    ConstraintProcessor processor_;
    std::vector<ConstraintRule> globalConstraints_;
    bool enabled_ = true;
    bool strictMode_ = false;
};

} // namespace chtl