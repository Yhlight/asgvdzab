#include "parser/NamespaceSystem.h"
#include "ast/ASTNode.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cctype>
#include <regex>

namespace chtl {

// 实现类
class NamespaceSystem::Impl {
public:
    NamespaceSystemConfig config;
    std::unordered_map<std::string, NamespaceInfo> namespaces;
    std::vector<std::string> errors;
    bool debug_mode;
    
    Impl(const NamespaceSystemConfig& cfg) : config(cfg), debug_mode(false) {}
    
    ~Impl() = default;
    
    // 内部方法实现
    bool isNamespaceDeclaration(const std::vector<Token>& tokens, size_t position);
    void skipWhitespace(const std::vector<Token>& tokens, size_t& position);
    bool isEndOfTokens(const std::vector<Token>& tokens, size_t position);
    
    // 命名空间验证辅助方法
    bool isValidNamespaceName(const std::string& name);
    bool isValidNestingDepth(size_t depth);
    bool hasValidNamespaceStructure(const NamespaceInfo& namespace_info);
    
    // 命名空间处理辅助方法
    std::string normalizeNamespaceName(const std::string& name);
    std::string createDefaultNamespaceName(const std::string& file_name);
    bool isReservedNamespaceName(const std::string& name);
    
    // 辅助方法
    void logDebug(const std::string& message);
    std::string extractNamespaceContent(const std::vector<Token>& tokens, size_t& position);
    std::string parseNamespaceIdentifier(const std::vector<Token>& tokens, size_t& position);
};

// NamespaceSystem实现
NamespaceSystem::NamespaceSystem(const NamespaceSystemConfig& config) 
    : pImpl(std::make_unique<Impl>(config)), state_machine(nullptr) {}

NamespaceSystem::~NamespaceSystem() = default;

std::shared_ptr<ASTNode> NamespaceSystem::parseNamespaceDeclaration(const std::vector<Token>& tokens, size_t& position) {
    if (!pImpl->isNamespaceDeclaration(tokens, position)) {
        addError("不是命名空间声明");
        return nullptr;
    }
    
    // 跳过[Namespace]
    position++;
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 解析命名空间
    NamespaceInfo namespace_info = parseNamespace(tokens, position);
    
    // 验证命名空间信息
    if (!validateNamespace(namespace_info)) {
        return nullptr;
    }
    
    // 添加命名空间到系统
    addNamespace(namespace_info);
    
    if (pImpl->debug_mode) {
        pImpl->logDebug("命名空间声明解析完成: " + namespace_info.name);
    }
    
    // 创建命名空间声明节点（这里需要创建具体的AST节点类型）
    // 暂时返回nullptr，等待具体的AST节点类型定义
    return nullptr; // TODO: 创建具体的命名空间声明AST节点
}

NamespaceInfo NamespaceSystem::parseNamespace(const std::vector<Token>& tokens, size_t& position) {
    NamespaceInfo namespace_info;
    
    // 解析命名空间名称
    if (position < tokens.size()) {
        namespace_info.name = tokens[position].value;
        namespace_info.line = tokens[position].line;
        namespace_info.column = tokens[position].column;
        position++;
    }
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 检查是否有花括号（嵌套命名空间）
    if (position < tokens.size() && tokens[position].value == "{") {
        position++; // 跳过开始花括号
        
        // 解析嵌套命名空间
        std::vector<NamespaceInfo> nested = parseNestedNamespaces(tokens, position);
        namespace_info.nested_namespaces = nested;
        
        // 检查结束花括号
        pImpl->skipWhitespace(tokens, position);
        if (position < tokens.size() && tokens[position].value == "}") {
            position++; // 跳过结束花括号
        } else {
            addError("命名空间缺少结束花括号");
        }
    } else {
        // 没有花括号，检查是否有嵌套命名空间（平级关系）
        pImpl->skipWhitespace(tokens, position);
        if (position < tokens.size() && tokens[position].value == "[Namespace]") {
            // 解析平级命名空间
            std::vector<NamespaceInfo> nested = parseNestedNamespaces(tokens, position);
            namespace_info.nested_namespaces = nested;
        }
    }
    
    return namespace_info;
}

std::vector<NamespaceInfo> NamespaceSystem::parseNestedNamespaces(const std::vector<Token>& tokens, size_t& position) {
    std::vector<NamespaceInfo> nested_namespaces;
    
    while (position < tokens.size()) {
        pImpl->skipWhitespace(tokens, position);
        
        // 检查是否到达结束
        if (position < tokens.size() && tokens[position].value == "}") {
            break;
        }
        
        // 检查是否是命名空间声明
        if (position < tokens.size() && tokens[position].value == "[Namespace]") {
            NamespaceInfo nested = parseNamespace(tokens, position);
            nested_namespaces.push_back(nested);
        } else {
            // 跳过其他内容
            position++;
        }
    }
    
    return nested_namespaces;
}

NamespaceItem NamespaceSystem::parseCustomElement(const std::vector<Token>& tokens, size_t& position) {
    NamespaceItem item;
    item.type = NamespaceItemType::CUSTOM_ELEMENT;
    
    // 检查[Custom] @Element
    if (position >= tokens.size() || tokens[position].value != "[Custom]") {
        addError("自定义元素缺少[Custom]");
        return item;
    }
    position++;
    
    pImpl->skipWhitespace(tokens, position);
    
    if (position >= tokens.size() || tokens[position].value != "@Element") {
        addError("自定义元素缺少@Element");
        return item;
    }
    position++;
    
    pImpl->skipWhitespace(tokens, position);
    
    // 解析元素名称
    if (position < tokens.size()) {
        item.name = tokens[position].value;
        item.line = tokens[position].line;
        item.column = tokens[position].column;
        position++;
    }
    
    return item;
}

NamespaceItem NamespaceSystem::parseCustomStyle(const std::vector<Token>& tokens, size_t& position) {
    NamespaceItem item;
    item.type = NamespaceItemType::CUSTOM_STYLE;
    
    // 检查[Custom] @Style
    if (position >= tokens.size() || tokens[position].value != "[Custom]") {
        addError("自定义样式组缺少[Custom]");
        return item;
    }
    position++;
    
    pImpl->skipWhitespace(tokens, position);
    
    if (position >= tokens.size() || tokens[position].value != "@Style") {
        addError("自定义样式组缺少@Style");
        return item;
    }
    position++;
    
    pImpl->skipWhitespace(tokens, position);
    
    // 解析样式组名称
    if (position < tokens.size()) {
        item.name = tokens[position].value;
        item.line = tokens[position].line;
        item.column = tokens[position].column;
        position++;
    }
    
    return item;
}

NamespaceItem NamespaceSystem::parseCustomVar(const std::vector<Token>& tokens, size_t& position) {
    NamespaceItem item;
    item.type = NamespaceItemType::CUSTOM_VAR;
    
    // 检查[Custom] @Var
    if (position >= tokens.size() || tokens[position].value != "[Custom]") {
        addError("自定义变量组缺少[Custom]");
        return item;
    }
    position++;
    
    pImpl->skipWhitespace(tokens, position);
    
    if (position >= tokens.size() || tokens[position].value != "@Var") {
        addError("自定义变量组缺少@Var");
        return item;
    }
    position++;
    
    pImpl->skipWhitespace(tokens, position);
    
    // 解析变量组名称
    if (position < tokens.size()) {
        item.name = tokens[position].value;
        item.line = tokens[position].line;
        item.column = tokens[position].column;
        position++;
    }
    
    return item;
}

NamespaceItem NamespaceSystem::parseTemplateElement(const std::vector<Token>& tokens, size_t& position) {
    NamespaceItem item;
    item.type = NamespaceItemType::TEMPLATE_ELEMENT;
    
    // 检查[Template] @Element
    if (position >= tokens.size() || tokens[position].value != "[Template]") {
        addError("元素模板缺少[Template]");
        return item;
    }
    position++;
    
    pImpl->skipWhitespace(tokens, position);
    
    if (position >= tokens.size() || tokens[position].value != "@Element") {
        addError("元素模板缺少@Element");
        return item;
    }
    position++;
    
    pImpl->skipWhitespace(tokens, position);
    
    // 解析模板名称
    if (position < tokens.size()) {
        item.name = tokens[position].value;
        item.line = tokens[position].line;
        item.column = tokens[position].column;
        position++;
    }
    
    return item;
}

NamespaceItem NamespaceSystem::parseTemplateStyle(const std::vector<Token>& tokens, size_t& position) {
    NamespaceItem item;
    item.type = NamespaceItemType::TEMPLATE_STYLE;
    
    // 检查[Template] @Style
    if (position >= tokens.size() || tokens[position].value != "[Template]") {
        addError("样式组模板缺少[Template]");
        return item;
    }
    position++;
    
    pImpl->skipWhitespace(tokens, position);
    
    if (position >= tokens.size() || tokens[position].value != "@Style") {
        addError("样式组模板缺少@Style");
        return item;
    }
    position++;
    
    pImpl->skipWhitespace(tokens, position);
    
    // 解析模板名称
    if (position < tokens.size()) {
        item.name = tokens[position].value;
        item.line = tokens[position].line;
        item.column = tokens[position].column;
        position++;
    }
    
    return item;
}

NamespaceItem NamespaceSystem::parseTemplateVar(const std::vector<Token>& tokens, size_t& position) {
    NamespaceItem item;
    item.type = NamespaceItemType::TEMPLATE_VAR;
    
    // 检查[Template] @Var
    if (position >= tokens.size() || tokens[position].value != "[Template]") {
        addError("变量组模板缺少[Template]");
        return item;
    }
    position++;
    
    pImpl->skipWhitespace(tokens, position);
    
    if (position >= tokens.size() || tokens[position].value != "@Var") {
        addError("变量组模板缺少@Var");
        return item;
    }
    position++;
    
    pImpl->skipWhitespace(tokens, position);
    
    // 解析模板名称
    if (position < tokens.size()) {
        item.name = tokens[position].value;
        item.line = tokens[position].line;
        item.column = tokens[position].column;
        position++;
    }
    
    return item;
}

void NamespaceSystem::addNamespace(const NamespaceInfo& namespace_info) {
    if (namespace_info.name.empty()) {
        addError("命名空间缺少名称");
        return;
    }
    
    pImpl->namespaces[namespace_info.name] = namespace_info;
    
    if (pImpl->debug_mode) {
        pImpl->logDebug("添加命名空间: " + namespace_info.name);
    }
}

void NamespaceSystem::removeNamespace(const std::string& name) {
    pImpl->namespaces.erase(name);
    
    if (pImpl->debug_mode) {
        pImpl->logDebug("删除命名空间: " + name);
    }
}

NamespaceInfo* NamespaceSystem::getNamespace(const std::string& name) {
    auto it = pImpl->namespaces.find(name);
    if (it != pImpl->namespaces.end()) {
        return &(it->second);
    }
    return nullptr;
}

std::vector<NamespaceInfo> NamespaceSystem::getAllNamespaces() const {
    std::vector<NamespaceInfo> result;
    for (const auto& [name, info] : pImpl->namespaces) {
        result.push_back(info);
    }
    return result;
}

bool NamespaceSystem::hasNamespace(const std::string& name) const {
    return pImpl->namespaces.find(name) != pImpl->namespaces.end();
}

void NamespaceSystem::addNamespaceItem(const std::string& namespace_name, const NamespaceItem& item) {
    auto* namespace_info = getNamespace(namespace_name);
    if (!namespace_info) {
        addError("命名空间不存在: " + namespace_name);
        return;
    }
    
    namespace_info->items.push_back(item);
    
    if (pImpl->debug_mode) {
        pImpl->logDebug("添加命名空间项: " + item.name + " 到命名空间: " + namespace_name);
    }
}

void NamespaceSystem::removeNamespaceItem(const std::string& namespace_name, const std::string& item_name) {
    auto* namespace_info = getNamespace(namespace_name);
    if (!namespace_info) {
        addError("命名空间不存在: " + namespace_name);
        return;
    }
    
    auto it = std::remove_if(namespace_info->items.begin(), namespace_info->items.end(),
        [&item_name](const NamespaceItem& item) { return item.name == item_name; });
    
    namespace_info->items.erase(it, namespace_info->items.end());
    
    if (pImpl->debug_mode) {
        pImpl->logDebug("删除命名空间项: " + item_name + " 从命名空间: " + namespace_name);
    }
}

NamespaceItem* NamespaceSystem::getNamespaceItem(const std::string& namespace_name, const std::string& item_name) {
    auto* namespace_info = getNamespace(namespace_name);
    if (!namespace_info) {
        return nullptr;
    }
    
    for (auto& item : namespace_info->items) {
        if (item.name == item_name) {
            return &item;
        }
    }
    
    return nullptr;
}

std::vector<NamespaceItem> NamespaceSystem::getNamespaceItems(const std::string& namespace_name) const {
    auto it = pImpl->namespaces.find(namespace_name);
    if (it == pImpl->namespaces.end()) {
        return {};
    }
    
    return it->second.items;
}

bool NamespaceSystem::mergeNamespaces(const std::string& target_namespace, const std::string& source_namespace) {
    auto* target = getNamespace(target_namespace);
    auto* source = getNamespace(source_namespace);
    
    if (!target || !source) {
        addError("源或目标命名空间不存在");
        return false;
    }
    
    // 合并项
    for (const auto& item : source->items) {
        target->items.push_back(item);
    }
    
    // 合并嵌套命名空间
    for (const auto& nested : source->nested_namespaces) {
        target->nested_namespaces.push_back(nested);
    }
    
    if (pImpl->debug_mode) {
        pImpl->logDebug("合并命名空间: " + source_namespace + " 到 " + target_namespace);
    }
    
    return true;
}

bool NamespaceSystem::mergeNamespaceItems(const std::string& target_namespace, const std::vector<NamespaceItem>& items) {
    auto* target = getNamespace(target_namespace);
    if (!target) {
        addError("目标命名空间不存在: " + target_namespace);
        return false;
    }
    
    for (const auto& item : items) {
        target->items.push_back(item);
    }
    
    if (pImpl->debug_mode) {
        pImpl->logDebug("合并命名空间项到命名空间: " + target_namespace);
    }
    
    return true;
}

std::vector<NamespaceConflict> NamespaceSystem::detectConflicts(const std::string& namespace_name) {
    std::vector<NamespaceConflict> conflicts;
    
    auto* namespace_info = getNamespace(namespace_name);
    if (!namespace_info) {
        return conflicts;
    }
    
    // 检查名称冲突
    for (const auto& [other_name, other_info] : pImpl->namespaces) {
        if (other_name == namespace_name) {
            continue;
        }
        
        // 检查项名称冲突
        for (const auto& item : namespace_info->items) {
            for (const auto& other_item : other_info.items) {
                if (item.name == other_item.name) {
                    NamespaceConflict conflict;
                    conflict.namespace1 = namespace_name;
                    conflict.namespace2 = other_name;
                    conflict.conflicting_item = item.name;
                    conflict.conflict_type = NamespaceConflictType::NAME_COLLISION;
                    conflict.line = item.line;
                    conflict.column = item.column;
                    conflicts.push_back(conflict);
                }
            }
        }
    }
    
    return conflicts;
}

std::vector<NamespaceConflict> NamespaceSystem::detectAllConflicts() {
    std::vector<NamespaceConflict> all_conflicts;
    
    for (const auto& [name, info] : pImpl->namespaces) {
        auto conflicts = detectConflicts(name);
        all_conflicts.insert(all_conflicts.end(), conflicts.begin(), conflicts.end());
    }
    
    return all_conflicts;
}

bool NamespaceSystem::hasConflicts(const std::string& namespace_name) {
    return !detectConflicts(namespace_name).empty();
}

bool NamespaceSystem::validateNamespace(const NamespaceInfo& namespace_info) {
    // 验证命名空间名称
    if (!pImpl->isValidNamespaceName(namespace_info.name)) {
        addError("无效的命名空间名称: " + namespace_info.name);
        return false;
    }
    
    // 验证嵌套深度
    if (!pImpl->isValidNestingDepth(namespace_info.nested_namespaces.size())) {
        addError("命名空间嵌套深度超出限制");
        return false;
    }
    
    // 验证命名空间结构
    if (!pImpl->hasValidNamespaceStructure(namespace_info)) {
        addError("命名空间结构无效");
        return false;
    }
    
    return true;
}

std::vector<std::string> NamespaceSystem::validateNamespaceList(const std::vector<NamespaceInfo>& namespaces) {
    std::vector<std::string> errors;
    
    for (const auto& namespace_info : namespaces) {
        if (!validateNamespace(namespace_info)) {
            auto namespace_errors = getErrors();
            errors.insert(errors.end(), namespace_errors.begin(), namespace_errors.end());
        }
    }
    
    return errors;
}

bool NamespaceSystem::validateNamespaceName(const std::string& name) {
    return pImpl->isValidNamespaceName(name);
}

std::string NamespaceSystem::resolveItemNamespace(const std::string& item_name, const std::string& target_namespace) {
    // 在目标命名空间中查找项
    for (const auto& [name, info] : pImpl->namespaces) {
        if (name == target_namespace) {
            for (const auto& item : info.items) {
                if (item.name == item_name) {
                    return name;
                }
            }
            
            // 检查嵌套命名空间
            for (const auto& nested : info.nested_namespaces) {
                for (const auto& item : nested.items) {
                    if (item.name == item_name) {
                        return nested.name;
                    }
                }
            }
        }
    }
    
    return "";
}

std::vector<std::string> NamespaceSystem::getAvailableNamespaces() {
    std::vector<std::string> result;
    for (const auto& [name, info] : pImpl->namespaces) {
        result.push_back(name);
    }
    return result;
}

std::vector<std::string> NamespaceSystem::getNamespaceHierarchy(const std::string& namespace_name) {
    std::vector<std::string> hierarchy;
    
    auto* namespace_info = getNamespace(namespace_name);
    if (!namespace_info) {
        return hierarchy;
    }
    
    hierarchy.push_back(namespace_name);
    
    // 添加嵌套命名空间
    for (const auto& nested : namespace_info->nested_namespaces) {
        hierarchy.push_back(nested.name);
    }
    
    return hierarchy;
}

void NamespaceSystem::setConfig(const NamespaceSystemConfig& config) {
    pImpl->config = config;
}

NamespaceSystemConfig NamespaceSystem::getConfig() const {
    return pImpl->config;
}

bool NamespaceSystem::hasErrors() const {
    return !pImpl->errors.empty();
}

std::vector<std::string> NamespaceSystem::getErrors() const {
    return pImpl->errors;
}

void NamespaceSystem::clearErrors() {
    pImpl->errors.clear();
}

void NamespaceSystem::addError(const std::string& error_message) {
    pImpl->errors.push_back(error_message);
    
    if (pImpl->debug_mode) {
        std::cerr << "命名空间系统错误: " << error_message << std::endl;
    }
}

std::string NamespaceSystem::getDebugInfo() const {
    std::ostringstream oss;
    oss << "命名空间系统调试信息:\n";
    oss << "命名空间总数: " << pImpl->namespaces.size() << "\n";
    oss << "启用命名空间: " << (pImpl->config.enable_namespaces ? "是" : "否") << "\n";
    oss << "启用嵌套: " << (pImpl->config.enable_nesting ? "是" : "否") << "\n";
    oss << "严格冲突检测: " << (pImpl->config.strict_conflict_detection ? "是" : "否") << "\n";
    oss << "自动解决冲突: " << (pImpl->config.auto_resolve_conflicts ? "是" : "否") << "\n";
    oss << "保留文件命名空间: " << (pImpl->config.preserve_file_namespace ? "是" : "否") << "\n";
    oss << "最大嵌套深度: " << pImpl->config.max_nesting_depth << "\n";
    oss << "错误数量: " << pImpl->errors.size() << "\n";
    
    for (const auto& [name, info] : pImpl->namespaces) {
        oss << "  命名空间: " << name << " (项数: " << info.items.size() 
            << ", 嵌套: " << info.nested_namespaces.size() << ")\n";
    }
    
    return oss.str();
}

void NamespaceSystem::enableDebugMode(bool enable) {
    pImpl->debug_mode = enable;
}

void NamespaceSystem::setStateMachine(std::shared_ptr<CHTLStateMachine> sm) {
    state_machine = sm;
}

// Impl类方法实现
bool NamespaceSystem::Impl::isNamespaceDeclaration(const std::vector<Token>& tokens, size_t position) {
    if (position >= tokens.size()) {
        return false;
    }
    
    const Token& token = tokens[position];
    return token.value == "[Namespace]";
}

void NamespaceSystem::Impl::skipWhitespace(const std::vector<Token>& tokens, size_t& position) {
    while (position < tokens.size() && 
           (tokens[position].value == " " || tokens[position].value == "\t" || 
            tokens[position].value == "\n" || tokens[position].value == "\r")) {
        position++;
    }
}

bool NamespaceSystem::Impl::isEndOfTokens(const std::vector<Token>& tokens, size_t position) {
    return position >= tokens.size();
}

bool NamespaceSystem::Impl::isValidNamespaceName(const std::string& name) {
    if (name.empty()) {
        return false;
    }
    
    // 检查是否只包含字母、数字、下划线
    for (char c : name) {
        if (!std::isalnum(c) && c != '_') {
            return false;
        }
    }
    
    return true;
}

bool NamespaceSystem::Impl::isValidNestingDepth(size_t depth) {
    return depth <= config.max_nesting_depth;
}

bool NamespaceSystem::Impl::hasValidNamespaceStructure(const NamespaceInfo& namespace_info) {
    // 检查命名空间名称
    if (!isValidNamespaceName(namespace_info.name)) {
        return false;
    }
    
    // 检查嵌套深度
    if (!isValidNestingDepth(namespace_info.nested_namespaces.size())) {
        return false;
    }
    
    // 检查嵌套命名空间结构
    for (const auto& nested : namespace_info.nested_namespaces) {
        if (!hasValidNamespaceStructure(nested)) {
            return false;
        }
    }
    
    return true;
}

std::string NamespaceSystem::Impl::normalizeNamespaceName(const std::string& name) {
    std::string normalized = name;
    
    // 转换为小写
    std::transform(normalized.begin(), normalized.end(), normalized.begin(), ::tolower);
    
    // 替换无效字符
    std::replace(normalized.begin(), normalized.end(), ' ', '_');
    std::replace(normalized.begin(), normalized.end(), '-', '_');
    
    return normalized;
}

std::string NamespaceSystem::Impl::createDefaultNamespaceName(const std::string& file_name) {
    if (file_name.empty()) {
        return "default";
    }
    
    // 提取文件名（不含扩展名）
    size_t last_dot = file_name.find_last_of('.');
    std::string name = (last_dot != std::string::npos) ? file_name.substr(0, last_dot) : file_name;
    
    return normalizeNamespaceName(name);
}

bool NamespaceSystem::Impl::isReservedNamespaceName(const std::string& name) {
    static const std::unordered_set<std::string> reserved_names = {
        "chtl", "system", "global", "root", "main", "default"
    };
    
    return reserved_names.find(name) != reserved_names.end();
}

std::string NamespaceSystem::Impl::extractNamespaceContent(const std::vector<Token>& tokens, size_t& position) {
    std::string content;
    
    while (position < tokens.size()) {
        content += tokens[position].value;
        position++;
    }
    
    return content;
}

std::string NamespaceSystem::Impl::parseNamespaceIdentifier(const std::vector<Token>& tokens, size_t& position) {
    std::string identifier;
    
    if (position < tokens.size() && tokens[position].type == TokenType::IDENTIFIER) {
        identifier = tokens[position].value;
        position++;
    }
    
    return identifier;
}

void NamespaceSystem::Impl::logDebug(const std::string& message) {
    if (debug_mode) {
        std::cout << "[DEBUG] " << message << std::endl;
    }
}

} // namespace chtl