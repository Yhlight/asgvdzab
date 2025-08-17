#include "../../include/Core/Context.h"
#include <algorithm>

namespace CHTL {
namespace Core {

// Scope类实现

void Scope::addSymbol(SymbolPtr symbol) {
    if (symbol) {
        symbols_[symbol->name] = symbol;
    }
}

SymbolPtr Scope::findSymbol(const String& name) const {
    auto it = symbols_.find(name);
    return it != symbols_.end() ? it->second : nullptr;
}

bool Scope::hasSymbol(const String& name) const {
    return symbols_.find(name) != symbols_.end();
}

void Scope::removeSymbol(const String& name) {
    symbols_.erase(name);
}

SymbolPtr Scope::lookupSymbol(const String& name) const {
    // 首先在当前作用域查找
    auto symbol = findSymbol(name);
    if (symbol) {
        return symbol;
    }
    
    // 在父作用域中查找
    if (parent_) {
        return parent_->lookupSymbol(name);
    }
    
    return nullptr;
}

// Context类实现

Context::Context() {
    globalScope_ = std::make_shared<Scope>(ScopeType::GLOBAL, "global");
    scopeStack_.push(globalScope_);
}

void Context::pushScope(ScopeType type, const String& name) {
    auto newScope = std::make_shared<Scope>(type, name);
    
    if (!scopeStack_.empty()) {
        newScope->setParent(scopeStack_.top());
    }
    
    scopeStack_.push(newScope);
}

void Context::popScope() {
    if (scopeStack_.size() > 1) { // 保留全局作用域
        scopeStack_.pop();
    }
}

ScopePtr Context::getCurrentScope() const {
    return scopeStack_.empty() ? nullptr : scopeStack_.top();
}

void Context::declareSymbol(const String& name, const String& type, const String& value) {
    auto symbol = std::make_shared<Symbol>(name, type, value);
    declareSymbol(symbol);
}

void Context::declareSymbol(SymbolPtr symbol) {
    if (symbol && !scopeStack_.empty()) {
        symbol->scope = scopeStack_.top()->getType();
        scopeStack_.top()->addSymbol(symbol);
    }
}

SymbolPtr Context::findSymbol(const String& name) const {
    if (scopeStack_.empty()) {
        return nullptr;
    }
    
    return scopeStack_.top()->lookupSymbol(name);
}

SymbolPtr Context::findSymbolInCurrentScope(const String& name) const {
    if (scopeStack_.empty()) {
        return nullptr;
    }
    
    return scopeStack_.top()->findSymbol(name);
}

bool Context::hasSymbol(const String& name) const {
    return findSymbol(name) != nullptr;
}

void Context::enterNamespace(const String& name) {
    namespaceStack_.push(name);
    pushScope(ScopeType::NAMESPACE, name);
}

void Context::exitNamespace() {
    if (!namespaceStack_.empty()) {
        namespaceStack_.pop();
        popScope();
    }
}

String Context::getCurrentNamespace() const {
    if (namespaceStack_.empty()) {
        return "";
    }
    
    String fullNamespace;
    std::stack<String> tempStack = namespaceStack_;
    std::vector<String> namespaces;
    
    while (!tempStack.empty()) {
        namespaces.push_back(tempStack.top());
        tempStack.pop();
    }
    
    std::reverse(namespaces.begin(), namespaces.end());
    
    for (size_t i = 0; i < namespaces.size(); ++i) {
        if (i > 0) fullNamespace += ".";
        fullNamespace += namespaces[i];
    }
    
    return fullNamespace;
}

String Context::getFullyQualifiedName(const String& name) const {
    String currentNs = getCurrentNamespace();
    return currentNs.empty() ? name : currentNs + "." + name;
}

void Context::addImport(const String& module, const String& alias) {
    imports_[module] = alias.empty() ? module : alias;
}

bool Context::hasImport(const String& module) const {
    return imports_.find(module) != imports_.end();
}

String Context::resolveImport(const String& name) const {
    for (const auto& [module, alias] : imports_) {
        if (alias == name) {
            return module;
        }
    }
    return name;
}

void Context::addError(const String& message, const Position& pos) {
    auto error = std::make_shared<ErrorInfo>();
    error->message = message;
    error->line = pos.line;
    error->column = pos.column;
    error->file = pos.filename;
    errors_.push_back(error);
}

void Context::addError(ErrorInfoPtr error) {
    if (error) {
        errors_.push_back(error);
    }
}

void Context::addWarning(const String& message, const Position& pos) {
    auto warning = std::make_shared<ErrorInfo>();
    warning->message = message;
    warning->line = pos.line;
    warning->column = pos.column;
    warning->file = pos.filename;
    warnings_.push_back(warning);
}

void Context::addConstraint(const String& constraint) {
    constraints_.insert(constraint);
}

bool Context::hasConstraint(const String& constraint) const {
    return constraints_.find(constraint) != constraints_.end();
}

void Context::removeConstraint(const String& constraint) {
    constraints_.erase(constraint);
}

void Context::registerTemplate(const String& name, const String& type, ASTNodePtr node) {
    if (node) {
        templates_[type][name] = node;
    }
}

void Context::registerCustom(const String& name, const String& type, ASTNodePtr node) {
    if (node) {
        customs_[type][name] = node;
    }
}

ASTNodePtr Context::findTemplate(const String& name, const String& type) const {
    auto typeIt = templates_.find(type);
    if (typeIt != templates_.end()) {
        auto nameIt = typeIt->second.find(name);
        if (nameIt != typeIt->second.end()) {
            return nameIt->second;
        }
    }
    return nullptr;
}

ASTNodePtr Context::findCustom(const String& name, const String& type) const {
    auto typeIt = customs_.find(type);
    if (typeIt != customs_.end()) {
        auto nameIt = typeIt->second.find(name);
        if (nameIt != typeIt->second.end()) {
            return nameIt->second;
        }
    }
    return nullptr;
}

bool Context::isInTemplate() const {
    if (scopeStack_.empty()) return false;
    return scopeStack_.top()->getType() == ScopeType::TEMPLATE;
}

bool Context::isInCustom() const {
    if (scopeStack_.empty()) return false;
    return scopeStack_.top()->getType() == ScopeType::CUSTOM;
}

bool Context::isInNamespace() const {
    return !namespaceStack_.empty();
}

bool Context::isInGlobalScope() const {
    return scopeStack_.size() == 1 && scopeStack_.top() == globalScope_;
}

void Context::reset() {
    // 清空作用域栈，只保留全局作用域
    while (scopeStack_.size() > 1) {
        scopeStack_.pop();
    }
    
    // 清空命名空间栈
    while (!namespaceStack_.empty()) {
        namespaceStack_.pop();
    }
    
    // 清空其他状态
    imports_.clear();
    errors_.clear();
    warnings_.clear();
    constraints_.clear();
    templates_.clear();
    customs_.clear();
    currentFile_.clear();
    
    // 重新初始化全局作用域
    if (globalScope_) {
        globalScope_ = std::make_shared<Scope>(ScopeType::GLOBAL, "global");
        scopeStack_.top() = globalScope_;
    }
}

std::shared_ptr<Context> Context::clone() const {
    auto cloned = std::make_shared<Context>();
    
    // 复制基本状态
    cloned->imports_ = imports_;
    cloned->options_ = options_;
    cloned->currentFile_ = currentFile_;
    cloned->constraints_ = constraints_;
    
    // 复制模板和自定义定义
    cloned->templates_ = templates_;
    cloned->customs_ = customs_;
    
    // 注意：不复制错误和警告，也不复制作用域栈
    // 克隆的上下文从干净的状态开始
    
    return cloned;
}

} // namespace Core
} // namespace CHTL