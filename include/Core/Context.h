#pragma once

#include "Types.h"
#include <stack>
#include <unordered_set>

namespace CHTL {
namespace Core {

// 作用域类型
enum class ScopeType {
    GLOBAL,         // 全局作用域
    NAMESPACE,      // 命名空间作用域
    TEMPLATE,       // 模板作用域
    CUSTOM,         // 自定义作用域
    ELEMENT,        // 元素作用域
    STYLE_BLOCK,    // 样式块作用域
    SCRIPT_BLOCK    // 脚本块作用域
};

// 符号信息
struct Symbol {
    String name;
    String type;
    String value;
    ASTNodePtr declaration;
    SourceRange location;
    ScopeType scope;
    StringMap attributes;
    
    Symbol() = default;
    Symbol(const String& n, const String& t, const String& v = "")
        : name(n), type(t), value(v), scope(ScopeType::GLOBAL) {}
};

using SymbolPtr = std::shared_ptr<Symbol>;
using SymbolMap = std::unordered_map<String, SymbolPtr>;

// 作用域
class Scope {
public:
    Scope(ScopeType type, const String& name = "") 
        : type_(type), name_(name) {}
    
    ScopeType getType() const { return type_; }
    const String& getName() const { return name_; }
    
    // 符号管理
    void addSymbol(SymbolPtr symbol);
    SymbolPtr findSymbol(const String& name) const;
    bool hasSymbol(const String& name) const;
    void removeSymbol(const String& name);
    
    const SymbolMap& getSymbols() const { return symbols_; }
    
    // 父作用域
    void setParent(std::shared_ptr<Scope> parent) { parent_ = parent; }
    std::shared_ptr<Scope> getParent() const { return parent_; }
    
    // 查找符号（包括父作用域）
    SymbolPtr lookupSymbol(const String& name) const;

private:
    ScopeType type_;
    String name_;
    SymbolMap symbols_;
    std::shared_ptr<Scope> parent_;
};

using ScopePtr = std::shared_ptr<Scope>;

// 上下文管理器
class Context {
public:
    Context();
    ~Context() = default;

    // 作用域管理
    void pushScope(ScopeType type, const String& name = "");
    void popScope();
    ScopePtr getCurrentScope() const;
    ScopePtr getGlobalScope() const { return globalScope_; }
    
    // 符号管理
    void declareSymbol(const String& name, const String& type, const String& value = "");
    void declareSymbol(SymbolPtr symbol);
    SymbolPtr findSymbol(const String& name) const;
    SymbolPtr findSymbolInCurrentScope(const String& name) const;
    bool hasSymbol(const String& name) const;
    
    // 命名空间管理
    void enterNamespace(const String& name);
    void exitNamespace();
    String getCurrentNamespace() const;
    String getFullyQualifiedName(const String& name) const;
    
    // 导入管理
    void addImport(const String& module, const String& alias = "");
    bool hasImport(const String& module) const;
    String resolveImport(const String& name) const;
    const StringMap& getImports() const { return imports_; }
    
    // 错误管理
    void addError(const String& message, const Position& pos);
    void addError(ErrorInfoPtr error);
    const ErrorList& getErrors() const { return errors_; }
    bool hasErrors() const { return !errors_.empty(); }
    void clearErrors() { errors_.clear(); }
    
    // 警告管理
    void addWarning(const String& message, const Position& pos);
    const ErrorList& getWarnings() const { return warnings_; }
    bool hasWarnings() const { return !warnings_.empty(); }
    void clearWarnings() { warnings_.clear(); }
    
    // 编译选项
    void setOptions(const CompileOptions& options) { options_ = options; }
    const CompileOptions& getOptions() const { return options_; }
    
    // 当前文件信息
    void setCurrentFile(const String& filename) { currentFile_ = filename; }
    const String& getCurrentFile() const { return currentFile_; }
    
    // 约束管理
    void addConstraint(const String& constraint);
    bool hasConstraint(const String& constraint) const;
    void removeConstraint(const String& constraint);
    const std::unordered_set<String>& getConstraints() const { return constraints_; }
    
    // 模板和自定义定义管理
    void registerTemplate(const String& name, const String& type, ASTNodePtr node);
    void registerCustom(const String& name, const String& type, ASTNodePtr node);
    ASTNodePtr findTemplate(const String& name, const String& type) const;
    ASTNodePtr findCustom(const String& name, const String& type) const;
    
    // 状态查询
    bool isInTemplate() const;
    bool isInCustom() const;
    bool isInNamespace() const;
    bool isInGlobalScope() const;
    
    // 重置上下文
    void reset();
    
    // 克隆上下文
    std::shared_ptr<Context> clone() const;

private:
    ScopePtr globalScope_;
    std::stack<ScopePtr> scopeStack_;
    std::stack<String> namespaceStack_;
    
    StringMap imports_;
    ErrorList errors_;
    ErrorList warnings_;
    CompileOptions options_;
    String currentFile_;
    std::unordered_set<String> constraints_;
    
    // 模板和自定义定义存储
    std::unordered_map<String, std::unordered_map<String, ASTNodePtr>> templates_;
    std::unordered_map<String, std::unordered_map<String, ASTNodePtr>> customs_;
};

// 上下文RAII管理器
class ScopeGuard {
public:
    ScopeGuard(Context& context, ScopeType type, const String& name = "")
        : context_(context) {
        context_.pushScope(type, name);
    }
    
    ~ScopeGuard() {
        context_.popScope();
    }
    
    // 禁止复制
    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;

private:
    Context& context_;
};

// 命名空间RAII管理器
class NamespaceGuard {
public:
    NamespaceGuard(Context& context, const String& name)
        : context_(context) {
        context_.enterNamespace(name);
    }
    
    ~NamespaceGuard() {
        context_.exitNamespace();
    }
    
    // 禁止复制
    NamespaceGuard(const NamespaceGuard&) = delete;
    NamespaceGuard& operator=(const NamespaceGuard&) = delete;

private:
    Context& context_;
};

} // namespace Core
} // namespace CHTL