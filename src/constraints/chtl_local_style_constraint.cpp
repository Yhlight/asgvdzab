#include "constraints/chtl_local_style_constraint.hpp"
#include <regex>
#include <sstream>
#include <iostream>

namespace chtl {

CHtlLocalStyleConstraint::CHtlLocalStyleConstraint() {
    // 局部样式约束器初始化
    // 注意：局部样式块与全局样式块允许相同的CHTL语法
}

LocalStyleConstraintResult CHtlLocalStyleConstraint::validateCHtlLocalStyleBlock(const std::string& styleContent) {
    LocalStyleConstraintResult result;
    
    auto statements = parseLocalStyleStatements(styleContent);
    size_t lineNumber = 1;
    
    for (const auto& statement : statements) {
        std::string trimmed = statement;
        // 移除前后空白字符
        size_t start = trimmed.find_first_not_of(" \t\r\n");
        if (start != std::string::npos) {
            trimmed = trimmed.substr(start);
            size_t end = trimmed.find_last_not_of(" \t\r\n");
            if (end != std::string::npos) {
                trimmed = trimmed.substr(0, end + 1);
            }
        } else {
            trimmed.clear();
        }
        
        if (trimmed.empty() || trimmed.find("//") == 0 || 
            (trimmed.find("/*") == 0 && trimmed.find("*/") != std::string::npos)) {
            lineNumber++;
            continue;
        }
        
        auto allowedTypes = checkAllowedElement(trimmed);
        
        if (allowedTypes.empty()) {
            addViolation(result, LocalStyleConstraintViolationType::UNKNOWN_SYNTAX_ELEMENT,
                "局部样式块中不允许的语法元素", trimmed, lineNumber, 0);
        } else {
            // 处理允许的元素
            for (auto type : allowedTypes) {
                switch (type) {
                    case LocalStyleAllowedElement::TEMPLATE_VARIABLE:
                        if (validateTemplateVariableReference(trimmed)) {
                            result.templateVariables.insert(trimmed);
                        } else {
                            addViolation(result, LocalStyleConstraintViolationType::MALFORMED_VARIABLE_REFERENCE,
                                "无效的模板变量引用", trimmed, lineNumber, 0);
                        }
                        break;
                        
                    case LocalStyleAllowedElement::CUSTOM_VARIABLE:
                    case LocalStyleAllowedElement::CUSTOM_VARIABLE_SPECIALIZATION:
                        if (validateCustomVariableReference(trimmed)) {
                            result.customVariables.insert(trimmed);
                        } else {
                            addViolation(result, LocalStyleConstraintViolationType::MALFORMED_VARIABLE_REFERENCE,
                                "无效的自定义变量引用", trimmed, lineNumber, 0);
                        }
                        break;
                        
                    case LocalStyleAllowedElement::TEMPLATE_STYLE_GROUP:
                    case LocalStyleAllowedElement::CUSTOM_STYLE_GROUP:
                    case LocalStyleAllowedElement::VALUELESS_STYLE_GROUP:
                    case LocalStyleAllowedElement::CUSTOM_STYLE_SPECIALIZATION:
                        if (validateStyleGroupReference(trimmed)) {
                            result.styleGroups.insert(trimmed);
                        } else {
                            addViolation(result, LocalStyleConstraintViolationType::MALFORMED_STYLE_GROUP_REFERENCE,
                                "无效的样式组引用", trimmed, lineNumber, 0);
                        }
                        break;
                        
                    case LocalStyleAllowedElement::DELETE_PROPERTY:
                    case LocalStyleAllowedElement::DELETE_INHERITANCE:
                        if (!validateDeleteStatement(trimmed)) {
                            addViolation(result, LocalStyleConstraintViolationType::INVALID_SYNTAX,
                                "无效的delete语句", trimmed, lineNumber, 0);
                        }
                        break;
                        
                    case LocalStyleAllowedElement::INHERITANCE:
                        if (!validateInheritStatement(trimmed)) {
                            addViolation(result, LocalStyleConstraintViolationType::INVALID_SYNTAX,
                                "无效的继承语句", trimmed, lineNumber, 0);
                        } else {
                            result.inheritances.insert(trimmed);
                        }
                        break;
                        
                    case LocalStyleAllowedElement::FROM_NAMESPACE:
                        if (!validateFromClause(trimmed)) {
                            addViolation(result, LocalStyleConstraintViolationType::MALFORMED_FROM_CLAUSE,
                                "格式错误的from子句", trimmed, lineNumber, 0);
                        } else {
                            result.fromClauses.insert(trimmed);
                        }
                        break;
                        
                    case LocalStyleAllowedElement::FULL_QUALIFIED_NAME:
                        if (!validateFullQualifiedName(trimmed)) {
                            addViolation(result, LocalStyleConstraintViolationType::INVALID_SYNTAX,
                                "无效的全缀名", trimmed, lineNumber, 0);
                        } else {
                            result.fullQualifiedNames.insert(trimmed);
                        }
                        break;
                        
                    case LocalStyleAllowedElement::GENERATOR_COMMENT:
                        if (validateGeneratorComment(trimmed)) {
                            result.foundGeneratorComments.insert(trimmed);
                        } else {
                            addViolation(result, LocalStyleConstraintViolationType::MALFORMED_GENERATOR_COMMENT,
                                "格式错误的生成器注释", trimmed, lineNumber, 0);
                        }
                        break;
                        
                    case LocalStyleAllowedElement::RAW_EMBEDDING:
                        if (validateRawEmbedding(trimmed)) {
                            result.foundRawEmbeddings.insert(trimmed);
                        } else {
                            addViolation(result, LocalStyleConstraintViolationType::INVALID_RAW_EMBEDDING,
                                "无效的原始嵌入", trimmed, lineNumber, 0);
                        }
                        break;
                        
                    case LocalStyleAllowedElement::CSS_SELECTOR:
                    case LocalStyleAllowedElement::CSS_PROPERTY:
                        result.cssStatements++;
                        break;
                }
            }
        }
        
        lineNumber++;
    }
    
    result.isValid = result.violations.empty();
    return result;
}

std::vector<LocalStyleAllowedElement> CHtlLocalStyleConstraint::checkAllowedElement(const std::string& element) {
    std::vector<LocalStyleAllowedElement> allowedTypes;
    
    // 检查生成器注释
    if (isGeneratorComment(element)) {
        allowedTypes.push_back(LocalStyleAllowedElement::GENERATOR_COMMENT);
    }
    
    // 检查原始嵌入 - 官方支持3种类型 (优先检查，避免与其他语法混淆)
    if (isRawEmbedding(element)) {
        allowedTypes.push_back(LocalStyleAllowedElement::RAW_EMBEDDING);
        return allowedTypes; // 原始嵌入是特殊存在，直接返回
    }
    
    // 检查变量引用
    if (isVariableReference(element)) {
        if (isTemplateVariableReference(element)) {
            allowedTypes.push_back(LocalStyleAllowedElement::TEMPLATE_VARIABLE);
        }
        if (isCustomVariableReference(element)) {
            allowedTypes.push_back(LocalStyleAllowedElement::CUSTOM_VARIABLE);
            // 检查是否是特例化
            if (element.find('=') != std::string::npos) {
                allowedTypes.push_back(LocalStyleAllowedElement::CUSTOM_VARIABLE_SPECIALIZATION);
            }
        }
    }
    
    // 检查样式组引用
    if (isStyleGroupReference(element)) {
        allowedTypes.push_back(LocalStyleAllowedElement::TEMPLATE_STYLE_GROUP);
        allowedTypes.push_back(LocalStyleAllowedElement::CUSTOM_STYLE_GROUP);
        if (element.find('{') != std::string::npos) {
            if (element.find("delete") != std::string::npos) {
                allowedTypes.push_back(LocalStyleAllowedElement::CUSTOM_STYLE_SPECIALIZATION);
            } else {
                allowedTypes.push_back(LocalStyleAllowedElement::VALUELESS_STYLE_GROUP);
            }
        }
    }
    
    // 检查继承语句
    if (isInheritStatement(element)) {
        allowedTypes.push_back(LocalStyleAllowedElement::INHERITANCE);
    }
    
    // 检查delete语句
    if (isDeleteStatement(element)) {
        if (element.find("@Style") != std::string::npos) {
            allowedTypes.push_back(LocalStyleAllowedElement::DELETE_INHERITANCE);
        } else {
            allowedTypes.push_back(LocalStyleAllowedElement::DELETE_PROPERTY);
        }
    }
    
    // 检查from子句
    if (isFromClause(element)) {
        allowedTypes.push_back(LocalStyleAllowedElement::FROM_NAMESPACE);
    }
    
    // 检查全缀名
    if (isFullQualifiedName(element)) {
        allowedTypes.push_back(LocalStyleAllowedElement::FULL_QUALIFIED_NAME);
    }
    
    // 检查基本CSS语法
    if (isBasicCSS(element)) {
        if (element.find(':') != std::string::npos && element.find(';') != std::string::npos) {
            allowedTypes.push_back(LocalStyleAllowedElement::CSS_PROPERTY);
        } else {
            allowedTypes.push_back(LocalStyleAllowedElement::CSS_SELECTOR);
        }
    }
    
    return allowedTypes;
}

bool CHtlLocalStyleConstraint::validateTemplateVariableReference(const std::string& varReference) {
    // 模板变量格式: 
    // 1. VariableGroupName(variableName) - 简写形式
    // 2. @Var VariableGroupName(variableName) - 完整形式
    std::regex templateVarPattern(R"((?:@Var\s+)?([A-Za-z_][A-Za-z0-9_]*)\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*\))");
    return std::regex_search(varReference, templateVarPattern);
}

bool CHtlLocalStyleConstraint::validateCustomVariableReference(const std::string& varReference) {
    // 自定义变量格式: 
    // 1. VariableGroupName(variableName) - 简写形式
    // 2. @Var VariableGroupName(variableName) - 完整形式
    // 3. VariableGroupName(variableName = value) - 特例化
    std::regex customVarPattern(R"((?:@Var\s+)?([A-Za-z_][A-Za-z0-9_]*)\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*(?:=\s*([^)]+))?\s*\))");
    return std::regex_search(varReference, customVarPattern);
}

bool CHtlLocalStyleConstraint::validateStyleGroupReference(const std::string& styleGroupRef) {
    // 样式组格式: @Style StyleGroupName 或 [Template/Custom] @Style StyleGroupName
    // 也可能是样式组继承: @Style BaseStyleGroup; (不带任何前缀)
    std::regex styleGroupPattern(R"((?:\[(Template|Custom)\]\s+)?@Style\s+([A-Za-z_][A-Za-z0-9_]*)(?:\s*;)?)");
    return std::regex_search(styleGroupRef, styleGroupPattern);
}

bool CHtlLocalStyleConstraint::validateInheritStatement(const std::string& inheritStatement) {
    // 继承格式: inherit @Style StyleGroupName 或直接 @Style BaseStyle
    std::regex inheritPattern(R"((?:inherit\s+)?@Style\s+([A-Za-z_][A-Za-z0-9_]*))");
    return std::regex_search(inheritStatement, inheritPattern);
}

bool CHtlLocalStyleConstraint::validateDeleteStatement(const std::string& deleteStatement) {
    // delete格式: 
    // 1. delete property1, property2, ... (delete属性)
    // 2. delete @Style StyleGroupName (delete继承)
    std::regex deletePattern(R"(delete\s+(?:@Style\s+([A-Za-z_][A-Za-z0-9_]*)|([A-Za-z-]+(?:\s*,\s*[A-Za-z-]+)*)))");
    return std::regex_search(deleteStatement, deletePattern);
}

bool CHtlLocalStyleConstraint::validateFromClause(const std::string& fromClause) {
    // from子句格式: ElementName from file.path (用于导入)
    // 注意：CHTL中的from主要用于导入，格式是从文件路径导入
    std::regex fromPattern(R"(([A-Za-z_][A-Za-z0-9_]*)\s+from\s+([A-Za-z_][A-Za-z0-9_./-]*))");
    return std::regex_search(fromClause, fromPattern);
}

bool CHtlLocalStyleConstraint::validateFullQualifiedName(const std::string& fullQualifiedName) {
    // 全缀名格式: [Custom] @Var/@Style/@Element Name 或 [Template] @Var/@Style/@Element Name
    // 这是CHTL中用于避免命名冲突的完整限定名语法
    std::regex fqnPattern(R"(\[(Custom|Template)\]\s+@(Style|Element|Var)\s+([A-Za-z_][A-Za-z0-9_]*))");
    return std::regex_search(fullQualifiedName, fqnPattern);
}

bool CHtlLocalStyleConstraint::validateGeneratorComment(const std::string& comment) {
    // 生成器注释格式: --注释内容
    return comment.find("--") == 0 && comment.find("-->") == std::string::npos;
}

bool CHtlLocalStyleConstraint::validateRawEmbedding(const std::string& embedding) {
    // 原始嵌入格式: [Origin] @Html/@Style/@JavaScript { 内容 }
    // 根据CHTL语法文档，只有这3种官方类型
    std::regex rawEmbeddingPattern(R"(\[Origin\]\s+@(Html|Style|JavaScript)\s*\{)");
    return std::regex_search(embedding, rawEmbeddingPattern);
}

std::vector<std::string> CHtlLocalStyleConstraint::parseLocalStyleStatements(const std::string& content) {
    std::vector<std::string> statements;
    std::istringstream stream(content);
    std::string line;
    
    while (std::getline(stream, line)) {
        statements.push_back(line);
    }
    
    return statements;
}

bool CHtlLocalStyleConstraint::isTemplateVariableReference(const std::string& statement) {
    // 模板变量引用: VariableGroupName(variableName) 或 @Var VariableGroupName(variableName)
    return validateTemplateVariableReference(statement);
}

bool CHtlLocalStyleConstraint::isCustomVariableReference(const std::string& statement) {
    // 自定义变量引用: VariableGroupName(variableName) 或特例化形式
    return validateCustomVariableReference(statement);
}

bool CHtlLocalStyleConstraint::isVariableReference(const std::string& statement) {
    // 变量引用模式: 
    // 1. VariableGroup(variableName) 格式 - 简写形式
    // 2. @Var VariableGroup(variableName) 格式 - 完整形式
    return (statement.find("@Var") != std::string::npos || 
           (statement.find('(') != std::string::npos && statement.find(')') != std::string::npos &&
            statement.find("function") == std::string::npos)); // 排除函数定义
}

bool CHtlLocalStyleConstraint::isStyleGroupReference(const std::string& statement) {
    // 样式组引用: @Style 关键字
    return statement.find("@Style") != std::string::npos;
}

bool CHtlLocalStyleConstraint::isInheritStatement(const std::string& statement) {
    // 继承语句: inherit关键字 或直接@Style引用
    return statement.find("inherit") != std::string::npos || 
           (statement.find("@Style") != std::string::npos && statement.find(";") != std::string::npos);
}

bool CHtlLocalStyleConstraint::isDeleteStatement(const std::string& statement) {
    // delete语句: delete关键字
    return statement.find("delete") == 0;
}

bool CHtlLocalStyleConstraint::isFromClause(const std::string& statement) {
    // from子句: from关键字
    return statement.find(" from ") != std::string::npos;
}

bool CHtlLocalStyleConstraint::isFullQualifiedName(const std::string& statement) {
    // 全缀名: [Custom] 或 [Template] 前缀
    return (statement.find("[Custom]") != std::string::npos || 
            statement.find("[Template]") != std::string::npos) &&
           (statement.find("@Style") != std::string::npos || 
            statement.find("@Element") != std::string::npos ||
            statement.find("@Var") != std::string::npos);
}

bool CHtlLocalStyleConstraint::isGeneratorComment(const std::string& statement) {
    // 生成器注释: --开头
    return statement.find("--") == 0 && statement.find("-->") == std::string::npos;
}

bool CHtlLocalStyleConstraint::isRawEmbedding(const std::string& statement) {
    // 原始嵌入模式: [Origin] @Html/@Style/@JavaScript (根据CHTL语法文档的3种官方类型)
    std::regex rawEmbeddingPattern(R"(\[Origin\]\s+@(Html|Style|JavaScript))");
    return std::regex_search(statement, rawEmbeddingPattern);
}

bool CHtlLocalStyleConstraint::isBasicCSS(const std::string& statement) {
    // 基本CSS语法检测（简化版）
    // CSS属性: property: value;
    // CSS选择器: 不以CHTL关键字开头的标识符
    
    if (statement.find("@") != std::string::npos || 
        statement.find("--") == 0 ||
        statement.find("[Origin]") != std::string::npos ||
        statement.find("[Custom]") != std::string::npos ||
        statement.find("[Template]") != std::string::npos ||
        statement.find("inherit") != std::string::npos ||
        statement.find("delete") == 0 ||
        statement.find(" from ") != std::string::npos) {
        return false;
    }
    
    // 检查是否为CSS属性或选择器
    return statement.find(':') != std::string::npos || 
           statement.find('{') != std::string::npos ||
           statement.find('}') != std::string::npos ||
           statement.find(';') != std::string::npos ||
           std::regex_match(statement, std::regex(R"(^[.#]?[A-Za-z0-9_-]+(?:\s*[,\s]\s*[.#]?[A-Za-z0-9_-]+)*\s*$)"));
}

void CHtlLocalStyleConstraint::addViolation(LocalStyleConstraintResult& result, 
                                          LocalStyleConstraintViolationType type,
                                          const std::string& message,
                                          const std::string& element,
                                          size_t line, 
                                          size_t column) {
    LocalStyleConstraintViolation violation;
    violation.type = type;
    violation.message = message;
    violation.element = element;
    violation.line = line;
    violation.column = column;
    
    result.violations.push_back(violation);
}

} // namespace chtl