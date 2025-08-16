#include "core/chtl_configuration.hpp"
#include "constraints/raw_embedding_parser.hpp"
#include <regex>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace chtl {

CHtlConfiguration CHtlConfigurationManager::instance_;

CHtlConfiguration::CHtlConfiguration() {
    resetToDefaults();
}

bool CHtlConfiguration::loadFromConfigBlock(const std::string& configBlock) {
    try {
        // 使用状态机解析器提取配置块内容
        std::regex configPattern(R"(\[Configuration\]\s*\{([^}]*)\})");
        std::smatch match;
        
        if (!std::regex_search(configBlock, match, configPattern)) {
            return false;
        }
        
        std::string content = match[1].str();
        
        // 解析基本配置项和子块
        std::istringstream stream(content);
        std::string line;
        std::string currentBlock;
        std::string blockType;
        bool inBlock = false;
        int braceLevel = 0;
        
        while (std::getline(stream, line)) {
            // 清理行（移除注释）
            size_t commentPos = line.find("//");
            if (commentPos != std::string::npos) {
                line = line.substr(0, commentPos);
            }
            
            // 检查是否进入子块
            if (line.find("[Name]") != std::string::npos) {
                blockType = "Name";
                inBlock = true;
                currentBlock.clear();
                continue;
            } else if (line.find("[OriginType]") != std::string::npos) {
                blockType = "OriginType";
                inBlock = true;
                currentBlock.clear();
                continue;
            }
            
            if (inBlock) {
                // 跟踪大括号层级
                for (char c : line) {
                    if (c == '{') braceLevel++;
                    else if (c == '}') braceLevel--;
                }
                
                currentBlock += line + "\n";
                
                // 如果大括号平衡，结束当前块
                if (braceLevel <= 0) {
                    if (blockType == "Name") {
                        parseNameBlock(currentBlock);
                    } else if (blockType == "OriginType") {
                        parseOriginTypeBlock(currentBlock);
                    }
                    inBlock = false;
                    blockType.clear();
                    currentBlock.clear();
                }
            } else {
                // 解析基本配置项
                parseConfigLine(line);
            }
        }
        
        return validate();
    } catch (const std::exception& e) {
        return false;
    }
}

bool CHtlConfiguration::parseConfigLine(const std::string& line) {
    // 移除空白字符
    std::string trimmed = line;
    trimmed.erase(0, trimmed.find_first_not_of(" \t"));
    trimmed.erase(trimmed.find_last_not_of(" \t") + 1);
    
    if (trimmed.empty() || trimmed[0] == '/' || trimmed[0] == '[') {
        return true; // 跳过空行、注释和块标记
    }
    
    // 解析 KEY = VALUE 格式
    size_t equalPos = trimmed.find('=');
    if (equalPos == std::string::npos) {
        return false;
    }
    
    std::string key = trimmed.substr(0, equalPos);
    std::string value = trimmed.substr(equalPos + 1);
    
    // 清理键和值
    key = cleanString(key);
    value = cleanString(value);
    
    // 解析配置项
    if (key == "INDEX_INITIAL_COUNT") {
        indexInitialCount_ = std::stoi(value);
    } else if (key == "DISABLE_NAME_GROUP") {
        disableNameGroup_ = (value == "true");
    } else if (key == "DISABLE_CUSTOM_ORIGIN_TYPE") {
        disableCustomOriginType_ = (value == "true");
    } else if (key == "DEBUG_MODE") {
        debugMode_ = (value == "true");
    } else if (key == "OPTION_COUNT") {
        optionCount_ = std::stoi(value);
    }
    
    return true;
}

bool CHtlConfiguration::parseNameBlock(const std::string& nameBlockContent) {
    std::istringstream stream(nameBlockContent);
    std::string line;
    
    while (std::getline(stream, line)) {
        // 移除注释
        size_t commentPos = line.find("//");
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }
        
        // 清理
        line = cleanString(line);
        if (line.empty() || line.find('=') == std::string::npos) {
            continue;
        }
        
        size_t equalPos = line.find('=');
        std::string key = cleanString(line.substr(0, equalPos));
        std::string value = cleanString(line.substr(equalPos + 1));
        
        if (value.front() == '[' && value.back() == ']') {
            // 数组值
            keywordNames_[key] = parseArrayValue(value);
        } else {
            // 单个值
            keywordNames_[key] = {value};
        }
    }
    
    return true;
}

bool CHtlConfiguration::parseOriginTypeBlock(const std::string& originTypeBlockContent) {
    std::istringstream stream(originTypeBlockContent);
    std::string line;
    
    while (std::getline(stream, line)) {
        // 移除注释
        size_t commentPos = line.find("//");
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }
        
        // 清理
        line = cleanString(line);
        if (line.empty() || line.find('=') == std::string::npos) {
            continue;
        }
        
        size_t equalPos = line.find('=');
        std::string key = cleanString(line.substr(0, equalPos));
        std::string value = cleanString(line.substr(equalPos + 1));
        
        // 验证ORIGINTYPE_前缀
        if (key.substr(0, 11) == "ORIGINTYPE_") {
            // 提取类型名
            std::string typeName = key.substr(11);
            
            // 验证值格式 @TypeName
            if (value.front() == '@' && isValidTypeName(value.substr(1))) {
                customOriginTypes_.insert(value.substr(1));
            }
        }
    }
    
    return true;
}

std::vector<std::string> CHtlConfiguration::parseArrayValue(const std::string& value) {
    std::vector<std::string> result;
    
    // 移除方括号
    std::string content = value.substr(1, value.length() - 2);
    
    // 按逗号分割
    std::istringstream stream(content);
    std::string item;
    
    while (std::getline(stream, item, ',')) {
        item = cleanString(item);
        if (!item.empty()) {
            result.push_back(item);
        }
    }
    
    return result;
}

std::string CHtlConfiguration::cleanString(const std::string& str) {
    std::string result = str;
    
    // 移除前后空白
    result.erase(0, result.find_first_not_of(" \t\n\r"));
    result.erase(result.find_last_not_of(" \t\n\r") + 1);
    
    // 移除分号
    if (!result.empty() && result.back() == ';') {
        result.pop_back();
    }
    
    return result;
}

bool CHtlConfiguration::isValidTypeName(const std::string& typeName) {
    if (typeName.empty() || !std::isalpha(typeName[0])) {
        return false;
    }
    
    for (char c : typeName) {
        if (!std::isalnum(c) && c != '_') {
            return false;
        }
    }
    
    return true;
}

void CHtlConfiguration::setDefaultKeywords() {
    keywordNames_["CUSTOM_STYLE"] = {"@Style", "@style", "@CSS", "@Css", "@css"};
    keywordNames_["CUSTOM_ELEMENT"] = {"@Element"};
    keywordNames_["CUSTOM_VAR"] = {"@Var"};
    keywordNames_["TEMPLATE_STYLE"] = {"@Style"};
    keywordNames_["TEMPLATE_ELEMENT"] = {"@Element"};
    keywordNames_["TEMPLATE_VAR"] = {"@Var"};
    keywordNames_["ORIGIN_HTML"] = {"@Html"};
    keywordNames_["ORIGIN_STYLE"] = {"@Style"};
    keywordNames_["ORIGIN_JAVASCRIPT"] = {"@JavaScript"};
    keywordNames_["IMPORT_HTML"] = {"@Html"};
    keywordNames_["IMPORT_STYLE"] = {"@Style"};
    keywordNames_["IMPORT_JAVASCRIPT"] = {"@JavaScript"};
    keywordNames_["IMPORT_CHTL"] = {"@Chtl"};
    keywordNames_["IMPORT_CRMOD"] = {"@CJmod"};
    keywordNames_["KEYWORD_INHERIT"] = {"inherit"};
    keywordNames_["KEYWORD_DELETE"] = {"delete"};
    keywordNames_["KEYWORD_INSERT"] = {"insert"};
    keywordNames_["KEYWORD_AFTER"] = {"after"};
    keywordNames_["KEYWORD_BEFORE"] = {"before"};
    keywordNames_["KEYWORD_REPLACE"] = {"replace"};
    keywordNames_["KEYWORD_ATTOP"] = {"at top"};
    keywordNames_["KEYWORD_ATBOTTOM"] = {"at bottom"};
    keywordNames_["KEYWORD_FROM"] = {"from"};
    keywordNames_["KEYWORD_AS"] = {"as"};
    keywordNames_["KEYWORD_EXCEPT"] = {"except"};
    keywordNames_["KEYWORD_TEXT"] = {"text"};
    keywordNames_["KEYWORD_STYLE"] = {"style"};
    keywordNames_["KEYWORD_SCRIPT"] = {"script"};
    keywordNames_["KEYWORD_CUSTOM"] = {"[Custom]"};
    keywordNames_["KEYWORD_TEMPLATE"] = {"[Template]"};
    keywordNames_["KEYWORD_ORIGIN"] = {"[Origin]"};
    keywordNames_["KEYWORD_IMPORT"] = {"[Import]"};
    keywordNames_["KEYWORD_NAMESPACE"] = {"[Namespace]"};
}

void CHtlConfiguration::resetToDefaults() {
    indexInitialCount_ = 0;
    disableNameGroup_ = true;
    disableCustomOriginType_ = false;
    debugMode_ = false;
    optionCount_ = 3;
    
    keywordNames_.clear();
    customOriginTypes_.clear();
    
    setDefaultKeywords();
}

bool CHtlConfiguration::validate() const {
    // 验证配置的有效性
    if (optionCount_ < 1 || optionCount_ > 10) {
        return false;
    }
    
    // 验证关键字名称
    for (const auto& pair : keywordNames_) {
        if (pair.second.empty() || pair.second.size() > static_cast<size_t>(optionCount_)) {
            return false;
        }
    }
    
    return true;
}

std::vector<std::string> CHtlConfiguration::getKeywordNames(const std::string& keyword) const {
    auto it = keywordNames_.find(keyword);
    if (it != keywordNames_.end()) {
        return it->second;
    }
    return {};
}

std::string CHtlConfiguration::getKeywordName(const std::string& keyword) const {
    auto names = getKeywordNames(keyword);
    return names.empty() ? "" : names[0];
}

bool CHtlConfiguration::isValidCustomOriginType(const std::string& type) const {
    if (disableCustomOriginType_) {
        return false;
    }
    
    // 检查基本类型
    if (type == "Html" || type == "Style" || type == "JavaScript") {
        return true;
    }
    
    // 检查自定义类型
    return customOriginTypes_.find(type) != customOriginTypes_.end();
}

std::unordered_set<std::string> CHtlConfiguration::getCustomOriginTypes() const {
    return customOriginTypes_;
}

CHtlConfiguration& CHtlConfigurationManager::getInstance() {
    return instance_;
}

bool CHtlConfigurationManager::loadConfigurationFile(const std::string& filePath) {
    // TODO: 实现文件加载逻辑
    return false;
}

bool CHtlConfigurationManager::applyConfigurationFromChtlFile(const std::string& chtlContent) {
    // 查找配置块
    std::regex configPattern(R"(\[Configuration\]\s*\{[^}]*\})");
    std::smatch match;
    
    if (std::regex_search(chtlContent, match, configPattern)) {
        return instance_.loadFromConfigBlock(match[0].str());
    }
    
    return true; // 没有配置块也是有效的
}

} // namespace chtl