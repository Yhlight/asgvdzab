#include "compiler/chtl/chtl_global_map.h"

namespace chtl {
namespace compiler {

CHTLGlobalMap::CHTLGlobalMap() {
}

CHTLGlobalMap::~CHTLGlobalMap() {
}

// 样式组模板管理
void CHTLGlobalMap::addStyleTemplate(const std::string& name, const StyleTemplate& tmpl) {
    styleTemplates_[name] = tmpl;
}

StyleTemplate* CHTLGlobalMap::getStyleTemplate(const std::string& name) {
    auto it = styleTemplates_.find(name);
    return (it != styleTemplates_.end()) ? &it->second : nullptr;
}

bool CHTLGlobalMap::hasStyleTemplate(const std::string& name) const {
    return styleTemplates_.find(name) != styleTemplates_.end();
}

// 元素模板管理
void CHTLGlobalMap::addElementTemplate(const std::string& name, const ElementTemplate& tmpl) {
    elementTemplates_[name] = tmpl;
}

ElementTemplate* CHTLGlobalMap::getElementTemplate(const std::string& name) {
    auto it = elementTemplates_.find(name);
    return (it != elementTemplates_.end()) ? &it->second : nullptr;
}

bool CHTLGlobalMap::hasElementTemplate(const std::string& name) const {
    return elementTemplates_.find(name) != elementTemplates_.end();
}

// 变量组模板管理
void CHTLGlobalMap::addVarTemplate(const std::string& name, const VarTemplate& tmpl) {
    varTemplates_[name] = tmpl;
}

VarTemplate* CHTLGlobalMap::getVarTemplate(const std::string& name) {
    auto it = varTemplates_.find(name);
    return (it != varTemplates_.end()) ? &it->second : nullptr;
}

bool CHTLGlobalMap::hasVarTemplate(const std::string& name) const {
    return varTemplates_.find(name) != varTemplates_.end();
}

std::string CHTLGlobalMap::resolveVarTemplateValue(const std::string& templateName, 
                                                   const std::string& varName) {
    auto tmpl = getVarTemplate(templateName);
    if (tmpl) {
        auto it = tmpl->variables.find(varName);
        if (it != tmpl->variables.end()) {
            return it->second;
        }
    }
    return "";
}

// 自定义管理
void CHTLGlobalMap::addCustomDefinition(const std::string& name, const CustomDefinition& custom) {
    customDefinitions_[name] = custom;
}

CustomDefinition* CHTLGlobalMap::getCustomDefinition(const std::string& name) {
    auto it = customDefinitions_.find(name);
    return (it != customDefinitions_.end()) ? &it->second : nullptr;
}

bool CHTLGlobalMap::hasCustomDefinition(const std::string& name) const {
    return customDefinitions_.find(name) != customDefinitions_.end();
}

// 配置组管理
void CHTLGlobalMap::addConfigurationGroup(const std::string& name, const ConfigurationGroup& config) {
    configurations_[name] = config;
}

ConfigurationGroup* CHTLGlobalMap::getConfigurationGroup(const std::string& name) {
    auto it = configurations_.find(name);
    return (it != configurations_.end()) ? &it->second : nullptr;
}

bool CHTLGlobalMap::hasConfigurationGroup(const std::string& name) const {
    return configurations_.find(name) != configurations_.end();
}

std::string CHTLGlobalMap::getConfigValue(const std::string& groupName, const std::string& key) {
    auto config = getConfigurationGroup(groupName);
    if (config) {
        auto it = config->settings.find(key);
        if (it != config->settings.end()) {
            return it->second;
        }
    }
    return "";
}

// 导入管理
void CHTLGlobalMap::addImport(const ImportInfo& import) {
    imports_.push_back(import);
}

// 命名空间管理
void CHTLGlobalMap::addNamespace(const std::string& name, const NamespaceInfo& ns) {
    namespaces_[name] = ns;
}

NamespaceInfo* CHTLGlobalMap::getNamespace(const std::string& name) {
    auto it = namespaces_.find(name);
    return (it != namespaces_.end()) ? &it->second : nullptr;
}

bool CHTLGlobalMap::hasNamespace(const std::string& name) const {
    return namespaces_.find(name) != namespaces_.end();
}

// 全局样式和脚本收集
void CHTLGlobalMap::addGlobalStyle(const std::string& style) {
    globalStyles_.push_back(style);
}

void CHTLGlobalMap::addGlobalScript(const std::string& script) {
    globalScripts_.push_back(script);
}

// 清空所有数据
void CHTLGlobalMap::clear() {
    styleTemplates_.clear();
    elementTemplates_.clear();
    varTemplates_.clear();
    customDefinitions_.clear();
    configurations_.clear();
    imports_.clear();
    namespaces_.clear();
    globalStyles_.clear();
    globalScripts_.clear();
}

} // namespace compiler
} // namespace chtl