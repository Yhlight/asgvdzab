#include "constraints/chtl_js_constraint.hpp"
#include <regex>
#include <sstream>
#include <algorithm>
#include <iostream>

using namespace chtl;

CHtlJSConstraint::CHtlJSConstraint() {
    // 初始化禁止的API调用
    forbiddenAPIs_.insert("eval");                    // 禁止eval
    forbiddenAPIs_.insert("Function");                // 禁止Function构造器
    forbiddenAPIs_.insert("document.write");          // 禁止document.write
    forbiddenAPIs_.insert("document.writeln");        // 禁止document.writeln
    forbiddenAPIs_.insert("innerHTML");               // 谨慎使用innerHTML(可能导致XSS)
    forbiddenAPIs_.insert("outerHTML");               // 谨慎使用outerHTML
    
    // 初始化禁止的CHTL元素(在JavaScript中不应该出现)
    forbiddenCHtlElements_.insert("@Element");        // 元素定义在JS中无效
    forbiddenCHtlElements_.insert("[Template]");      // 模板定义在JS中无效
    forbiddenCHtlElements_.insert("[Custom]");        // 自定义元素定义在JS中无效
    forbiddenCHtlElements_.insert("style{");          // 样式块在JS中无效
    forbiddenCHtlElements_.insert("text{");           // 文本块在JS中无效
    forbiddenCHtlElements_.insert("@Style");          // 样式组在JS中无效(除非是变量引用)
}

CHtlJSConstraintResult CHtlJSConstraint::validateCHtlJSBlock(
    const std::string& jsContent,
    const std::vector<std::string>& availableNamespaces,
    const std::vector<std::string>& availableTemplateVars,
    const std::vector<std::string>& availableCustomVars,
    const std::vector<std::string>& availableDOMElements) {
    
    CHtlJSConstraintResult result;
    
    // 检查禁止的API调用
    auto forbiddenAPIs = checkForbiddenAPIs(jsContent);
    for (const auto& api : forbiddenAPIs) {
        addViolation(result, CHtlJSConstraintViolationType::FORBIDDEN_API,
            "禁止在CHTL JavaScript块中使用危险API: " + api, api, 0, 0);
    }
    
    // 检查禁止的CHTL元素
    auto forbiddenElements = checkForbiddenCHtlElements(jsContent);
    for (const auto& element : forbiddenElements) {
        addViolation(result, CHtlJSConstraintViolationType::FORBIDDEN_CHTL_ELEMENT,
            "CHTL JavaScript块中不允许使用的CHTL语法: " + element, element, 0, 0);
    }
    
    // 解析JavaScript内容为语句
    auto statements = parseJSStatements(jsContent);
    
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
            // 验证具体的语法元素
            for (auto type : allowedTypes) {
                switch (type) {
                    case CHtlJSAllowedElement::TEMPLATE_VARIABLE:
                        if (!validateTemplateVariableReference(trimmed)) {
                            addViolation(result, CHtlJSConstraintViolationType::INVALID_VARIABLE_REFERENCE,
                                "无效的模板变量引用", trimmed, lineNumber, 0);
                        } else {
                            result.usedTemplateVariables.insert(trimmed);
                        }
                        break;
                        
                    case CHtlJSAllowedElement::CUSTOM_VARIABLE:
                        if (!validateCustomVariableReference(trimmed)) {
                            addViolation(result, CHtlJSConstraintViolationType::INVALID_VARIABLE_REFERENCE,
                                "无效的自定义变量引用", trimmed, lineNumber, 0);
                        } else {
                            result.usedCustomVariables.insert(trimmed);
                        }
                        break;
                        
                    case CHtlJSAllowedElement::FROM_NAMESPACE:
                        if (!validateFromClause(trimmed)) {
                            addViolation(result, CHtlJSConstraintViolationType::MALFORMED_FROM_CLAUSE,
                                "格式错误的from子句", trimmed, lineNumber, 0);
                        }
                        break;
                        
                    case CHtlJSAllowedElement::CHTL_DOM_ACCESS:
                        if (!validateCHtlDOMAccess(trimmed)) {
                            addViolation(result, CHtlJSConstraintViolationType::INVALID_DOM_ACCESS,
                                "无效的CHTL DOM访问", trimmed, lineNumber, 0);
                        } else {
                            result.accessedDOMElements.insert(trimmed);
                        }
                        break;
                        
                    default:
                        // 其他类型(纯JavaScript、注释等)基本都允许
                        break;
                }
            }
        }
        // 注意：对于JavaScript，我们比较宽松，大部分JavaScript代码都是允许的
        // 只检查明确禁止的API和CHTL元素
        
        lineNumber++;
    }
    
    result.isValid = result.violations.empty();
    return result;
}

std::vector<CHtlJSAllowedElement> CHtlJSConstraint::checkAllowedElement(const std::string& element) {
    std::vector<CHtlJSAllowedElement> allowedTypes;
    
    // 检查生成器注释
    if (isGeneratorComment(element)) {
        allowedTypes.push_back(CHtlJSAllowedElement::GENERATOR_COMMENT);
    }
    
    // 检查原始嵌入
    if (isRawEmbedding(element)) {
        allowedTypes.push_back(CHtlJSAllowedElement::RAW_EMBEDDING);
    }
    
    // 检查变量引用
    if (isVariableReference(element)) {
        // 判断是模板变量还是自定义变量
        if (element.find("[Template]") != std::string::npos) {
            allowedTypes.push_back(CHtlJSAllowedElement::TEMPLATE_VARIABLE);
        } else if (element.find("[Custom]") != std::string::npos) {
            allowedTypes.push_back(CHtlJSAllowedElement::CUSTOM_VARIABLE);
        } else {
            // 默认可能是模板变量或自定义变量
            allowedTypes.push_back(CHtlJSAllowedElement::TEMPLATE_VARIABLE);
            allowedTypes.push_back(CHtlJSAllowedElement::CUSTOM_VARIABLE);
        }
    }
    
    // 检查from子句
    if (isFromClause(element)) {
        allowedTypes.push_back(CHtlJSAllowedElement::FROM_NAMESPACE);
    }
    
    // 检查全缀名
    if (isFullQualifiedName(element)) {
        allowedTypes.push_back(CHtlJSAllowedElement::FULL_QUALIFIED_NAME);
    }
    
    // 检查CHTL DOM访问
    if (isCHtlDOMAccess(element)) {
        allowedTypes.push_back(CHtlJSAllowedElement::CHTL_DOM_ACCESS);
    }
    
    // 对于JavaScript，默认认为是纯JavaScript代码
    if (allowedTypes.empty()) {
        allowedTypes.push_back(CHtlJSAllowedElement::PURE_JAVASCRIPT);
    }
    
    return allowedTypes;
}

bool CHtlJSConstraint::validateTemplateVariableReference(const std::string& varReference) {
    // 模板变量格式: @Var VariableGroupName(variableName) 或 VariableGroupName(variableName)
    std::regex templateVarPattern(R"((?:@Var\s+)?([A-Za-z_][A-Za-z0-9_]*)\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*\))");
    return std::regex_search(varReference, templateVarPattern);
}

bool CHtlJSConstraint::validateCustomVariableReference(const std::string& varReference) {
    // 自定义变量格式: [Custom] @Var VariableGroupName(variableName) 或类似的格式
    std::regex customVarPattern(R"((?:\[Custom\]\s+)?(?:@Var\s+)?([A-Za-z_][A-Za-z0-9_]*)\s*(?:\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*\))?(?:\s*\[\s*([A-Za-z_][A-Za-z0-9_]*)\s*\])?)");
    return std::regex_search(varReference, customVarPattern);
}

bool CHtlJSConstraint::validateFromClause(const std::string& fromClause) {
    // from子句格式: ElementName from namespace.subnamespace
    std::regex fromPattern(R"(([A-Za-z_][A-Za-z0-9_]*)\s+from\s+([A-Za-z_][A-Za-z0-9_.]*))");
    return std::regex_search(fromClause, fromPattern);
}

bool CHtlJSConstraint::validateCHtlDOMAccess(const std::string& domAccess) {
    // CHTL DOM访问格式: chtl.getElementById(), chtl.getElementByClass(), 等
    std::regex chtlDOMPattern(R"(chtl\.\w+\()");
    return std::regex_search(domAccess, chtlDOMPattern);
}

std::vector<std::string> CHtlJSConstraint::checkForbiddenAPIs(const std::string& jsContent) {
    std::vector<std::string> foundAPIs;
    
    for (const auto& api : forbiddenAPIs_) {
        if (jsContent.find(api) != std::string::npos) {
            foundAPIs.push_back(api);
        }
    }
    
    return foundAPIs;
}

std::vector<std::string> CHtlJSConstraint::checkForbiddenCHtlElements(const std::string& jsContent) {
    std::vector<std::string> foundElements;
    
    for (const auto& element : forbiddenCHtlElements_) {
        if (jsContent.find(element) != std::string::npos) {
            foundElements.push_back(element);
        }
    }
    
    return foundElements;
}

std::vector<std::string> CHtlJSConstraint::parseJSStatements(const std::string& content) {
    std::vector<std::string> statements;
    std::istringstream stream(content);
    std::string line;
    
    while (std::getline(stream, line)) {
        statements.push_back(line);
    }
    
    return statements;
}

bool CHtlJSConstraint::isPureJavaScript(const std::string& statement) {
    // 基本的JavaScript语法检查
    // 如果不包含CHTL特有的语法，则认为是纯JavaScript
    return statement.find("@") == std::string::npos &&
           statement.find("[Template]") == std::string::npos &&
           statement.find("[Custom]") == std::string::npos &&
           statement.find("[Origin]") == std::string::npos;
}

bool CHtlJSConstraint::isVariableReference(const std::string& statement) {
    // 变量引用模式: @Var, VariableGroup(), 等
    return statement.find("@Var") != std::string::npos || 
           (statement.find('(') != std::string::npos && statement.find(')') != std::string::npos &&
            !statement.find("function") == 0);  // 排除函数定义
}

bool CHtlJSConstraint::isCHtlDOMAccess(const std::string& statement) {
    // CHTL DOM访问模式: chtl.xxx
    return statement.find("chtl.") != std::string::npos;
}

bool CHtlJSConstraint::isGeneratorComment(const std::string& statement) {
    // 生成器注释模式: --注释内容
    return statement.find("--") == 0 && statement.find("-->") == std::string::npos;
}

bool CHtlJSConstraint::isRawEmbedding(const std::string& statement) {
    // 原始嵌入模式: [Origin]
    return statement.find("[Origin]") != std::string::npos;
}

bool CHtlJSConstraint::isFromClause(const std::string& statement) {
    // from子句模式
    return statement.find(" from ") != std::string::npos;
}

bool CHtlJSConstraint::isFullQualifiedName(const std::string& statement) {
    // 全缀名格式: namespace.subnamespace.ElementName
    std::regex fqnPattern(R"([A-Za-z_][A-Za-z0-9_]*(?:\.[A-Za-z_][A-Za-z0-9_]*)+)");
    return std::regex_search(statement, fqnPattern);
}

void CHtlJSConstraint::addViolation(CHtlJSConstraintResult& result, 
                                   CHtlJSConstraintViolationType type,
                                   const std::string& message,
                                   const std::string& element,
                                   size_t line, 
                                   size_t column) {
    CHtlJSConstraintViolation violation;
    violation.type = type;
    violation.message = message;
    violation.element = element;
    violation.line = line;
    violation.column = column;
    
    result.violations.push_back(violation);
    result.isValid = false;
}