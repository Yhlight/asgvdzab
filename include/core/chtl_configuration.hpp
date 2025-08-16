#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <unordered_set>

namespace chtl {

/**
 * CHTL配置组系统
 * 
 * 支持自定义关键字名称、原始嵌入类型、行为配置等
 * 符合CHTL语法文档中的[Configuration]语法规范
 */
class CHtlConfiguration {
public:
    CHtlConfiguration();
    
    /**
     * 从配置块字符串加载配置
     * @param configBlock 配置块内容，格式为[Configuration] { ... }
     * @return 是否加载成功
     */
    bool loadFromConfigBlock(const std::string& configBlock);
    
    /**
     * 获取索引起始计数
     */
    int getIndexInitialCount() const { return indexInitialCount_; }
    
    /**
     * 是否禁用Name配置组
     */
    bool isNameGroupDisabled() const { return disableNameGroup_; }
    
    /**
     * 是否禁用自定义原始嵌入类型
     */
    bool isCustomOriginTypeDisabled() const { return disableCustomOriginType_; }
    
    /**
     * 是否启用DEBUG模式
     */
    bool isDebugMode() const { return debugMode_; }
    
    /**
     * 获取组选项数量限制
     */
    int getOptionCount() const { return optionCount_; }
    
    /**
     * 获取关键字名称（支持多个值）
     */
    std::vector<std::string> getKeywordNames(const std::string& keyword) const;
    
    /**
     * 获取单个关键字名称
     */
    std::string getKeywordName(const std::string& keyword) const;
    
    /**
     * 检查是否为有效的自定义原始嵌入类型
     */
    bool isValidCustomOriginType(const std::string& type) const;
    
    /**
     * 获取所有自定义原始嵌入类型
     */
    std::unordered_set<std::string> getCustomOriginTypes() const;
    
    /**
     * 重置为默认配置
     */
    void resetToDefaults();
    
    /**
     * 验证配置的有效性
     */
    bool validate() const;

private:
    // 基本配置选项
    int indexInitialCount_;
    bool disableNameGroup_;
    bool disableCustomOriginType_;
    bool debugMode_;
    int optionCount_;
    
    // Name配置块 - 关键字名称映射
    std::unordered_map<std::string, std::vector<std::string>> keywordNames_;
    
    // OriginType配置块 - 自定义原始嵌入类型
    std::unordered_set<std::string> customOriginTypes_;
    
    /**
     * 解析配置项
     */
    bool parseConfigLine(const std::string& line);
    
    /**
     * 解析Name配置块
     */
    bool parseNameBlock(const std::string& nameBlockContent);
    
    /**
     * 解析OriginType配置块
     */
    bool parseOriginTypeBlock(const std::string& originTypeBlockContent);
    
    /**
     * 解析数组值 [val1, val2, val3]
     */
    std::vector<std::string> parseArrayValue(const std::string& value);
    
    /**
     * 清理字符串（移除前后空白和引号）
     */
    std::string cleanString(const std::string& str);
    
    /**
     * 验证类型名称格式
     */
    bool isValidTypeName(const std::string& typeName);
    
    /**
     * 设置默认关键字名称
     */
    void setDefaultKeywords();
};

/**
 * 全局配置实例管理器
 */
class CHtlConfigurationManager {
public:
    /**
     * 获取全局配置实例
     */
    static CHtlConfiguration& getInstance();
    
    /**
     * 加载配置文件
     */
    static bool loadConfigurationFile(const std::string& filePath);
    
    /**
     * 从CHTL文件中提取并应用配置
     */
    static bool applyConfigurationFromChtlFile(const std::string& chtlContent);

private:
    static CHtlConfiguration instance_;
};

} // namespace chtl