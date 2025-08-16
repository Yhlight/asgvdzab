#include "constraints/chtl_script_constraint.hpp"
#include <regex>
#include <sstream>
#include <algorithm>
#include <iostream>

using namespace chtl;

CHtlScriptConstraint::CHtlScriptConstraint() {
    // 初始化禁止的CHTL语法模式
    forbiddenCHtlPatterns_.insert("@Var");           // 变量引用
    forbiddenCHtlPatterns_.insert("@Style");         // 样式组
    forbiddenCHtlPatterns_.insert("@Element");       // 元素
    forbiddenCHtlPatterns_.insert("[Custom]");       // 自定义全缀名
    forbiddenCHtlPatterns_.insert("[Template]");     // 模板全缀名
    forbiddenCHtlPatterns_.insert("inherit");        // 继承
    forbiddenCHtlPatterns_.insert("delete");         // Delete操作
    forbiddenCHtlPatterns_.insert("style{");         // 样式块
    forbiddenCHtlPatterns_.insert("style {");        // 样式块
    forbiddenCHtlPatterns_.insert("text{");          // 文本块
    forbiddenCHtlPatterns_.insert("text {");         // 文本块
    forbiddenCHtlPatterns_.insert(" from ");         // From子句
}

CHtlScriptConstraintResult CHtlScriptConstraint::validateCHtlScriptBlock(const std::string& scriptContent) {
    CHtlScriptConstraintResult result;
    
    // 检查禁止的CHTL语法
    auto forbiddenElements = checkForbiddenCHtlSyntax(scriptContent);
    for (auto element : forbiddenElements) {
        std::string elementName;
        switch (element) {
            case CHtlScriptForbiddenElement::TEMPLATE_VARIABLE:
                elementName = "模板变量";
                break;
            case CHtlScriptForbiddenElement::CUSTOM_VARIABLE:
                elementName = "自定义变量";
                break;
            case CHtlScriptForbiddenElement::VARIABLE_SPECIALIZATION:
                elementName = "变量特例化";
                break;
            case CHtlScriptForbiddenElement::NAMESPACE_FROM:
                elementName = "命名空间from子句";
                break;
            case CHtlScriptForbiddenElement::TEMPLATE_STYLE_GROUP:
            case CHtlScriptForbiddenElement::CUSTOM_STYLE_GROUP:
                elementName = "样式组";
                break;
            case CHtlScriptForbiddenElement::TEMPLATE_ELEMENT:
            case CHtlScriptForbiddenElement::CUSTOM_ELEMENT:
                elementName = "元素定义";
                break;
            case CHtlScriptForbiddenElement::FULL_QUALIFIED_NAME:
                elementName = "全缀名";
                break;
            case CHtlScriptForbiddenElement::STYLE_BLOCKS:
                elementName = "样式块";
                break;
            case CHtlScriptForbiddenElement::TEXT_BLOCKS:
                elementName = "文本块";
                break;
            case CHtlScriptForbiddenElement::INHERITANCE:
                elementName = "继承语句";
                break;
            case CHtlScriptForbiddenElement::DELETE_OPERATIONS:
                elementName = "Delete操作";
                break;
        }
        
        addViolation(result, CHtlScriptConstraintViolationType::FORBIDDEN_CHTL_SYNTAX,
            element, "CHTL Script块中禁止使用: " + elementName, "", 0, 0);
    }
    
    // 解析脚本内容为语句
    auto statements = parseScriptStatements(scriptContent);
    
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
        
        if (!allowedTypes.empty()) {
            // 记录允许的元素
            for (auto type : allowedTypes) {
                switch (type) {
                    case CHtlScriptAllowedElement::GENERATOR_COMMENT:
                        if (validateGeneratorComment(trimmed)) {
                            result.foundGeneratorComments.insert(trimmed);
                        } else {
                            addViolation(result, CHtlScriptConstraintViolationType::MALFORMED_GENERATOR_COMMENT,
                                CHtlScriptForbiddenElement::TEMPLATE_VARIABLE, // 占位符
                                "格式错误的生成器注释", trimmed, lineNumber, 0);
                        }
                        break;
                        
                    case CHtlScriptAllowedElement::RAW_EMBEDDING:
                        if (validateRawEmbedding(trimmed)) {
                            result.foundRawEmbeddings.insert(trimmed);
                        } else {
                            addViolation(result, CHtlScriptConstraintViolationType::INVALID_RAW_EMBEDDING,
                                CHtlScriptForbiddenElement::TEMPLATE_VARIABLE, // 占位符
                                "无效的原始嵌入", trimmed, lineNumber, 0);
                        }
                        break;
                        
                    case CHtlScriptAllowedElement::PURE_JAVASCRIPT:
                        result.pureJavaScriptLines++;
                        break;
                }
            }
        }
        
        lineNumber++;
    }
    
    result.isValid = result.violations.empty();
    return result;
}

std::vector<CHtlScriptAllowedElement> CHtlScriptConstraint::checkAllowedElement(const std::string& element) {
    std::vector<CHtlScriptAllowedElement> allowedTypes;
    
    // 检查生成器注释
    if (isGeneratorComment(element)) {
        allowedTypes.push_back(CHtlScriptAllowedElement::GENERATOR_COMMENT);
    }
    
    // 检查原始嵌入 - 类型无用，任何 [Origin] @AnyType 都允许
    if (isRawEmbedding(element)) {
        allowedTypes.push_back(CHtlScriptAllowedElement::RAW_EMBEDDING);
    }
    
    // 对于Script，默认认为是纯JavaScript代码（如果不是其他特殊类型）
    if (allowedTypes.empty() && isPureJavaScript(element)) {
        allowedTypes.push_back(CHtlScriptAllowedElement::PURE_JAVASCRIPT);
    }
    
    return allowedTypes;
}

std::vector<CHtlScriptForbiddenElement> CHtlScriptConstraint::checkForbiddenCHtlSyntax(const std::string& scriptContent) {
    std::vector<CHtlScriptForbiddenElement> foundForbidden;
    
    // 原始嵌入是特别的存在，可以在任何地方使用，内容原样输出，不做约束检查
    // 移除所有原始嵌入块，只检查其他内容 (支持基本类型和自定义类型，支持带名原始嵌入)
    std::regex originPattern(R"(\[Origin\]\s+@[A-Za-z][A-Za-z0-9]*(?:\s+[A-Za-z_][A-Za-z0-9_]*)?\s*\{[^}]*\})");
    std::string contentWithoutOrigin = std::regex_replace(scriptContent, originPattern, "");
    
    // 检查模板变量和自定义变量
    if (isTemplateVariableReference(contentWithoutOrigin) || isCustomVariableReference(contentWithoutOrigin)) {
        foundForbidden.push_back(CHtlScriptForbiddenElement::TEMPLATE_VARIABLE);
    }
    
    // 检查变量特例化
    if (isVariableSpecialization(contentWithoutOrigin)) {
        foundForbidden.push_back(CHtlScriptForbiddenElement::VARIABLE_SPECIALIZATION);
    }
    
    // 检查命名空间from子句
    if (isNamespaceFromClause(contentWithoutOrigin)) {
        foundForbidden.push_back(CHtlScriptForbiddenElement::NAMESPACE_FROM);
    }
    
    // 检查其他CHTL特有语法（排除原始嵌入）
    if (isCHtlSpecificSyntax(contentWithoutOrigin)) {
        if (contentWithoutOrigin.find("@Style") != std::string::npos) {
            foundForbidden.push_back(CHtlScriptForbiddenElement::TEMPLATE_STYLE_GROUP);
        }
        if (contentWithoutOrigin.find("@Element") != std::string::npos) {
            foundForbidden.push_back(CHtlScriptForbiddenElement::TEMPLATE_ELEMENT);
        }
        if (contentWithoutOrigin.find("[Custom]") != std::string::npos || contentWithoutOrigin.find("[Template]") != std::string::npos) {
            foundForbidden.push_back(CHtlScriptForbiddenElement::FULL_QUALIFIED_NAME);
        }
        if (contentWithoutOrigin.find("style{") != std::string::npos || contentWithoutOrigin.find("style {") != std::string::npos) {
            foundForbidden.push_back(CHtlScriptForbiddenElement::STYLE_BLOCKS);
        }
        if (contentWithoutOrigin.find("text{") != std::string::npos || contentWithoutOrigin.find("text {") != std::string::npos) {
            foundForbidden.push_back(CHtlScriptForbiddenElement::TEXT_BLOCKS);
        }
        if (contentWithoutOrigin.find("inherit") != std::string::npos) {
            foundForbidden.push_back(CHtlScriptForbiddenElement::INHERITANCE);
        }
        if (contentWithoutOrigin.find("delete") != std::string::npos) {
            foundForbidden.push_back(CHtlScriptForbiddenElement::DELETE_OPERATIONS);
        }
    }
    
    return foundForbidden;
}

bool CHtlScriptConstraint::validateGeneratorComment(const std::string& comment) {
    // 生成器注释格式: -- 注释内容
    return comment.find("--") == 0 && comment.find("-->") == std::string::npos;
}

bool CHtlScriptConstraint::validateRawEmbedding(const std::string& embedding) {
    // 原始嵌入格式: 
    // 1. 定义: [Origin] @Type [name] { 内容 }
    // 2. 引用: [Origin] @Type [name];
    // 支持基本类型和自定义类型，支持带名原始嵌入
    std::regex rawEmbeddingDefPattern(R"(\[Origin\]\s+@[A-Za-z][A-Za-z0-9]*(?:\s+[A-Za-z_][A-Za-z0-9_]*)?\s*\{)");
    std::regex rawEmbeddingRefPattern(R"(\[Origin\]\s+@[A-Za-z][A-Za-z0-9]*(?:\s+[A-Za-z_][A-Za-z0-9_]*)?\s*;)");
    return std::regex_search(embedding, rawEmbeddingDefPattern) || 
           std::regex_search(embedding, rawEmbeddingRefPattern);
}

std::vector<std::string> CHtlScriptConstraint::parseScriptStatements(const std::string& content) {
    std::vector<std::string> statements;
    std::istringstream stream(content);
    std::string line;
    
    while (std::getline(stream, line)) {
        statements.push_back(line);
    }
    
    return statements;
}

bool CHtlScriptConstraint::isPureJavaScript(const std::string& statement) {
    // 基本的JavaScript语法检查
    // 如果不包含CHTL特有的语法，则认为是纯JavaScript
    return statement.find("@") == std::string::npos &&
           statement.find("[Origin]") == std::string::npos &&
           statement.find("[Custom]") == std::string::npos &&
           statement.find("[Template]") == std::string::npos &&
           statement.find("--") != 0; // 排除生成器注释
}

bool CHtlScriptConstraint::isGeneratorComment(const std::string& statement) {
    // 生成器注释模式: --注释内容
    return statement.find("--") == 0 && statement.find("-->") == std::string::npos;
}

bool CHtlScriptConstraint::isRawEmbedding(const std::string& statement) {
    // 原始嵌入模式: [Origin] @Type [name]
    // 支持基本类型和自定义类型，支持带名原始嵌入
    std::regex rawEmbeddingPattern(R"(\[Origin\]\s+@[A-Za-z][A-Za-z0-9]*(?:\s+[A-Za-z_][A-Za-z0-9_]*)?)");
    return std::regex_search(statement, rawEmbeddingPattern);
}

bool CHtlScriptConstraint::isTemplateVariableReference(const std::string& statement) {
    // 模板变量格式: VariableGroupName(variableName) 或 @Var VariableGroupName(variableName)
    std::regex templateVarPattern(R"((?:@Var\s+)?([A-Za-z_][A-Za-z0-9_]*)\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*\))");
    return std::regex_search(statement, templateVarPattern);
}

bool CHtlScriptConstraint::isCustomVariableReference(const std::string& statement) {
    // 自定义变量格式: 同模板变量
    return isTemplateVariableReference(statement);
}

bool CHtlScriptConstraint::isVariableSpecialization(const std::string& statement) {
    // 变量特例化格式: VariableGroupName(variableName = value)
    std::regex specializationPattern(R"(([A-Za-z_][A-Za-z0-9_]*)\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*=\s*([^)]+)\))");
    return std::regex_search(statement, specializationPattern);
}

bool CHtlScriptConstraint::isNamespaceFromClause(const std::string& statement) {
    // From子句格式: ElementName from namespace.path
    return statement.find(" from ") != std::string::npos;
}

bool CHtlScriptConstraint::isCHtlSpecificSyntax(const std::string& statement) {
    // 检查是否包含任何CHTL特有语法
    for (const auto& pattern : forbiddenCHtlPatterns_) {
        if (statement.find(pattern) != std::string::npos) {
            return true;
        }
    }
    return false;
}

void CHtlScriptConstraint::addViolation(CHtlScriptConstraintResult& result, 
                                       CHtlScriptConstraintViolationType type,
                                       CHtlScriptForbiddenElement forbiddenElement,
                                       const std::string& message,
                                       const std::string& element,
                                       size_t line, 
                                       size_t column) {
    CHtlScriptConstraintViolation violation;
    violation.type = type;
    violation.forbiddenElement = forbiddenElement;
    violation.message = message;
    violation.element = element;
    violation.line = line;
    violation.column = column;
    
    result.violations.push_back(violation);
    result.isValid = false;
}