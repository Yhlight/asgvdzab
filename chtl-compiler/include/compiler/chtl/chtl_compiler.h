#ifndef CHTL_COMPILER_CHTL_COMPILER_H
#define CHTL_COMPILER_CHTL_COMPILER_H

#include "compiler/base_compiler.h"
#include "compiler/chtl/chtl_lexer.h"
#include "compiler/chtl/chtl_parser.h"
#include "compiler/chtl/chtl_ast.h"
#include <unordered_map>
#include <stack>
#include <string>
#include <memory>
#include "compiler/chtl/chtl_global_map.h"

namespace chtl {
namespace compiler {

// CHTL编译器状态
enum class CHTLCompilerState {
    NORMAL,              // 普通状态
    IN_ELEMENT,          // 在元素内部
    IN_STYLE_BLOCK,      // 在style块内
    IN_SCRIPT_BLOCK,     // 在script块内
    IN_TEMPLATE,         // 在模板定义内
    IN_CUSTOM,           // 在自定义定义内
    IN_NAMESPACE,        // 在命名空间内
    IN_CONFIGURATION,    // 在配置组内
    IN_ORIGIN,           // 在原始嵌入块内
    IN_TEXT,             // 在text块内
};

// CHTL编译上下文
struct CHTLContext {
    // 全局映射表
    std::shared_ptr<CHTLGlobalMap> globalMap;
    
    // 状态管理
    CHTLCompilerState state;
    std::stack<CHTLCompilerState> stateStack;
    
    // 当前元素信息
    std::string currentElement;
    std::stack<std::string> elementStack;
    std::string currentElementId;
    std::string currentElementClass;
    
    // 局部样式和脚本收集
    std::string localStyles;
    std::string localScripts;
    
    // ID生成器
    size_t idCounter;
    size_t classIdCounter;
    
    // 当前命名空间
    std::string currentNamespace;
    
    // 错误和警告
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
    
    CHTLContext() : 
        globalMap(std::make_shared<CHTLGlobalMap>()),
        state(CHTLCompilerState::NORMAL),
        idCounter(0), 
        classIdCounter(0) {}
    
    // 状态管理方法
    void pushState(CHTLCompilerState newState) {
        stateStack.push(state);
        state = newState;
    }
    
    void popState() {
        if (!stateStack.empty()) {
            state = stateStack.top();
            stateStack.pop();
        }
    }
    
    // ID生成方法
    std::string generateElementId(const std::string& prefix = "chtl") {
        return prefix + "_" + std::to_string(++idCounter);
    }
    
    std::string generateClassName(const std::string& base = "") {
        if (!base.empty()) {
            return base + "_auto_" + std::to_string(++classIdCounter);
        }
        return currentElement + "_auto_" + std::to_string(++classIdCounter);
    }
};

class CHTLCompiler : public BaseCompiler {
public:
    CHTLCompiler();
    ~CHTLCompiler() override;
    
    // 实现BaseCompiler接口
    CompilationResult compile(const CodeSegment& segment, 
                            const CompilerOptions& options) override;
    std::string getName() const override;
    bool supports(CodeSegmentType type) const override;
    void reset() override;
    
private:
    std::unique_ptr<CHTLLexer> lexer_;
    std::unique_ptr<CHTLParser> parser_;
    CHTLContext context_;
    
    // 编译各种AST节点
    void compileNode(std::shared_ptr<ast::ASTNode> node, std::stringstream& output);
    void compileDocument(std::shared_ptr<ast::DocumentNode> node, std::stringstream& output);
    void compileElement(std::shared_ptr<ast::ElementNode> node, std::stringstream& output);
    void compileText(std::shared_ptr<ast::TextNode> node, std::stringstream& output);
    void compileStyle(std::shared_ptr<ast::StyleNode> node, std::stringstream& output);
    void compileScript(std::shared_ptr<ast::ScriptNode> node, std::stringstream& output);
    void compileTemplate(std::shared_ptr<ast::TemplateNode> node, std::stringstream& output);
    void compileCustom(std::shared_ptr<ast::CustomNode> node, std::stringstream& output);
    void compileImport(std::shared_ptr<ast::ImportNode> node, std::stringstream& output);
    void compileNamespace(std::shared_ptr<ast::NamespaceNode> node, std::stringstream& output);
    void compileOrigin(std::shared_ptr<ast::OriginNode> node, std::stringstream& output);
    void compileConfiguration(std::shared_ptr<ast::ConfigurationNode> node, std::stringstream& output);
    void compileComment(std::shared_ptr<ast::CommentNode> node, std::stringstream& output);
    
    // 辅助方法
    std::string extractInlineStyles(const std::string& styleContent);
    std::string processLocalStyle(const std::string& styleContent);
    std::string expandTemplate(const std::string& templateName, 
                             const std::map<std::string, std::string>& params);
    std::string replaceVarReferences(const std::string& content);
    void processImport(const std::string& path, const std::string& alias);
};

} // namespace compiler
} // namespace chtl

#endif // CHTL_COMPILER_CHTL_COMPILER_H