#ifndef CHTL_COMPILER_CHTL_COMPILER_H
#define CHTL_COMPILER_CHTL_COMPILER_H

#include "compiler/base_compiler.h"
#include "compiler/chtl/chtl_lexer.h"
#include "compiler/chtl/chtl_parser.h"
#include "compiler/chtl/chtl_ast.h"
#include <unordered_map>
#include <stack>

namespace chtl {
namespace compiler {

// CHTL编译器上下文
struct CHTLContext {
    // 模板和自定义定义
    std::unordered_map<std::string, std::shared_ptr<ast::TemplateNode>> templates;
    std::unordered_map<std::string, std::shared_ptr<ast::CustomNode>> customs;
    
    // 变量组
    std::unordered_map<std::string, std::map<std::string, std::string>> varGroups;
    
    // 命名空间栈
    std::stack<std::string> namespaceStack;
    
    // 当前元素信息（用于局部样式块）
    std::string currentElement;
    std::string currentClassName;
    std::string currentId;
    
    // 全局样式和脚本收集
    std::vector<std::string> globalStyles;
    std::vector<std::string> globalScripts;
    
    // 唯一ID计数器
    std::unordered_map<std::string, size_t> idCounters;
    
    // 配置选项
    struct Configuration {
        int indexInitialCount = 0;
        bool disableNameGroup = false;
        bool debugMode = false;
        std::unordered_map<std::string, std::vector<std::string>> nameMap;
    } config;
};

class CHTLCompiler : public BaseCompiler {
public:
    CHTLCompiler();
    ~CHTLCompiler() override;
    
    // 实现基类接口
    CompilationResult compile(const CodeSegment& segment,
                            const CompilerOptions& options = CompilerOptions()) override;
    
    std::string getName() const override { return "CHTL Compiler"; }
    
    bool supports(CodeSegmentType type) const override {
        return type == CodeSegmentType::CHTL;
    }
    
    void reset() override;

private:
    std::unique_ptr<CHTLLexer> lexer_;
    std::unique_ptr<CHTLParser> parser_;
    CHTLContext context_;
    
    // 编译AST节点
    std::string compileNode(const std::shared_ptr<ast::ASTNode>& node);
    
    // 编译各种节点类型
    std::string compileElement(const std::shared_ptr<ast::ElementNode>& node);
    std::string compileText(const std::shared_ptr<ast::TextNode>& node);
    std::string compileStyle(const std::shared_ptr<ast::StyleNode>& node);
    std::string compileScript(const std::shared_ptr<ast::ScriptNode>& node);
    std::string compileTemplate(const std::shared_ptr<ast::TemplateNode>& node);
    std::string compileCustom(const std::shared_ptr<ast::CustomNode>& node);
    std::string compileImport(const std::shared_ptr<ast::ImportNode>& node);
    std::string compileOrigin(const std::shared_ptr<ast::OriginNode>& node);
    
    // 处理局部样式块
    void processLocalStyle(const std::shared_ptr<ast::StyleNode>& node,
                          std::string& inlineStyles,
                          std::string& globalStyles);
    
    // 处理自动类名和ID
    std::string generateAutoClassName(const std::string& selector);
    std::string generateAutoId(const std::string& selector);
    
    // 处理模板展开
    std::string expandTemplate(const std::string& templateName,
                              const std::map<std::string, std::string>& params);
    
    // 处理变量替换
    std::string replaceVariables(const std::string& content);
    
    // 工具方法
    std::string escapeHtml(const std::string& str);
    std::string formatAttributes(const std::map<std::string, std::string>& attrs);
    std::string getUniqueId(const std::string& prefix);
};

} // namespace compiler
} // namespace chtl

#endif // CHTL_COMPILER_CHTL_COMPILER_H