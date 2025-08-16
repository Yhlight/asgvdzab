#include "parser/CustomSystem.h"
#include "ast/ASTNode.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cctype>
#include <regex>

namespace chtl {

// 实现类
class CustomSystem::Impl {
public:
    CustomSystemConfig config;
    std::unordered_map<std::string, CustomInfo> customs;
    std::vector<std::string> errors;
    bool debug_mode;
    
    // 基础模板引用
    std::shared_ptr<TemplateSystem> template_system;
    
    Impl(const CustomSystemConfig& cfg) : config(cfg), debug_mode(false) {}
    
    ~Impl() = default;
    
    // 内部方法实现
    bool isCustomDeclaration(const std::vector<Token>& tokens, size_t position);
    bool isCustomUsage(const std::vector<Token>& tokens, size_t position);
    CustomType detectCustomType(const std::vector<Token>& tokens, size_t position);
    void skipWhitespace(const std::vector<Token>& tokens, size_t& position);
    bool isEndOfTokens(const std::vector<Token>& tokens, size_t position);
    
    // 自定义验证辅助方法
    bool isValidCustomName(const std::string& name);
    bool isValidCustomContent(const std::string& content);
    bool hasValidBaseTemplate(const std::string& base_template);
    
    // 特例化应用辅助方法
    std::string parseCSSProperties(const std::string& content);
    std::vector<std::string> extractPropertyNames(const std::string& content);
    std::string removeProperty(const std::string& content, const std::string& property);
    std::string insertElementAtPosition(const std::string& content, const std::string& element, InsertPosition pos, const std::string& target);
    
    // 辅助方法
    void logDebug(const std::string& message);
    std::string extractCustomContent(const std::vector<Token>& tokens, size_t& position);
    std::vector<std::string> parseCommaSeparatedList(const std::vector<Token>& tokens, size_t& position);
};

// CustomSystem实现
CustomSystem::CustomSystem(const CustomSystemConfig& config) 
    : pImpl(std::make_unique<Impl>(config)), state_machine(nullptr) {}

CustomSystem::~CustomSystem() = default;

std::shared_ptr<ASTNode> CustomSystem::parseCustomDeclaration(const std::vector<Token>& tokens, size_t& position) {
    if (!pImpl->isCustomDeclaration(tokens, position)) {
        addError("不是自定义声明");
        return nullptr;
    }
    
    // 跳过[Custom]
    position++;
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 检测自定义类型
    CustomType custom_type = pImpl->detectCustomType(tokens, position);
    
    // 根据类型解析自定义
    CustomInfo custom_info;
    switch (custom_type) {
        case CustomType::STYLE_CUSTOM:
            custom_info = parseStyleCustom(tokens, position);
            break;
        case CustomType::ELEMENT_CUSTOM:
            custom_info = parseElementCustom(tokens, position);
            break;
        case CustomType::VAR_CUSTOM:
            custom_info = parseVarCustom(tokens, position);
            break;
        default:
            addError("未知的自定义类型");
            return nullptr;
    }
    
    // 验证自定义
    if (!validateCustom(custom_info)) {
        return nullptr;
    }
    
    // 添加自定义到系统
    addCustom(custom_info);
    
    if (pImpl->debug_mode) {
        pImpl->logDebug("自定义声明解析完成: " + custom_info.name + " (类型: " + std::to_string(static_cast<int>(custom_info.type)) + ")");
    }
    
    // 创建自定义声明节点（这里需要创建具体的AST节点类型）
    // 暂时返回nullptr，等待具体的AST节点类型定义
    return nullptr; // TODO: 创建具体的自定义声明AST节点
}

CustomInfo CustomSystem::parseStyleCustom(const std::vector<Token>& tokens, size_t& position) {
    CustomInfo custom_info;
    custom_info.type = CustomType::STYLE_CUSTOM;
    
    // 跳过@Style
    if (position < tokens.size() && tokens[position].value == "@Style") {
        position++;
    } else {
        addError("自定义样式组缺少@Style关键字");
        return custom_info;
    }
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 解析自定义名称
    if (position < tokens.size() && tokens[position].type == TokenType::IDENTIFIER) {
        custom_info.name = tokens[position].value;
        custom_info.line = tokens[position].line;
        custom_info.column = tokens[position].column;
        position++;
    } else {
        addError("自定义样式组缺少名称");
        return custom_info;
    }
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 检查左大括号
    if (position >= tokens.size() || tokens[position].value != "{") {
        addError("自定义样式组缺少左大括号");
        return custom_info;
    }
    position++;
    
    // 解析自定义内容
    custom_info.custom_content = pImpl->extractCustomContent(tokens, position);
    
    // 检查右大括号
    if (position >= tokens.size() || tokens[position].value != "}") {
        addError("自定义样式组缺少右大括号");
        return custom_info;
    }
    position++;
    
    return custom_info;
}

CustomInfo CustomSystem::parseElementCustom(const std::vector<Token>& tokens, size_t& position) {
    CustomInfo custom_info;
    custom_info.type = CustomType::ELEMENT_CUSTOM;
    
    // 跳过@Element
    if (position < tokens.size() && tokens[position].value == "@Element") {
        position++;
    } else {
        addError("自定义元素缺少@Element关键字");
        return custom_info;
    }
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 解析自定义名称
    if (position < tokens.size() && tokens[position].type == TokenType::IDENTIFIER) {
        custom_info.name = tokens[position].value;
        custom_info.line = tokens[position].line;
        custom_info.column = tokens[position].column;
        position++;
    } else {
        addError("自定义元素缺少名称");
        return custom_info;
    }
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 检查左大括号
    if (position >= tokens.size() || tokens[position].value != "{") {
        addError("自定义元素缺少左大括号");
        return custom_info;
    }
    position++;
    
    // 解析自定义内容
    custom_info.custom_content = pImpl->extractCustomContent(tokens, position);
    
    // 检查右大括号
    if (position >= tokens.size() || tokens[position].value != "}") {
        addError("自定义元素缺少右大括号");
        return custom_info;
    }
    position++;
    
    return custom_info;
}

CustomInfo CustomSystem::parseVarCustom(const std::vector<Token>& tokens, size_t& position) {
    CustomInfo custom_info;
    custom_info.type = CustomType::VAR_CUSTOM;
    
    // 跳过@Var
    if (position < tokens.size() && tokens[position].value == "@Var") {
        position++;
    } else {
        addError("自定义变量组缺少@Var关键字");
        return custom_info;
    }
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 解析自定义名称
    if (position < tokens.size() && tokens[position].type == TokenType::IDENTIFIER) {
        custom_info.name = tokens[position].value;
        custom_info.line = tokens[position].line;
        custom_info.column = tokens[position].column;
        position++;
    } else {
        addError("自定义变量组缺少名称");
        return custom_info;
    }
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 检查左大括号
    if (position >= tokens.size() || tokens[position].value != "{") {
        addError("自定义变量组缺少左大括号");
        return custom_info;
    }
    position++;
    
    // 解析自定义内容
    custom_info.custom_content = pImpl->extractCustomContent(tokens, position);
    
    // 检查右大括号
    if (position >= tokens.size() || tokens[position].value != "}") {
        addError("自定义变量组缺少右大括号");
        return custom_info;
    }
    position++;
    
    return custom_info;
}

std::vector<SpecializationInfo> CustomSystem::parseSpecializations(const std::vector<Token>& tokens, size_t& position) {
    std::vector<SpecializationInfo> specializations;
    
    while (position < tokens.size()) {
        pImpl->skipWhitespace(tokens, position);
        
        if (position >= tokens.size()) {
            break;
        }
        
        // 检查是否是特例化操作
        if (tokens[position].value == "delete") {
            position++;
            pImpl->skipWhitespace(tokens, position);
            
            // 检查是否是删除继承
            if (position < tokens.size() && tokens[position].value == "@Style") {
                specializations.push_back(parseDeleteInheritance(tokens, position));
            } else {
                specializations.push_back(parseDeleteProperty(tokens, position));
            }
        } else if (tokens[position].value == "insert") {
            specializations.push_back(parseInsertElement(tokens, position));
        } else if (tokens[position].value == "modify") {
            specializations.push_back(parseModifyValue(tokens, position));
        } else {
            // 不是特例化操作，退出
            break;
        }
    }
    
    return specializations;
}

SpecializationInfo CustomSystem::parseDeleteProperty(const std::vector<Token>& tokens, size_t& position) {
    SpecializationInfo spec;
    spec.type = SpecializationType::DELETE_PROPERTY;
    spec.line = tokens[position].line;
    spec.column = tokens[position].column;
    
    // 解析要删除的属性列表
    auto properties = pImpl->parseCommaSeparatedList(tokens, position);
    if (!properties.empty()) {
        spec.target = properties[0]; // 第一个属性作为主要目标
                    // 其他属性可以存储在metadata中
            for (size_t i = 1; i < properties.size(); i++) {
                // 暂时跳过metadata存储，因为SpecializationInfo结构体中没有metadata成员
            }
    }
    
    return spec;
}

SpecializationInfo CustomSystem::parseDeleteInheritance(const std::vector<Token>& tokens, size_t& position) {
    SpecializationInfo spec;
    spec.type = SpecializationType::DELETE_INHERITANCE;
    spec.line = tokens[position].line;
    spec.column = tokens[position].column;
    
    // 跳过@Style
    if (position < tokens.size() && tokens[position].value == "@Style") {
        position++;
    }
    
    pImpl->skipWhitespace(tokens, position);
    
    // 解析要删除的继承模板名称
    if (position < tokens.size() && tokens[position].type == TokenType::IDENTIFIER) {
        spec.target = tokens[position].value;
        position++;
    }
    
    return spec;
}

SpecializationInfo CustomSystem::parseInsertElement(const std::vector<Token>& tokens, size_t& position) {
    SpecializationInfo spec;
    spec.type = SpecializationType::INSERT_ELEMENT;
    spec.line = tokens[position].line;
    spec.column = tokens[position].column;
    
    // 跳过insert关键字
    position++;
    pImpl->skipWhitespace(tokens, position);
    
    // 解析插入位置
    std::string pos_str = parseInsertPosition(tokens, position);
    if (pos_str == "after") {
        spec.insert_pos = InsertPosition::AFTER;
    } else if (pos_str == "before") {
        spec.insert_pos = InsertPosition::BEFORE;
    } else if (pos_str == "replace") {
        spec.insert_pos = InsertPosition::REPLACE;
    } else if (pos_str == "at top") {
        spec.insert_pos = InsertPosition::AT_TOP;
    } else if (pos_str == "at bottom") {
        spec.insert_pos = InsertPosition::AT_BOTTOM;
    } else {
        spec.insert_pos = InsertPosition::AFTER; // 默认
    }
    
    pImpl->skipWhitespace(tokens, position);
    
    // 解析目标元素（如果有）
    if (position < tokens.size() && tokens[position].type == TokenType::IDENTIFIER) {
        spec.target = tokens[position].value;
        position++;
        
        // 检查是否有索引
        if (position < tokens.size() && tokens[position].value == "[") {
            position++;
            spec.target += "[" + parseIndexSelector(tokens, position) + "]";
            if (position < tokens.size() && tokens[position].value == "]") {
                position++;
            }
        }
    }
    
    pImpl->skipWhitespace(tokens, position);
    
    // 检查左大括号
    if (position < tokens.size() && tokens[position].value == "{") {
        position++;
        spec.insert_content = pImpl->extractCustomContent(tokens, position);
    }
    
    return spec;
}

SpecializationInfo CustomSystem::parseModifyValue(const std::vector<Token>& tokens, size_t& position) {
    SpecializationInfo spec;
    spec.type = SpecializationType::MODIFY_VALUE;
    spec.line = tokens[position].line;
    spec.column = tokens[position].column;
    
    // 跳过modify关键字
    position++;
    pImpl->skipWhitespace(tokens, position);
    
    // 解析属性名
    if (position < tokens.size() && tokens[position].type == TokenType::IDENTIFIER) {
        spec.target = tokens[position].value;
        position++;
    }
    
    pImpl->skipWhitespace(tokens, position);
    
    // 检查等号
    if (position < tokens.size() && tokens[position].value == "=") {
        position++;
        pImpl->skipWhitespace(tokens, position);
        
        // 解析新值
        while (position < tokens.size() && tokens[position].value != ";" && tokens[position].value != ",") {
            spec.value += tokens[position].value;
            position++;
        }
        
        // 移除首尾空白字符
        spec.value.erase(0, spec.value.find_first_not_of(" \t\n\r"));
        spec.value.erase(spec.value.find_last_not_of(" \t\n\r") + 1);
    }
    
    return spec;
}

SpecializationInfo CustomSystem::parseIndexAccess(const std::vector<Token>& tokens, size_t& position) {
    SpecializationInfo spec;
    spec.type = SpecializationType::INDEX_ACCESS;
    spec.line = tokens[position].line;
    spec.column = tokens[position].column;
    
    // 检查左方括号
    if (position < tokens.size() && tokens[position].value == "[") {
        position++;
        spec.target = parseIndexSelector(tokens, position);
        
        if (position < tokens.size() && tokens[position].value == "]") {
            position++;
        }
    }
    
    return spec;
}

std::shared_ptr<ASTNode> CustomSystem::parseCustomUsage(const std::vector<Token>& tokens, size_t& position) {
    if (!pImpl->isCustomUsage(tokens, position)) {
        addError("不是自定义使用");
        return nullptr;
    }
    
    // 解析自定义名称
    std::string custom_name;
    if (position < tokens.size() && tokens[position].type == TokenType::IDENTIFIER) {
        custom_name = tokens[position].value;
        position++;
    } else {
        addError("自定义使用缺少名称");
        return nullptr;
    }
    
    // 检查自定义是否存在
    if (!hasCustom(custom_name)) {
        addError("自定义不存在: " + custom_name);
        return nullptr;
    }
    
    // 检查是否有索引访问
    if (position < tokens.size() && tokens[position].value == "[") {
        position++;
        std::string index = parseIndexSelector(tokens, position);
        custom_name += "[" + index + "]";
        
        if (position < tokens.size() && tokens[position].value == "]") {
            position++;
        }
    }
    
    // 检查左大括号
    if (position < tokens.size() && tokens[position].value == "{") {
        position++;
        
        // 解析特例化操作
        auto specializations = parseSpecializations(tokens, position);
        
        if (position < tokens.size() && tokens[position].value == "}") {
            position++;
        }
        
        if (pImpl->debug_mode) {
            pImpl->logDebug("自定义使用解析完成: " + custom_name + " (特例化数量: " + std::to_string(specializations.size()) + ")");
        }
    }
    
    // 创建自定义使用节点（这里需要创建具体的AST节点类型）
    // 暂时返回nullptr，等待具体的AST节点类型定义
    return nullptr; // TODO: 创建具体的自定义使用AST节点
}

std::string CustomSystem::parseIndexSelector(const std::vector<Token>& tokens, size_t& position) {
    std::string index;
    
    while (position < tokens.size() && tokens[position].value != "]") {
        index += tokens[position].value;
        position++;
    }
    
    // 移除首尾空白字符
    index.erase(0, index.find_first_not_of(" \t\n\r"));
    index.erase(index.find_last_not_of(" \t\n\r") + 1);
    
    return index;
}

std::string CustomSystem::parseInsertPosition(const std::vector<Token>& tokens, size_t& position) {
    if (position >= tokens.size()) {
        return "after";
    }
    
    std::string pos = tokens[position].value;
    if (pos == "after" || pos == "before" || pos == "replace" || pos == "at" || pos == "top" || pos == "bottom") {
        position++;
        
        // 处理 "at top" 和 "at bottom"
        if (pos == "at" && position < tokens.size()) {
            pos += " " + tokens[position].value;
            position++;
        }
        
        return pos;
    }
    
    return "after"; // 默认位置
}

void CustomSystem::addCustom(const CustomInfo& custom_info) {
    if (custom_info.name.empty()) {
        addError("自定义名称不能为空");
        return;
    }
    
    pImpl->customs[custom_info.name] = custom_info;
    
    if (pImpl->debug_mode) {
        pImpl->logDebug("添加自定义: " + custom_info.name);
    }
}

void CustomSystem::removeCustom(const std::string& name) {
    pImpl->customs.erase(name);
    
    if (pImpl->debug_mode) {
        pImpl->logDebug("删除自定义: " + name);
    }
}

CustomInfo* CustomSystem::getCustom(const std::string& name) {
    auto it = pImpl->customs.find(name);
    if (it != pImpl->customs.end()) {
        return &(it->second);
    }
    return nullptr;
}

std::vector<CustomInfo> CustomSystem::getAllCustoms() const {
    std::vector<CustomInfo> result;
    for (const auto& [name, info] : pImpl->customs) {
        result.push_back(info);
    }
    return result;
}

std::vector<CustomInfo> CustomSystem::getCustomsByType(CustomType type) const {
    std::vector<CustomInfo> result;
    for (const auto& [name, info] : pImpl->customs) {
        if (info.type == type) {
            result.push_back(info);
        }
    }
    return result;
}

bool CustomSystem::hasCustom(const std::string& name) const {
    return pImpl->customs.find(name) != pImpl->customs.end();
}

bool CustomSystem::validateCustom(const CustomInfo& custom_info) {
    // 验证自定义名称
    if (!validateCustomName(custom_info.name)) {
        addError("无效的自定义名称: " + custom_info.name);
        return false;
    }
    
    // 验证自定义内容
    if (!pImpl->isValidCustomContent(custom_info.custom_content)) {
        addError("无效的自定义内容: " + custom_info.name);
        return false;
    }
    
    // 验证特例化操作
    for (const auto& spec : custom_info.specializations) {
        if (!validateSpecialization(spec)) {
            return false;
        }
    }
    
    return true;
}

std::vector<std::string> CustomSystem::validateCustomList(const std::vector<CustomInfo>& customs) {
    std::vector<std::string> errors;
    
    for (const auto& custom_info : customs) {
        if (!validateCustom(custom_info)) {
            auto custom_errors = getErrors();
            errors.insert(errors.end(), custom_errors.begin(), custom_errors.end());
        }
    }
    
    return errors;
}

bool CustomSystem::validateCustomName(const std::string& name) {
    if (name.empty()) {
        return false;
    }
    
    // 检查长度限制
    if (name.length() > pImpl->config.max_custom_name_length) {
        return false;
    }
    
    // 检查是否只包含字母、数字、下划线，且不能以数字开头
    if (std::isdigit(name[0])) {
        return false;
    }
    
    for (char c : name) {
        if (!std::isalnum(c) && c != '_') {
            return false;
        }
    }
    
    return true;
}

bool CustomSystem::validateSpecialization(const SpecializationInfo& spec) {
    // 基本验证
    if (spec.target.empty()) {
        addError("特例化操作缺少目标");
        return false;
    }
    
    // 根据类型进行特定验证
    switch (spec.type) {
        case SpecializationType::DELETE_PROPERTY:
            // 删除属性需要有效的属性名
            break;
        case SpecializationType::INSERT_ELEMENT:
            // 插入元素需要有效的内容
            if (spec.insert_content.empty()) {
                addError("插入元素操作缺少内容");
                return false;
            }
            break;
        case SpecializationType::MODIFY_VALUE:
            // 修改值需要有效的新值
            if (spec.value.empty()) {
                addError("修改值操作缺少新值");
                return false;
            }
            break;
        default:
            break;
    }
    
    return true;
}

std::string CustomSystem::applyStyleCustom(const std::string& custom_name, const std::vector<SpecializationInfo>& specs) {
    auto* custom_info = getCustom(custom_name);
    if (!custom_info || custom_info->type != CustomType::STYLE_CUSTOM) {
        addError("自定义样式组不存在: " + custom_name);
        return "";
    }
    
    std::string result = custom_info->custom_content;
    
    // 应用特例化操作
    for (const auto& spec : specs) {
        switch (spec.type) {
            case SpecializationType::DELETE_PROPERTY:
                result = applyDeleteProperty(result, {spec.target});
                break;
            case SpecializationType::MODIFY_VALUE:
                result = applyModifyValue(result, spec.target, spec.value);
                break;
            default:
                break;
        }
    }
    
    return result;
}

std::string CustomSystem::applyElementCustom(const std::string& custom_name, const std::vector<SpecializationInfo>& specs) {
    auto* custom_info = getCustom(custom_name);
    if (!custom_info || custom_info->type != CustomType::ELEMENT_CUSTOM) {
        addError("自定义元素不存在: " + custom_name);
        return "";
    }
    
    std::string result = custom_info->custom_content;
    
    // 应用特例化操作
    for (const auto& spec : specs) {
        switch (spec.type) {
            case SpecializationType::DELETE_PROPERTY:
                // 删除元素
                break;
            case SpecializationType::INSERT_ELEMENT:
                result = applyInsertElement(result, spec);
                break;
            case SpecializationType::INDEX_ACCESS:
                result = applyIndexAccess(result, spec.target);
                break;
            default:
                break;
        }
    }
    
    return result;
}

std::string CustomSystem::applyVarCustom(const std::string& custom_name, const std::string& variable_name, const std::vector<SpecializationInfo>& specs) {
    auto* custom_info = getCustom(custom_name);
    if (!custom_info || custom_info->type != CustomType::VAR_CUSTOM) {
        addError("自定义变量组不存在: " + custom_name);
        return "";
    }
    
    // 在变量组中查找指定的变量
    std::istringstream iss(custom_info->custom_content);
    std::string line;
    
    while (std::getline(iss, line)) {
        size_t colon_pos = line.find(":");
        if (colon_pos != std::string::npos) {
            std::string var_name = line.substr(0, colon_pos);
            // 移除首尾空白字符
            var_name.erase(0, var_name.find_first_not_of(" \t\n\r"));
            var_name.erase(var_name.find_last_not_of(" \t\n\r") + 1);
            
            if (var_name == variable_name) {
                std::string var_value = line.substr(colon_pos + 1);
                // 移除首尾空白字符和引号
                var_value.erase(0, var_value.find_first_not_of(" \t\n\r\"'"));
                var_value.erase(var_value.find_last_not_of(" \t\n\r\"'") + 1);
                
                // 应用特例化操作
                for (const auto& spec : specs) {
                    if (spec.type == SpecializationType::MODIFY_VALUE) {
                        var_value = spec.value;
                    }
                }
                
                return var_value;
            }
        }
    }
    
    addError("变量不存在: " + variable_name + " 在自定义 " + custom_name + " 中");
    return "";
}

std::string CustomSystem::applyDeleteProperty(const std::string& base_content, const std::vector<std::string>& properties) {
    std::string result = base_content;
    
    for (const auto& property : properties) {
        result = pImpl->removeProperty(result, property);
    }
    
    return result;
}

std::string CustomSystem::applyDeleteInheritance(const std::string& base_content, const std::vector<std::string>& inheritances) {
    std::string result = base_content;
    
    for (const auto& inheritance : inheritances) {
        // 移除继承声明
        std::regex inheritance_regex("@" + inheritance + "\\s*;?");
        result = std::regex_replace(result, inheritance_regex, "");
    }
    
    return result;
}

std::string CustomSystem::applyInsertElement(const std::string& base_content, const SpecializationInfo& spec) {
    return pImpl->insertElementAtPosition(base_content, spec.insert_content, spec.insert_pos, spec.target);
}

std::string CustomSystem::applyModifyValue(const std::string& base_content, const std::string& property, const std::string& new_value) {
    std::string result = base_content;
    
    // 查找并替换属性值
    std::regex property_regex("(" + property + "\\s*:\\s*)[^;]+");
    std::string replacement = "$1" + new_value;
    result = std::regex_replace(result, property_regex, replacement);
    
    return result;
}

std::string CustomSystem::applyIndexAccess(const std::string& base_content, const std::string& index) {
    // 这里需要实现索引访问逻辑
    // 暂时返回原始内容
    return base_content;
}

void CustomSystem::setConfig(const CustomSystemConfig& config) {
    pImpl->config = config;
}

CustomSystemConfig CustomSystem::getConfig() const {
    return pImpl->config;
}

bool CustomSystem::hasErrors() const {
    return !pImpl->errors.empty();
}

std::vector<std::string> CustomSystem::getErrors() const {
    return pImpl->errors;
}

void CustomSystem::clearErrors() {
    pImpl->errors.clear();
}

void CustomSystem::addError(const std::string& error_message) {
    pImpl->errors.push_back(error_message);
    
    if (pImpl->debug_mode) {
        std::cerr << "自定义系统错误: " << error_message << std::endl;
    }
}

std::string CustomSystem::getDebugInfo() const {
    std::ostringstream oss;
    oss << "自定义系统调试信息:\n";
    oss << "自定义总数: " << pImpl->customs.size() << "\n";
    oss << "自定义样式组: " << getCustomsByType(CustomType::STYLE_CUSTOM).size() << "\n";
    oss << "自定义元素: " << getCustomsByType(CustomType::ELEMENT_CUSTOM).size() << "\n";
    oss << "自定义变量组: " << getCustomsByType(CustomType::VAR_CUSTOM).size() << "\n";
    oss << "错误数量: " << pImpl->errors.size() << "\n";
    oss << "自定义样式组: " << (pImpl->config.enable_style_customs ? "启用" : "禁用") << "\n";
    oss << "自定义元素: " << (pImpl->config.enable_element_customs ? "启用" : "禁用") << "\n";
    oss << "自定义变量组: " << (pImpl->config.enable_var_customs ? "启用" : "禁用") << "\n";
    oss << "特例化操作: " << (pImpl->config.enable_specialization ? "启用" : "禁用") << "\n";
    oss << "索引访问: " << (pImpl->config.enable_index_access ? "启用" : "禁用") << "\n";
    oss << "插入操作: " << (pImpl->config.enable_insert_operations ? "启用" : "禁用") << "\n";
    return oss.str();
}

void CustomSystem::enableDebugMode(bool enable) {
    pImpl->debug_mode = enable;
}

void CustomSystem::setStateMachine(std::shared_ptr<CHTLStateMachine> sm) {
    state_machine = sm;
}



// Impl类方法实现
bool CustomSystem::Impl::isCustomDeclaration(const std::vector<Token>& tokens, size_t position) {
    if (position + 1 >= tokens.size()) {
        return false;
    }
    
    const Token& token1 = tokens[position];
    const Token& token2 = tokens[position + 1];
    
    return token1.value == "[Custom]" && 
           (token2.value == "@Style" || token2.value == "@Element" || token2.value == "@Var");
}

bool CustomSystem::Impl::isCustomUsage(const std::vector<Token>& tokens, size_t position) {
    if (position >= tokens.size()) {
        return false;
    }
    
    const Token& token = tokens[position];
    return token.type == TokenType::IDENTIFIER && 
           (customs.find(token.value) != customs.end() || 
            token.value.find("@Style") == 0 || 
            token.value.find("@Element") == 0 || 
            token.value.find("@Var") == 0);
}

CustomType CustomSystem::Impl::detectCustomType(const std::vector<Token>& tokens, size_t position) {
    if (position >= tokens.size()) {
        return CustomType::STYLE_CUSTOM;
    }
    
    const Token& token = tokens[position];
    if (token.value == "@Style") {
        return CustomType::STYLE_CUSTOM;
    } else if (token.value == "@Element") {
        return CustomType::ELEMENT_CUSTOM;
    } else if (token.value == "@Var") {
        return CustomType::VAR_CUSTOM;
    }
    
    return CustomType::STYLE_CUSTOM;
}

void CustomSystem::Impl::skipWhitespace(const std::vector<Token>& tokens, size_t& position) {
    while (position < tokens.size() && 
           (tokens[position].value == " " || tokens[position].value == "\t" || 
            tokens[position].value == "\n" || tokens[position].value == "\r")) {
        position++;
    }
}

bool CustomSystem::Impl::isEndOfTokens(const std::vector<Token>& tokens, size_t position) {
    return position >= tokens.size();
}

bool CustomSystem::Impl::isValidCustomName(const std::string& name) {
    if (name.empty()) {
        return false;
    }
    
    // 检查是否只包含字母、数字、下划线，且不能以数字开头
    if (std::isdigit(name[0])) {
        return false;
    }
    
    for (char c : name) {
        if (!std::isalnum(c) && c != '_') {
            return false;
        }
    }
    
    return true;
}

bool CustomSystem::Impl::isValidCustomContent(const std::string& content) {
    return !content.empty();
}

bool CustomSystem::Impl::hasValidBaseTemplate(const std::string& base_template) {
    // 这里需要检查基础模板是否存在
    // 暂时返回true
    return true;
}

std::string CustomSystem::Impl::parseCSSProperties(const std::string& content) {
    // 简单的CSS属性解析
    return content;
}

std::vector<std::string> CustomSystem::Impl::extractPropertyNames(const std::string& content) {
    std::vector<std::string> properties;
    std::istringstream iss(content);
    std::string line;
    
    while (std::getline(iss, line)) {
        size_t colon_pos = line.find(":");
        if (colon_pos != std::string::npos) {
            std::string property = line.substr(0, colon_pos);
            // 移除首尾空白字符
            property.erase(0, property.find_first_not_of(" \t\n\r"));
            property.erase(property.find_last_not_of(" \t\n\r") + 1);
            properties.push_back(property);
        }
    }
    
    return properties;
}

std::string CustomSystem::Impl::removeProperty(const std::string& content, const std::string& property) {
    std::string result = content;
    
    // 查找并移除属性
    std::regex property_regex(property + "\\s*:[^;]+;?\\s*");
    result = std::regex_replace(result, property_regex, "");
    
    return result;
}

std::string CustomSystem::Impl::insertElementAtPosition(const std::string& content, const std::string& element, InsertPosition pos, const std::string& target) {
    std::string result = content;
    
    switch (pos) {
        case InsertPosition::AT_TOP:
            result = element + "\n" + result;
            break;
        case InsertPosition::AT_BOTTOM:
            result = result + "\n" + element;
            break;
        case InsertPosition::AFTER:
        case InsertPosition::BEFORE:
        case InsertPosition::REPLACE:
            // 这里需要实现更复杂的插入逻辑
            break;
    }
    
    return result;
}

std::vector<std::string> CustomSystem::Impl::parseCommaSeparatedList(const std::vector<Token>& tokens, size_t& position) {
    std::vector<std::string> items;
    
    while (position < tokens.size() && tokens[position].value != ";" && tokens[position].value != "}") {
        skipWhitespace(tokens, position);
        
        if (position >= tokens.size()) {
            break;
        }
        
        // 解析项目
        std::string item;
        while (position < tokens.size() && tokens[position].value != "," && tokens[position].value != ";" && tokens[position].value != "}") {
            item += tokens[position].value;
            position++;
        }
        
        // 移除首尾空白字符
        item.erase(0, item.find_first_not_of(" \t\n\r"));
        item.erase(item.find_last_not_of(" \t\n\r") + 1);
        
        if (!item.empty()) {
            items.push_back(item);
        }
        
        // 跳过逗号
        if (position < tokens.size() && tokens[position].value == ",") {
            position++;
        }
    }
    
    return items;
}

std::string CustomSystem::Impl::extractCustomContent(const std::vector<Token>& tokens, size_t& position) {
    std::string content;
    int brace_count = 1; // 已经跳过了左大括号
    
    while (position < tokens.size() && brace_count > 0) {
        if (tokens[position].value == "{") {
            brace_count++;
        } else if (tokens[position].value == "}") {
            brace_count--;
        }
        
        if (brace_count > 0) {
            content += tokens[position].value;
        }
        
        position++;
    }
    
    // 移除最后一个右大括号（如果存在）
    if (!content.empty() && content.back() == '}') {
        content.pop_back();
    }
    
    return content;
}

void CustomSystem::Impl::logDebug(const std::string& message) {
    if (debug_mode) {
        std::cout << "[DEBUG] " << message << std::endl;
    }
}

} // namespace chtl