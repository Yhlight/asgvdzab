#include "CHTLGlobalMap.hpp"
#include <algorithm>

namespace chtl {

// Scope实现
void Scope::addSymbol(const std::string& name, std::shared_ptr<SymbolInfo> symbol) {
    symbols_[name] = symbol;
}

std::shared_ptr<SymbolInfo> Scope::findSymbol(const std::string& name) const {
    auto it = symbols_.find(name);
    if (it != symbols_.end()) {
        return it->second;
    }
    
    // 递归查找父作用域
    if (parent_) {
        return parent_->findSymbol(name);
    }
    
    return nullptr;
}

std::shared_ptr<SymbolInfo> Scope::findLocalSymbol(const std::string& name) const {
    auto it = symbols_.find(name);
    return (it != symbols_.end()) ? it->second : nullptr;
}

std::vector<std::shared_ptr<SymbolInfo>> Scope::getAllSymbols() const {
    std::vector<std::shared_ptr<SymbolInfo>> result;
    for (const auto& pair : symbols_) {
        result.push_back(pair.second);
    }
    return result;
}

// CHTLGlobalMap实现
CHTLGlobalMap::CHTLGlobalMap() {
    globalScope_ = std::make_unique<Scope>(ScopeType::GLOBAL, "global");
    currentScope_ = globalScope_.get();
    
    // 设置默认配置
    configurations_["INDEX_INITIAL_COUNT"] = "0";
    configurations_["DISABLE_NAME_GROUP"] = "true";
    configurations_["DEBUG_MODE"] = "false";
    configurations_["OPTION_COUNT"] = "3";
}

CHTLGlobalMap::~CHTLGlobalMap() = default;

void CHTLGlobalMap::enterScope(ScopeType type, const std::string& name) {
    auto newScope = std::make_unique<Scope>(type, name, currentScope_);
    Scope* scopePtr = newScope.get();
    
    // 将新作用域添加到当前作用域的符号表中
    auto scopeInfo = std::make_shared<SymbolInfo>(name, SymbolType::NAMESPACE);
    scopeInfo->data = std::shared_ptr<void>(newScope.release(), [](void* p) {
        delete static_cast<Scope*>(p);
    });
    currentScope_->addSymbol(name, scopeInfo);
    
    currentScope_ = scopePtr;
}

void CHTLGlobalMap::exitScope() {
    if (currentScope_ && currentScope_->getParent()) {
        currentScope_ = currentScope_->getParent();
    }
}

void CHTLGlobalMap::addSymbol(const std::string& name, SymbolType type, const std::string& namespace_) {
    auto symbol = std::make_shared<SymbolInfo>(name, type, namespace_.empty() ? getCurrentNamespace() : namespace_);
    currentScope_->addSymbol(name, symbol);
}

std::shared_ptr<SymbolInfo> CHTLGlobalMap::findSymbol(const std::string& name) const {
    return currentScope_->findSymbol(name);
}

std::shared_ptr<SymbolInfo> CHTLGlobalMap::findSymbol(const std::string& name, const std::string& namespace_) const {
    // 先在指定命名空间查找
    std::string fullName = namespace_.empty() ? name : namespace_ + "." + name;
    auto symbol = currentScope_->findSymbol(fullName);
    
    if (!symbol && !namespace_.empty()) {
        // 如果没找到，尝试在命名空间作用域中查找
        auto nsSymbol = currentScope_->findSymbol(namespace_);
        if (nsSymbol && nsSymbol->data) {
            Scope* nsScope = static_cast<Scope*>(nsSymbol->data.get());
            symbol = nsScope->findLocalSymbol(name);
        }
    }
    
    return symbol;
}

void CHTLGlobalMap::enterNamespace(const std::string& name) {
    namespaceStack_.push_back(name);
    enterScope(ScopeType::NAMESPACE, name);
}

void CHTLGlobalMap::exitNamespace() {
    if (!namespaceStack_.empty()) {
        namespaceStack_.pop_back();
        exitScope();
    }
}

std::string CHTLGlobalMap::getCurrentNamespace() const {
    return getFullNamespacePath();
}

void CHTLGlobalMap::addImport(const std::string& moduleName, const std::string& alias) {
    imports_[moduleName] = alias.empty() ? moduleName : alias;
}

bool CHTLGlobalMap::hasImport(const std::string& moduleName) const {
    return imports_.find(moduleName) != imports_.end();
}

std::string CHTLGlobalMap::getImportAlias(const std::string& moduleName) const {
    auto it = imports_.find(moduleName);
    return (it != imports_.end()) ? it->second : "";
}

void CHTLGlobalMap::setConfiguration(const std::string& key, const std::string& value) {
    configurations_[key] = value;
}

std::string CHTLGlobalMap::getConfiguration(const std::string& key) const {
    auto it = configurations_.find(key);
    return (it != configurations_.end()) ? it->second : "";
}

void CHTLGlobalMap::clear() {
    globalScope_ = std::make_unique<Scope>(ScopeType::GLOBAL, "global");
    currentScope_ = globalScope_.get();
    namespaceStack_.clear();
    imports_.clear();
    // 保留默认配置
}

std::string CHTLGlobalMap::getFullNamespacePath() const {
    std::string path;
    for (size_t i = 0; i < namespaceStack_.size(); ++i) {
        if (i > 0) path += ".";
        path += namespaceStack_[i];
    }
    return path;
}

} // namespace chtl