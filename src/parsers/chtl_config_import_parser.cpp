#include "parsers/chtl_config_import_parser.hpp"
#include "parsers/chtl_basic_parser.hpp"
#include <regex>
#include <algorithm>
#include <sstream>
#include <filesystem>

namespace chtl {

// ===== ConfigurationRegistry 实现 =====

ConfigurationRegistry::ConfigurationRegistry() : initialized_(false) {
    initializeDefaults();
}

ConfigurationRegistry::~ConfigurationRegistry() = default;

bool ConfigurationRegistry::registerConfiguration(const ConfigurationGroup& config) {
    // 合并配置项
    for (const auto& item : config.items) {
        globalConfig_.items[item.first] = item.second;
    }
    
    // 合并Name配置
    if (config.hasNameConfig) {
        globalConfig_.nameConfig = config.nameConfig;
        globalConfig_.hasNameConfig = true;
    }
    
    return true;
}

const ConfigurationItem* ConfigurationRegistry::getConfigurationItem(const std::string& name) const {
    auto it = globalConfig_.items.find(name);
    return (it != globalConfig_.items.end()) ? &it->second : nullptr;
}

const NameConfiguration* ConfigurationRegistry::getNameConfiguration() const {
    return globalConfig_.hasNameConfig ? &globalConfig_.nameConfig : nullptr;
}

std::vector<std::string> ConfigurationRegistry::getKeywordMapping(const std::string& keyword) const {
    if (!globalConfig_.hasNameConfig) {
        return {};
    }
    
    auto it = globalConfig_.nameConfig.keywordMappings.find(keyword);
    return (it != globalConfig_.nameConfig.keywordMappings.end()) ? it->second : std::vector<std::string>{};
}

bool ConfigurationRegistry::hasConfiguration(const std::string& name) const {
    return globalConfig_.items.find(name) != globalConfig_.items.end();
}

void ConfigurationRegistry::clear() {
    globalConfig_.items.clear();
    globalConfig_.hasNameConfig = false;
    globalConfig_.nameConfig.keywordMappings.clear();
    initialized_ = false;
}

void ConfigurationRegistry::applyDefaultConfiguration() {
    if (!initialized_) {
        initializeDefaults();
    }
}

void ConfigurationRegistry::initializeDefaults() {
    // 应用默认配置项
    globalConfig_.items["INDEX_INITIAL_COUNT"] = ConfigurationItem("INDEX_INITIAL_COUNT", ConfigurationType::INTEGER, "0");
    globalConfig_.items["DISABLE_NAME_GROUP"] = ConfigurationItem("DISABLE_NAME_GROUP", ConfigurationType::BOOLEAN, "false");
    globalConfig_.items["DEBUG_MODE"] = ConfigurationItem("DEBUG_MODE", ConfigurationType::BOOLEAN, "false");
    
    // 默认Name配置
    globalConfig_.hasNameConfig = true;
    globalConfig_.nameConfig.keywordMappings["CUSTOM_STYLE"] = {"@Style"};
    globalConfig_.nameConfig.keywordMappings["CUSTOM_ELEMENT"] = {"@Element"};
    globalConfig_.nameConfig.keywordMappings["CUSTOM_VAR"] = {"@Var"};
    globalConfig_.nameConfig.keywordMappings["TEMPLATE_STYLE"] = {"@Style"};
    globalConfig_.nameConfig.keywordMappings["TEMPLATE_ELEMENT"] = {"@Element"};
    globalConfig_.nameConfig.keywordMappings["TEMPLATE_VAR"] = {"@Var"};
    globalConfig_.nameConfig.keywordMappings["ORIGIN_HTML"] = {"@Html"};
    globalConfig_.nameConfig.keywordMappings["ORIGIN_STYLE"] = {"@Style"};
    globalConfig_.nameConfig.keywordMappings["ORIGIN_JAVASCRIPT"] = {"@JavaScript"};
    globalConfig_.nameConfig.keywordMappings["IMPORT_HTML"] = {"@Html"};
    globalConfig_.nameConfig.keywordMappings["IMPORT_STYLE"] = {"@Style"};
    globalConfig_.nameConfig.keywordMappings["IMPORT_JAVASCRIPT"] = {"@JavaScript"};
    globalConfig_.nameConfig.keywordMappings["IMPORT_CHTL"] = {"@Chtl"};
    globalConfig_.nameConfig.keywordMappings["IMPORT_CJMOD"] = {"@CJmod"};
    globalConfig_.nameConfig.keywordMappings["KEYWORD_INHERIT"] = {"inherit"};
    globalConfig_.nameConfig.keywordMappings["KEYWORD_DELETE"] = {"delete"};
    globalConfig_.nameConfig.keywordMappings["KEYWORD_INSERT"] = {"insert"};
    globalConfig_.nameConfig.keywordMappings["KEYWORD_AFTER"] = {"after"};
    globalConfig_.nameConfig.keywordMappings["KEYWORD_BEFORE"] = {"before"};
    globalConfig_.nameConfig.keywordMappings["KEYWORD_REPLACE"] = {"replace"};
    globalConfig_.nameConfig.keywordMappings["KEYWORD_ATTOP"] = {"at top"};
    globalConfig_.nameConfig.keywordMappings["KEYWORD_ATBOTTOM"] = {"at bottom"};
    globalConfig_.nameConfig.keywordMappings["KEYWORD_FROM"] = {"from"};
    globalConfig_.nameConfig.keywordMappings["KEYWORD_AS"] = {"as"};
    globalConfig_.nameConfig.keywordMappings["KEYWORD_EXCEPT"] = {"except"};
    globalConfig_.nameConfig.keywordMappings["KEYWORD_TEXT"] = {"text"};
    globalConfig_.nameConfig.keywordMappings["KEYWORD_STYLE"] = {"style"};
    globalConfig_.nameConfig.keywordMappings["KEYWORD_SCRIPT"] = {"script"};
    globalConfig_.nameConfig.keywordMappings["KEYWORD_CUSTOM"] = {"[Custom]"};
    globalConfig_.nameConfig.keywordMappings["KEYWORD_TEMPLATE"] = {"[Template]"};
    globalConfig_.nameConfig.keywordMappings["KEYWORD_ORIGIN"] = {"[Origin]"};
    globalConfig_.nameConfig.keywordMappings["KEYWORD_IMPORT"] = {"[Import]"};
    globalConfig_.nameConfig.keywordMappings["KEYWORD_NAMESPACE"] = {"[Namespace]"};
    globalConfig_.nameConfig.optionCount = 3;
    
    initialized_ = true;
}

// ===== ImportRegistry 实现 =====

ImportRegistry::ImportRegistry() = default;

ImportRegistry::~ImportRegistry() = default;

bool ImportRegistry::registerImport(const ImportDefinition& importDef) {
    // 检查循环依赖
    if (hasCircularDependency(importDef.path)) {
        return false;
    }
    
    // 标准化路径
    std::string normalizedPath = normalizePath(importDef.path);
    
    // 检查是否已导入
    if (importedPaths_.find(normalizedPath) != importedPaths_.end()) {
        return false; // 重复导入
    }
    
    imports_.push_back(importDef);
    importedPaths_.insert(normalizedPath);
    return true;
}

const ImportDefinition* ImportRegistry::findImport(const std::string& identifier) const {
    for (const auto& importDef : imports_) {
        if (!importDef.alias.empty() && importDef.alias == identifier) {
            return &importDef;
        }
        if (!importDef.targetName.empty() && importDef.targetName == identifier) {
            return &importDef;
        }
    }
    return nullptr;
}

const std::vector<ImportDefinition>& ImportRegistry::getAllImports() const {
    return imports_;
}

bool ImportRegistry::hasCircularDependency(const std::string& path) const {
    std::string normalizedPath = normalizePath(path);
    return importedPaths_.find(normalizedPath) != importedPaths_.end();
}

void ImportRegistry::clear() {
    imports_.clear();
    importedPaths_.clear();
}

std::string ImportRegistry::resolveImportPath(const std::string& path, ImportPathType pathType) const {
    switch (pathType) {
        case ImportPathType::RELATIVE_PATH:
            return path;
        case ImportPathType::ABSOLUTE_PATH:
            return path;
        case ImportPathType::MODULE_PATH:
            // 将'.'替换为'/'
            return std::regex_replace(path, std::regex("\\."), "/");
        case ImportPathType::WILDCARD_PATH:
            return path;
    }
    return path;
}

std::string ImportRegistry::normalizePath(const std::string& path) const {
    // 简单的路径标准化
    std::string normalized = path;
    
    // 将'.'替换为'/'
    normalized = std::regex_replace(normalized, std::regex("\\."), "/");
    
    // 移除双斜杠
    normalized = std::regex_replace(normalized, std::regex("//+"), "/");
    
    return normalized;
}

bool ImportRegistry::isValidPath(const std::string& path) const {
    if (path.empty()) return false;
    
    // 基本路径验证
    std::regex pathRegex(R"(^[a-zA-Z0-9._/\-]+$)");
    return std::regex_match(path, pathRegex);
}

// ===== ConfigImportScanner 实现 =====

ConfigImportScanner::ConfigImportScanner() = default;

ConfigImportScanner::~ConfigImportScanner() = default;

std::vector<ConfigurationItem> ConfigImportScanner::scanConfigurationContent(const std::string& content) {
    std::vector<ConfigurationItem> items;
    
    std::istringstream stream(content);
    std::string line;
    int lineNumber = 1;
    
    while (std::getline(stream, line)) {
        line = trimWhitespace(line);
        
        // 跳过空行和注释
        if (line.empty() || line.substr(0, 2) == "//") {
            lineNumber++;
            continue;
        }
        
        // 解析配置项
        ConfigurationItem item = parseConfigurationItem(line);
        if (!item.name.empty()) {
            item.position = Position{static_cast<size_t>(lineNumber), 1};
            items.push_back(item);
        }
        
        lineNumber++;
    }
    
    return items;
}

ImportDefinition ConfigImportScanner::scanImportStatement(const std::string& statement) {
    ImportDefinition importDef;
    
    // 解析导入语句的正则表达式
    std::regex importRegex(R"(\[Import\]\s*(?:\[(Custom|Template)\])?\s*@(\w+)(?:\s+(\w+))?\s+from\s+([^\s;]+)(?:\s+as\s+(\w+))?)", std::regex_constants::icase);
    std::smatch match;
    
    if (std::regex_search(statement, match, importRegex)) {
        std::string prefix = match[1].str();  // Custom 或 Template
        std::string type = match[2].str();    // Html, Style, JavaScript, etc.
        std::string target = match[3].str();  // 目标名称 (可选)
        std::string path = match[4].str();    // 路径
        std::string alias = match[5].str();   // 别名 (可选)
        
        // 解析导入类型
        importDef.type = parseImportType(type, !prefix.empty());
        importDef.targetName = target;
        importDef.path = path;
        importDef.alias = alias;
        importDef.pathType = determinePathType(path);
    }
    
    return importDef;
}

std::vector<std::string> ConfigImportScanner::scanArrayValues(const std::string& arrayContent) {
    std::vector<std::string> values;
    
    // 移除方括号
    std::string content = arrayContent;
    if (!content.empty() && content.front() == '[' && content.back() == ']') {
        content = content.substr(1, content.length() - 2);
    }
    
    // 按逗号分割
    std::istringstream stream(content);
    std::string value;
    
    while (std::getline(stream, value, ',')) {
        value = trimWhitespace(value);
        if (!value.empty()) {
            values.push_back(value);
        }
    }
    
    return values;
}

bool ConfigImportScanner::validateConfigurationSyntax(const std::string& content) {
    // 基本的配置语法验证
    std::regex configRegex(R"(^\s*[A-Z_][A-Z0-9_]*\s*=\s*.+;\s*$)");
    return std::regex_match(content, configRegex);
}

bool ConfigImportScanner::validateImportSyntax(const std::string& statement) {
    // 基本的导入语法验证
    std::regex importRegex(R"(\[Import\].*@\w+.*from.*)", std::regex_constants::icase);
    return std::regex_search(statement, importRegex);
}

ConfigurationItem ConfigImportScanner::parseConfigurationItem(const std::string& line) {
    ConfigurationItem item;
    
    // 解析配置项的正则表达式: NAME = VALUE;
    std::regex configRegex(R"(^\s*([A-Z_][A-Z0-9_]*)\s*=\s*(.+);\s*$)");
    std::smatch match;
    
    if (std::regex_match(line, match, configRegex)) {
        item.name = match[1].str();
        std::string value = trimWhitespace(match[2].str());
        
        item.type = determineConfigurationType(value);
        
        if (item.type == ConfigurationType::ARRAY) {
            item.arrayValues = scanArrayValues(value);
        } else {
            item.value = value;
        }
    }
    
    return item;
}

ConfigurationType ConfigImportScanner::determineConfigurationType(const std::string& value) {
    if (value == "true" || value == "false") {
        return ConfigurationType::BOOLEAN;
    }
    
    if (std::regex_match(value, std::regex(R"(^\d+$)"))) {
        return ConfigurationType::INTEGER;
    }
    
    if (!value.empty() && value.front() == '[' && value.back() == ']') {
        return ConfigurationType::ARRAY;
    }
    
    return ConfigurationType::STRING;
}

ImportType ConfigImportScanner::parseImportType(const std::string& typeStr, bool hasPrefix) {
    if (typeStr == "Html") {
        return hasPrefix ? ImportType::CUSTOM_ELEMENT : ImportType::HTML_IMPORT;
    } else if (typeStr == "Style") {
        return hasPrefix ? ImportType::CUSTOM_STYLE : ImportType::STYLE_IMPORT;
    } else if (typeStr == "JavaScript") {
        return ImportType::JAVASCRIPT_IMPORT;
    } else if (typeStr == "Chtl") {
        return ImportType::CHTL_IMPORT;
    } else if (typeStr == "CJmod") {
        return ImportType::CJMOD_IMPORT;
    } else if (typeStr == "Element") {
        return hasPrefix ? ImportType::TEMPLATE_ELEMENT : ImportType::CUSTOM_ELEMENT;
    } else if (typeStr == "Var") {
        return hasPrefix ? ImportType::TEMPLATE_VAR : ImportType::CUSTOM_VAR;
    }
    
    return ImportType::HTML_IMPORT;
}

ImportPathType ConfigImportScanner::determinePathType(const std::string& path) {
    if (!path.empty() && path.front() == '/') {
        return ImportPathType::ABSOLUTE_PATH;
    }
    
    if (path.find("*") != std::string::npos) {
        return ImportPathType::WILDCARD_PATH;
    }
    
    // 检查是否以文件扩展名结尾
    bool hasFileExtension = (path.length() >= 5 && path.substr(path.length()-5) == ".chtl") ||
                           (path.length() >= 5 && path.substr(path.length()-5) == ".html") ||
                           (path.length() >= 4 && path.substr(path.length()-4) == ".css") ||
                           (path.length() >= 3 && path.substr(path.length()-3) == ".js");
    
    if (path.find(".") != std::string::npos && !hasFileExtension) {
        return ImportPathType::MODULE_PATH;
    }
    
    return ImportPathType::RELATIVE_PATH;
}

std::string ConfigImportScanner::trimWhitespace(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    
    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

std::vector<std::string> ConfigImportScanner::splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream stream(str);
    std::string token;
    
    while (std::getline(stream, token, delimiter)) {
        token = trimWhitespace(token);
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    
    return tokens;
}

bool ConfigImportScanner::isValidIdentifier(const std::string& identifier) {
    if (identifier.empty()) return false;
    
    std::regex identifierRegex(R"(^[a-zA-Z_][a-zA-Z0-9_]*$)");
    return std::regex_match(identifier, identifierRegex);
}

// ===== ConfigImportStateMachine 实现 =====

ConfigImportStateMachine::ConfigImportStateMachine() = default;

ConfigImportStateMachine::~ConfigImportStateMachine() = default;

void ConfigImportStateMachine::reset() {
    configGroup_ = ConfigurationGroup{};
    importDefinitions_.clear();
    currentItemName_.clear();
    currentItemValue_.clear();
    currentArrayValues_.clear();
    currentImport_ = ImportDefinition{};
}

bool ConfigImportStateMachine::processConfigurationToken(const Token& token, ConfigImportContext& context) {
    switch (context.configState) {
        case ConfigParseState::INITIAL:
            handleConfigInitialState(token, context);
            break;
        case ConfigParseState::READING_CONFIG_HEADER:
            handleConfigHeaderState(token, context);
            break;
        case ConfigParseState::READING_CONFIG_CONTENT:
            handleConfigContentState(token, context);
            break;
        case ConfigParseState::READING_CONFIG_ITEM:
            handleConfigItemState(token, context);
            break;
        case ConfigParseState::READING_NAME_BLOCK:
            handleNameBlockState(token, context);
            break;
        case ConfigParseState::READING_NAME_ITEM:
            handleNameItemState(token, context);
            break;
        case ConfigParseState::READING_ARRAY_VALUES:
            handleArrayValuesState(token, context);
            break;
    }
    
    return true;
}

bool ConfigImportStateMachine::processImportToken(const Token& token, ConfigImportContext& context) {
    switch (context.importState) {
        case ImportParseState::INITIAL:
            handleImportInitialState(token, context);
            break;
        case ImportParseState::READING_IMPORT_HEADER:
            handleImportHeaderState(token, context);
            break;
        case ImportParseState::READING_IMPORT_TYPE:
            handleImportTypeState(token, context);
            break;
        case ImportParseState::READING_TARGET_NAME:
            handleTargetNameState(token, context);
            break;
        case ImportParseState::READING_FROM_KEYWORD:
            handleFromKeywordState(token, context);
            break;
        case ImportParseState::READING_IMPORT_PATH:
            handleImportPathState(token, context);
            break;
        case ImportParseState::READING_AS_KEYWORD:
            handleAsKeywordState(token, context);
            break;
        case ImportParseState::READING_ALIAS:
            handleAliasState(token, context);
            break;
    }
    
    return true;
}

const ConfigurationGroup& ConfigImportStateMachine::getConfigurationGroup() const {
    return configGroup_;
}

const std::vector<ImportDefinition>& ConfigImportStateMachine::getImportDefinitions() const {
    return importDefinitions_;
}

// 配置组状态处理方法实现

void ConfigImportStateMachine::handleConfigInitialState(const Token& token, ConfigImportContext& context) {
    if (token.type == TokenType::LEFT_BRACKET && token.value == "[") {
        context.configState = ConfigParseState::READING_CONFIG_HEADER;
    }
}

void ConfigImportStateMachine::handleConfigHeaderState(const Token& token, ConfigImportContext& context) {
    if (token.type == TokenType::IDENTIFIER && token.value == "Configuration") {
        context.configState = ConfigParseState::READING_CONFIG_CONTENT;
    }
}

void ConfigImportStateMachine::handleConfigContentState(const Token& token, ConfigImportContext& context) {
    if (token.type == TokenType::LEFT_BRACE) {
        context.nestingLevel++;
    } else if (token.type == TokenType::RIGHT_BRACE) {
        context.nestingLevel--;
        if (context.nestingLevel == 0) {
            context.configState = ConfigParseState::INITIAL;
        }
    } else if (token.type == TokenType::IDENTIFIER) {
        if (token.value == "Name" && !context.inNameBlock) {
            context.configState = ConfigParseState::READING_NAME_BLOCK;
            context.inNameBlock = true;
        } else {
            currentItemName_ = token.value;
            context.configState = ConfigParseState::READING_CONFIG_ITEM;
        }
    }
}

void ConfigImportStateMachine::handleConfigItemState(const Token& token, ConfigImportContext& context) {
    if (token.type == TokenType::ASSIGNMENT || (token.type == TokenType::OPERATOR && token.value == "=")) {
        // 等待值
    } else if (token.type == TokenType::SEMICOLON) {
        // 配置项结束
        addConfigurationItem(currentItemName_, currentItemValue_);
        currentItemName_.clear();
        currentItemValue_.clear();
        context.configState = ConfigParseState::READING_CONFIG_CONTENT;
    } else if (token.type == TokenType::LEFT_BRACKET) {
        // 数组开始
        context.configState = ConfigParseState::READING_ARRAY_VALUES;
    } else {
        // 收集值
        if (!currentItemValue_.empty()) {
            currentItemValue_ += " ";
        }
        currentItemValue_ += token.value;
    }
}

void ConfigImportStateMachine::handleNameBlockState(const Token& token, ConfigImportContext& context) {
    if (token.type == TokenType::LEFT_BRACE) {
        context.nestingLevel++;
        context.configState = ConfigParseState::READING_NAME_ITEM;
    } else if (token.type == TokenType::RIGHT_BRACE) {
        context.nestingLevel--;
        context.inNameBlock = false;
        context.configState = ConfigParseState::READING_CONFIG_CONTENT;
    }
}

void ConfigImportStateMachine::handleNameItemState(const Token& token, ConfigImportContext& context) {
    if (token.type == TokenType::IDENTIFIER) {
        currentItemName_ = token.value;
    } else if (token.type == TokenType::ASSIGNMENT || (token.type == TokenType::OPERATOR && token.value == "=")) {
        // 等待值
    } else if (token.type == TokenType::LEFT_BRACKET) {
        context.configState = ConfigParseState::READING_ARRAY_VALUES;
    } else if (token.type == TokenType::SEMICOLON) {
        // Name配置项结束
        if (!currentArrayValues_.empty()) {
            addNameConfigurationItem(currentItemName_, currentArrayValues_);
        } else {
            addNameConfigurationItem(currentItemName_, {currentItemValue_});
        }
        currentItemName_.clear();
        currentItemValue_.clear();
        currentArrayValues_.clear();
        context.configState = ConfigParseState::READING_NAME_ITEM;
    } else {
        // 收集值
        if (!currentItemValue_.empty()) {
            currentItemValue_ += " ";
        }
        currentItemValue_ += token.value;
    }
}

void ConfigImportStateMachine::handleArrayValuesState(const Token& token, ConfigImportContext& context) {
    if (token.type == TokenType::RIGHT_BRACKET) {
        context.configState = context.inNameBlock ? ConfigParseState::READING_NAME_ITEM : ConfigParseState::READING_CONFIG_ITEM;
    } else if (token.type == TokenType::OPERATOR && token.value == ",") {
        // 数组项分隔符
        if (!currentItemValue_.empty()) {
            currentArrayValues_.push_back(currentItemValue_);
            currentItemValue_.clear();
        }
    } else {
        // 收集数组项值
        if (!currentItemValue_.empty()) {
            currentItemValue_ += " ";
        }
        currentItemValue_ += token.value;
    }
}

// 导入状态处理方法实现

void ConfigImportStateMachine::handleImportInitialState(const Token& token, ConfigImportContext& context) {
    if (token.type == TokenType::LEFT_BRACKET && token.value == "[") {
        context.importState = ImportParseState::READING_IMPORT_HEADER;
    }
}

void ConfigImportStateMachine::handleImportHeaderState(const Token& token, ConfigImportContext& context) {
    if (token.type == TokenType::IDENTIFIER && token.value == "Import") {
        context.importState = ImportParseState::READING_IMPORT_TYPE;
    }
}

void ConfigImportStateMachine::handleImportTypeState(const Token& token, ConfigImportContext& context) {
    if (token.type == TokenType::OPERATOR && token.value == "@") {
        // 等待类型标识符
    } else if (token.type == TokenType::IDENTIFIER) {
        currentImport_.type = parseImportType(token.value);
        context.currentImportType = currentImport_.type;
        context.importState = ImportParseState::READING_TARGET_NAME;
    }
}

void ConfigImportStateMachine::handleTargetNameState(const Token& token, ConfigImportContext& context) {
    if (token.type == TokenType::IDENTIFIER && token.value == "from") {
        context.importState = ImportParseState::READING_IMPORT_PATH;
    } else if (token.type == TokenType::IDENTIFIER) {
        currentImport_.targetName = token.value;
    }
}

void ConfigImportStateMachine::handleFromKeywordState(const Token& token, ConfigImportContext& context) {
    context.importState = ImportParseState::READING_IMPORT_PATH;
}

void ConfigImportStateMachine::handleImportPathState(const Token& token, ConfigImportContext& context) {
    if (token.type == TokenType::IDENTIFIER && token.value == "as") {
        context.importState = ImportParseState::READING_ALIAS;
    } else if (token.type == TokenType::SEMICOLON) {
        // 导入结束
        addImportDefinition(currentImport_);
        currentImport_ = ImportDefinition{};
        context.importState = ImportParseState::INITIAL;
    } else {
        // 收集路径
        currentImport_.path = token.value;
    }
}

void ConfigImportStateMachine::handleAsKeywordState(const Token& token, ConfigImportContext& context) {
    context.importState = ImportParseState::READING_ALIAS;
}

void ConfigImportStateMachine::handleAliasState(const Token& token, ConfigImportContext& context) {
    if (token.type == TokenType::SEMICOLON) {
        // 导入结束
        addImportDefinition(currentImport_);
        currentImport_ = ImportDefinition{};
        context.importState = ImportParseState::INITIAL;
    } else if (token.type == TokenType::IDENTIFIER) {
        currentImport_.alias = token.value;
    }
}

// 工具方法实现

void ConfigImportStateMachine::addConfigurationItem(const std::string& name, const std::string& value) {
    ConfigurationItem item(name, ConfigurationType::STRING, value);
    
    // 确定类型
    if (value == "true" || value == "false") {
        item.type = ConfigurationType::BOOLEAN;
    } else if (std::regex_match(value, std::regex(R"(^\d+$)"))) {
        item.type = ConfigurationType::INTEGER;
    }
    
    configGroup_.items[name] = item;
}

void ConfigImportStateMachine::addNameConfigurationItem(const std::string& name, const std::vector<std::string>& values) {
    configGroup_.hasNameConfig = true;
    configGroup_.nameConfig.keywordMappings[name] = values;
}

void ConfigImportStateMachine::addImportDefinition(const ImportDefinition& importDef) {
    importDefinitions_.push_back(importDef);
}

ImportType ConfigImportStateMachine::parseImportType(const std::string& typeStr, bool hasCustomPrefix, bool hasTemplatePrefix) {
    if (typeStr == "Html") {
        return ImportType::HTML_IMPORT;
    } else if (typeStr == "Style") {
        return ImportType::STYLE_IMPORT;
    } else if (typeStr == "JavaScript") {
        return ImportType::JAVASCRIPT_IMPORT;
    } else if (typeStr == "Chtl") {
        return ImportType::CHTL_IMPORT;
    } else if (typeStr == "CJmod") {
        return ImportType::CJMOD_IMPORT;
    } else if (typeStr == "Element") {
        if (hasTemplatePrefix) return ImportType::TEMPLATE_ELEMENT;
        if (hasCustomPrefix) return ImportType::CUSTOM_ELEMENT;
        return ImportType::CUSTOM_ELEMENT;
    } else if (typeStr == "Var") {
        if (hasTemplatePrefix) return ImportType::TEMPLATE_VAR;
        if (hasCustomPrefix) return ImportType::CUSTOM_VAR;
        return ImportType::CUSTOM_VAR;
    }
    
    return ImportType::HTML_IMPORT;
}

// ===== CHTLConfigImportParser 实现 =====

CHTLConfigImportParser::CHTLConfigImportParser() 
    : configRegistry_(std::make_shared<ConfigurationRegistry>())
    , importRegistry_(std::make_shared<ImportRegistry>())
    , strictMode_(true) {
}

CHTLConfigImportParser::~CHTLConfigImportParser() = default;

ParseResult CHTLConfigImportParser::parseConfigurationDeclaration(const std::string& source, const std::string& filename) {
    errors_.clear();
    warnings_.clear();
    
    ParseResult result;
    result.success = false;
    
    // 词法分析
    auto lexResult = lexer_.tokenize(source, filename);
    if (!lexResult.success) {
        errors_.insert(errors_.end(), lexResult.errors.begin(), lexResult.errors.end());
        result.errors = errors_;
        return result;
    }
    
    try {
        // 解析配置组
        ConfigurationGroup configGroup = parseConfigurationGroup(lexResult.tokens);
        
        // 注册配置组
        if (!configRegistry_->registerConfiguration(configGroup)) {
            reportError("配置组注册失败");
        }
        
        // 创建配置组节点
        auto configNode = createConfigurationNode(configGroup);
        
        result.success = errors_.empty();
        result.rootNode = configNode;
        result.errors = errors_;
        result.warnings = warnings_;
        
    } catch (const std::exception& e) {
        reportError("配置组解析失败: " + std::string(e.what()));
        result.success = false;
        result.errors = errors_;
    }
    
    return result;
}

ParseResult CHTLConfigImportParser::parseImportDeclaration(const std::string& source, const std::string& filename) {
    errors_.clear();
    warnings_.clear();
    
    ParseResult result;
    result.success = false;
    
    // 词法分析
    auto lexResult = lexer_.tokenize(source, filename);
    if (!lexResult.success) {
        errors_.insert(errors_.end(), lexResult.errors.begin(), lexResult.errors.end());
        result.errors = errors_;
        return result;
    }
    
    try {
        // 解析导入定义
        ImportDefinition importDef = parseImportDefinition(lexResult.tokens);
        
        // 注册导入
        if (!importRegistry_->registerImport(importDef)) {
            reportError("导入注册失败: " + importDef.path);
        }
        
        // 创建导入节点
        auto importNode = createImportNode(importDef);
        
        result.success = errors_.empty();
        result.rootNode = importNode;
        result.errors = errors_;
        result.warnings = warnings_;
        
    } catch (const std::exception& e) {
        reportError("导入解析失败: " + std::string(e.what()));
        result.success = false;
        result.errors = errors_;
    }
    
    return result;
}

void CHTLConfigImportParser::setConfigurationRegistry(std::shared_ptr<ConfigurationRegistry> registry) {
    configRegistry_ = registry;
}

std::shared_ptr<ConfigurationRegistry> CHTLConfigImportParser::getConfigurationRegistry() const {
    return configRegistry_;
}

void CHTLConfigImportParser::setImportRegistry(std::shared_ptr<ImportRegistry> registry) {
    importRegistry_ = registry;
}

std::shared_ptr<ImportRegistry> CHTLConfigImportParser::getImportRegistry() const {
    return importRegistry_;
}

void CHTLConfigImportParser::setStrictMode(bool strict) {
    strictMode_ = strict;
}

const std::vector<ParseError>& CHTLConfigImportParser::getErrors() const {
    return errors_;
}

const std::vector<ParseWarning>& CHTLConfigImportParser::getWarnings() const {
    return warnings_;
}

// 核心解析方法实现

ConfigurationGroup CHTLConfigImportParser::parseConfigurationGroup(const std::vector<Token>& tokens) {
    ConfigurationGroup config;
    size_t index = 0;
    
    // 解析 [Configuration]
    if (index >= tokens.size() || tokens[index].type != TokenType::LEFT_BRACKET) {
        throw std::runtime_error("期望 '[Configuration]' 开始");
    }
    index++; // 跳过 '['
    
    if (index >= tokens.size() || tokens[index].value != "Configuration") {
        throw std::runtime_error("期望 'Configuration' 关键字");
    }
    index++; // 跳过 'Configuration'
    
    if (index >= tokens.size() || tokens[index].type != TokenType::RIGHT_BRACKET) {
        throw std::runtime_error("期望 ']' 结束");
    }
    index++; // 跳过 ']'
    
    // 跳过空白
    while (index < tokens.size() && 
           (tokens[index].type == TokenType::WHITESPACE || tokens[index].type == TokenType::NEWLINE)) {
        index++;
    }
    
    // 解析配置组内容 { ... }
    if (index >= tokens.size() || tokens[index].type != TokenType::LEFT_BRACE) {
        throw std::runtime_error("期望 '{' 开始配置组内容");
    }
    index++; // 跳过 '{'
    
    int braceCount = 1;
    while (index < tokens.size() && braceCount > 0) {
        const auto& token = tokens[index];
        
        if (token.type == TokenType::LEFT_BRACE) {
            braceCount++;
        } else if (token.type == TokenType::RIGHT_BRACE) {
            braceCount--;
            if (braceCount == 0) {
                break; // 配置组结束
            }
        }
        
        // 解析配置项或Name块
        if (token.type == TokenType::IDENTIFIER) {
            if (token.value == "Name" && braceCount == 1) {
                // 解析Name配置块
                config.nameConfig = parseNameConfiguration(tokens, index);
                config.hasNameConfig = true;
                continue;
            } else {
                // 解析普通配置项
                ConfigurationItem item = parseConfigurationItem(tokens, index);
                if (!item.name.empty()) {
                    config.items[item.name] = item;
                }
                continue;
            }
        }
        
        index++;
    }
    
    if (braceCount > 0) {
        throw std::runtime_error("未闭合的配置组");
    }
    
    return config;
}

ImportDefinition CHTLConfigImportParser::parseImportDefinition(const std::vector<Token>& tokens) {
    ImportDefinition importDef;
    size_t index = 0;
    
    // 解析 [Import]
    if (index >= tokens.size() || tokens[index].type != TokenType::LEFT_BRACKET) {
        throw std::runtime_error("期望 '[Import]' 开始");
    }
    index++; // 跳过 '['
    
    if (index >= tokens.size() || tokens[index].value != "Import") {
        throw std::runtime_error("期望 'Import' 关键字");
    }
    index++; // 跳过 'Import'
    
    if (index >= tokens.size() || tokens[index].type != TokenType::RIGHT_BRACKET) {
        throw std::runtime_error("期望 ']' 结束");
    }
    index++; // 跳过 ']'
    
    // 跳过空白
    while (index < tokens.size() && 
           (tokens[index].type == TokenType::WHITESPACE || tokens[index].type == TokenType::NEWLINE)) {
        index++;
    }
    
    // 检查是否有Custom或Template前缀
    bool hasCustomPrefix = false;
    bool hasTemplatePrefix = false;
    
    if (index < tokens.size() && tokens[index].type == TokenType::LEFT_BRACKET) {
        index++; // 跳过 '['
        if (index < tokens.size() && tokens[index].type == TokenType::IDENTIFIER) {
            if (tokens[index].value == "Custom") {
                hasCustomPrefix = true;
            } else if (tokens[index].value == "Template") {
                hasTemplatePrefix = true;
            }
            index++; // 跳过前缀
        }
        if (index < tokens.size() && tokens[index].type == TokenType::RIGHT_BRACKET) {
            index++; // 跳过 ']'
        }
    }
    
    // 解析导入类型
    ImportType baseType = parseImportType(tokens, index);
    
    // 根据前缀调整类型
    if (hasCustomPrefix) {
        switch (baseType) {
            case ImportType::STYLE_IMPORT: importDef.type = ImportType::CUSTOM_STYLE; break;
            case ImportType::CUSTOM_ELEMENT: importDef.type = ImportType::CUSTOM_ELEMENT; break;
            case ImportType::CUSTOM_VAR: importDef.type = ImportType::CUSTOM_VAR; break;
            default: importDef.type = baseType; break;
        }
    } else if (hasTemplatePrefix) {
        switch (baseType) {
            case ImportType::STYLE_IMPORT: importDef.type = ImportType::TEMPLATE_STYLE; break;
            case ImportType::CUSTOM_ELEMENT: importDef.type = ImportType::TEMPLATE_ELEMENT; break;
            case ImportType::CUSTOM_VAR: importDef.type = ImportType::TEMPLATE_VAR; break;
            default: importDef.type = baseType; break;
        }
    } else {
        importDef.type = baseType;
    }
    
    // 解析目标名称 (可选，用于Custom/Template导入)
    if (hasCustomPrefix || hasTemplatePrefix) {
        // 跳过空白
        while (index < tokens.size() && 
               (tokens[index].type == TokenType::WHITESPACE || tokens[index].type == TokenType::NEWLINE)) {
            index++;
        }
        
        if (index < tokens.size() && tokens[index].type == TokenType::IDENTIFIER && tokens[index].value != "from") {
            importDef.targetName = tokens[index].value;
            index++;
        }
    }
    
    // 解析from关键字
    while (index < tokens.size() && 
           (tokens[index].type == TokenType::WHITESPACE || tokens[index].type == TokenType::NEWLINE)) {
        index++;
    }
    
    if (index >= tokens.size() || tokens[index].value != "from") {
        throw std::runtime_error("期望 'from' 关键字");
    }
    index++; // 跳过 'from'
    
    // 解析导入路径
    importDef.path = parseImportPath(tokens, index);
    
    // 解析as别名 (可选)
    while (index < tokens.size() && 
           (tokens[index].type == TokenType::WHITESPACE || tokens[index].type == TokenType::NEWLINE)) {
        index++;
    }
    
    if (index < tokens.size() && tokens[index].value == "as") {
        index++; // 跳过 'as'
        importDef.alias = parseAlias(tokens, index);
    }
    
    // 确定路径类型
    importDef.pathType = ConfigImportUtils::determinePathType(importDef.path);
    
    return importDef;
}

NameConfiguration CHTLConfigImportParser::parseNameConfiguration(const std::vector<Token>& tokens, size_t& index) {
    NameConfiguration nameConfig;
    
    // 跳过'Name'
    index++;
    
    // 跳过空白和 ']'
    while (index < tokens.size() && 
           (tokens[index].type == TokenType::WHITESPACE || tokens[index].type == TokenType::NEWLINE || tokens[index].type == TokenType::RIGHT_BRACKET)) {
        index++;
    }
    
    // 期望 '{'
    if (index >= tokens.size() || tokens[index].type != TokenType::LEFT_BRACE) {
        throw std::runtime_error("期望 '{' 开始Name配置块");
    }
    index++; // 跳过 '{'
    
    int braceCount = 1;
    while (index < tokens.size() && braceCount > 0) {
        const auto& token = tokens[index];
        
        if (token.type == TokenType::LEFT_BRACE) {
            braceCount++;
        } else if (token.type == TokenType::RIGHT_BRACE) {
            braceCount--;
            if (braceCount == 0) {
                break; // Name块结束
            }
        }
        
        // 解析Name配置项
        if (token.type == TokenType::IDENTIFIER) {
            ConfigurationItem item = parseConfigurationItem(tokens, index);
            if (!item.name.empty()) {
                if (item.type == ConfigurationType::ARRAY) {
                    nameConfig.keywordMappings[item.name] = item.arrayValues;
                } else {
                    nameConfig.keywordMappings[item.name] = {item.value};
                }
            }
            continue;
        }
        
        index++;
    }
    
    if (braceCount > 0) {
        throw std::runtime_error("未闭合的Name配置块");
    }
    
    index++; // 跳过最后的 '}'
    
    return nameConfig;
}

// 其他解析方法的实现...

ConfigurationItem CHTLConfigImportParser::parseConfigurationItem(const std::vector<Token>& tokens, size_t& index) {
    ConfigurationItem item;
    
    if (index >= tokens.size() || tokens[index].type != TokenType::IDENTIFIER) {
        throw std::runtime_error("期望配置项名称");
    }
    
    item.name = tokens[index].value;
    item.position = tokens[index].position;
    index++; // 跳过名称
    
    // 跳过空白
    while (index < tokens.size() && 
           (tokens[index].type == TokenType::WHITESPACE || tokens[index].type == TokenType::NEWLINE)) {
        index++;
    }
    
    // 期望 '=' 或 ASSIGNMENT
    if (index >= tokens.size() || 
        (tokens[index].type != TokenType::ASSIGNMENT && !(tokens[index].type == TokenType::OPERATOR && tokens[index].value == "="))) {
        throw std::runtime_error("期望 '=' 在配置项中");
    }
    index++; // 跳过 '='
    
    // 跳过空白
    while (index < tokens.size() && 
           (tokens[index].type == TokenType::WHITESPACE || tokens[index].type == TokenType::NEWLINE)) {
        index++;
    }
    
    // 解析值
    if (index < tokens.size() && tokens[index].type == TokenType::LEFT_BRACKET) {
        // 数组值
        item.arrayValues = parseArrayValue(tokens, index);
        item.type = ConfigurationType::ARRAY;
    } else {
        // 单个值
        std::string value;
        while (index < tokens.size() && tokens[index].type != TokenType::SEMICOLON) {
            if (tokens[index].type != TokenType::WHITESPACE && tokens[index].type != TokenType::NEWLINE) {
                // 不在@符号后添加空格
                bool isAfterAtSymbol = !value.empty() && value.back() == '@';
                bool needSpace = !value.empty() && !isAfterAtSymbol;
                if (needSpace) value += " ";
                value += tokens[index].value;
            }
            index++;
        }
        
        item.value = value;
        item.type = determineValueType(value);
    }
    
    // 期望分号
    if (index < tokens.size() && tokens[index].type == TokenType::SEMICOLON) {
        index++; // 跳过 ';'
    }
    
    return item;
}

std::vector<std::string> CHTLConfigImportParser::parseArrayValue(const std::vector<Token>& tokens, size_t& index) {
    std::vector<std::string> values;
    
    if (index >= tokens.size() || tokens[index].type != TokenType::LEFT_BRACKET) {
        return values;
    }
    index++; // 跳过 '['
    
    std::string currentValue;
    while (index < tokens.size() && tokens[index].type != TokenType::RIGHT_BRACKET) {
        if (tokens[index].type == TokenType::OPERATOR && tokens[index].value == ",") {
            // 数组项分隔符
            if (!currentValue.empty()) {
                // 去除尾随空格并添加值
                size_t end = currentValue.find_last_not_of(" \t\r\n");
                if (end != std::string::npos) {
                    values.push_back(currentValue.substr(0, end + 1));
                }
                currentValue.clear();
            }
        } else if (tokens[index].type != TokenType::WHITESPACE && tokens[index].type != TokenType::NEWLINE) {
            currentValue += tokens[index].value;
        }
        index++;
    }
    
    // 添加最后一个值
    if (!currentValue.empty()) {
        // 去除尾随空格并添加值
        size_t end = currentValue.find_last_not_of(" \t\r\n");
        if (end != std::string::npos) {
            values.push_back(currentValue.substr(0, end + 1));
        }
    }
    
    if (index < tokens.size() && tokens[index].type == TokenType::RIGHT_BRACKET) {
        index++; // 跳过 ']'
    }
    
    return values;
}

ConfigurationType CHTLConfigImportParser::determineValueType(const std::string& value) {
    if (value == "true" || value == "false") {
        return ConfigurationType::BOOLEAN;
    }
    
    if (std::regex_match(value, std::regex(R"(^\d+$)"))) {
        return ConfigurationType::INTEGER;
    }
    
    return ConfigurationType::STRING;
}

ImportType CHTLConfigImportParser::parseImportType(const std::vector<Token>& tokens, size_t& index) {
    // 跳过空白
    while (index < tokens.size() && 
           (tokens[index].type == TokenType::WHITESPACE || tokens[index].type == TokenType::NEWLINE)) {
        index++;
    }
    
    // 期望 '@'
    if (index >= tokens.size() || tokens[index].type != TokenType::OPERATOR || tokens[index].value != "@") {
        throw std::runtime_error("期望导入类型标识符 '@'");
    }
    index++; // 跳过 '@'
    
    if (index >= tokens.size() || tokens[index].type != TokenType::IDENTIFIER) {
        throw std::runtime_error("期望导入类型名称");
    }
    
    std::string typeStr = tokens[index].value;
    index++; // 跳过类型名称
    
    if (typeStr == "Html") {
        return ImportType::HTML_IMPORT;
    } else if (typeStr == "Style") {
        return ImportType::STYLE_IMPORT;
    } else if (typeStr == "JavaScript") {
        return ImportType::JAVASCRIPT_IMPORT;
    } else if (typeStr == "Chtl") {
        return ImportType::CHTL_IMPORT;
    } else if (typeStr == "CJmod") {
        return ImportType::CJMOD_IMPORT;
    } else if (typeStr == "Element") {
        return ImportType::CUSTOM_ELEMENT;
    } else if (typeStr == "Var") {
        return ImportType::CUSTOM_VAR;
    } else {
        throw std::runtime_error("未知的导入类型: " + typeStr);
    }
}

std::string CHTLConfigImportParser::parseImportPath(const std::vector<Token>& tokens, size_t& index) {
    // 跳过空白
    while (index < tokens.size() && 
           (tokens[index].type == TokenType::WHITESPACE || tokens[index].type == TokenType::NEWLINE)) {
        index++;
    }
    
    if (index >= tokens.size()) {
        throw std::runtime_error("期望导入路径");
    }
    
    // 收集路径的所有部分，直到遇到'as'或';'
    std::string path;
    while (index < tokens.size()) {
        // 检查结束条件
        if (tokens[index].type == TokenType::SEMICOLON) {
            break;
        }
        if ((tokens[index].type == TokenType::IDENTIFIER || tokens[index].type == TokenType::KEYWORD) && tokens[index].value == "as") {
            break;
        }
        
        if (tokens[index].type != TokenType::WHITESPACE && tokens[index].type != TokenType::NEWLINE) {
            path += tokens[index].value;
        }
        index++;
    }
    
    if (!isValidImportPath(path)) {
        reportWarning("导入路径格式可能不正确: " + path);
    }
    
    return path;
}

std::string CHTLConfigImportParser::parseAlias(const std::vector<Token>& tokens, size_t& index) {
    // 跳过空白
    while (index < tokens.size() && 
           (tokens[index].type == TokenType::WHITESPACE || tokens[index].type == TokenType::NEWLINE)) {
        index++;
    }
    
    if (index >= tokens.size() || tokens[index].type != TokenType::IDENTIFIER) {
        throw std::runtime_error("期望别名");
    }
    
    std::string alias = tokens[index].value;
    index++;
    
    return alias;
}

CHTLASTNodePtr CHTLConfigImportParser::createConfigurationNode(const ConfigurationGroup& config) {
    auto configNode = std::make_shared<ConfigurationDeclarationNode>(config.position);
    
    // 添加配置项作为属性
    for (const auto& item : config.items) {
        std::string valueStr = item.second.value;
        if (item.second.type == ConfigurationType::ARRAY) {
            valueStr = "[";
            for (size_t i = 0; i < item.second.arrayValues.size(); ++i) {
                if (i > 0) valueStr += ", ";
                valueStr += item.second.arrayValues[i];
            }
            valueStr += "]";
        }
        
        auto propNode = std::make_shared<AttributeNode>(item.first, valueStr, LiteralType::UNQUOTED, item.second.position);
        configNode->properties.push_back(propNode);
    }
    
    return configNode;
}

CHTLASTNodePtr CHTLConfigImportParser::createImportNode(const ImportDefinition& importDef) {
    std::string typeStr = getImportTypeString(importDef.type);
    auto importNode = std::make_shared<ImportDeclarationNode>(typeStr, importDef.targetName, importDef.path, importDef.position);
    
    if (!importDef.alias.empty()) {
        importNode->setAlias(importDef.alias);
    }
    
    return importNode;
}

void CHTLConfigImportParser::reportError(const std::string& message, const Position& position) {
    errors_.emplace_back(message, position);
}

void CHTLConfigImportParser::reportWarning(const std::string& message, const Position& position) {
    warnings_.emplace_back(message, position);
}

bool CHTLConfigImportParser::isValidConfigurationName(const std::string& name) const {
    if (name.empty()) return false;
    
    std::regex nameRegex(R"(^[A-Z_][A-Z0-9_]*$)");
    return std::regex_match(name, nameRegex);
}

bool CHTLConfigImportParser::isValidImportPath(const std::string& path) const {
    if (path.empty()) return false;
    
    std::regex pathRegex(R"(^[a-zA-Z0-9._/\-*]+$)");
    return std::regex_match(path, pathRegex);
}

std::string CHTLConfigImportParser::getImportTypeString(ImportType type) const {
    switch (type) {
        case ImportType::HTML_IMPORT: return "@Html";
        case ImportType::STYLE_IMPORT: return "@Style";
        case ImportType::JAVASCRIPT_IMPORT: return "@JavaScript";
        case ImportType::CHTL_IMPORT: return "@Chtl";
        case ImportType::CJMOD_IMPORT: return "@CJmod";
        case ImportType::CUSTOM_ELEMENT: return "[Custom] @Element";
        case ImportType::CUSTOM_STYLE: return "[Custom] @Style";
        case ImportType::CUSTOM_VAR: return "[Custom] @Var";
        case ImportType::TEMPLATE_ELEMENT: return "[Template] @Element";
        case ImportType::TEMPLATE_STYLE: return "[Template] @Style";
        case ImportType::TEMPLATE_VAR: return "[Template] @Var";
    }
    return "@Html";
}

std::string CHTLConfigImportParser::getConfigurationTypeString(ConfigurationType type) const {
    switch (type) {
        case ConfigurationType::BOOLEAN: return "boolean";
        case ConfigurationType::INTEGER: return "integer";
        case ConfigurationType::STRING: return "string";
        case ConfigurationType::ARRAY: return "array";
        case ConfigurationType::KEYWORD_MAPPING: return "keyword_mapping";
    }
    return "string";
}

// ===== ConfigImportUtils 实现 =====

bool ConfigImportUtils::isConfigurationDeclaration(const std::string& source) {
    std::regex configRegex(R"(\[Configuration\]\s*\{)", std::regex_constants::icase);
    return std::regex_search(source, configRegex);
}

bool ConfigImportUtils::isImportDeclaration(const std::string& source) {
    std::regex importRegex(R"(\[Import\].*@\w+.*from)", std::regex_constants::icase);
    return std::regex_search(source, importRegex);
}

std::string ConfigImportUtils::extractConfigurationName(const std::string& source) {
    std::regex nameRegex(R"(([A-Z_][A-Z0-9_]*)\s*=)");
    std::smatch match;
    if (std::regex_search(source, match, nameRegex)) {
        return match[1].str();
    }
    return "";
}

std::string ConfigImportUtils::extractImportType(const std::string& source) {
    std::regex typeRegex(R"(@(\w+))");
    std::smatch match;
    if (std::regex_search(source, match, typeRegex)) {
        return match[1].str();
    }
    return "";
}

std::string ConfigImportUtils::extractImportPath(const std::string& source) {
    std::regex pathRegex(R"(from\s+([^\s;]+))");
    std::smatch match;
    if (std::regex_search(source, match, pathRegex)) {
        return match[1].str();
    }
    return "";
}

bool ConfigImportUtils::validateConfigurationSyntax(const std::string& source) {
    return isConfigurationDeclaration(source);
}

bool ConfigImportUtils::validateImportSyntax(const std::string& source) {
    return isImportDeclaration(source);
}

ImportPathType ConfigImportUtils::determinePathType(const std::string& path) {
    if (!path.empty() && path.front() == '/') {
        return ImportPathType::ABSOLUTE_PATH;
    }
    
    if (path.find("*") != std::string::npos) {
        return ImportPathType::WILDCARD_PATH;
    }
    
    // 检查是否以文件扩展名结尾
    bool hasFileExtension = (path.length() >= 5 && path.substr(path.length()-5) == ".chtl") ||
                           (path.length() >= 5 && path.substr(path.length()-5) == ".html") ||
                           (path.length() >= 4 && path.substr(path.length()-4) == ".css") ||
                           (path.length() >= 3 && path.substr(path.length()-3) == ".js");
    
    if (path.find(".") != std::string::npos && !hasFileExtension) {
        return ImportPathType::MODULE_PATH;
    }
    
    return ImportPathType::RELATIVE_PATH;
}

std::string ConfigImportUtils::normalizePath(const std::string& path) {
    std::string normalized = path;
    
    // 将'.'替换为'/'
    normalized = std::regex_replace(normalized, std::regex("\\."), "/");
    
    // 移除双斜杠
    normalized = std::regex_replace(normalized, std::regex("//+"), "/");
    
    return normalized;
}

bool ConfigImportUtils::isValidPath(const std::string& path) {
    if (path.empty()) return false;
    
    std::regex pathRegex(R"(^[a-zA-Z0-9._/\-*]+$)");
    return std::regex_match(path, pathRegex);
}

std::vector<std::string> ConfigImportUtils::parseArrayValue(const std::string& arrayStr) {
    std::vector<std::string> values;
    
    std::string content = arrayStr;
    if (!content.empty() && content.front() == '[' && content.back() == ']') {
        content = content.substr(1, content.length() - 2);
    }
    
    std::istringstream stream(content);
    std::string value;
    
    while (std::getline(stream, value, ',')) {
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);
        if (!value.empty()) {
            values.push_back(value);
        }
    }
    
    return values;
}

std::string ConfigImportUtils::formatConfigurationValue(const std::string& value, ConfigurationType type) {
    switch (type) {
        case ConfigurationType::BOOLEAN:
            return (value == "true" || value == "1") ? "true" : "false";
        case ConfigurationType::INTEGER:
            return value;
        case ConfigurationType::STRING:
            return value;
        case ConfigurationType::ARRAY:
            return value;
        case ConfigurationType::KEYWORD_MAPPING:
            return value;
    }
    return value;
}

bool ConfigImportUtils::isValidConfigurationName(const std::string& name) {
    if (name.empty()) return false;
    
    std::regex nameRegex(R"(^[A-Z_][A-Z0-9_]*$)");
    return std::regex_match(name, nameRegex);
}

bool ConfigImportUtils::isValidAlias(const std::string& alias) {
    if (alias.empty()) return false;
    
    std::regex aliasRegex(R"(^[a-zA-Z_][a-zA-Z0-9_]*$)");
    return std::regex_match(alias, aliasRegex);
}

} // namespace chtl