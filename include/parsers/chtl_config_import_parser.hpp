#pragma once

#include "ast/chtl_ast.hpp"
#include "lexer/chtl_lexer.hpp"
#include "common/types.hpp"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace chtl {

/**
 * 配置项类型枚举
 */
enum class ConfigurationType {
    BOOLEAN,            // 布尔类型
    INTEGER,            // 整数类型
    STRING,             // 字符串类型
    ARRAY,              // 数组类型
    KEYWORD_MAPPING     // 关键字映射类型
};

/**
 * 配置项定义
 */
struct ConfigurationItem {
    std::string name;                       // 配置项名称
    ConfigurationType type;                 // 配置项类型
    std::string value;                      // 字符串值
    std::vector<std::string> arrayValues;   // 数组值
    Position position;                      // 位置信息
    
    ConfigurationItem() : type(ConfigurationType::STRING) {}
    
    ConfigurationItem(const std::string& itemName, ConfigurationType itemType, 
                     const std::string& itemValue = "", const Position& pos = Position{})
        : name(itemName), type(itemType), value(itemValue), position(pos) {}
};

/**
 * Name配置块
 */
struct NameConfiguration {
    std::unordered_map<std::string, std::vector<std::string>> keywordMappings;
    int optionCount;
    Position position;
    
    NameConfiguration() : optionCount(3) {}
};

/**
 * 配置组定义
 */
struct ConfigurationGroup {
    std::unordered_map<std::string, ConfigurationItem> items;  // 配置项
    NameConfiguration nameConfig;                              // Name配置块
    bool hasNameConfig;                                        // 是否有Name配置块
    Position position;                                         // 位置信息
    
    ConfigurationGroup() : hasNameConfig(false) {}
};

/**
 * 导入类型枚举
 */
enum class ImportType {
    HTML_IMPORT,        // @Html 导入HTML文件
    STYLE_IMPORT,       // @Style 导入CSS文件
    JAVASCRIPT_IMPORT,  // @JavaScript 导入JS文件
    CHTL_IMPORT,        // @Chtl 导入CHTL文件
    CJMOD_IMPORT,       // @CJmod 导入CJmod文件
    CUSTOM_ELEMENT,     // [Custom] @Element 导入自定义元素
    CUSTOM_STYLE,       // [Custom] @Style 导入自定义样式组
    CUSTOM_VAR,         // [Custom] @Var 导入自定义变量组
    TEMPLATE_ELEMENT,   // [Template] @Element 导入元素模板
    TEMPLATE_STYLE,     // [Template] @Style 导入样式组模板
    TEMPLATE_VAR        // [Template] @Var 导入变量组模板
};

/**
 * 导入路径类型
 */
enum class ImportPathType {
    RELATIVE_PATH,      // 相对路径
    ABSOLUTE_PATH,      // 绝对路径
    MODULE_PATH,        // 模块路径 (使用'.'表示'/')
    WILDCARD_PATH       // 通配符路径
};

/**
 * 导入定义
 */
struct ImportDefinition {
    ImportType type;                    // 导入类型
    std::string targetName;             // 目标名称 (用于Custom/Template导入)
    std::string path;                   // 导入路径
    std::string alias;                  // 别名 (as 语法)
    ImportPathType pathType;            // 路径类型
    Position position;                  // 位置信息
    
    ImportDefinition() : type(ImportType::HTML_IMPORT), pathType(ImportPathType::RELATIVE_PATH) {}
    
    ImportDefinition(ImportType importType, const std::string& importPath, 
                    const Position& pos = Position{})
        : type(importType), path(importPath), pathType(ImportPathType::RELATIVE_PATH), position(pos) {}
};

/**
 * 配置组注册表
 * 管理全局配置项
 */
class ConfigurationRegistry {
public:
    ConfigurationRegistry();
    ~ConfigurationRegistry();

    /**
     * 注册配置组
     */
    bool registerConfiguration(const ConfigurationGroup& config);

    /**
     * 获取配置项
     */
    const ConfigurationItem* getConfigurationItem(const std::string& name) const;

    /**
     * 获取Name配置
     */
    const NameConfiguration* getNameConfiguration() const;

    /**
     * 获取关键字映射
     */
    std::vector<std::string> getKeywordMapping(const std::string& keyword) const;

    /**
     * 检查配置项是否存在
     */
    bool hasConfiguration(const std::string& name) const;

    /**
     * 清空注册表
     */
    void clear();

    /**
     * 应用默认配置
     */
    void applyDefaultConfiguration();

private:
    ConfigurationGroup globalConfig_;
    bool initialized_;
    
    // 初始化默认配置
    void initializeDefaults();
};

/**
 * 导入注册表
 * 管理所有导入的文件和模块
 */
class ImportRegistry {
public:
    ImportRegistry();
    ~ImportRegistry();

    /**
     * 注册导入
     */
    bool registerImport(const ImportDefinition& importDef);

    /**
     * 查找导入
     */
    const ImportDefinition* findImport(const std::string& identifier) const;

    /**
     * 获取所有导入
     */
    const std::vector<ImportDefinition>& getAllImports() const;

    /**
     * 检查循环依赖
     */
    bool hasCircularDependency(const std::string& path) const;

    /**
     * 清空注册表
     */
    void clear();

    /**
     * 解析导入路径
     */
    std::string resolveImportPath(const std::string& path, ImportPathType pathType) const;

private:
    std::vector<ImportDefinition> imports_;
    std::unordered_set<std::string> importedPaths_;
    
    // 路径解析
    std::string normalizePath(const std::string& path) const;
    bool isValidPath(const std::string& path) const;
};

/**
 * 配置组解析状态
 */
enum class ConfigParseState {
    INITIAL,                    // 初始状态
    READING_CONFIG_HEADER,      // 读取配置组头 ([Configuration])
    READING_CONFIG_CONTENT,     // 读取配置组内容
    READING_CONFIG_ITEM,        // 读取配置项
    READING_NAME_BLOCK,         // 读取Name配置块
    READING_NAME_ITEM,          // 读取Name配置项
    READING_ARRAY_VALUES        // 读取数组值
};

/**
 * 导入解析状态
 */
enum class ImportParseState {
    INITIAL,                    // 初始状态
    READING_IMPORT_HEADER,      // 读取导入头 ([Import])
    READING_IMPORT_TYPE,        // 读取导入类型
    READING_TARGET_NAME,        // 读取目标名称
    READING_FROM_KEYWORD,       // 读取from关键字
    READING_IMPORT_PATH,        // 读取导入路径
    READING_AS_KEYWORD,         // 读取as关键字
    READING_ALIAS               // 读取别名
};

/**
 * 配置组与导入系统上下文
 */
struct ConfigImportContext {
    ConfigParseState configState;      // 配置解析状态
    ImportParseState importState;      // 导入解析状态
    std::string currentSection;        // 当前段落
    int nestingLevel;                  // 嵌套层级
    bool inNameBlock;                  // 是否在Name块中
    std::string currentConfigItem;     // 当前配置项
    ImportType currentImportType;      // 当前导入类型
    
    ConfigImportContext() 
        : configState(ConfigParseState::INITIAL)
        , importState(ImportParseState::INITIAL)
        , nestingLevel(0)
        , inNameBlock(false)
        , currentImportType(ImportType::HTML_IMPORT) {}
};

/**
 * 配置组与导入系统扫描器
 */
class ConfigImportScanner {
public:
    ConfigImportScanner();
    ~ConfigImportScanner();

    /**
     * 扫描配置组内容
     */
    std::vector<ConfigurationItem> scanConfigurationContent(const std::string& content);

    /**
     * 扫描导入语句
     */
    ImportDefinition scanImportStatement(const std::string& statement);

    /**
     * 扫描数组值
     */
    std::vector<std::string> scanArrayValues(const std::string& arrayContent);

    /**
     * 验证配置项语法
     */
    bool validateConfigurationSyntax(const std::string& content);

    /**
     * 验证导入语法
     */
    bool validateImportSyntax(const std::string& statement);

private:
    // 配置项解析
    ConfigurationItem parseConfigurationItem(const std::string& line);
    ConfigurationType determineConfigurationType(const std::string& value);
    
    // 导入解析
    ImportType parseImportType(const std::string& typeStr, bool hasPrefix = false);
    ImportPathType determinePathType(const std::string& path);
    
    // 工具方法
    std::string trimWhitespace(const std::string& str);
    std::vector<std::string> splitString(const std::string& str, char delimiter);
    bool isValidIdentifier(const std::string& identifier);
};

/**
 * 配置组与导入系统状态机
 */
class ConfigImportStateMachine {
public:
    ConfigImportStateMachine();
    ~ConfigImportStateMachine();

    /**
     * 重置状态机
     */
    void reset();

    /**
     * 处理配置组token
     */
    bool processConfigurationToken(const Token& token, ConfigImportContext& context);

    /**
     * 处理导入token
     */
    bool processImportToken(const Token& token, ConfigImportContext& context);

    /**
     * 获取配置组结果
     */
    const ConfigurationGroup& getConfigurationGroup() const;

    /**
     * 获取导入定义结果
     */
    const std::vector<ImportDefinition>& getImportDefinitions() const;

private:
    ConfigurationGroup configGroup_;
    std::vector<ImportDefinition> importDefinitions_;
    
    // 临时状态
    std::string currentItemName_;
    std::string currentItemValue_;
    std::vector<std::string> currentArrayValues_;
    ImportDefinition currentImport_;

    // 配置组状态处理
    void handleConfigInitialState(const Token& token, ConfigImportContext& context);
    void handleConfigHeaderState(const Token& token, ConfigImportContext& context);
    void handleConfigContentState(const Token& token, ConfigImportContext& context);
    void handleConfigItemState(const Token& token, ConfigImportContext& context);
    void handleNameBlockState(const Token& token, ConfigImportContext& context);
    void handleNameItemState(const Token& token, ConfigImportContext& context);
    void handleArrayValuesState(const Token& token, ConfigImportContext& context);

    // 导入状态处理
    void handleImportInitialState(const Token& token, ConfigImportContext& context);
    void handleImportHeaderState(const Token& token, ConfigImportContext& context);
    void handleImportTypeState(const Token& token, ConfigImportContext& context);
    void handleTargetNameState(const Token& token, ConfigImportContext& context);
    void handleFromKeywordState(const Token& token, ConfigImportContext& context);
    void handleImportPathState(const Token& token, ConfigImportContext& context);
    void handleAsKeywordState(const Token& token, ConfigImportContext& context);
    void handleAliasState(const Token& token, ConfigImportContext& context);

    // 工具方法
    void addConfigurationItem(const std::string& name, const std::string& value);
    void addNameConfigurationItem(const std::string& name, const std::vector<std::string>& values);
    void addImportDefinition(const ImportDefinition& importDef);
    ImportType parseImportType(const std::string& typeStr, bool hasCustomPrefix = false, bool hasTemplatePrefix = false);
};

/**
 * CHTL配置组与导入系统解析器
 * 严格按照CHTL语法文档实现配置组和导入功能
 */
class CHTLConfigImportParser {
public:
    CHTLConfigImportParser();
    ~CHTLConfigImportParser();

    /**
     * 解析配置组声明 [Configuration] { ... }
     */
    ParseResult parseConfigurationDeclaration(const std::string& source, const std::string& filename = "");

    /**
     * 解析导入声明 [Import] @Type from path as alias
     */
    ParseResult parseImportDeclaration(const std::string& source, const std::string& filename = "");

    /**
     * 设置配置注册表
     */
    void setConfigurationRegistry(std::shared_ptr<ConfigurationRegistry> registry);

    /**
     * 获取配置注册表
     */
    std::shared_ptr<ConfigurationRegistry> getConfigurationRegistry() const;

    /**
     * 设置导入注册表
     */
    void setImportRegistry(std::shared_ptr<ImportRegistry> registry);

    /**
     * 获取导入注册表
     */
    std::shared_ptr<ImportRegistry> getImportRegistry() const;

    /**
     * 设置严格模式
     */
    void setStrictMode(bool strict);

    /**
     * 获取解析错误
     */
    const std::vector<ParseError>& getErrors() const;

    /**
     * 获取解析警告
     */
    const std::vector<ParseWarning>& getWarnings() const;

private:
    std::shared_ptr<ConfigurationRegistry> configRegistry_;
    std::shared_ptr<ImportRegistry> importRegistry_;
    ConfigImportScanner scanner_;
    ConfigImportStateMachine stateMachine_;
    CHTLLexer lexer_;
    
    bool strictMode_;
    std::vector<ParseError> errors_;
    std::vector<ParseWarning> warnings_;

    // 核心解析方法
    ConfigurationGroup parseConfigurationGroup(const std::vector<Token>& tokens);
    ImportDefinition parseImportDefinition(const std::vector<Token>& tokens);
    NameConfiguration parseNameConfiguration(const std::vector<Token>& tokens, size_t& index);
    
    // 配置项解析
    ConfigurationItem parseConfigurationItem(const std::vector<Token>& tokens, size_t& index);
    std::vector<std::string> parseArrayValue(const std::vector<Token>& tokens, size_t& index);
    ConfigurationType determineValueType(const std::string& value);

    // 导入解析
    ImportType parseImportType(const std::vector<Token>& tokens, size_t& index);
    std::string parseImportPath(const std::vector<Token>& tokens, size_t& index);
    std::string parseAlias(const std::vector<Token>& tokens, size_t& index);

    // AST节点创建
    CHTLASTNodePtr createConfigurationNode(const ConfigurationGroup& config);
    CHTLASTNodePtr createImportNode(const ImportDefinition& importDef);

    // 工具方法
    void reportError(const std::string& message, const Position& position = Position{});
    void reportWarning(const std::string& message, const Position& position = Position{});
    bool isValidConfigurationName(const std::string& name) const;
    bool isValidImportPath(const std::string& path) const;
    std::string getImportTypeString(ImportType type) const;
    std::string getConfigurationTypeString(ConfigurationType type) const;
};

/**
 * 配置组与导入系统工具类
 */
class ConfigImportUtils {
public:
    /**
     * 检查是否为配置组声明
     */
    static bool isConfigurationDeclaration(const std::string& source);

    /**
     * 检查是否为导入声明
     */
    static bool isImportDeclaration(const std::string& source);

    /**
     * 提取配置项名称
     */
    static std::string extractConfigurationName(const std::string& source);

    /**
     * 提取导入类型
     */
    static std::string extractImportType(const std::string& source);

    /**
     * 提取导入路径
     */
    static std::string extractImportPath(const std::string& source);

    /**
     * 验证配置语法
     */
    static bool validateConfigurationSyntax(const std::string& source);

    /**
     * 验证导入语法
     */
    static bool validateImportSyntax(const std::string& source);

    /**
     * 解析路径类型
     */
    static ImportPathType determinePathType(const std::string& path);

    /**
     * 标准化路径
     */
    static std::string normalizePath(const std::string& path);

    /**
     * 检查路径有效性
     */
    static bool isValidPath(const std::string& path);

    /**
     * 解析数组值
     */
    static std::vector<std::string> parseArrayValue(const std::string& arrayStr);

    /**
     * 格式化配置值
     */
    static std::string formatConfigurationValue(const std::string& value, ConfigurationType type);

    /**
     * 检查配置名称有效性
     */
    static bool isValidConfigurationName(const std::string& name);

    /**
     * 检查导入别名有效性
     */
    static bool isValidAlias(const std::string& alias);
};

} // namespace chtl