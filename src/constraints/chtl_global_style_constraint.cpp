#include "constraints/chtl_global_style_constraint.hpp"
#include <regex>
#include <sstream>
#include <algorithm>
#include <iostream>

using namespace chtl;

CHtlGlobalStyleConstraint::CHtlGlobalStyleConstraint() {
    // 初始化禁止的局部样式特性模式
    forbiddenLocalPatterns_.insert("&");           // 伪类简化语法
    forbiddenLocalPatterns_.insert("@media");      // 媒体查询(局部样式特性)
    forbiddenLocalPatterns_.insert("@keyframes");  // 动画关键帧(局部样式特性)
    forbiddenLocalPatterns_.insert(":hover");      // 伪类(应该在局部使用)
    forbiddenLocalPatterns_.insert(":focus");      // 伪类(应该在局部使用)
    forbiddenLocalPatterns_.insert("::before");    // 伪元素(应该在局部使用)
    forbiddenLocalPatterns_.insert("::after");     // 伪元素(应该在局部使用)
}

GlobalStyleConstraintResult CHtlGlobalStyleConstraint::validateGlobalStyleBlock(
    const std::string& styleContent,
    const std::vector<std::string>& availableNamespaces,
    const std::vector<std::string>& availableTemplateVars,
    const std::vector<std::string>& availableCustomVars,
    const std::vector<std::string>& availableStyleGroups) {
    
    GlobalStyleConstraintResult result;
    
    // 解析样式内容为语句
    auto statements = parseStyleStatements(styleContent);
    
    size_t lineNumber = 1;
    for (const auto& statement : statements) {
        // 跳过空行和纯空白
        if (statement.empty() || std::all_of(statement.begin(), statement.end(), ::isspace)) {
            lineNumber++;
            continue;
        }
        
        std::string trimmed = statement;
        // 移除前后空白
        trimmed.erase(0, trimmed.find_first_not_of(" \t\n\r"));
        trimmed.erase(trimmed.find_last_not_of(" \t\n\r") + 1);
        
        // 检查是否为允许的元素类型
        auto allowedTypes = checkAllowedElement(trimmed);
        
        if (allowedTypes.empty()) {
            // 检查是否为禁止的局部样式特性
            auto forbiddenFeatures = checkForbiddenLocalStyleFeatures(trimmed);
            if (!forbiddenFeatures.empty()) {
                for (const auto& feature : forbiddenFeatures) {
                    addViolation(result, ConstraintViolationType::LOCAL_STYLE_FEATURE,
                        "全局样式块中不允许使用局部样式特性: " + feature,
                        trimmed, lineNumber, 0);
                }
            } else {
                addViolation(result, ConstraintViolationType::FORBIDDEN_ELEMENT,
                    "全局样式块中不允许的语法元素",
                    trimmed, lineNumber, 0);
            }
        } else {
            // 验证具体的语法元素
            for (auto type : allowedTypes) {
                switch (type) {
                    case GlobalStyleAllowedElement::TEMPLATE_VARIABLE:
                        if (!validateTemplateVariableReference(trimmed)) {
                            addViolation(result, ConstraintViolationType::INVALID_VARIABLE_REFERENCE,
                                "无效的模板变量引用", trimmed, lineNumber, 0);
                        } else {
                            result.usedTemplateVariables.insert(trimmed);
                        }
                        break;
                        
                    case GlobalStyleAllowedElement::CUSTOM_VARIABLE:
                        if (!validateCustomVariableReference(trimmed)) {
                            addViolation(result, ConstraintViolationType::INVALID_VARIABLE_REFERENCE,
                                "无效的自定义变量引用", trimmed, lineNumber, 0);
                        } else {
                            result.usedCustomVariables.insert(trimmed);
                        }
                        break;
                        
                    case GlobalStyleAllowedElement::TEMPLATE_STYLE_GROUP:
                    case GlobalStyleAllowedElement::CUSTOM_STYLE_GROUP:
                        if (!validateStyleGroupReference(trimmed)) {
                            addViolation(result, ConstraintViolationType::INVALID_STYLE_GROUP_REFERENCE,
                                "无效的样式组引用", trimmed, lineNumber, 0);
                        } else {
                            result.usedStyleGroups.insert(trimmed);
                        }
                        break;
                        
                    case GlobalStyleAllowedElement::DELETE_PROPERTY:
                    case GlobalStyleAllowedElement::DELETE_INHERITANCE:
                        if (!validateDeleteStatement(trimmed)) {
                            addViolation(result, ConstraintViolationType::INVALID_SYNTAX,
                                "无效的delete语句", trimmed, lineNumber, 0);
                        }
                        break;
                        
                    case GlobalStyleAllowedElement::INHERITANCE:
                        if (!validateInheritStatement(trimmed)) {
                            addViolation(result, ConstraintViolationType::INVALID_SYNTAX,
                                "无效的继承语句", trimmed, lineNumber, 0);
                        } else {
                            result.inheritances.insert(trimmed);
                        }
                        break;
                        
                    case GlobalStyleAllowedElement::FROM_NAMESPACE:
                        if (!validateFromClause(trimmed)) {
                            addViolation(result, ConstraintViolationType::MALFORMED_FROM_CLAUSE,
                                "格式错误的from子句", trimmed, lineNumber, 0);
                        }
                        break;
                        
                    case GlobalStyleAllowedElement::FULL_QUALIFIED_NAME:
                        if (!validateFullQualifiedName(trimmed)) {
                            addViolation(result, ConstraintViolationType::INVALID_SYNTAX,
                                "无效的全缀名", trimmed, lineNumber, 0);
                        }
                        break;
                        
                    default:
                        // CSS选择器、CSS属性、生成器注释、原始嵌入等基本都允许
                        break;
                }
            }
        }
        
        lineNumber++;
    }
    
    result.isValid = result.violations.empty();
    return result;
}

std::vector<GlobalStyleAllowedElement> CHtlGlobalStyleConstraint::checkAllowedElement(const std::string& element) {
    std::vector<GlobalStyleAllowedElement> allowedTypes;
    
    // 检查生成器注释
    if (isGeneratorComment(element)) {
        allowedTypes.push_back(GlobalStyleAllowedElement::GENERATOR_COMMENT);
    }
    
    // 检查原始嵌入 (优先检查，避免与其他语法混淆)
    if (isRawEmbedding(element)) {
        allowedTypes.push_back(GlobalStyleAllowedElement::RAW_EMBEDDING);
        return allowedTypes; // 原始嵌入是特殊存在，直接返回
    }
    
    // 检查变量引用
    if (isVariableReference(element)) {
        // 判断是模板变量还是自定义变量
        if (element.find("[Template]") != std::string::npos) {
            allowedTypes.push_back(GlobalStyleAllowedElement::TEMPLATE_VARIABLE);
        } else if (element.find("[Custom]") != std::string::npos) {
            allowedTypes.push_back(GlobalStyleAllowedElement::CUSTOM_VARIABLE);
        } else {
            // 默认可能是模板变量或自定义变量
            allowedTypes.push_back(GlobalStyleAllowedElement::TEMPLATE_VARIABLE);
            allowedTypes.push_back(GlobalStyleAllowedElement::CUSTOM_VARIABLE);
        }
    }
    
    // 检查样式组引用
    if (isStyleGroupReference(element)) {
        if (element.find("[Template]") != std::string::npos) {
            allowedTypes.push_back(GlobalStyleAllowedElement::TEMPLATE_STYLE_GROUP);
        } else if (element.find("[Custom]") != std::string::npos) {
            allowedTypes.push_back(GlobalStyleAllowedElement::CUSTOM_STYLE_GROUP);
        } else {
            allowedTypes.push_back(GlobalStyleAllowedElement::TEMPLATE_STYLE_GROUP);
            allowedTypes.push_back(GlobalStyleAllowedElement::CUSTOM_STYLE_GROUP);
        }
    }
    
    // 检查delete语句
    if (isDeleteStatement(element)) {
        if (element.find("@Style") != std::string::npos || element.find("@Var") != std::string::npos) {
            allowedTypes.push_back(GlobalStyleAllowedElement::DELETE_INHERITANCE);
        } else {
            allowedTypes.push_back(GlobalStyleAllowedElement::DELETE_PROPERTY);
        }
    }
    
    // 检查继承语句
    if (isInheritStatement(element)) {
        allowedTypes.push_back(GlobalStyleAllowedElement::INHERITANCE);
    }
    
    // 检查from子句
    if (isFromClause(element)) {
        allowedTypes.push_back(GlobalStyleAllowedElement::FROM_NAMESPACE);
    }
    
    // 检查全缀名
    if (validateFullQualifiedName(element)) {
        allowedTypes.push_back(GlobalStyleAllowedElement::FULL_QUALIFIED_NAME);
    }
    
    // 检查CSS选择器
    if (isCSSSelector(element)) {
        allowedTypes.push_back(GlobalStyleAllowedElement::CSS_SELECTOR);
    }
    
    // 检查CSS属性
    if (isCSSProperty(element)) {
        allowedTypes.push_back(GlobalStyleAllowedElement::CSS_PROPERTY);
    }
    
    return allowedTypes;
}

bool CHtlGlobalStyleConstraint::validateTemplateVariableReference(const std::string& varReference) {
    // 模板变量格式: 
    // 1. VariableGroupName(variableName) - 简写形式
    // 2. @Var VariableGroupName(variableName) - 完整形式
    std::regex templateVarPattern(R"((?:@Var\s+)?([A-Za-z_][A-Za-z0-9_]*)\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*\))");
    return std::regex_search(varReference, templateVarPattern);
}

bool CHtlGlobalStyleConstraint::validateCustomVariableReference(const std::string& varReference) {
    // 自定义变量格式: 
    // 1. VariableGroupName(variableName) - 简写形式
    // 2. @Var VariableGroupName(variableName) - 完整形式
    // 3. VariableGroupName(variableName = value) - 特例化
    std::regex customVarPattern(R"((?:@Var\s+)?([A-Za-z_][A-Za-z0-9_]*)\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*(?:=\s*([^)]+))?\s*\))");
    return std::regex_search(varReference, customVarPattern);
}

bool CHtlGlobalStyleConstraint::validateStyleGroupReference(const std::string& styleGroupRef) {
    // 样式组格式: @Style StyleGroupName 或 [Template/Custom] @Style StyleGroupName
    // 也可能是样式组继承: @Style BaseStyleGroup; (不带任何前缀)
    std::regex styleGroupPattern(R"((?:\[(Template|Custom)\]\s+)?@Style\s+([A-Za-z_][A-Za-z0-9_]*)(?:\s*;)?)");
    return std::regex_search(styleGroupRef, styleGroupPattern);
}

bool CHtlGlobalStyleConstraint::validateDeleteStatement(const std::string& deleteStatement) {
    // delete语句格式: delete property1, property2 或 delete @Style StyleGroupName
    std::regex deletePattern(R"(delete\s+(.+))");
    return std::regex_match(deleteStatement, deletePattern);
}

bool CHtlGlobalStyleConstraint::validateInheritStatement(const std::string& inheritStatement) {
    // 继承语句格式: inherit @Style StyleGroupName
    std::regex inheritPattern(R"(inherit\s+@Style\s+([A-Za-z_][A-Za-z0-9_]*))");
    return std::regex_match(inheritStatement, inheritPattern);
}

bool CHtlGlobalStyleConstraint::validateFromClause(const std::string& fromClause) {
    // from子句格式: ElementName from file.path (用于导入)
    // 注意：CHTL中的from主要用于导入，格式是从文件路径导入
    std::regex fromPattern(R"(([A-Za-z_][A-Za-z0-9_]*)\s+from\s+([A-Za-z_][A-Za-z0-9_./-]*))");
    return std::regex_search(fromClause, fromPattern);
}

bool CHtlGlobalStyleConstraint::validateFullQualifiedName(const std::string& fullQualifiedName) {
    // 全缀名格式: [Custom] @Style StyleName 或 [Template] @Style StyleName
    // 这是CHTL中用于避免命名冲突的完整限定名语法
    std::regex fqnPattern(R"(\[(Custom|Template)\]\s+@(Style|Element|Var)\s+([A-Za-z_][A-Za-z0-9_]*))");
    return std::regex_search(fullQualifiedName, fqnPattern);
}

std::vector<std::string> CHtlGlobalStyleConstraint::checkForbiddenLocalStyleFeatures(const std::string& styleContent) {
    std::vector<std::string> forbiddenFeatures;
    
    for (const auto& pattern : forbiddenLocalPatterns_) {
        if (styleContent.find(pattern) != std::string::npos) {
            forbiddenFeatures.push_back(pattern);
        }
    }
    
    // 检查嵌套的style块
    if (styleContent.find("style{") != std::string::npos || 
        styleContent.find("style {") != std::string::npos) {
        forbiddenFeatures.push_back("nested style block");
    }
    
    return forbiddenFeatures;
}

std::vector<std::string> CHtlGlobalStyleConstraint::parseStyleStatements(const std::string& content) {
    std::vector<std::string> statements;
    std::istringstream stream(content);
    std::string line;
    
    while (std::getline(stream, line)) {
        statements.push_back(line);
    }
    
    return statements;
}

bool CHtlGlobalStyleConstraint::isCSSSelector(const std::string& statement) {
    // CSS选择器模式: .class, #id, element, element.class, element#id, 等
    // 排除CSS属性(包含冒号)
    if (statement.find(':') != std::string::npos && 
        statement.find("::") == std::string::npos &&  // 允许伪元素选择器
        statement.find("://") == std::string::npos) {  // 排除URL
        return false;
    }
    
    std::regex selectorPattern(R"(^[.#]?[A-Za-z_][A-Za-z0-9_-]*(?:[.#][A-Za-z_][A-Za-z0-9_-]*)*(?:\s*[>+~]\s*[.#]?[A-Za-z_][A-Za-z0-9_-]*)*\s*\{?)");
    return std::regex_match(statement, selectorPattern);
}

bool CHtlGlobalStyleConstraint::isCSSProperty(const std::string& statement) {
    // CSS属性模式: property: value; 或 property: value
    std::regex propertyPattern(R"([A-Za-z-]+\s*:\s*.+;?)");
    return std::regex_match(statement, propertyPattern);
}

bool CHtlGlobalStyleConstraint::isVariableReference(const std::string& statement) {
    // 变量引用模式: 
    // 1. VariableGroup(variableName) 格式 - 简写形式
    // 2. @Var VariableGroup(variableName) 格式 - 完整形式
    return (statement.find("@Var") != std::string::npos || 
           (statement.find('(') != std::string::npos && statement.find(')') != std::string::npos &&
            statement.find("function") == std::string::npos)); // 排除函数定义
}

bool CHtlGlobalStyleConstraint::isStyleGroupReference(const std::string& statement) {
    // 样式组引用模式: @Style
    return statement.find("@Style") != std::string::npos;
}

bool CHtlGlobalStyleConstraint::isDeleteStatement(const std::string& statement) {
    // delete语句模式
    return statement.find("delete") == 0;
}

bool CHtlGlobalStyleConstraint::isInheritStatement(const std::string& statement) {
    // inherit语句模式
    return statement.find("inherit") == 0;
}

bool CHtlGlobalStyleConstraint::isFromClause(const std::string& statement) {
    // from子句模式
    return statement.find(" from ") != std::string::npos;
}

bool CHtlGlobalStyleConstraint::isGeneratorComment(const std::string& statement) {
    // 生成器注释模式: --注释内容
    return statement.find("--") == 0 && statement.find("-->") == std::string::npos;
}

bool CHtlGlobalStyleConstraint::isRawEmbedding(const std::string& statement) {
    // 原始嵌入模式: [Origin] @Type [name] 
    // 支持基本类型 (@Html, @Style, @JavaScript) 和自定义类型 (任何@开头的类型)
    // 支持带名原始嵌入: [Origin] @Type name
    std::regex rawEmbeddingPattern(R"(\[Origin\]\s+@[A-Za-z][A-Za-z0-9]*(?:\s+[A-Za-z_][A-Za-z0-9_]*)?)");
    return std::regex_search(statement, rawEmbeddingPattern);
}

void CHtlGlobalStyleConstraint::addViolation(GlobalStyleConstraintResult& result, 
                                           ConstraintViolationType type,
                                           const std::string& message,
                                           const std::string& element,
                                           size_t line, 
                                           size_t column) {
    ConstraintViolation violation;
    violation.type = type;
    violation.message = message;
    violation.element = element;
    violation.line = line;
    violation.column = column;
    
    result.violations.push_back(violation);
    result.isValid = false;
}