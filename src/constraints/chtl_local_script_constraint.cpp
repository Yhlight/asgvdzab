#include "constraints/chtl_local_script_constraint.hpp"
#include "constraints/raw_embedding_parser.hpp"
#include <regex>
#include <sstream>
#include <iostream>

namespace chtl {

CHtlLocalScriptConstraint::CHtlLocalScriptConstraint() {
    // 局部script约束器初始化
    // 初始化禁止的CHTL语法模式（样式和元素相关）
    forbiddenCHtlPatterns_.insert("@Style");
    forbiddenCHtlPatterns_.insert("@Element");
    forbiddenCHtlPatterns_.insert("style {");
    forbiddenCHtlPatterns_.insert("text {");
    forbiddenCHtlPatterns_.insert("inherit @Style");
    forbiddenCHtlPatterns_.insert("delete @Style");
    forbiddenCHtlPatterns_.insert("[Custom] @Style");
    forbiddenCHtlPatterns_.insert("[Template] @Style");
    forbiddenCHtlPatterns_.insert("[Custom] @Element");
    forbiddenCHtlPatterns_.insert("[Template] @Element");
}

LocalScriptConstraintResult CHtlLocalScriptConstraint::validateCHtlLocalScriptBlock(const std::string& scriptContent) {
    LocalScriptConstraintResult result;
    
    // 首先检查禁止的CHTL语法
    auto forbiddenElements = checkForbiddenCHtlSyntax(scriptContent);
    if (!forbiddenElements.empty()) {
        for (auto element : forbiddenElements) {
            addViolation(result, LocalScriptConstraintViolationType::FORBIDDEN_CHTL_SYNTAX,
                element, "局部script块中禁止使用此CHTL语法", "", 0, 0);
        }
    }
    
    auto statements = parseLocalScriptStatements(scriptContent);
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
            // 检查是否为JavaScript模板语法（特供语法）
            if (isJavaScriptTemplateSyntax(trimmed)) {
                result.jsTemplateSyntax.insert(trimmed);
            } else {
                // 检查是否为纯JavaScript（默认允许）
                if (isPureJavaScript(trimmed)) {
                    result.pureJavaScriptLines++;
                } else {
                    addViolation(result, LocalScriptConstraintViolationType::UNKNOWN_SYNTAX_ELEMENT,
                        LocalScriptForbiddenElement::TEMPLATE_STYLE_GROUP, // 占位符
                        "局部script块中不允许的语法元素", trimmed, lineNumber, 0);
                }
            }
        } else {
            // 处理允许的元素
            for (auto type : allowedTypes) {
                switch (type) {
                    case LocalScriptAllowedElement::TEMPLATE_VARIABLE:
                        if (validateTemplateVariableReference(trimmed)) {
                            result.templateVariables.insert(trimmed);
                        } else {
                            addViolation(result, LocalScriptConstraintViolationType::INVALID_VARIABLE_REFERENCE,
                                LocalScriptForbiddenElement::TEMPLATE_STYLE_GROUP, // 占位符
                                "无效的模板变量引用", trimmed, lineNumber, 0);
                        }
                        break;
                        
                    case LocalScriptAllowedElement::CUSTOM_VARIABLE:
                    case LocalScriptAllowedElement::VARIABLE_SPECIALIZATION:
                        if (validateCustomVariableReference(trimmed)) {
                            result.customVariables.insert(trimmed);
                        } else {
                            addViolation(result, LocalScriptConstraintViolationType::INVALID_VARIABLE_REFERENCE,
                                LocalScriptForbiddenElement::TEMPLATE_STYLE_GROUP, // 占位符
                                "无效的自定义变量引用", trimmed, lineNumber, 0);
                        }
                        break;
                        
                    case LocalScriptAllowedElement::NAMESPACE_FROM:
                        if (validateFromClause(trimmed)) {
                            result.fromClauses.insert(trimmed);
                        } else {
                            addViolation(result, LocalScriptConstraintViolationType::MALFORMED_FROM_CLAUSE,
                                LocalScriptForbiddenElement::TEMPLATE_STYLE_GROUP, // 占位符
                                "格式错误的from子句", trimmed, lineNumber, 0);
                        }
                        break;
                        
                    case LocalScriptAllowedElement::GENERATOR_COMMENT:
                        if (validateGeneratorComment(trimmed)) {
                            result.foundGeneratorComments.insert(trimmed);
                        } else {
                            addViolation(result, LocalScriptConstraintViolationType::MALFORMED_GENERATOR_COMMENT,
                                LocalScriptForbiddenElement::TEMPLATE_STYLE_GROUP, // 占位符
                                "格式错误的生成器注释", trimmed, lineNumber, 0);
                        }
                        break;
                        
                    case LocalScriptAllowedElement::RAW_EMBEDDING:
                        if (validateRawEmbedding(trimmed)) {
                            result.foundRawEmbeddings.insert(trimmed);
                        } else {
                            addViolation(result, LocalScriptConstraintViolationType::INVALID_RAW_EMBEDDING,
                                LocalScriptForbiddenElement::TEMPLATE_STYLE_GROUP, // 占位符
                                "无效的原始嵌入", trimmed, lineNumber, 0);
                        }
                        break;
                        
                    case LocalScriptAllowedElement::PURE_JAVASCRIPT:
                        result.pureJavaScriptLines++;
                        break;
                        
                    case LocalScriptAllowedElement::JAVASCRIPT_TEMPLATE_SYNTAX:
                        result.jsTemplateSyntax.insert(trimmed);
                        break;
                }
            }
        }
        
        lineNumber++;
    }
    
    result.isValid = result.violations.empty();
    return result;
}

std::vector<LocalScriptAllowedElement> CHtlLocalScriptConstraint::checkAllowedElement(const std::string& element) {
    std::vector<LocalScriptAllowedElement> allowedTypes;
    
    // 检查生成器注释（特殊存在，任意地方都可以写）
    if (isGeneratorComment(element)) {
        allowedTypes.push_back(LocalScriptAllowedElement::GENERATOR_COMMENT);
        return allowedTypes; // 生成器注释优先返回
    }
    
    // 检查原始嵌入（特殊存在，任意地方都可以写）
    if (isRawEmbedding(element)) {
        allowedTypes.push_back(LocalScriptAllowedElement::RAW_EMBEDDING);
        return allowedTypes; // 原始嵌入优先返回
    }
    
    // 检查JavaScript模板语法（{{&}}等特供语法）
    if (isJavaScriptTemplateSyntax(element)) {
        allowedTypes.push_back(LocalScriptAllowedElement::JAVASCRIPT_TEMPLATE_SYNTAX);
        return allowedTypes; // 模板语法优先返回
    }
    
    // 检查模板变量引用
    if (isTemplateVariableReference(element)) {
        allowedTypes.push_back(LocalScriptAllowedElement::TEMPLATE_VARIABLE);
    }
    
    // 检查自定义变量引用
    if (isCustomVariableReference(element)) {
        allowedTypes.push_back(LocalScriptAllowedElement::CUSTOM_VARIABLE);
        // 检查是否是特例化
        if (isVariableSpecialization(element)) {
            allowedTypes.push_back(LocalScriptAllowedElement::VARIABLE_SPECIALIZATION);
        }
    }
    
    // 检查命名空间from子句
    if (isNamespaceFromClause(element)) {
        allowedTypes.push_back(LocalScriptAllowedElement::NAMESPACE_FROM);
    }
    
    // 对于局部script，默认认为是纯JavaScript代码（如果不是其他特殊类型）
    if (allowedTypes.empty() && isPureJavaScript(element)) {
        allowedTypes.push_back(LocalScriptAllowedElement::PURE_JAVASCRIPT);
    }
    
    return allowedTypes;
}

std::vector<LocalScriptForbiddenElement> CHtlLocalScriptConstraint::checkForbiddenCHtlSyntax(const std::string& scriptContent) {
    std::vector<LocalScriptForbiddenElement> foundForbidden;
    
    // 使用状态机解析器正确移除原始嵌入块
    RawEmbeddingParser parser;
    std::string contentWithoutOrigin = parser.removeRawEmbeddingBlocks(scriptContent);
    
    // 移除生成器注释
    std::regex commentPattern(R"(--[^\r\n]*)");
    std::string contentWithoutSpecial = std::regex_replace(contentWithoutOrigin, commentPattern, "");
    
    // 检查禁止的样式相关语法
    if (isForbiddenStyleSyntax(contentWithoutSpecial)) {
        foundForbidden.push_back(LocalScriptForbiddenElement::TEMPLATE_STYLE_GROUP);
        foundForbidden.push_back(LocalScriptForbiddenElement::CUSTOM_STYLE_GROUP);
    }
    
    // 检查禁止的元素相关语法
    if (isForbiddenElementSyntax(contentWithoutSpecial)) {
        foundForbidden.push_back(LocalScriptForbiddenElement::TEMPLATE_ELEMENT);
        foundForbidden.push_back(LocalScriptForbiddenElement::CUSTOM_ELEMENT);
    }
    
    // 检查样式块
    if (contentWithoutSpecial.find("style {") != std::string::npos) {
        foundForbidden.push_back(LocalScriptForbiddenElement::STYLE_BLOCKS);
    }
    
    // 检查文本块
    if (contentWithoutSpecial.find("text {") != std::string::npos) {
        foundForbidden.push_back(LocalScriptForbiddenElement::TEXT_BLOCKS);
    }
    
    // 检查继承语法
    if (contentWithoutSpecial.find("inherit @Style") != std::string::npos) {
        foundForbidden.push_back(LocalScriptForbiddenElement::INHERITANCE);
    }
    
    // 检查delete操作（样式相关）
    if (contentWithoutSpecial.find("delete @Style") != std::string::npos ||
        (contentWithoutSpecial.find("delete ") != std::string::npos && 
         contentWithoutSpecial.find("@Style") != std::string::npos)) {
        foundForbidden.push_back(LocalScriptForbiddenElement::DELETE_OPERATIONS);
    }
    
    // 检查样式/元素相关的全缀名
    if ((contentWithoutSpecial.find("[Custom] @Style") != std::string::npos ||
         contentWithoutSpecial.find("[Template] @Style") != std::string::npos ||
         contentWithoutSpecial.find("[Custom] @Element") != std::string::npos ||
         contentWithoutSpecial.find("[Template] @Element") != std::string::npos)) {
        foundForbidden.push_back(LocalScriptForbiddenElement::FULL_QUALIFIED_NAME);
    }
    
    return foundForbidden;
}

bool CHtlLocalScriptConstraint::validateTemplateVariableReference(const std::string& varReference) {
    // 模板变量格式: 
    // 1. VariableGroupName(variableName) - 简写形式
    // 2. @Var VariableGroupName(variableName) - 完整形式
    std::regex templateVarPattern(R"((?:@Var\s+)?([A-Za-z_][A-Za-z0-9_]*)\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*\))");
    return std::regex_search(varReference, templateVarPattern);
}

bool CHtlLocalScriptConstraint::validateCustomVariableReference(const std::string& varReference) {
    // 自定义变量格式: 
    // 1. VariableGroupName(variableName) - 简写形式
    // 2. @Var VariableGroupName(variableName) - 完整形式
    // 3. VariableGroupName(variableName = value) - 特例化
    std::regex customVarPattern(R"((?:@Var\s+)?([A-Za-z_][A-Za-z0-9_]*)\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*(?:=\s*([^)]+))?\s*\))");
    return std::regex_search(varReference, customVarPattern);
}

bool CHtlLocalScriptConstraint::validateVariableSpecialization(const std::string& specialization) {
    // 变量组特例化格式: VariableGroupName(variableName = value)
    std::regex specializationPattern(R"(([A-Za-z_][A-Za-z0-9_]*)\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*=\s*([^)]+)\s*\))");
    return std::regex_search(specialization, specializationPattern);
}

bool CHtlLocalScriptConstraint::validateFromClause(const std::string& fromClause) {
    // from子句格式: ElementName from namespace.path (用于导入)
    std::regex fromPattern(R"(([A-Za-z_][A-Za-z0-9_]*)\s+from\s+([A-Za-z_][A-Za-z0-9_./-]*))");
    return std::regex_search(fromClause, fromPattern);
}

bool CHtlLocalScriptConstraint::validateGeneratorComment(const std::string& comment) {
    // 生成器注释格式: --注释内容
    return comment.find("--") == 0 && comment.find("-->") == std::string::npos;
}

bool CHtlLocalScriptConstraint::validateRawEmbedding(const std::string& embedding) {
    // 使用状态机解析器验证原始嵌入
    RawEmbeddingParser parser;
    auto blocks = parser.parseRawEmbeddings(embedding);
    
    // 如果找到了原始嵌入块，并且这个语句主要是原始嵌入，则认为有效
    if (!blocks.empty()) {
        // 检查是否整个语句都被原始嵌入块覆盖（允许前后有少量空白）
        std::string trimmed = embedding;
        size_t start = trimmed.find_first_not_of(" \t\n\r");
        size_t end = trimmed.find_last_not_of(" \t\n\r");
        
        if (start != std::string::npos && end != std::string::npos) {
            // 检查第一个块是否覆盖了主要内容
            const auto& block = blocks[0];
            return block.startPos <= start && block.endPos >= end;
        }
    }
    
    return false;
}

bool CHtlLocalScriptConstraint::isJavaScriptTemplateSyntax(const std::string& text) {
    // JavaScript模板语法: {{variable}}, {{{html}}}, {{&unescaped}}, 等等
    // 这些是JavaScript本身具有的特供功能，不应被误禁
    std::regex jsTemplatePattern(R"(\{\{[^}]*\}\}|\{\{\{[^}]*\}\}\}|\{\{&[^}]*\}\})");
    return std::regex_search(text, jsTemplatePattern);
}

std::vector<std::string> CHtlLocalScriptConstraint::parseLocalScriptStatements(const std::string& content) {
    std::vector<std::string> statements;
    
    // 首先使用状态机解析器提取原始嵌入块
    RawEmbeddingParser parser;
    auto rawBlocks = parser.parseRawEmbeddings(content);
    
    // 创建一个标记原始嵌入位置的副本
    std::string remaining = content;
    std::vector<std::pair<size_t, size_t>> blockRanges;
    
    for (const auto& block : rawBlocks) {
        blockRanges.push_back({block.startPos, block.endPos});
        // 将原始嵌入块作为独立语句添加
        statements.push_back(content.substr(block.startPos, block.endPos - block.startPos));
    }
    
    // 移除已处理的原始嵌入块，按倒序避免位置偏移
    std::sort(blockRanges.begin(), blockRanges.end(), 
              [](const std::pair<size_t, size_t>& a, const std::pair<size_t, size_t>& b) {
                  return a.first > b.first;
              });
    
    for (const auto& range : blockRanges) {
        remaining.erase(range.first, range.second - range.first);
    }
    
    // 将剩余内容按行解析
    std::istringstream stream(remaining);
    std::string line;
    
    while (std::getline(stream, line)) {
        // 跳过空行和只有空白的行
        std::string trimmed = line;
        size_t start = trimmed.find_first_not_of(" \t");
        if (start != std::string::npos) {
            statements.push_back(line);
        }
    }
    
    return statements;
}

bool CHtlLocalScriptConstraint::isPureJavaScript(const std::string& statement) {
    // 检查是否为纯JavaScript代码
    // 排除CHTL特定语法，但允许JavaScript本身的语法
    
    if (statement.find("@") != std::string::npos || 
        statement.find("--") == 0 ||
        statement.find("[Origin]") != std::string::npos ||
        statement.find(" from ") != std::string::npos) {
        return false;
    }
    
    // JavaScript基本语法模式
    return true; // 默认认为是JavaScript代码
}

bool CHtlLocalScriptConstraint::isGeneratorComment(const std::string& statement) {
    // 生成器注释: --开头
    return statement.find("--") == 0 && statement.find("-->") == std::string::npos;
}

bool CHtlLocalScriptConstraint::isRawEmbedding(const std::string& statement) {
    // 使用状态机解析器检测原始嵌入
    RawEmbeddingParser parser;
    auto blocks = parser.parseRawEmbeddings(statement);
    
    // 如果找到原始嵌入块，检查是否是这个语句的主要内容
    if (!blocks.empty()) {
        std::string trimmed = statement;
        size_t start = trimmed.find_first_not_of(" \t\n\r");
        size_t end = trimmed.find_last_not_of(" \t\n\r");
        
        if (start != std::string::npos && end != std::string::npos) {
            // 检查原始嵌入是否从语句开始或接近开始
            const auto& block = blocks[0];
            return block.startPos <= start + 5; // 允许少量偏移
        }
    }
    
    return false;
}

bool CHtlLocalScriptConstraint::isTemplateVariableReference(const std::string& statement) {
    // 模板变量引用: VariableGroupName(variableName) 或 @Var VariableGroupName(variableName)
    return validateTemplateVariableReference(statement);
}

bool CHtlLocalScriptConstraint::isCustomVariableReference(const std::string& statement) {
    // 自定义变量引用: VariableGroupName(variableName) 或特例化形式
    return validateCustomVariableReference(statement);
}

bool CHtlLocalScriptConstraint::isVariableSpecialization(const std::string& statement) {
    // 变量特例化: 包含 = 的变量引用
    return statement.find('=') != std::string::npos && validateVariableSpecialization(statement);
}

bool CHtlLocalScriptConstraint::isNamespaceFromClause(const std::string& statement) {
    // 命名空间from子句: from关键字
    return statement.find(" from ") != std::string::npos;
}

bool CHtlLocalScriptConstraint::isForbiddenStyleSyntax(const std::string& statement) {
    // 检查是否为禁止的样式相关CHTL语法
    return statement.find("@Style") != std::string::npos ||
           statement.find("inherit @Style") != std::string::npos ||
           statement.find("delete @Style") != std::string::npos ||
           statement.find("[Custom] @Style") != std::string::npos ||
           statement.find("[Template] @Style") != std::string::npos;
}

bool CHtlLocalScriptConstraint::isForbiddenElementSyntax(const std::string& statement) {
    // 检查是否为禁止的元素相关CHTL语法
    return statement.find("@Element") != std::string::npos ||
           statement.find("text {") != std::string::npos ||
           statement.find("[Custom] @Element") != std::string::npos ||
           statement.find("[Template] @Element") != std::string::npos;
}

void CHtlLocalScriptConstraint::addViolation(LocalScriptConstraintResult& result, 
                                           LocalScriptConstraintViolationType type,
                                           LocalScriptForbiddenElement forbiddenElement,
                                           const std::string& message,
                                           const std::string& element,
                                           size_t line, 
                                           size_t column) {
    LocalScriptConstraintViolation violation;
    violation.type = type;
    violation.forbiddenElement = forbiddenElement;
    violation.message = message;
    violation.element = element;
    violation.line = line;
    violation.column = column;
    
    result.violations.push_back(violation);
}

} // namespace chtl