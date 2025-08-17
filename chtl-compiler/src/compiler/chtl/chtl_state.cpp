#include "compiler/chtl/chtl_state.h"
#include <algorithm>

namespace chtl {

// GlobalMap实现

void GlobalMap::addTemplate(const std::string& name, std::unique_ptr<TemplateDefinition> def) {
    if (!def) return;
    
    std::string full_name = resolveNamespace(name);
    def->namespace_path = getCurrentNamespace();
    
    switch (def->type) {
        case TemplateType::STYLE:
            style_templates_[full_name] = std::move(def);
            break;
        case TemplateType::ELEMENT:
            element_templates_[full_name] = std::move(def);
            break;
        case TemplateType::VAR:
            var_templates_[full_name] = std::move(def);
            break;
    }
}

TemplateDefinition* GlobalMap::getTemplate(const std::string& name, TemplateType type) const {
    std::string full_name = resolveNamespace(name);
    
    switch (type) {
        case TemplateType::STYLE:
            if (auto it = style_templates_.find(full_name); it != style_templates_.end()) {
                return it->second.get();
            }
            break;
        case TemplateType::ELEMENT:
            if (auto it = element_templates_.find(full_name); it != element_templates_.end()) {
                return it->second.get();
            }
            break;
        case TemplateType::VAR:
            if (auto it = var_templates_.find(full_name); it != var_templates_.end()) {
                return it->second.get();
            }
            break;
    }
    
    return nullptr;
}

std::vector<std::string> GlobalMap::getTemplatesInNamespace(const std::string& ns_path, TemplateType type) const {
    std::vector<std::string> result;
    
    const auto& templates = (type == TemplateType::STYLE) ? style_templates_ :
                           (type == TemplateType::ELEMENT) ? element_templates_ :
                           var_templates_;
    
    for (const auto& [name, def] : templates) {
        if (def->namespace_path == ns_path) {
            result.push_back(name);
        }
    }
    
    return result;
}

void GlobalMap::addCustom(const std::string& name, std::unique_ptr<CustomDefinition> def) {
    if (!def) return;
    
    std::string full_name = resolveNamespace(name);
    def->namespace_path = getCurrentNamespace();
    
    switch (def->type) {
        case CustomType::STYLE:
            custom_styles_[full_name] = std::move(def);
            break;
        case CustomType::ELEMENT:
            custom_elements_[full_name] = std::move(def);
            break;
        case CustomType::VAR:
            custom_vars_[full_name] = std::move(def);
            break;
    }
}

CustomDefinition* GlobalMap::getCustom(const std::string& name, CustomType type) const {
    std::string full_name = resolveNamespace(name);
    
    switch (type) {
        case CustomType::STYLE:
            if (auto it = custom_styles_.find(full_name); it != custom_styles_.end()) {
                return it->second.get();
            }
            break;
        case CustomType::ELEMENT:
            if (auto it = custom_elements_.find(full_name); it != custom_elements_.end()) {
                return it->second.get();
            }
            break;
        case CustomType::VAR:
            if (auto it = custom_vars_.find(full_name); it != custom_vars_.end()) {
                return it->second.get();
            }
            break;
    }
    
    return nullptr;
}

std::vector<std::string> GlobalMap::getCustomsInNamespace(const std::string& ns_path, CustomType type) const {
    std::vector<std::string> result;
    
    const auto& customs = (type == CustomType::STYLE) ? custom_styles_ :
                         (type == CustomType::ELEMENT) ? custom_elements_ :
                         custom_vars_;
    
    for (const auto& [name, def] : customs) {
        if (def->namespace_path == ns_path) {
            result.push_back(name);
        }
    }
    
    return result;
}

void GlobalMap::addOrigin(const std::string& key, std::unique_ptr<OriginDefinition> def) {
    if (!def) return;
    
    def->namespace_path = getCurrentNamespace();
    origins_[key] = std::move(def);
}

OriginDefinition* GlobalMap::getOrigin(const std::string& key) const {
    if (auto it = origins_.find(key); it != origins_.end()) {
        return it->second.get();
    }
    return nullptr;
}

void GlobalMap::enterNamespace(const std::string& name) {
    std::string full_path = getCurrentNamespace();
    if (!full_path.empty()) {
        full_path += ".";
    }
    full_path += name;
    
    namespace_stack_.push_back(name);
    
    // 创建命名空间定义
    if (namespaces_.find(full_path) == namespaces_.end()) {
        NamespaceDefinition ns_def;
        ns_def.name = name;
        ns_def.parent_path = getCurrentNamespace();
        namespaces_[full_path] = ns_def;
        
        // 更新父命名空间的子命名空间列表
        if (!ns_def.parent_path.empty()) {
            if (auto it = namespaces_.find(ns_def.parent_path); it != namespaces_.end()) {
                it->second.children.push_back(name);
            }
        }
    }
}

void GlobalMap::exitNamespace() {
    if (!namespace_stack_.empty()) {
        namespace_stack_.pop_back();
    }
}

std::string GlobalMap::getCurrentNamespace() const {
    std::string result;
    for (const auto& ns : namespace_stack_) {
        if (!result.empty()) {
            result += ".";
        }
        result += ns;
    }
    return result;
}

std::string GlobalMap::resolveNamespace(const std::string& name) const {
    // 如果名称已经包含命名空间路径，直接返回
    if (name.find('.') != std::string::npos) {
        return name;
    }
    
    // 否则添加当前命名空间前缀
    std::string current_ns = getCurrentNamespace();
    if (!current_ns.empty()) {
        return current_ns + "." + name;
    }
    
    return name;
}

// CompilerState实现

CompilerState::CompilerState(GlobalMap& global_map) : global_map_(global_map) {}

void CompilerState::addError(const std::string& msg, size_t line, size_t column) {
    CompilerError error;
    error.message = msg;
    error.line = static_cast<int>(line);
    error.column = static_cast<int>(column);
    error.file = ""; // 当前文件名应该从某处获取
    errors_.push_back(error);
}

void CompilerState::addWarning(const std::string& msg, size_t line, size_t column) {
    CompilerError warning;
    warning.message = msg;
    warning.line = static_cast<int>(line);
    warning.column = static_cast<int>(column);
    warning.file = "";
    warnings_.push_back(warning);
}

void CompilerState::pushLocalStyleContext() {
    LocalStyleContext context;
    context.element_class = generateClassName();
    local_style_stack_.push(context);
}

void CompilerState::popLocalStyleContext() {
    if (!local_style_stack_.empty()) {
        // 将局部样式转换为全局样式
        const auto& context = local_style_stack_.top();
        
        if (!context.styles.empty()) {
            std::string global_style;
            for (const auto& [selector, style] : context.styles) {
                global_style += selector + " {\n" + style + "\n}\n";
            }
            addGlobalStyle(global_style);
        }
        
        local_style_stack_.pop();
    }
}

void CompilerState::addLocalStyle(const std::string& selector, const std::string& style) {
    if (!local_style_stack_.empty()) {
        auto& context = local_style_stack_.top();
        
        // 处理&符号
        std::string processed_selector = selector;
        if (processed_selector.find('&') != std::string::npos) {
            // 替换&为当前元素的类名
            size_t pos = 0;
            while ((pos = processed_selector.find('&', pos)) != std::string::npos) {
                processed_selector.replace(pos, 1, "." + context.element_class);
                pos += context.element_class.length() + 1;
            }
        }
        
        context.styles.push_back({processed_selector, style});
    }
}

std::string CompilerState::generateClassName() {
    return "chtl-auto-" + std::to_string(++class_counter_);
}

void CompilerState::addGlobalStyle(const std::string& style) {
    global_styles_.push_back(style);
}

void CompilerState::addGlobalScript(const std::string& script) {
    global_scripts_.push_back(script);
}

void CompilerState::pushElementContext(const std::string& tag_name) {
    element_stack_.push_back(tag_name);
}

void CompilerState::popElementContext() {
    if (!element_stack_.empty()) {
        element_stack_.pop_back();
    }
}

std::string CompilerState::getCurrentElement() const {
    return element_stack_.empty() ? "" : element_stack_.back();
}

bool CompilerState::isAllowed(const std::string& item) const {
    // 检查当前约束规则
    for (const auto& rule : except_rules_) {
        if (rule == item) {
            return false; // 精确匹配
        }
        
        // 类型约束检查
        if (rule == "@Html" && item.find('<') != std::string::npos) {
            return false; // 禁止HTML元素
        }
        
        if (rule == "[Custom]" && item.find("[Custom]") != std::string::npos) {
            return false; // 禁止自定义类型
        }
        
        if (rule == "[Template]" && item.find("[Template]") != std::string::npos) {
            return false; // 禁止模板类型
        }
        
        // 检查特定类型的模板或自定义
        if (rule.find("[Template] @") != std::string::npos ||
            rule.find("[Custom] @") != std::string::npos) {
            if (item == rule) {
                return false;
            }
        }
    }
    
    return true;
}

void CompilerState::addExceptRule(const std::string& rule) {
    except_rules_.push_back(rule);
}

void CompilerState::clearExceptRules() {
    except_rules_.clear();
}

} // namespace chtl