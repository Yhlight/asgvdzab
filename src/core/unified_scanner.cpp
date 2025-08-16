#include "core/unified_scanner.hpp"
#include <regex>
#include <sstream>
#include <iostream>
#include <algorithm>

namespace chtl {

// ==================== LanguageDetector 实现 ====================

LanguageType LanguageDetector::detectLanguage(const std::string& content, const std::string& context) {
    if (context == "style" || context == "local_style") {
        return isCssSyntax(content) ? LanguageType::CSS : LanguageType::CHTL;
    } else if (context == "script" || context == "local_script") {
        return isJavaScriptSyntax(content) ? LanguageType::CHTL_JS : LanguageType::JAVASCRIPT;
    } else if (context == "origin_html") {
        return LanguageType::HTML;
    } else if (context == "origin_css") {
        return LanguageType::CSS;
    } else if (context == "origin_js") {
        return LanguageType::JAVASCRIPT;
    }
    
    // 自动检测
    if (isChtlSyntax(content)) {
        return LanguageType::CHTL;
    } else if (isCssSyntax(content)) {
        return LanguageType::CSS;
    } else if (isJavaScriptSyntax(content)) {
        return LanguageType::JAVASCRIPT;
    }
    
    return LanguageType::MIXED;
}

bool LanguageDetector::isChtlSyntax(const std::string& content) {
    // 检测CHTL特有语法
    std::vector<std::regex> chtlPatterns = {
        std::regex(R"(@(Var|Style|Element)\s+\w+)"),           // @Var/@Style/@Element
        std::regex(R"(\[(?:Custom|Template)\])"),              // [Custom]/[Template]
        std::regex(R"(\[Origin\]\s+@\w+)"),                    // [Origin] @Type
        std::regex(R"(\[Import\]\s+@\w+)"),                    // [Import] @Type
        std::regex(R"(\[Namespace\])"),                        // [Namespace]
        std::regex(R"(\[Configuration\])"),                    // [Configuration]
        std::regex(R"(\w+\s+from\s+[\w./]+)"),                 // from语句
        std::regex(R"(inherit\s+@\w+)"),                       // inherit语句
        std::regex(R"(delete\s+@?\w+)"),                       // delete语句
        std::regex(R"(\w+\(\w+\s*(?:=\s*[^)]+)?\))"),          // 变量组语法
        std::regex(R"(--.*)"),                                 // 生成器注释
    };
    
    for (const auto& pattern : chtlPatterns) {
        if (std::regex_search(content, pattern)) {
            return true;
        }
    }
    
    return false;
}

bool LanguageDetector::isCssSyntax(const std::string& content) {
    // 检测CSS特有语法
    std::vector<std::regex> cssPatterns = {
        std::regex(R"([.#]?[\w-]+\s*\{[^}]*\})"),              // CSS选择器和规则
        std::regex(R"([\w-]+\s*:\s*[^;]+;)"),                  // CSS属性
        std::regex(R"(@(?:media|import|keyframes|charset))"),   // CSS @规则
        std::regex(R"(:\w+\s*\()"),                            // CSS伪类/函数
    };
    
    for (const auto& pattern : cssPatterns) {
        if (std::regex_search(content, pattern)) {
            return true;
        }
    }
    
    return false;
}

bool LanguageDetector::isJavaScriptSyntax(const std::string& content) {
    // 检测JavaScript特有语法
    std::vector<std::regex> jsPatterns = {
        std::regex(R"(\b(?:function|var|let|const|if|else|for|while|return|class|import|export)\b)"),
        std::regex(R"(\{[\s\S]*\})"),                          // 对象字面量
        std::regex(R"(\[[\s\S]*\])"),                          // 数组字面量
        std::regex(R"(=>\s*\{)"),                              // 箭头函数
        std::regex(R"(\.\w+\s*\()"),                           // 方法调用
        std::regex(R"(\{\{[^}]*\}\})"),                        // 模板语法
    };
    
    for (const auto& pattern : jsPatterns) {
        if (std::regex_search(content, pattern)) {
            return true;
        }
    }
    
    return false;
}

// ==================== UnifiedScanner 实现 ====================

UnifiedScanner::UnifiedScanner() 
    : config_(nullptr), debugMode_(false) {
}

UnifiedScanner::UnifiedScanner(CHtlConfiguration* config) 
    : config_(config), debugMode_(false) {
}

ScanResult UnifiedScanner::scanChtlFile(const std::string& content, const std::string& filename) {
    ScanResult result;
    
    debugLog("开始扫描CHTL文件: " + filename);
    
    // 1. 应用配置
    if (!filename.empty()) {
        CHtlConfigurationManager::applyConfigurationFromChtlFile(content);
        if (!config_) {
            config_ = &CHtlConfigurationManager::getInstance();
        }
    }
    
    // 2. 扫描原始嵌入块
    scanRawEmbeddingBlocks(content, result);
    
    // 3. 扫描局部样式块
    auto styleBlocks = findLocalBlocks(content, "style");
    for (const auto& block : styleBlocks) {
        scanLocalStyleBlock(content.substr(block.first, block.second - block.first), block.first, result);
    }
    
    // 4. 扫描局部脚本块
    auto scriptBlocks = findLocalBlocks(content, "script");
    for (const auto& block : scriptBlocks) {
        scanLocalScriptBlock(content.substr(block.first, block.second - block.first), block.first, result);
    }
    
    // 5. 扫描CHTL主体内容
    scanChtlMainContent(content, result);
    
    // 6. 扫描CHTL语法元素
    scanChtlElements(content, result);
    
    // 7. 添加元数据
    result.metadata["filename"] = filename;
    result.metadata["language_count"] = std::to_string(result.codeBlocks.size());
    result.metadata["chtl_elements_count"] = std::to_string(result.chtlElements.size());
    
    debugLog("扫描完成，找到 " + std::to_string(result.codeBlocks.size()) + " 个代码块");
    
    return result;
}

ScanResult UnifiedScanner::scanCodeBlock(const std::string& content, LanguageType expectedType) {
    ScanResult result;
    
    LanguageType detectedType = LanguageDetector::detectLanguage(content);
    
    CodeBlock block;
    block.content = content;
    block.language = (expectedType != LanguageType::MIXED) ? expectedType : detectedType;
    block.startLine = 1;
    block.endLine = std::count(content.begin(), content.end(), '\n') + 1;
    block.startPos = 0;
    block.endPos = content.length();
    
    result.codeBlocks.push_back(block);
    
    return result;
}

void UnifiedScanner::scanRawEmbeddingBlocks(const std::string& content, ScanResult& result) {
    auto rawBlocks = rawEmbeddingParser_.parseRawEmbeddings(content);
    
    for (const auto& rawBlock : rawBlocks) {
        CodeBlock block;
        block.content = rawBlock.content;
        block.startPos = rawBlock.startPos;
        block.endPos = rawBlock.endPos;
        block.startLine = calculateLineNumber(content, rawBlock.startPos);
        block.endLine = calculateLineNumber(content, rawBlock.endPos);
        block.blockType = "Origin";
        block.blockName = rawBlock.name;
        block.isOriginBlock = true;
        
        // 根据类型确定语言
        if (rawBlock.type == "Html") {
            block.language = LanguageType::HTML;
            block.context = "origin_html";
        } else if (rawBlock.type == "Style") {
            block.language = LanguageType::CSS;
            block.context = "origin_css";
        } else if (rawBlock.type == "JavaScript") {
            block.language = LanguageType::JAVASCRIPT;
            block.context = "origin_js";
        } else {
            // 自定义类型，根据内容检测
            block.language = LanguageDetector::detectLanguage(rawBlock.content);
            block.context = "origin_custom";
        }
        
        result.codeBlocks.push_back(block);
        
        // 添加CHTL元素
        ChtlElement element;
        element.type = ChtlElement::RAW_EMBEDDING;
        element.name = rawBlock.type;
        element.value = rawBlock.name;
        element.line = block.startLine;
        element.position = rawBlock.startPos;
        result.chtlElements.push_back(element);
        
        debugLog("找到原始嵌入: " + rawBlock.type + " " + rawBlock.name);
    }
}

void UnifiedScanner::scanLocalStyleBlock(const std::string& content, size_t startPos, ScanResult& result) {
    CodeBlock block;
    block.content = content;
    block.language = LanguageType::CSS;
    block.startPos = startPos;
    block.endPos = startPos + content.length();
    block.startLine = calculateLineNumber(result.metadata["full_content"], startPos);
    block.endLine = calculateLineNumber(result.metadata["full_content"], startPos + content.length());
    block.blockType = "style";
    block.isLocalBlock = true;
    block.context = "local_style";
    
    // 检测是否包含CHTL语法
    if (LanguageDetector::isChtlSyntax(content)) {
        block.language = LanguageType::CHTL;
        addWarning(result, "局部样式块包含CHTL语法，建议使用纯CSS");
    }
    
    result.codeBlocks.push_back(block);
    debugLog("找到局部样式块");
}

void UnifiedScanner::scanLocalScriptBlock(const std::string& content, size_t startPos, ScanResult& result) {
    CodeBlock block;
    block.content = content;
    block.startPos = startPos;
    block.endPos = startPos + content.length();
    block.startLine = calculateLineNumber(result.metadata["full_content"], startPos);
    block.endLine = calculateLineNumber(result.metadata["full_content"], startPos + content.length());
    block.blockType = "script";
    block.isLocalBlock = true;
    
    // 检测语言类型
    bool hasChtlSyntax = LanguageDetector::isChtlSyntax(content);
    bool hasJsSyntax = LanguageDetector::isJavaScriptSyntax(content);
    
    if (hasChtlSyntax && hasJsSyntax) {
        block.language = LanguageType::CHTL_JS;
        block.context = "local_script_mixed";
    } else if (hasChtlSyntax) {
        block.language = LanguageType::CHTL;
        block.context = "local_script_chtl";
    } else {
        block.language = LanguageType::JAVASCRIPT;
        block.context = "local_script_js";
    }
    
    result.codeBlocks.push_back(block);
    debugLog("找到局部脚本块: " + block.context);
}

void UnifiedScanner::scanChtlMainContent(const std::string& content, ScanResult& result) {
    // 移除已扫描的块（原始嵌入、局部块等）
    std::string mainContent = content;
    
    // 移除原始嵌入块
    mainContent = rawEmbeddingParser_.removeRawEmbeddingBlocks(mainContent);
    
    // 移除局部块
    // TODO: 实现更精确的局部块移除逻辑
    
    if (!mainContent.empty()) {
        CodeBlock block;
        block.content = mainContent;
        block.language = LanguageType::CHTL;
        block.startLine = 1;
        block.endLine = std::count(mainContent.begin(), mainContent.end(), '\n') + 1;
        block.startPos = 0;
        block.endPos = mainContent.length();
        block.blockType = "main";
        block.context = "chtl_main";
        
        result.codeBlocks.push_back(block);
        debugLog("扫描CHTL主体内容");
    }
}

void UnifiedScanner::scanChtlElements(const std::string& content, ScanResult& result) {
    std::istringstream stream(content);
    std::string line;
    size_t lineNumber = 1;
    
    while (std::getline(stream, line)) {
        ChtlElement element;
        element.line = lineNumber;
        
        // 尝试解析不同类型的CHTL元素
        if (parseTemplateVariable(line, element) ||
            parseCustomVariable(line, element) ||
            parseStyleGroup(line, element) ||
            parseElementDefinition(line, element) ||
            parseImportStatement(line, element) ||
            parseNamespaceDeclaration(line, element)) {
            
            result.chtlElements.push_back(element);
            debugLog("找到CHTL元素: " + element.name);
        }
        
        lineNumber++;
    }
}

bool UnifiedScanner::parseTemplateVariable(const std::string& line, ChtlElement& element) {
    // 模板变量: VariableGroupName(variableName) 或 @Var VariableGroupName(variableName)
    std::regex pattern(R"((?:@Var\s+)?(\w+)\((\w+)(?:\s*=\s*([^)]+))?\))");
    std::smatch match;
    
    if (std::regex_search(line, match, pattern)) {
        element.type = ChtlElement::TEMPLATE_VARIABLE;
        element.name = match[2].str();  // 变量名
        element.namespace_ = match[1].str();  // 变量组名
        if (match[3].matched) {
            element.value = match[3].str();  // 默认值
        }
        return true;
    }
    
    return false;
}

bool UnifiedScanner::parseCustomVariable(const std::string& line, ChtlElement& element) {
    // 自定义变量（模式与模板变量相同，但上下文不同）
    if (parseTemplateVariable(line, element)) {
        element.type = ChtlElement::CUSTOM_VARIABLE;
        return true;
    }
    
    return false;
}

bool UnifiedScanner::parseStyleGroup(const std::string& line, ChtlElement& element) {
    // 样式组: @Style StyleGroupName 或 [Custom] @Style StyleGroupName
    std::regex pattern(R"((?:\[(?:Custom|Template)\]\s+)?@Style\s+(\w+))");
    std::smatch match;
    
    if (std::regex_search(line, match, pattern)) {
        element.type = line.find("[Custom]") != std::string::npos ? 
                      ChtlElement::CUSTOM_STYLE_GROUP : ChtlElement::TEMPLATE_STYLE_GROUP;
        element.name = match[1].str();
        return true;
    }
    
    return false;
}

bool UnifiedScanner::parseElementDefinition(const std::string& line, ChtlElement& element) {
    // 元素定义: @Element ElementName 或 [Custom] @Element ElementName
    std::regex pattern(R"((?:\[(?:Custom|Template)\]\s+)?@Element\s+(\w+))");
    std::smatch match;
    
    if (std::regex_search(line, match, pattern)) {
        element.type = line.find("[Custom]") != std::string::npos ? 
                      ChtlElement::CUSTOM_ELEMENT : ChtlElement::TEMPLATE_ELEMENT;
        element.name = match[1].str();
        return true;
    }
    
    return false;
}

bool UnifiedScanner::parseImportStatement(const std::string& line, ChtlElement& element) {
    // 导入语句: [Import] @Type from path/file
    std::regex pattern(R"(\[Import\]\s+@(\w+)\s+from\s+([\w./]+))");
    std::smatch match;
    
    if (std::regex_search(line, match, pattern)) {
        element.type = ChtlElement::IMPORT_STATEMENT;
        element.name = match[1].str();  // 导入类型
        element.value = match[2].str(); // 路径
        return true;
    }
    
    return false;
}

bool UnifiedScanner::parseNamespaceDeclaration(const std::string& line, ChtlElement& element) {
    // 命名空间声明: [Namespace] name
    std::regex pattern(R"(\[Namespace\]\s+(\w+))");
    std::smatch match;
    
    if (std::regex_search(line, match, pattern)) {
        element.type = ChtlElement::NAMESPACE_DECLARATION;
        element.name = match[1].str();
        return true;
    }
    
    return false;
}

std::vector<std::pair<size_t, size_t>> UnifiedScanner::findLocalBlocks(const std::string& content, const std::string& blockType) {
    std::vector<std::pair<size_t, size_t>> blocks;
    
    // 查找 blockType { ... } 模式
    std::regex blockPattern(blockType + R"(\s*\{)");
    std::sregex_iterator iter(content.begin(), content.end(), blockPattern);
    std::sregex_iterator end;
    
    for (; iter != end; ++iter) {
        const auto& match = *iter;
        size_t startPos = match.position();
        
        // 查找匹配的右大括号
        size_t bracePos = startPos + match.length() - 1; // 左大括号位置
        size_t endPos = findMatchingBrace(content, bracePos);
        
        if (endPos != std::string::npos) {
            blocks.push_back({startPos, endPos + 1});
        }
    }
    
    return blocks;
}

size_t UnifiedScanner::calculateLineNumber(const std::string& content, size_t position) {
    if (position >= content.length()) {
        return std::count(content.begin(), content.end(), '\n') + 1;
    }
    
    return std::count(content.begin(), content.begin() + position, '\n') + 1;
}

size_t UnifiedScanner::findMatchingBrace(const std::string& content, size_t leftBracePos) {
    if (leftBracePos >= content.length() || content[leftBracePos] != '{') {
        return std::string::npos;
    }
    
    int braceCount = 1;
    size_t pos = leftBracePos + 1;
    
    while (pos < content.length() && braceCount > 0) {
        char ch = content[pos];
        if (ch == '{') {
            braceCount++;
        } else if (ch == '}') {
            braceCount--;
        }
        pos++;
    }
    
    return (braceCount == 0) ? pos - 1 : std::string::npos;
}

void UnifiedScanner::addError(ScanResult& result, const std::string& message) {
    result.errors.push_back(message);
    if (errorHandler_) {
        errorHandler_("错误: " + message);
    }
}

void UnifiedScanner::addWarning(ScanResult& result, const std::string& message) {
    result.warnings.push_back(message);
    if (errorHandler_) {
        errorHandler_("警告: " + message);
    }
}

void UnifiedScanner::debugLog(const std::string& message) {
    if (debugMode_) {
        std::cout << "[UnifiedScanner] " << message << std::endl;
    }
}

// ==================== ScannerFactory 实现 ====================

std::unique_ptr<UnifiedScanner> ScannerFactory::createUnifiedScanner(CHtlConfiguration* config) {
    return std::make_unique<UnifiedScanner>(config);
}

std::unique_ptr<UnifiedScanner> ScannerFactory::createLanguageScanner(LanguageType language, CHtlConfiguration* config) {
    auto scanner = std::make_unique<UnifiedScanner>(config);
    
    // 根据语言类型设置特定的扫描行为
    // TODO: 实现语言特定的优化
    
    return scanner;
}

} // namespace chtl