#pragma once

#include "ast/chtl_ast.hpp"
#include "lexer/chtl_lexer.hpp"
#include "parsers/chtl_template_parser.hpp"
#include "common/types.hpp"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace chtl {

/**
 * 自定义类型枚举
 */
enum class CustomType {
    STYLE_CUSTOM,      // @Style 自定义样式组
    ELEMENT_CUSTOM,    // @Element 自定义元素
    VAR_CUSTOM         // @Var 自定义变量组
};

/**
 * 特例化操作类型
 */
enum class SpecializationType {
    DELETE_PROPERTY,        // delete property;
    DELETE_ELEMENT,         // delete element;
    DELETE_INHERITANCE,     // delete @Style/@Element/@Var;
    INSERT_ELEMENT,         // insert position selector { ... }
    INDEX_ACCESS,           // element[index]
    VALUELESS_PROPERTY,     // property, (无值属性)
    VARIABLE_OVERRIDE       // ThemeColor(var = value)
};

// InsertPosition 已在 ast/chtl_ast.hpp 中定义

/**
 * 特例化操作定义
 */
struct SpecializationOperation {
    SpecializationType type;        // 操作类型
    std::string target;             // 目标 (属性名、元素名等)
    InsertPosition position;        // 插入位置 (仅用于插入操作)
    std::string selector;           // 选择器 (仅用于插入操作)
    CHTLASTNodePtr content;         // 操作内容
    int index;                      // 索引 (用于索引访问)
    Position location;              // 位置信息
    
    SpecializationOperation(SpecializationType t, const std::string& tgt, const Position& pos = Position{})
        : type(t), target(tgt), position(InsertPosition::AFTER), index(-1), location(pos) {}
};

/**
 * 无值属性定义
 */
struct ValuelessProperty {
    std::string name;               // 属性名
    Position position;              // 位置信息
    
    ValuelessProperty(const std::string& n, const Position& pos = Position{})
        : name(n), position(pos) {}
};

/**
 * 自定义定义
 */
struct CustomDefinition {
    CustomType type;                                    // 自定义类型
    std::string name;                                   // 自定义名称
    CHTLASTNodePtr content;                             // 自定义内容
    std::vector<ValuelessProperty> valuelessProperties; // 无值属性列表 (用于样式组)
    std::vector<SpecializationOperation> specializations; // 特例化操作列表
    std::vector<TemplateReference> inheritance;         // 继承列表
    Position position;                                  // 位置信息
    
    CustomDefinition() : type(CustomType::STYLE_CUSTOM), name(""), position(Position{}) {}
    
    CustomDefinition(CustomType t, const std::string& n, const Position& pos = Position{})
        : type(t), name(n), position(pos) {}
};

/**
 * 自定义使用引用
 */
struct CustomReference {
    CustomType type;                                    // 自定义类型
    std::string customName;                             // 自定义名称
    std::vector<SpecializationOperation> specializations; // 使用时的特例化操作
    std::unordered_map<std::string, std::string> variableOverrides; // 变量覆盖
    Position position;                                  // 位置信息
    
    CustomReference(CustomType t, const std::string& name, const Position& pos = Position{})
        : type(t), customName(name), position(pos) {}
};

/**
 * 自定义注册表
 * 管理所有已定义的自定义内容
 */
class CustomRegistry {
public:
    CustomRegistry();
    ~CustomRegistry();

    /**
     * 注册自定义定义
     */
    bool registerCustom(const CustomDefinition& customDef);

    /**
     * 查找自定义定义
     */
    const CustomDefinition* findCustom(CustomType type, const std::string& name) const;

    /**
     * 获取所有自定义
     */
    const std::unordered_map<std::string, CustomDefinition>& getAllCustoms() const;

    /**
     * 清空注册表
     */
    void clear();

    /**
     * 验证自定义引用
     */
    bool validateCustomReference(const CustomReference& ref) const;

private:
    std::unordered_map<std::string, CustomDefinition> customs_;
    
    // 生成自定义键名
    std::string generateCustomKey(CustomType type, const std::string& name) const;
};

/**
 * 自定义解析状态
 */
enum class CustomParseState {
    INITIAL,                    // 初始状态
    READING_CUSTOM_HEADER,      // 读取自定义头 ([Custom] @Style)
    READING_CUSTOM_NAME,        // 读取自定义名称
    READING_CUSTOM_CONTENT,     // 读取自定义内容
    READING_SPECIALIZATION,     // 读取特例化操作
    READING_VALUELESS_PROPS,    // 读取无值属性
    READING_INSERT_OP,          // 读取插入操作
    READING_DELETE_OP,          // 读取删除操作
    READING_INDEX_ACCESS,       // 读取索引访问
    READING_USAGE              // 读取自定义使用
};

/**
 * 自定义系统上下文
 */
struct CustomContext {
    CustomType currentType;             // 当前自定义类型
    std::string currentName;            // 当前自定义名称
    int nestingLevel;                   // 嵌套层级
    bool inSpecialization;              // 是否在特例化操作中
    std::string currentTarget;          // 当前操作目标
    
    CustomContext() : currentType(CustomType::STYLE_CUSTOM), nestingLevel(0), inSpecialization(false) {}
};

/**
 * 自定义系统扫描器
 * 负责扫描和识别自定义语法中的特殊token
 */
class CustomScanner {
public:
    CustomScanner();
    ~CustomScanner();

    /**
     * 扫描自定义内容
     * @param content 自定义内容
     * @param context 自定义上下文
     * @return 扫描结果
     */
    std::vector<Token> scanCustomContent(const std::string& content, const CustomContext& context);

private:
    std::string content_;
    size_t currentPos_;
    int line_;
    int column_;
    CustomContext context_;

    // 扫描方法
    char currentChar() const;
    char peekChar(size_t offset = 1) const;
    void advance();
    bool isEOF() const;
    Position getCurrentPosition() const;
    
    // 特定扫描方法
    Token scanDeleteKeyword();
    Token scanInsertKeyword();
    Token scanPositionKeyword();
    Token scanIndexAccess();
    Token scanValuelessProperty();
    
    // 工具方法
    bool isAlpha(char c) const;
    bool isDigit(char c) const;
    bool isAlphaNumeric(char c) const;
    bool isWhitespace(char c) const;
    void skipWhitespace();
};

/**
 * 自定义系统状态机
 */
class CustomStateMachine {
public:
    CustomStateMachine();
    ~CustomStateMachine();

    /**
     * 重置状态机
     */
    void reset();

    /**
     * 处理token
     * @param token 输入token
     * @param context 自定义上下文
     * @return 是否成功处理
     */
    bool processToken(const Token& token, CustomContext& context);

    /**
     * 获取当前状态
     */
    CustomParseState getCurrentState() const;

    /**
     * 获取收集的特例化操作
     */
    const std::vector<SpecializationOperation>& getSpecializations() const;

    /**
     * 获取收集的无值属性
     */
    const std::vector<ValuelessProperty>& getValuelessProperties() const;

private:
    CustomParseState currentState_;
    std::vector<SpecializationOperation> specializations_;
    std::vector<ValuelessProperty> valuelessProperties_;
    
    // 临时状态
    std::string currentOperation_;
    std::string currentTarget_;
    InsertPosition currentPosition_;
    int currentIndex_;
    Position currentLocation_;

    // 状态转换方法
    void handleInitialState(const Token& token, CustomContext& context);
    void handleReadingSpecializationState(const Token& token, CustomContext& context);
    void handleReadingValuelessPropsState(const Token& token, CustomContext& context);
    void handleReadingInsertOpState(const Token& token, CustomContext& context);
    void handleReadingDeleteOpState(const Token& token, CustomContext& context);
    void handleReadingIndexAccessState(const Token& token, CustomContext& context);

    // 工具方法
    void addSpecialization(const SpecializationOperation& op);
    void addValuelessProperty(const ValuelessProperty& prop);
    InsertPosition parseInsertPosition(const std::string& posStr);
    bool isValidInsertPosition(const std::string& posStr) const;
    bool isValidDeleteTarget(const std::string& target) const;
};

/**
 * CHTL自定义系统解析器
 * 严格按照CHTL语法文档实现自定义解析
 */
class CHTLCustomParser {
public:
    CHTLCustomParser();
    ~CHTLCustomParser();

    /**
     * 解析自定义声明 [Custom] @Type Name { ... }
     * @param source 源代码
     * @param filename 文件名
     * @return 解析结果
     */
    ParseResult parseCustomDeclaration(const std::string& source, const std::string& filename = "");

    /**
     * 解析自定义使用 @Style CustomName { 特例化操作 }
     * @param usage 自定义使用代码
     * @param context 当前上下文
     * @return 自定义引用
     */
    CustomReference parseCustomUsage(const std::string& usage, const std::string& context = "");

    /**
     * 展开自定义引用
     * @param ref 自定义引用
     * @return 展开后的AST节点
     */
    CHTLASTNodePtr expandCustomReference(const CustomReference& ref);

    /**
     * 设置自定义注册表
     */
    void setCustomRegistry(std::shared_ptr<CustomRegistry> registry);

    /**
     * 获取自定义注册表
     */
    std::shared_ptr<CustomRegistry> getCustomRegistry() const;

    /**
     * 设置模板注册表 (用于模板和自定义的互操作)
     */
    void setTemplateRegistry(std::shared_ptr<TemplateRegistry> templateRegistry);

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
    std::shared_ptr<CustomRegistry> customRegistry_;
    std::shared_ptr<TemplateRegistry> templateRegistry_;
    CustomScanner scanner_;
    CustomStateMachine stateMachine_;
    CHTLLexer lexer_;
    CustomParseState currentState_;
    
    bool strictMode_;
    std::vector<ParseError> errors_;
    std::vector<ParseWarning> warnings_;

    // 核心解析方法
    CustomDefinition parseCustomDefinition(const std::vector<Token>& tokens);
    CustomType parseCustomType(const std::string& typeStr);
    std::string parseCustomName(const std::vector<Token>& tokens, size_t& index);
    CHTLASTNodePtr parseCustomContent(const std::vector<Token>& tokens, size_t& index, CustomType type);
    std::vector<SpecializationOperation> parseSpecializations(const std::vector<Token>& tokens, size_t& index);
    std::vector<ValuelessProperty> parseValuelessProperties(const std::vector<Token>& tokens, size_t& index);

    // 自定义展开方法
    CHTLASTNodePtr expandStyleCustom(const CustomDefinition& customDef, const CustomReference& ref);
    CHTLASTNodePtr expandElementCustom(const CustomDefinition& customDef, const CustomReference& ref);
    CHTLASTNodePtr expandVariableCustom(const CustomDefinition& customDef, const CustomReference& ref);

    // 特例化操作处理方法
    void applySpecializations(CHTLASTNodePtr& target, const std::vector<SpecializationOperation>& specializations);
    void applyDeleteOperation(CHTLASTNodePtr& target, const SpecializationOperation& op);
    void applyInsertOperation(CHTLASTNodePtr& target, const SpecializationOperation& op);
    void applyIndexAccessOperation(CHTLASTNodePtr& target, const SpecializationOperation& op);

    // 工具方法
    void reportError(const std::string& message, const Position& position = Position{});
    void reportWarning(const std::string& message, const Position& position = Position{});
    bool isValidCustomName(const std::string& name) const;
    bool isCompatibleWithTemplate(const CustomDefinition& customDef, const TemplateDefinition& templateDef) const;
};

/**
 * 自定义工具类
 * 提供自定义相关的工具方法
 */
class CustomUtils {
public:
    /**
     * 检查是否为自定义声明
     */
    static bool isCustomDeclaration(const std::string& source);

    /**
     * 检查是否为自定义使用
     */
    static bool isCustomUsage(const std::string& source);

    /**
     * 检查是否为特例化操作
     */
    static bool isSpecializationOperation(const std::string& source);

    /**
     * 提取自定义类型字符串
     */
    static std::string extractCustomTypeString(const std::string& source);

    /**
     * 提取自定义名称
     */
    static std::string extractCustomName(const std::string& source);

    /**
     * 验证自定义语法
     */
    static bool validateCustomSyntax(const std::string& source);

    /**
     * 解析索引访问
     */
    static std::pair<std::string, int> parseIndexAccess(const std::string& expression);

    /**
     * 验证插入位置
     */
    static bool isValidInsertPosition(const std::string& position);
};

} // namespace chtl