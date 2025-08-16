#pragma once

#include "ast/chtl_ast.hpp"
#include "lexer/pure_chtl_tokens.hpp"
#include "common/types.hpp"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace chtl {

/**
 * CHTL解析器错误信息
 */
struct CHTLParseError {
    std::string message;        // 错误消息
    Position position;          // 错误位置
    std::string context;        // 错误上下文
    std::string suggestion;     // 修复建议
};

/**
 * CHTL解析结果
 */
struct CHTLParseResult {
    bool success;                           // 解析是否成功
    CHTLASTNodePtr ast;                    // 解析得到的AST
    std::vector<CHTLParseError> errors;    // 错误列表
    std::vector<std::string> warnings;     // 警告列表
    
    CHTLParseResult() : success(false) {}
};

/**
 * CHTL解析器配置
 */
struct CHTLParserConfig {
    bool enableStrictMode;              // 严格模式，不允许语法扩展
    bool enableWarnings;                // 启用警告
    bool enableCommentsInAST;           // 是否在AST中保留注释
    std::string defaultNamespace;       // 默认命名空间
    
    CHTLParserConfig() 
        : enableStrictMode(true)
        , enableWarnings(true) 
        , enableCommentsInAST(false)
        , defaultNamespace("") {}
};

/**
 * CHTL解析器类
 * 严格按照CHTL语法文档.md实现
 * 支持完整的CHTL语法，包括：
 * - 文本节点 (text { })
 * - 元素节点 (div, span, etc.)
 * - 属性 (id: value;)
 * - 局部样式块 (style { })
 * - 模板系统 ([Template] @Style/@Element/@Var)
 * - 自定义系统 ([Custom] 与特例化)
 * - 原始嵌入 ([Origin])
 * - 导入系统 ([Import])
 * - 命名空间 ([Namespace])
 * - 配置组 ([Configuration])
 * - 注释 (//, 块注释, --)
 * - 字面量 (双引号, 单引号, 无修饰)
 * - CE对等式 (: 与 = 等价)
 */
class CHTLParser {
public:
    explicit CHTLParser(const CHTLParserConfig& config = CHTLParserConfig());
    ~CHTLParser() = default;

    /**
     * 解析CHTL源代码
     * @param source CHTL源代码
     * @param filename 文件名（用于错误报告）
     * @return 解析结果
     */
    CHTLParseResult parse(const std::string& source, const std::string& filename = "");

    /**
     * 解析Token序列
     * @param tokens Token序列
     * @param filename 文件名
     * @return 解析结果
     */
    CHTLParseResult parseTokens(const std::vector<PureCHTLToken>& tokens, const std::string& filename = "");

private:
    CHTLParserConfig config_;
    std::vector<PureCHTLToken> tokens_;
    size_t currentTokenIndex_;
    std::string filename_;
    std::vector<CHTLParseError> errors_;
    std::vector<std::string> warnings_;

    // ===== Token管理 =====
    const PureCHTLToken& currentToken() const;
    const PureCHTLToken& peekToken(size_t offset = 1) const;
    void advance();
    bool isAtEnd() const;
    bool match(PureCHTLTokenType type);
    bool check(PureCHTLTokenType type) const;
    
    // ===== 错误处理 =====
    void reportError(const std::string& message, const std::string& suggestion = "");
    void reportWarning(const std::string& message);
    void synchronize(); // 错误恢复
    
    // ===== 主要解析方法 =====
    CHTLASTNodePtr parseProgram();
    
    // ===== 顶级声明解析 =====
    CHTLASTNodePtr parseTopLevelDeclaration();
    CHTLASTNodePtr parseTemplateDeclaration();      // [Template]
    CHTLASTNodePtr parseCustomDeclaration();        // [Custom]
    CHTLASTNodePtr parseOriginDeclaration();        // [Origin]
    CHTLASTNodePtr parseImportDeclaration();        // [Import]
    CHTLASTNodePtr parseNamespaceDeclaration();     // [Namespace]
    CHTLASTNodePtr parseConfigurationDeclaration(); // [Configuration]
    
    // ===== 元素和文本解析 =====
    CHTLASTNodePtr parseElement();                  // HTML元素
    CHTLASTNodePtr parseTextNode();                 // text { }
    CHTLASTNodePtr parseAttribute();                // 属性 id: value;
    
    // ===== 样式解析 =====
    CHTLASTNodePtr parseStyleBlock();               // style { }
    CHTLASTNodePtr parseInlineStyle();              // 内联样式属性
    CHTLASTNodePtr parseClassSelector();            // .className
    CHTLASTNodePtr parseIdSelector();               // #idName
    CHTLASTNodePtr parseContextReference();         // &
    CHTLASTNodePtr parsePseudoSelector();           // &:hover, &::before
    
    // ===== 模板解析 =====
    CHTLASTNodePtr parseTemplateStyle();            // [Template] @Style
    CHTLASTNodePtr parseTemplateElement();          // [Template] @Element  
    CHTLASTNodePtr parseTemplateVar();              // [Template] @Var
    CHTLASTNodePtr parseTemplateUsage();            // @Style Name;
    CHTLASTNodePtr parseInheritStatement();         // inherit @Style Name;
    
    // ===== 自定义解析 =====
    CHTLASTNodePtr parseCustomStyle();              // [Custom] @Style
    CHTLASTNodePtr parseCustomElement();            // [Custom] @Element
    CHTLASTNodePtr parseCustomVar();                // [Custom] @Var
    CHTLASTNodePtr parseCustomUsage();              // 自定义使用
    CHTLASTNodePtr parseCustomSpecialization();     // 特例化块
    CHTLASTNodePtr parseDeleteOperation();          // delete
    CHTLASTNodePtr parseInsertOperation();          // insert
    CHTLASTNodePtr parseIndexAccess();              // [index]
    
    // ===== 变量组解析 =====
    CHTLASTNodePtr parseVarGroup();                 // 变量组定义
    CHTLASTNodePtr parseVarProperty();              // 变量属性
    CHTLASTNodePtr parseVarUsage();                 // VarName(prop)
    CHTLASTNodePtr parseVarSpecialization();        // VarName(prop = value)
    
    // ===== 原始嵌入解析 =====
    CHTLASTNodePtr parseOriginHtml();               // [Origin] @Html
    CHTLASTNodePtr parseOriginStyle();              // [Origin] @Style
    CHTLASTNodePtr parseOriginJavaScript();         // [Origin] @JavaScript
    
    // ===== 导入解析 =====
    CHTLASTNodePtr parseImportHtml();               // [Import] @Html
    CHTLASTNodePtr parseImportStyle();              // [Import] @Style
    CHTLASTNodePtr parseImportJavaScript();         // [Import] @JavaScript
    CHTLASTNodePtr parseImportChtl();               // [Import] @Chtl
    CHTLASTNodePtr parseImportCustom();             // [Import] [Custom]
    CHTLASTNodePtr parseImportTemplate();           // [Import] [Template]
    CHTLASTNodePtr parseImportCjmod();              // [Import] @CJmod
    
    // ===== 命名空间解析 =====
    CHTLASTNodePtr parseNamespaceUsage();           // from namespace
    
    // ===== 配置解析 =====
    CHTLASTNodePtr parseConfigProperty();           // 配置属性
    CHTLASTNodePtr parseNameGroup();                // [Name] { }
    
    // ===== 约束解析 =====
    CHTLASTNodePtr parseExceptStatement();          // except 约束
    
    // ===== 注释解析 =====
    CHTLASTNodePtr parseComment();                  // //, /**/, --
    
    // ===== 字面量解析 =====
    CHTLASTNodePtr parseStringLiteral();            // 字符串字面量
    CHTLASTNodePtr parseIdentifier();               // 标识符
    
    // ===== 工具方法 =====
    std::string parseIdentifierString();            // 解析标识符字符串
    std::string parseStringLiteralValue();          // 解析字符串值
    std::string parseBlockContent();                // 解析块内容 { ... }
    std::vector<std::string> parseIdentifierList(); // 解析标识符列表
    std::string parseFilePath();                    // 解析文件路径
    
    // ===== 验证方法 =====
    bool isValidElementName(const std::string& name) const;
    bool isValidAttributeName(const std::string& name) const;
    bool isValidClassName(const std::string& name) const;
    bool isValidIdName(const std::string& name) const;
    bool isValidNamespace(const std::string& name) const;
    
    // ===== 位置信息 =====
    Position getCurrentPosition() const;
    void savePosition(CHTLASTNodePtr node) const;
};

} // namespace chtl