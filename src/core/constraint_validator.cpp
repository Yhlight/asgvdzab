#include "core/constraint_validator.hpp"
#include <iostream>
#include <algorithm>
#include <regex>

namespace chtl {

// ==================== GlobalStyleConstraintValidator 实现 ====================

GlobalStyleConstraintValidator::GlobalStyleConstraintValidator() {
    constraint_ = std::make_unique<CHtlGlobalStyleConstraint>();
}

ConstraintValidationResult GlobalStyleConstraintValidator::validate(const CodeFragment& fragment) {
    ConstraintValidationResult result;
    result.fragmentId = "fragment_" + std::to_string(fragment.startPos);
    result.fragmentType = fragment.type;
    result.constraintType = getValidatorName();
    
    if (!isApplicable(fragment)) {
        result.isValid = true;
        return result;
    }
    
    // 使用现有的约束类进行验证
    try {
        // 简化验证：检查基本语法元素
        std::string content = fragment.content;
        
        // 检查模板变量
        std::regex templateVarRegex(R"(\{\{[^}]+\}\})");
        if (std::regex_search(content, templateVarRegex)) {
            // 模板变量在全局样式中是允许的
        }
        
        // 检查原始嵌入
        std::regex rawEmbeddingRegex(R"(\[Origin\]\s*@\w+)");
        if (std::regex_search(content, rawEmbeddingRegex)) {
            // 原始嵌入是允许的
        }
        
        // 检查不允许的局部语法
        if (content.find("script {") != std::string::npos) {
            result.isValid = false;
            result.errors.push_back("全局样式块中不允许使用script语法");
        }
        
        if (content.find("{{&") != std::string::npos) {
            result.isValid = false;
            result.errors.push_back("全局样式块中不允许使用局部特供语法");
        }
        
    } catch (const std::exception& e) {
        result.isValid = false;
        result.errors.push_back("验证过程中发生异常: " + std::string(e.what()));
    }
    
    return result;
}

bool GlobalStyleConstraintValidator::isApplicable(const CodeFragment& fragment) const {
    return fragment.type == CodeFragmentType::CSS_FRAGMENT || 
           isGlobalStyleContext(fragment);
}

bool GlobalStyleConstraintValidator::isGlobalStyleContext(const CodeFragment& fragment) const {
    // 检查上下文是否为全局样式
    return fragment.context.find("global_style") != std::string::npos ||
           fragment.content.find("[Template]") != std::string::npos ||
           fragment.content.find("[Custom]") != std::string::npos;
}

// ==================== LocalStyleConstraintValidator 实现 ====================

LocalStyleConstraintValidator::LocalStyleConstraintValidator() {
    constraint_ = std::make_unique<CHtlLocalStyleConstraint>();
}

ConstraintValidationResult LocalStyleConstraintValidator::validate(const CodeFragment& fragment) {
    ConstraintValidationResult result;
    result.fragmentId = "fragment_" + std::to_string(fragment.startPos);
    result.fragmentType = fragment.type;
    result.constraintType = getValidatorName();
    
    if (!isApplicable(fragment)) {
        result.isValid = true;
        return result;
    }
    
    try {
        std::string content = fragment.content;
        
        // 检查允许的语法元素
        std::regex chtlVarRegex(R"(\{\{[^}]+\}\})");
        std::regex styleGroupRegex(R"(@Style\s+\w+)");
        std::regex deleteRegex(R"(delete\s+\w+)");
        std::regex inheritRegex(R"(inherit\s+\w+)");
        
        // 局部样式可以使用这些语法
        
        // 检查不允许的语法
        if (content.find("script {") != std::string::npos) {
            result.isValid = false;
            result.errors.push_back("局部样式块中不允许使用script语法");
        }
        
    } catch (const std::exception& e) {
        result.isValid = false;
        result.errors.push_back("验证过程中发生异常: " + std::string(e.what()));
    }
    
    return result;
}

bool LocalStyleConstraintValidator::isApplicable(const CodeFragment& fragment) const {
    return fragment.type == CodeFragmentType::CHTL_FRAGMENT && 
           isLocalStyleContext(fragment);
}

bool LocalStyleConstraintValidator::isLocalStyleContext(const CodeFragment& fragment) const {
    return fragment.context.find("local_style") != std::string::npos ||
           fragment.content.find("style {") != std::string::npos;
}

// ==================== LocalScriptConstraintValidator 实现 ====================

LocalScriptConstraintValidator::LocalScriptConstraintValidator() {
    constraint_ = std::make_unique<CHtlLocalScriptConstraint>();
}

ConstraintValidationResult LocalScriptConstraintValidator::validate(const CodeFragment& fragment) {
    ConstraintValidationResult result;
    result.fragmentId = "fragment_" + std::to_string(fragment.startPos);
    result.fragmentType = fragment.type;
    result.constraintType = getValidatorName();
    
    if (!isApplicable(fragment)) {
        result.isValid = true;
        return result;
    }
    
    try {
        std::string content = fragment.content;
        
        // 检查允许的CHTL语法
        std::regex chtlVarRegex(R"(\{\{[^}]+\}\})");
        std::regex varGroupRegex(R"(\w+Group\([^)]+\))");
        std::regex fromClauseRegex(R"(\w+\s+from\s+"[^"]+")");
        std::regex commentRegex(R"(--.*)");
        std::regex rawEmbeddingRegex(R"(\[Origin\]\s*@\w+)");
        
        // 这些在局部脚本中都是允许的
        
        // 检查特供语法 {{&}} 也是允许的
        if (content.find("{{&") != std::string::npos) {
            // 特供语法允许
        }
        
    } catch (const std::exception& e) {
        result.isValid = false;
        result.errors.push_back("验证过程中发生异常: " + std::string(e.what()));
    }
    
    return result;
}

bool LocalScriptConstraintValidator::isApplicable(const CodeFragment& fragment) const {
    return fragment.type == CodeFragmentType::CHTL_JS_FRAGMENT && 
           isLocalScriptContext(fragment);
}

bool LocalScriptConstraintValidator::isLocalScriptContext(const CodeFragment& fragment) const {
    return fragment.context.find("local_script") != std::string::npos ||
           fragment.content.find("script {") != std::string::npos;
}

// ==================== NonLocalScriptConstraintValidator 实现 ====================

NonLocalScriptConstraintValidator::NonLocalScriptConstraintValidator() {
    constraint_ = std::make_unique<CHtlScriptConstraint>();
}

ConstraintValidationResult NonLocalScriptConstraintValidator::validate(const CodeFragment& fragment) {
    ConstraintValidationResult result;
    result.fragmentId = "fragment_" + std::to_string(fragment.startPos);
    result.fragmentType = fragment.type;
    result.constraintType = getValidatorName();
    
    if (!isApplicable(fragment)) {
        result.isValid = true;
        return result;
    }
    
    try {
        std::string content = fragment.content;
        
        // 非局部脚本禁止CHTL语法（除了注释和原始嵌入）
        std::regex chtlVarRegex(R"(\{\{[^}]+\}\})");
        std::regex commentRegex(R"(--.*)");
        std::regex rawEmbeddingRegex(R"(\[Origin\]\s*@\w+)");
        
        if (std::regex_search(content, chtlVarRegex)) {
            // 检查是否在注释或原始嵌入中
            // 简化处理：如果有变量引用且不在允许的上下文中，标记为错误
            if (content.find("--") == std::string::npos && 
                content.find("[Origin]") == std::string::npos) {
                result.isValid = false;
                result.errors.push_back("非局部脚本中不允许使用CHTL变量语法");
            }
        }
        
    } catch (const std::exception& e) {
        result.isValid = false;
        result.errors.push_back("验证过程中发生异常: " + std::string(e.what()));
    }
    
    return result;
}

bool NonLocalScriptConstraintValidator::isApplicable(const CodeFragment& fragment) const {
    return fragment.type == CodeFragmentType::JS_FRAGMENT && 
           isNonLocalScriptContext(fragment);
}

bool NonLocalScriptConstraintValidator::isNonLocalScriptContext(const CodeFragment& fragment) const {
    return fragment.context.find("script") != std::string::npos &&
           fragment.context.find("local_script") == std::string::npos;
}

// ==================== ConstraintValidationManager 实现 ====================

ConstraintValidationManager::ConstraintValidationManager() 
    : strictMode_(false), debugMode_(false) {
}

ConstraintValidationManager::~ConstraintValidationManager() = default;

void ConstraintValidationManager::registerValidator(std::unique_ptr<IConstraintValidator> validator) {
    if (validator) {
        std::string name = validator->getValidatorName();
        validatorEnabled_[name] = true;
        validators_.push_back(std::move(validator));
    }
}

void ConstraintValidationManager::initializeDefaultValidators() {
    registerValidator(std::make_unique<GlobalStyleConstraintValidator>());
    registerValidator(std::make_unique<LocalStyleConstraintValidator>());
    registerValidator(std::make_unique<LocalScriptConstraintValidator>());
    registerValidator(std::make_unique<NonLocalScriptConstraintValidator>());
}

ConstraintValidationResult ConstraintValidationManager::validateFragment(const CodeFragment& fragment) {
    auto applicableValidators = findApplicableValidators(fragment);
    
    if (applicableValidators.empty()) {
        ConstraintValidationResult result;
        result.isValid = true;
        result.fragmentId = "fragment_" + std::to_string(fragment.startPos);
        result.fragmentType = fragment.type;
        result.constraintType = "none";
        return result;
    }
    
    // 使用第一个适用的验证器
    auto result = applicableValidators[0]->validate(fragment);
    
    if (debugMode_) {
        logValidationResult(result);
    }
    
    return result;
}

std::vector<ConstraintValidationResult> ConstraintValidationManager::validateAllFragments(
    const std::vector<CodeFragment>& fragments) {
    std::vector<ConstraintValidationResult> results;
    
    for (const auto& fragment : fragments) {
        auto result = validateFragment(fragment);
        results.push_back(result);
        
        if (strictMode_ && !result.isValid) {
            break; // 严格模式下遇到第一个错误就停止
        }
    }
    
    return results;
}

ConstraintValidationManager::ValidationSummary ConstraintValidationManager::analyzeValidationResults(
    const std::vector<ConstraintValidationResult>& results) {
    ValidationSummary summary;
    summary.totalFragments = results.size();
    summary.validFragments = 0;
    summary.invalidFragments = 0;
    summary.totalErrors = 0;
    summary.totalWarnings = 0;
    summary.overallValid = true;
    
    for (const auto& result : results) {
        if (result.isValid) {
            summary.validFragments++;
        } else {
            summary.invalidFragments++;
            summary.overallValid = false;
        }
        
        summary.totalErrors += result.errors.size();
        summary.totalWarnings += result.warnings.size();
        
        // 统计各验证器的错误数
        if (!result.errors.empty()) {
            summary.errorsByValidator[result.constraintType] += result.errors.size();
        }
        if (!result.warnings.empty()) {
            summary.warningsByValidator[result.constraintType] += result.warnings.size();
        }
    }
    
    return summary;
}

void ConstraintValidationManager::enableValidator(const std::string& validatorName, bool enabled) {
    validatorEnabled_[validatorName] = enabled;
}

std::vector<std::string> ConstraintValidationManager::getAvailableValidators() const {
    std::vector<std::string> names;
    for (const auto& validator : validators_) {
        names.push_back(validator->getValidatorName());
    }
    return names;
}

bool ConstraintValidationManager::isValidatorEnabled(const std::string& validatorName) const {
    auto it = validatorEnabled_.find(validatorName);
    return it != validatorEnabled_.end() && it->second;
}

std::vector<IConstraintValidator*> ConstraintValidationManager::findApplicableValidators(
    const CodeFragment& fragment) {
    std::vector<IConstraintValidator*> applicable;
    
    for (const auto& validator : validators_) {
        if (isValidatorEnabled(validator->getValidatorName()) && 
            validator->isApplicable(fragment)) {
            applicable.push_back(validator.get());
        }
    }
    
    return applicable;
}

void ConstraintValidationManager::logValidationResult(const ConstraintValidationResult& result) {
    if (debugMode_) {
        std::cout << "[ConstraintValidation] Fragment " << result.fragmentId 
                  << " validated by " << result.constraintType 
                  << " - " << (result.isValid ? "PASS" : "FAIL") << std::endl;
        
        if (!result.isValid) {
            for (const auto& error : result.errors) {
                std::cout << "  Error: " << error << std::endl;
            }
        }
    }
}

// ==================== PreCompilationValidator 实现 ====================

PreCompilationValidator::PreCompilationValidator() 
    : failOnFirstError_(false), debugMode_(false) {
}

void PreCompilationValidator::setValidationManager(std::shared_ptr<ConstraintValidationManager> manager) {
    validationManager_ = manager;
}

void PreCompilationValidator::setSkipValidationForType(CodeFragmentType type, bool skip) {
    skipValidationFor_[type] = skip;
}

PreCompilationValidator::PreValidationResult PreCompilationValidator::validateBeforeCompilation(
    const std::vector<CodeFragment>& fragments) {
    PreValidationResult result;
    result.canProceedToCompilation = true;
    
    if (!validationManager_) {
        result.canProceedToCompilation = false;
        result.blockerErrors.push_back("验证管理器未初始化");
        result.recommendedAction = "初始化验证管理器";
        return result;
    }
    
    // 过滤需要跳过的片段类型
    std::vector<CodeFragment> filteredFragments;
    for (const auto& fragment : fragments) {
        auto it = skipValidationFor_.find(fragment.type);
        if (it == skipValidationFor_.end() || !it->second) {
            filteredFragments.push_back(fragment);
        }
    }
    
    // 执行验证
    result.validationResults = validationManager_->validateAllFragments(filteredFragments);
    result.summary = validationManager_->analyzeValidationResults(result.validationResults);
    
    // 分析结果
    for (const auto& validationResult : result.validationResults) {
        if (!validationResult.isValid) {
            for (const auto& error : validationResult.errors) {
                auto severity = classifyError(error, ValidationContext::GENERAL_CHTL_BLOCK);
                if (severity == ErrorSeverity::CRITICAL) {
                    result.blockerErrors.push_back(error);
                    result.canProceedToCompilation = false;
                }
                
                if (failOnFirstError_) {
                    result.canProceedToCompilation = false;
                    break;
                }
            }
        }
    }
    
    result.recommendedAction = generateRecommendation(result.validationResults);
    
    return result;
}

PreCompilationValidator::ErrorSeverity PreCompilationValidator::classifyError(
    const std::string& error, ValidationContext context) {
    
    if (isCriticalError(error, context)) {
        return ErrorSeverity::CRITICAL;
    }
    
    if (error.find("不允许") != std::string::npos || 
        error.find("语法错误") != std::string::npos) {
        return ErrorSeverity::ERROR;
    }
    
    return ErrorSeverity::WARNING;
}

std::vector<std::string> PreCompilationValidator::generateFixSuggestions(
    const ConstraintValidationResult& result) {
    std::vector<std::string> suggestions;
    
    for (const auto& error : result.errors) {
        if (error.find("不允许使用") != std::string::npos) {
            suggestions.push_back("请检查语法是否在正确的上下文中使用");
        } else if (error.find("语法错误") != std::string::npos) {
            suggestions.push_back("请检查语法格式是否正确");
        } else {
            suggestions.push_back("请参考CHTL语法文档");
        }
    }
    
    return suggestions;
}

bool PreCompilationValidator::isCriticalError(const std::string& error, ValidationContext context) {
    return error.find("语法错误") != std::string::npos ||
           error.find("编译失败") != std::string::npos;
}

std::string PreCompilationValidator::generateRecommendation(
    const std::vector<ConstraintValidationResult>& results) {
    
    int errorCount = 0;
    int warningCount = 0;
    
    for (const auto& result : results) {
        errorCount += result.errors.size();
        warningCount += result.warnings.size();
    }
    
    if (errorCount == 0 && warningCount == 0) {
        return "所有验证通过，可以继续编译";
    } else if (errorCount > 0) {
        return "修复 " + std::to_string(errorCount) + " 个错误后重试";
    } else {
        return "存在 " + std::to_string(warningCount) + " 个警告，建议检查";
    }
}

// ==================== ContextAwareValidator 实现 ====================

ContextAwareValidator::ContextAwareValidator() {
    initializeContextRules();
}

ValidationContext ContextAwareValidator::detectContext(const CodeFragment& fragment) {
    if (isInStyleBlock(fragment)) {
        if (isLocalBlock(fragment)) {
            return ValidationContext::LOCAL_STYLE_BLOCK;
        } else {
            return ValidationContext::GLOBAL_STYLE_BLOCK;
        }
    } else if (isInScriptBlock(fragment)) {
        if (isLocalBlock(fragment)) {
            return ValidationContext::LOCAL_SCRIPT_BLOCK;
        } else {
            return ValidationContext::NON_LOCAL_SCRIPT_BLOCK;
        }
    } else if (fragment.content.find("[Origin]") != std::string::npos) {
        return ValidationContext::RAW_EMBEDDING_BLOCK;
    }
    
    return ValidationContext::GENERAL_CHTL_BLOCK;
}

std::string ContextAwareValidator::analyzeFragmentContext(const CodeFragment& fragment) {
    auto context = detectContext(fragment);
    
    switch (context) {
        case ValidationContext::GLOBAL_STYLE_BLOCK:
            return "全局样式块";
        case ValidationContext::LOCAL_STYLE_BLOCK:
            return "局部样式块";
        case ValidationContext::LOCAL_SCRIPT_BLOCK:
            return "局部脚本块";
        case ValidationContext::NON_LOCAL_SCRIPT_BLOCK:
            return "非局部脚本块";
        case ValidationContext::RAW_EMBEDDING_BLOCK:
            return "原始嵌入块";
        default:
            return "一般CHTL块";
    }
}

bool ContextAwareValidator::shouldValidateInContext(ValidationContext context, const std::string& syntaxElement) {
    auto it = contextRules_.find(context);
    if (it != contextRules_.end()) {
        const auto& rules = it->second;
        return std::find(rules.begin(), rules.end(), syntaxElement) != rules.end();
    }
    return false;
}

std::vector<std::string> ContextAwareValidator::getContextSpecificRules(ValidationContext context) {
    auto it = contextRules_.find(context);
    if (it != contextRules_.end()) {
        return it->second;
    }
    return {};
}

void ContextAwareValidator::setContextInheritance(ValidationContext child, ValidationContext parent) {
    contextInheritance_[child] = parent;
}

bool ContextAwareValidator::inheritsValidationRules(ValidationContext context, const std::string& rule) {
    auto it = contextInheritance_.find(context);
    if (it != contextInheritance_.end()) {
        ValidationContext parent = it->second;
        return shouldValidateInContext(parent, rule);
    }
    return false;
}

void ContextAwareValidator::initializeContextRules() {
    // 全局样式块规则
    contextRules_[ValidationContext::GLOBAL_STYLE_BLOCK] = {
        "template_variable", "custom_variable", "style_group", 
        "template_style_group", "custom_style_group", "delete_property",
        "delete_inherit", "inherit", "generator_comment", "full_prefix_name",
        "raw_embedding", "namespace_from"
    };
    
    // 局部样式块规则
    contextRules_[ValidationContext::LOCAL_STYLE_BLOCK] = {
        "template_variable", "custom_variable", "custom_variable_specialization",
        "template_style_group", "custom_style_group", "valueless_style_group",
        "custom_style_group_specialization", "delete_property", "delete_inherit",
        "inherit", "generator_comment", "full_prefix_name", "raw_embedding",
        "namespace_from"
    };
    
    // 局部脚本块规则
    contextRules_[ValidationContext::LOCAL_SCRIPT_BLOCK] = {
        "template_variable", "custom_variable_group", "variable_group_specialization",
        "namespace_from", "generator_comment", "raw_embedding", "special_syntax"
    };
    
    // 非局部脚本块规则
    contextRules_[ValidationContext::NON_LOCAL_SCRIPT_BLOCK] = {
        "generator_comment", "raw_embedding"
    };
    
    // 原始嵌入块规则
    contextRules_[ValidationContext::RAW_EMBEDDING_BLOCK] = {
        "any_content" // 原始嵌入允许任何内容
    };
}

bool ContextAwareValidator::isInStyleBlock(const CodeFragment& fragment) {
    return fragment.content.find("style {") != std::string::npos ||
           fragment.content.find("@Style") != std::string::npos ||
           fragment.type == CodeFragmentType::CSS_FRAGMENT;
}

bool ContextAwareValidator::isInScriptBlock(const CodeFragment& fragment) {
    return fragment.content.find("script {") != std::string::npos ||
           fragment.type == CodeFragmentType::JS_FRAGMENT ||
           fragment.type == CodeFragmentType::CHTL_JS_FRAGMENT;
}

bool ContextAwareValidator::isLocalBlock(const CodeFragment& fragment) {
    return fragment.context.find("local") != std::string::npos ||
           fragment.content.find("script {") != std::string::npos ||
           fragment.content.find("style {") != std::string::npos;
}

} // namespace chtl