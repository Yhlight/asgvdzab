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
 * 约束类型枚举
 */
enum class ConstraintType {
    PRECISE,        // 精确约束 (具体的HTML元素或自定义/模板对象)
    TYPE_BASED,     // 类型约束 (@Html, [Custom], [Template])
    GLOBAL          // 全局约束 (在命名空间内)
};

/**
 * 约束目标类型
 */
enum class ConstraintTargetType {
    HTML_ELEMENT,           // HTML元素 (span, div等)
    CUSTOM_ELEMENT,         // [Custom] @Element
    CUSTOM_STYLE,           // [Custom] @Style  
    CUSTOM_VAR,             // [Custom] @Var
    TEMPLATE_ELEMENT,       // [Template] @Element
    TEMPLATE_STYLE,         // [Template] @Style
    TEMPLATE_VAR,           // [Template] @Var
    HTML_TYPE,              // @Html 类型
    CUSTOM_TYPE,            // [Custom] 类型
    TEMPLATE_TYPE           // [Template] 类型
};

/**
 * 约束目标定义
 */
struct ConstraintTarget {
    ConstraintTargetType type;      // 约束目标类型
    std::string name;               // 目标名称 (如果适用)
    Position position;              // 位置信息
    
    ConstraintTarget() : type(ConstraintTargetType::HTML_ELEMENT) {}
    
    ConstraintTarget(ConstraintTargetType targetType, const std::string& targetName = "", 
                    const Position& pos = Position{})
        : type(targetType), name(targetName), position(pos) {}
};

/**
 * 约束定义
 */
struct ConstraintDefinition {
    ConstraintType type;                            // 约束类型
    std::vector<ConstraintTarget> targets;          // 约束目标列表
    Position position;                              // 位置信息
    
    ConstraintDefinition() : type(ConstraintType::PRECISE) {}
    
    ConstraintDefinition(ConstraintType constraintType, const Position& pos = Position{})
        : type(constraintType), position(pos) {}
};

/**
 * 命名空间定义
 */
struct NamespaceDefinition {
    std::string name;                               // 命名空间名称
    std::string fullPath;                           // 完整路径 (如 "space.room2")
    std::vector<NamespaceDefinition> children;      // 嵌套命名空间
    std::vector<ConstraintDefinition> constraints;  // 全局约束
    Position position;                              // 位置信息
    bool hasBlock;                                  // 是否有花括号包围
    
    NamespaceDefinition() : hasBlock(false) {}
    
    NamespaceDefinition(const std::string& namespaceName, const Position& pos = Position{})
        : name(namespaceName), fullPath(namespaceName), position(pos), hasBlock(false) {}
};

/**
 * 命名空间注册表
 * 管理所有命名空间定义和层次结构
 */
class NamespaceRegistry {
public:
    NamespaceRegistry();
    ~NamespaceRegistry();

    /**
     * 注册命名空间
     */
    bool registerNamespace(const NamespaceDefinition& namespaceDef);

    /**
     * 查找命名空间
     */
    const NamespaceDefinition* findNamespace(const std::string& fullPath) const;

    /**
     * 获取所有顶级命名空间
     */
    const std::vector<NamespaceDefinition>& getTopLevelNamespaces() const;

    /**
     * 检查命名空间路径是否有效
     */
    bool isValidNamespacePath(const std::string& path) const;

    /**
     * 获取命名空间的完整路径
     */
    std::string getFullPath(const std::string& parentPath, const std::string& childName) const;

    /**
     * 检查约束是否允许使用指定目标
     */
    bool isConstraintViolated(const std::string& namespacePath, const ConstraintTarget& target) const;

    /**
     * 清空注册表
     */
    void clear();

private:
    std::vector<NamespaceDefinition> topLevelNamespaces_;
    std::unordered_map<std::string, const NamespaceDefinition*> pathToNamespace_;
    
    // 辅助方法
    void indexNamespace(const NamespaceDefinition& namespaceDef, const std::string& parentPath = "");
    bool isConstraintApplicable(const ConstraintDefinition& constraint, const ConstraintTarget& target) const;
};

/**
 * 约束注册表
 * 管理所有约束定义
 */
class ConstraintRegistry {
public:
    ConstraintRegistry();
    ~ConstraintRegistry();

    /**
     * 注册约束
     */
    bool registerConstraint(const std::string& scope, const ConstraintDefinition& constraint);

    /**
     * 获取作用域的约束
     */
    std::vector<ConstraintDefinition> getConstraints(const std::string& scope) const;

    /**
     * 检查约束违反
     */
    bool hasConstraintViolation(const std::string& scope, const ConstraintTarget& target) const;

    /**
     * 清空注册表
     */
    void clear();

private:
    std::unordered_map<std::string, std::vector<ConstraintDefinition>> scopeConstraints_;
    
    // 检查单个约束是否违反
    bool isViolated(const ConstraintDefinition& constraint, const ConstraintTarget& target) const;
};

/**
 * 命名空间解析状态
 */
enum class NamespaceParseState {
    INITIAL,                    // 初始状态
    READING_NAMESPACE_HEADER,   // 读取命名空间头 ([Namespace])
    READING_NAMESPACE_NAME,     // 读取命名空间名称
    READING_NAMESPACE_CONTENT,  // 读取命名空间内容
    READING_CONSTRAINT,         // 读取约束
    READING_CONSTRAINT_TARGETS  // 读取约束目标
};

/**
 * 命名空间与约束系统上下文
 */
struct NamespaceContext {
    NamespaceParseState state;              // 解析状态
    std::string currentNamespace;           // 当前命名空间
    std::string currentScope;               // 当前作用域
    int nestingLevel;                       // 嵌套层级
    bool expectingBlock;                    // 是否期望花括号块
    std::vector<std::string> namespacePath; // 命名空间路径栈
    
    NamespaceContext() 
        : state(NamespaceParseState::INITIAL)
        , nestingLevel(0)
        , expectingBlock(false) {}
};

/**
 * 命名空间与约束系统扫描器
 */
class NamespaceScanner {
public:
    NamespaceScanner();
    ~NamespaceScanner();

    /**
     * 扫描命名空间声明
     */
    NamespaceDefinition scanNamespaceDeclaration(const std::string& content);

    /**
     * 扫描约束声明
     */
    ConstraintDefinition scanConstraintDeclaration(const std::string& content);

    /**
     * 扫描约束目标
     */
    std::vector<ConstraintTarget> scanConstraintTargets(const std::string& content);

    /**
     * 验证命名空间语法
     */
    bool validateNamespaceSyntax(const std::string& content);

    /**
     * 验证约束语法
     */
    bool validateConstraintSyntax(const std::string& content);

private:
    // 命名空间解析
    std::string parseNamespaceName(const std::string& line);
    
    // 约束解析
    ConstraintType parseConstraintType(const std::string& line);
    ConstraintTarget parseConstraintTarget(const std::string& targetStr);
    ConstraintTargetType determineTargetType(const std::string& targetStr);
    
    // 工具方法
    std::string trimWhitespace(const std::string& str);
    std::vector<std::string> splitTargets(const std::string& str);
    bool isValidIdentifier(const std::string& identifier);
    bool isValidNamespaceName(const std::string& name);
};

/**
 * 命名空间与约束系统状态机
 */
class NamespaceStateMachine {
public:
    NamespaceStateMachine();
    ~NamespaceStateMachine();

    /**
     * 重置状态机
     */
    void reset();

    /**
     * 处理命名空间token
     */
    bool processNamespaceToken(const Token& token, NamespaceContext& context);

    /**
     * 处理约束token
     */
    bool processConstraintToken(const Token& token, NamespaceContext& context);

    /**
     * 获取命名空间定义结果
     */
    const std::vector<NamespaceDefinition>& getNamespaceDefinitions() const;

    /**
     * 获取约束定义结果
     */
    const std::vector<ConstraintDefinition>& getConstraintDefinitions() const;

private:
    std::vector<NamespaceDefinition> namespaceDefinitions_;
    std::vector<ConstraintDefinition> constraintDefinitions_;
    
    // 临时状态
    std::string currentNamespaceName_;
    std::vector<ConstraintTarget> currentConstraintTargets_;
    ConstraintDefinition currentConstraint_;

    // 命名空间状态处理
    void handleNamespaceInitialState(const Token& token, NamespaceContext& context);
    void handleNamespaceHeaderState(const Token& token, NamespaceContext& context);
    void handleNamespaceNameState(const Token& token, NamespaceContext& context);
    void handleNamespaceContentState(const Token& token, NamespaceContext& context);

    // 约束状态处理
    void handleConstraintInitialState(const Token& token, NamespaceContext& context);
    void handleConstraintTargetsState(const Token& token, NamespaceContext& context);

    // 工具方法
    void addNamespaceDefinition(const NamespaceDefinition& namespaceDef);
    void addConstraintDefinition(const ConstraintDefinition& constraint);
    ConstraintTargetType parseConstraintTargetType(const std::string& typeStr, bool hasPrefix = false);
};

/**
 * CHTL命名空间与约束系统解析器
 * 严格按照CHTL语法文档实现命名空间和约束功能
 */
class CHTLNamespaceParser {
public:
    CHTLNamespaceParser();
    ~CHTLNamespaceParser();

    /**
     * 解析命名空间声明 [Namespace] name { ... }
     */
    ParseResult parseNamespaceDeclaration(const std::string& source, const std::string& filename = "");

    /**
     * 解析约束声明 except targets;
     */
    ParseResult parseConstraintDeclaration(const std::string& source, const std::string& filename = "");

    /**
     * 设置命名空间注册表
     */
    void setNamespaceRegistry(std::shared_ptr<NamespaceRegistry> registry);

    /**
     * 获取命名空间注册表
     */
    std::shared_ptr<NamespaceRegistry> getNamespaceRegistry() const;

    /**
     * 设置约束注册表
     */
    void setConstraintRegistry(std::shared_ptr<ConstraintRegistry> registry);

    /**
     * 获取约束注册表
     */
    std::shared_ptr<ConstraintRegistry> getConstraintRegistry() const;

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
    std::shared_ptr<NamespaceRegistry> namespaceRegistry_;
    std::shared_ptr<ConstraintRegistry> constraintRegistry_;
    NamespaceScanner scanner_;
    NamespaceStateMachine stateMachine_;
    CHTLLexer lexer_;
    
    bool strictMode_;
    std::vector<ParseError> errors_;
    std::vector<ParseWarning> warnings_;

    // 核心解析方法
    NamespaceDefinition parseNamespaceDefinition(const std::vector<Token>& tokens);
    ConstraintDefinition parseConstraintDefinition(const std::vector<Token>& tokens);
    
    // 命名空间解析
    std::string parseNamespaceName(const std::vector<Token>& tokens, size_t& index);
    std::vector<NamespaceDefinition> parseNestedNamespaces(const std::vector<Token>& tokens, size_t& index);
    
    // 约束解析
    ConstraintType parseConstraintType(const std::vector<Token>& tokens, size_t& index);
    std::vector<ConstraintTarget> parseConstraintTargets(const std::vector<Token>& tokens, size_t& index);
    ConstraintTarget parseConstraintTarget(const std::vector<Token>& tokens, size_t& index);

    // AST节点创建
    CHTLASTNodePtr createNamespaceNode(const NamespaceDefinition& namespaceDef);
    CHTLASTNodePtr createConstraintNode(const ConstraintDefinition& constraint);

    // 工具方法
    void reportError(const std::string& message, const Position& position = Position{});
    void reportWarning(const std::string& message, const Position& position = Position{});
    bool isValidNamespaceName(const std::string& name) const;
    bool isValidConstraintTarget(const std::string& target) const;
    std::string getConstraintTypeString(ConstraintType type) const;
    std::string getConstraintTargetTypeString(ConstraintTargetType type) const;
};

/**
 * 命名空间与约束系统工具类
 */
class NamespaceUtils {
public:
    /**
     * 检查是否为命名空间声明
     */
    static bool isNamespaceDeclaration(const std::string& source);

    /**
     * 检查是否为约束声明
     */
    static bool isConstraintDeclaration(const std::string& source);

    /**
     * 提取命名空间名称
     */
    static std::string extractNamespaceName(const std::string& source);

    /**
     * 提取约束目标
     */
    static std::vector<std::string> extractConstraintTargets(const std::string& source);

    /**
     * 验证命名空间语法
     */
    static bool validateNamespaceSyntax(const std::string& source);

    /**
     * 验证约束语法
     */
    static bool validateConstraintSyntax(const std::string& source);

    /**
     * 标准化命名空间路径
     */
    static std::string normalizeNamespacePath(const std::string& path);

    /**
     * 分割命名空间路径
     */
    static std::vector<std::string> splitNamespacePath(const std::string& path);

    /**
     * 连接命名空间路径
     */
    static std::string joinNamespacePath(const std::vector<std::string>& pathParts);

    /**
     * 检查命名空间名称有效性
     */
    static bool isValidNamespaceName(const std::string& name);

    /**
     * 检查约束目标有效性
     */
    static bool isValidConstraintTarget(const std::string& target);

    /**
     * 解析约束目标类型
     */
    static ConstraintTargetType parseConstraintTargetType(const std::string& target);

    /**
     * 格式化约束目标
     */
    static std::string formatConstraintTarget(const ConstraintTarget& target);
};

} // namespace chtl