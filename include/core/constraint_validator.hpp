#pragma once

#include "core/chtl_unified_scanner.hpp"
#include "constraints/chtl_global_style_constraint.hpp"
#include "constraints/chtl_local_style_constraint.hpp"
#include "constraints/chtl_local_script_constraint.hpp"
#include "constraints/chtl_script_constraint.hpp"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace chtl {

/**
 * 约束验证结果
 */
struct ConstraintValidationResult {
    bool isValid;                           // 是否通过验证
    std::string fragmentId;                 // 片段标识
    CodeFragmentType fragmentType;          // 片段类型
    std::vector<std::string> errors;        // 错误列表
    std::vector<std::string> warnings;      // 警告列表
    std::string constraintType;             // 使用的约束类型
    
    ConstraintValidationResult() : isValid(true), fragmentType(CodeFragmentType::CHTL_FRAGMENT) {}
};

/**
 * 约束验证上下文
 */
enum class ValidationContext {
    GLOBAL_STYLE_BLOCK,    // 全局样式块
    LOCAL_STYLE_BLOCK,     // 局部样式块  
    LOCAL_SCRIPT_BLOCK,    // 局部脚本块
    NON_LOCAL_SCRIPT_BLOCK,// 非局部脚本块
    RAW_EMBEDDING_BLOCK,   // 原始嵌入块
    GENERAL_CHTL_BLOCK     // 一般CHTL块
};

/**
 * 约束验证器接口
 * 定义统一的验证接口
 */
class IConstraintValidator {
public:
    virtual ~IConstraintValidator() = default;
    virtual ConstraintValidationResult validate(const CodeFragment& fragment) = 0;
    virtual ValidationContext getSupportedContext() const = 0;
    virtual std::string getValidatorName() const = 0;
    virtual bool isApplicable(const CodeFragment& fragment) const = 0;
};

/**
 * 全局样式约束验证器
 */
class GlobalStyleConstraintValidator : public IConstraintValidator {
public:
    GlobalStyleConstraintValidator();
    
    ConstraintValidationResult validate(const CodeFragment& fragment) override;
    ValidationContext getSupportedContext() const override { return ValidationContext::GLOBAL_STYLE_BLOCK; }
    std::string getValidatorName() const override { return "GlobalStyleValidator"; }
    bool isApplicable(const CodeFragment& fragment) const override;
    
private:
    std::unique_ptr<CHtlGlobalStyleConstraint> constraint_;
    bool isGlobalStyleContext(const CodeFragment& fragment) const;
};

/**
 * 局部样式约束验证器
 */
class LocalStyleConstraintValidator : public IConstraintValidator {
public:
    LocalStyleConstraintValidator();
    
    ConstraintValidationResult validate(const CodeFragment& fragment) override;
    ValidationContext getSupportedContext() const override { return ValidationContext::LOCAL_STYLE_BLOCK; }
    std::string getValidatorName() const override { return "LocalStyleValidator"; }
    bool isApplicable(const CodeFragment& fragment) const override;
    
private:
    std::unique_ptr<CHtlLocalStyleConstraint> constraint_;
    bool isLocalStyleContext(const CodeFragment& fragment) const;
};

/**
 * 局部脚本约束验证器
 */
class LocalScriptConstraintValidator : public IConstraintValidator {
public:
    LocalScriptConstraintValidator();
    
    ConstraintValidationResult validate(const CodeFragment& fragment) override;
    ValidationContext getSupportedContext() const override { return ValidationContext::LOCAL_SCRIPT_BLOCK; }
    std::string getValidatorName() const override { return "LocalScriptValidator"; }
    bool isApplicable(const CodeFragment& fragment) const override;
    
private:
    std::unique_ptr<CHtlLocalScriptConstraint> constraint_;
    bool isLocalScriptContext(const CodeFragment& fragment) const;
};

/**
 * 非局部脚本约束验证器
 */
class NonLocalScriptConstraintValidator : public IConstraintValidator {
public:
    NonLocalScriptConstraintValidator();
    
    ConstraintValidationResult validate(const CodeFragment& fragment) override;
    ValidationContext getSupportedContext() const override { return ValidationContext::NON_LOCAL_SCRIPT_BLOCK; }
    std::string getValidatorName() const override { return "NonLocalScriptValidator"; }
    bool isApplicable(const CodeFragment& fragment) const override;
    
private:
    std::unique_ptr<CHtlScriptConstraint> constraint_;
    bool isNonLocalScriptContext(const CodeFragment& fragment) const;
};

/**
 * 约束验证管理器
 * 管理所有约束验证器，提供统一的验证接口
 */
class ConstraintValidationManager {
public:
    ConstraintValidationManager();
    ~ConstraintValidationManager();
    
    // 验证器注册
    void registerValidator(std::unique_ptr<IConstraintValidator> validator);
    void initializeDefaultValidators();
    
    // 片段验证
    ConstraintValidationResult validateFragment(const CodeFragment& fragment);
    std::vector<ConstraintValidationResult> validateAllFragments(const std::vector<CodeFragment>& fragments);
    
    // 批量验证结果分析
    struct ValidationSummary {
        int totalFragments;
        int validFragments;
        int invalidFragments;
        int totalErrors;
        int totalWarnings;
        std::unordered_map<std::string, int> errorsByValidator;
        std::unordered_map<std::string, int> warningsByValidator;
        bool overallValid;
    };
    
    ValidationSummary analyzeValidationResults(const std::vector<ConstraintValidationResult>& results);
    
    // 配置和控制
    void setStrictMode(bool enabled) { strictMode_ = enabled; }
    void setDebugMode(bool enabled) { debugMode_ = enabled; }
    void enableValidator(const std::string& validatorName, bool enabled);
    
    // 查询功能
    std::vector<std::string> getAvailableValidators() const;
    bool isValidatorEnabled(const std::string& validatorName) const;
    
private:
    std::vector<std::unique_ptr<IConstraintValidator>> validators_;
    std::unordered_map<std::string, bool> validatorEnabled_;
    bool strictMode_;
    bool debugMode_;
    
    std::vector<IConstraintValidator*> findApplicableValidators(const CodeFragment& fragment);
    void logValidationResult(const ConstraintValidationResult& result);
};

/**
 * 编译前验证器
 * 在编译开始前对所有片段进行约束验证
 */
class PreCompilationValidator {
public:
    PreCompilationValidator();
    
    // 验证配置
    void setValidationManager(std::shared_ptr<ConstraintValidationManager> manager);
    void setFailOnFirstError(bool enabled) { failOnFirstError_ = enabled; }
    void setSkipValidationForType(CodeFragmentType type, bool skip);
    
    // 验证执行
    struct PreValidationResult {
        bool canProceedToCompilation;           // 是否可以继续编译
        std::vector<ConstraintValidationResult> validationResults;
        ConstraintValidationManager::ValidationSummary summary;
        std::vector<std::string> blockerErrors;  // 阻止编译的严重错误
        std::string recommendedAction;           // 推荐的修复操作
    };
    
    PreValidationResult validateBeforeCompilation(const std::vector<CodeFragment>& fragments);
    
    // 错误分类和处理
    enum class ErrorSeverity {
        WARNING,      // 警告，不阻止编译
        ERROR,        // 错误，建议修复但可选择忽略
        CRITICAL      // 严重错误，必须修复才能编译
    };
    
    ErrorSeverity classifyError(const std::string& error, ValidationContext context);
    std::vector<std::string> generateFixSuggestions(const ConstraintValidationResult& result);
    
private:
    std::shared_ptr<ConstraintValidationManager> validationManager_;
    std::unordered_map<CodeFragmentType, bool> skipValidationFor_;
    bool failOnFirstError_;
    bool debugMode_;
    
    bool isCriticalError(const std::string& error, ValidationContext context);
    std::string generateRecommendation(const std::vector<ConstraintValidationResult>& results);
};

/**
 * 上下文感知验证器
 * 根据代码片段的上下文信息选择合适的验证策略
 */
class ContextAwareValidator {
public:
    ContextAwareValidator();
    
    // 上下文检测
    ValidationContext detectContext(const CodeFragment& fragment);
    std::string analyzeFragmentContext(const CodeFragment& fragment);
    
    // 上下文特定验证
    bool shouldValidateInContext(ValidationContext context, const std::string& syntaxElement);
    std::vector<std::string> getContextSpecificRules(ValidationContext context);
    
    // 继承和覆盖规则
    void setContextInheritance(ValidationContext child, ValidationContext parent);
    bool inheritsValidationRules(ValidationContext context, const std::string& rule);
    
private:
    std::unordered_map<ValidationContext, std::vector<std::string>> contextRules_;
    std::unordered_map<ValidationContext, ValidationContext> contextInheritance_;
    
    void initializeContextRules();
    bool isInStyleBlock(const CodeFragment& fragment);
    bool isInScriptBlock(const CodeFragment& fragment);
    bool isLocalBlock(const CodeFragment& fragment);
};

} // namespace chtl