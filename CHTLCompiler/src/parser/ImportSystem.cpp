#include "parser/ImportSystem.h"
#include "ast/ASTNode.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cctype>
#include <regex>
#include <filesystem>

namespace chtl {

// 实现类
class ImportSystem::Impl {
public:
    ImportSystemConfig config;
    std::unordered_map<std::string, ImportInfo> imports;
    std::unordered_map<std::string, ModuleInfo> modules;
    std::vector<std::string> errors;
    bool debug_mode;
    
    Impl(const ImportSystemConfig& cfg) : config(cfg), debug_mode(false) {}
    
    ~Impl() = default;
    
    // 内部方法实现
    bool isImportDeclaration(const std::vector<Token>& tokens, size_t position);
    ImportType detectImportType(const std::vector<Token>& tokens, size_t position);
    void skipWhitespace(const std::vector<Token>& tokens, size_t& position);
    bool isEndOfTokens(const std::vector<Token>& tokens, size_t position);
    
    // 导入验证辅助方法
    bool isValidImportName(const std::string& name);
    bool isValidFilePath(const std::string& file_path);
    bool hasValidFileExtension(const std::string& file_path, ImportType type);
    
    // 导入处理辅助方法
    std::string normalizeFilePath(const std::string& file_path);
    std::string extractFileExtension(const std::string& file_path);
    bool fileExists(const std::string& file_path);
    
    // 辅助方法
    void logDebug(const std::string& message);
    std::string extractImportContent(const std::vector<Token>& tokens, size_t& position);
    std::string parseImportIdentifier(const std::vector<Token>& tokens, size_t& position);
};

// ImportSystem实现
ImportSystem::ImportSystem(const ImportSystemConfig& config) 
    : pImpl(std::make_unique<Impl>(config)), state_machine(nullptr) {}

ImportSystem::~ImportSystem() = default;

std::shared_ptr<ASTNode> ImportSystem::parseImportDeclaration(const std::vector<Token>& tokens, size_t& position) {
    if (!pImpl->isImportDeclaration(tokens, position)) {
        addError("不是导入声明");
        return nullptr;
    }
    
    // 跳过[Import]
    position++;
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 检测导入类型
    ImportType import_type = pImpl->detectImportType(tokens, position);
    
    ImportInfo import_info;
    import_info.type = import_type;
    import_info.line = tokens[position].line;
    import_info.column = tokens[position].column;
    
    // 根据类型解析导入
    switch (import_type) {
        case ImportType::HTML_IMPORT:
            import_info = parseHtmlImport(tokens, position);
            break;
        case ImportType::STYLE_IMPORT:
            import_info = parseStyleImport(tokens, position);
            break;
        case ImportType::JAVASCRIPT_IMPORT:
            import_info = parseJavaScriptImport(tokens, position);
            break;
        case ImportType::CHTL_IMPORT:
            import_info = parseChtlImport(tokens, position);
            break;
        case ImportType::CJMOD_IMPORT:
            import_info = parseCjmodImport(tokens, position);
            break;
        case ImportType::CUSTOM_ELEMENT_IMPORT:
            import_info = parseCustomElementImport(tokens, position);
            break;
        case ImportType::CUSTOM_STYLE_IMPORT:
            import_info = parseCustomStyleImport(tokens, position);
            break;
        case ImportType::CUSTOM_VAR_IMPORT:
            import_info = parseCustomVarImport(tokens, position);
            break;
        case ImportType::TEMPLATE_ELEMENT_IMPORT:
            import_info = parseTemplateElementImport(tokens, position);
            break;
        case ImportType::TEMPLATE_STYLE_IMPORT:
            import_info = parseTemplateStyleImport(tokens, position);
            break;
        case ImportType::TEMPLATE_VAR_IMPORT:
            import_info = parseTemplateVarImport(tokens, position);
            break;
        default:
            addError("未知的导入类型");
            return nullptr;
    }
    
    // 验证导入信息
    if (!validateImport(import_info)) {
        return nullptr;
    }
    
    // 添加导入到系统
    addImport(import_info);
    
    if (pImpl->debug_mode) {
        pImpl->logDebug("导入声明解析完成: " + import_info.name);
    }
    
    // 创建导入声明节点（这里需要创建具体的AST节点类型）
    // 暂时返回nullptr，等待具体的AST节点类型定义
    return nullptr; // TODO: 创建具体的导入声明AST节点
}

ImportInfo ImportSystem::parseHtmlImport(const std::vector<Token>& tokens, size_t& position) {
    ImportInfo import_info;
    import_info.type = ImportType::HTML_IMPORT;
    
    // 检查@Html
    if (position >= tokens.size() || tokens[position].value != "@Html") {
        addError("HTML导入缺少@Html关键字");
        return import_info;
    }
    position++;
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 解析from语法
    if (position < tokens.size() && tokens[position].value == "from") {
        import_info.syntax_type = ImportSyntaxType::FROM_ONLY;
        position++;
        pImpl->skipWhitespace(tokens, position);
        
        // 解析文件路径
        if (position < tokens.size()) {
            import_info.file_path = tokens[position].value;
            position++;
        }
        
        // 检查是否有as
        pImpl->skipWhitespace(tokens, position);
        if (position < tokens.size() && tokens[position].value == "as") {
            import_info.syntax_type = ImportSyntaxType::FROM_AS;
            position++;
            pImpl->skipWhitespace(tokens, position);
            
            // 解析命名
            if (position < tokens.size()) {
                import_info.name = tokens[position].value;
                position++;
            }
        } else {
            // 没有as，使用文件名作为名称
            import_info.name = pImpl->extractFileExtension(import_info.file_path);
        }
    } else {
        // 直接导入
        import_info.syntax_type = ImportSyntaxType::DIRECT_IMPORT;
        if (position < tokens.size()) {
            import_info.file_path = tokens[position].value;
            import_info.name = pImpl->extractFileExtension(import_info.file_path);
            position++;
        }
    }
    
    return import_info;
}

ImportInfo ImportSystem::parseStyleImport(const std::vector<Token>& tokens, size_t& position) {
    ImportInfo import_info;
    import_info.type = ImportType::STYLE_IMPORT;
    
    // 检查@Style
    if (position >= tokens.size() || tokens[position].value != "@Style") {
        addError("CSS导入缺少@Style关键字");
        return import_info;
    }
    position++;
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 解析from语法
    if (position < tokens.size() && tokens[position].value == "from") {
        import_info.syntax_type = ImportSyntaxType::FROM_ONLY;
        position++;
        pImpl->skipWhitespace(tokens, position);
        
        // 解析文件路径
        if (position < tokens.size()) {
            import_info.file_path = tokens[position].value;
            position++;
        }
        
        // 检查是否有as
        pImpl->skipWhitespace(tokens, position);
        if (position < tokens.size() && tokens[position].value == "as") {
            import_info.syntax_type = ImportSyntaxType::FROM_AS;
            position++;
            pImpl->skipWhitespace(tokens, position);
            
            // 解析命名
            if (position < tokens.size()) {
                import_info.name = tokens[position].value;
                position++;
            }
        } else {
            // 没有as，使用文件名作为名称
            import_info.name = pImpl->extractFileExtension(import_info.file_path);
        }
    } else {
        // 直接导入
        import_info.syntax_type = ImportSyntaxType::DIRECT_IMPORT;
        if (position < tokens.size()) {
            import_info.file_path = tokens[position].value;
            import_info.name = pImpl->extractFileExtension(import_info.file_path);
            position++;
        }
    }
    
    return import_info;
}

ImportInfo ImportSystem::parseJavaScriptImport(const std::vector<Token>& tokens, size_t& position) {
    ImportInfo import_info;
    import_info.type = ImportType::JAVASCRIPT_IMPORT;
    
    // 检查@JavaScript
    if (position >= tokens.size() || tokens[position].value != "@JavaScript") {
        addError("JavaScript导入缺少@JavaScript关键字");
        return import_info;
    }
    position++;
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 解析from语法
    if (position < tokens.size() && tokens[position].value == "from") {
        import_info.syntax_type = ImportSyntaxType::FROM_ONLY;
        position++;
        pImpl->skipWhitespace(tokens, position);
        
        // 解析文件路径
        if (position < tokens.size()) {
            import_info.file_path = tokens[position].value;
            position++;
        }
        
        // 检查是否有as
        pImpl->skipWhitespace(tokens, position);
        if (position < tokens.size() && tokens[position].value == "as") {
            import_info.syntax_type = ImportSyntaxType::FROM_AS;
            position++;
            pImpl->skipWhitespace(tokens, position);
            
            // 解析命名
            if (position < tokens.size()) {
                import_info.name = tokens[position].value;
                position++;
            }
        } else {
            // 没有as，使用文件名作为名称
            import_info.name = pImpl->extractFileExtension(import_info.file_path);
        }
    } else {
        // 直接导入
        import_info.syntax_type = ImportSyntaxType::DIRECT_IMPORT;
        if (position < tokens.size()) {
            import_info.file_path = tokens[position].value;
            import_info.name = pImpl->extractFileExtension(import_info.file_path);
            position++;
        }
    }
    
    return import_info;
}

ImportInfo ImportSystem::parseChtlImport(const std::vector<Token>& tokens, size_t& position) {
    ImportInfo import_info;
    import_info.type = ImportType::CHTL_IMPORT;
    
    // 检查@Chtl
    if (position >= tokens.size() || tokens[position].value != "@Chtl") {
        addError("CHTL导入缺少@Chtl关键字");
        return import_info;
    }
    position++;
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 解析from语法
    if (position < tokens.size() && tokens[position].value == "from") {
        import_info.syntax_type = ImportSyntaxType::FROM_ONLY;
        position++;
        pImpl->skipWhitespace(tokens, position);
        
        // 解析文件路径
        if (position < tokens.size()) {
            import_info.file_path = tokens[position].value;
            position++;
        }
        
        // CHTL导入没有as语法
        import_info.name = pImpl->extractFileExtension(import_info.file_path);
    } else {
        addError("CHTL导入必须使用from语法");
    }
    
    return import_info;
}

ImportInfo ImportSystem::parseCjmodImport(const std::vector<Token>& tokens, size_t& position) {
    ImportInfo import_info;
    import_info.type = ImportType::CJMOD_IMPORT;
    
    // 检查@CJmod
    if (position >= tokens.size() || tokens[position].value != "@CJmod") {
        addError("CJmod导入缺少@CJmod关键字");
        return import_info;
    }
    position++;
    
    // 跳过空白字符
    pImpl->skipWhitespace(tokens, position);
    
    // 解析from语法
    if (position < tokens.size() && tokens[position].value == "from") {
        import_info.syntax_type = ImportSyntaxType::FROM_ONLY;
        position++;
        pImpl->skipWhitespace(tokens, position);
        
        // 解析文件路径
        if (position < tokens.size()) {
            import_info.file_path = tokens[position].value;
            position++;
        }
        
        // CJmod导入没有as语法
        import_info.name = pImpl->extractFileExtension(import_info.file_path);
    } else {
        addError("CJmod导入必须使用from语法");
    }
    
    return import_info;
}

ImportInfo ImportSystem::parseCustomElementImport(const std::vector<Token>& tokens, size_t& position) {
    ImportInfo import_info;
    import_info.type = ImportType::CUSTOM_ELEMENT_IMPORT;
    
    // 检查[Custom] @Element
    if (position >= tokens.size() || tokens[position].value != "[Custom]") {
        addError("自定义元素导入缺少[Custom]");
        return import_info;
    }
    position++;
    
    pImpl->skipWhitespace(tokens, position);
    
    if (position >= tokens.size() || tokens[position].value != "@Element") {
        addError("自定义元素导入缺少@Element");
        return import_info;
    }
    position++;
    
    pImpl->skipWhitespace(tokens, position);
    
    // 解析目标名称
    if (position < tokens.size()) {
        import_info.target_name = tokens[position].value;
        position++;
    }
    
    pImpl->skipWhitespace(tokens, position);
    
    // 解析from语法
    if (position < tokens.size() && tokens[position].value == "from") {
        import_info.syntax_type = ImportSyntaxType::FROM_ONLY;
        position++;
        pImpl->skipWhitespace(tokens, position);
        
        // 解析文件路径
        if (position < tokens.size()) {
            import_info.file_path = tokens[position].value;
            position++;
        }
        
        // 检查是否有as
        pImpl->skipWhitespace(tokens, position);
        if (position < tokens.size() && tokens[position].value == "as") {
            import_info.syntax_type = ImportSyntaxType::FROM_AS;
            position++;
            pImpl->skipWhitespace(tokens, position);
            
            // 解析命名
            if (position < tokens.size()) {
                import_info.name = tokens[position].value;
                position++;
            }
        } else {
            // 没有as，使用目标名称作为名称
            import_info.name = import_info.target_name;
        }
    } else {
        addError("自定义元素导入必须使用from语法");
    }
    
    return import_info;
}

ImportInfo ImportSystem::parseCustomStyleImport(const std::vector<Token>& tokens, size_t& position) {
    ImportInfo import_info;
    import_info.type = ImportType::CUSTOM_STYLE_IMPORT;
    
    // 检查[Custom] @Style
    if (position >= tokens.size() || tokens[position].value != "[Custom]") {
        addError("自定义样式组导入缺少[Custom]");
        return import_info;
    }
    position++;
    
    pImpl->skipWhitespace(tokens, position);
    
    if (position >= tokens.size() || tokens[position].value != "@Style") {
        addError("自定义样式组导入缺少@Style");
        return import_info;
    }
    position++;
    
    pImpl->skipWhitespace(tokens, position);
    
    // 解析目标名称
    if (position < tokens.size()) {
        import_info.target_name = tokens[position].value;
        position++;
    }
    
    pImpl->skipWhitespace(tokens, position);
    
    // 解析from语法
    if (position < tokens.size() && tokens[position].value == "from") {
        import_info.syntax_type = ImportSyntaxType::FROM_ONLY;
        position++;
        pImpl->skipWhitespace(tokens, position);
        
        // 解析文件路径
        if (position < tokens.size()) {
            import_info.file_path = tokens[position].value;
            position++;
        }
        
        // 检查是否有as
        pImpl->skipWhitespace(tokens, position);
        if (position < tokens.size() && tokens[position].value == "as") {
            import_info.syntax_type = ImportSyntaxType::FROM_AS;
            position++;
            pImpl->skipWhitespace(tokens, position);
            
            // 解析命名
            if (position < tokens.size()) {
                import_info.name = tokens[position].value;
                position++;
            }
        } else {
            // 没有as，使用目标名称作为名称
            import_info.name = import_info.target_name;
        }
    } else {
        addError("自定义样式组导入必须使用from语法");
    }
    
    return import_info;
}

ImportInfo ImportSystem::parseCustomVarImport(const std::vector<Token>& tokens, size_t& position) {
    ImportInfo import_info;
    import_info.type = ImportType::CUSTOM_VAR_IMPORT;
    
    // 检查[Custom] @Var
    if (position >= tokens.size() || tokens[position].value != "[Custom]") {
        addError("自定义变量组导入缺少[Custom]");
        return import_info;
    }
    position++;
    
    pImpl->skipWhitespace(tokens, position);
    
    if (position >= tokens.size() || tokens[position].value != "@Var") {
        addError("自定义变量组导入缺少@Var");
        return import_info;
    }
    position++;
    
    pImpl->skipWhitespace(tokens, position);
    
    // 解析目标名称
    if (position < tokens.size()) {
        import_info.target_name = tokens[position].value;
        position++;
    }
    
    pImpl->skipWhitespace(tokens, position);
    
    // 解析from语法
    if (position < tokens.size() && tokens[position].value == "from") {
        import_info.syntax_type = ImportSyntaxType::FROM_ONLY;
        position++;
        pImpl->skipWhitespace(tokens, position);
        
        // 解析文件路径
        if (position < tokens.size()) {
            import_info.file_path = tokens[position].value;
            position++;
        }
        
        // 检查是否有as
        pImpl->skipWhitespace(tokens, position);
        if (position < tokens.size() && tokens[position].value == "as") {
            import_info.syntax_type = ImportSyntaxType::FROM_AS;
            position++;
            pImpl->skipWhitespace(tokens, position);
            
            // 解析命名
            if (position < tokens.size()) {
                import_info.name = tokens[position].value;
                position++;
            }
        } else {
            // 没有as，使用目标名称作为名称
            import_info.name = import_info.target_name;
        }
    } else {
        addError("自定义变量组导入必须使用from语法");
    }
    
    return import_info;
}

ImportInfo ImportSystem::parseTemplateElementImport(const std::vector<Token>& tokens, size_t& position) {
    ImportInfo import_info;
    import_info.type = ImportType::TEMPLATE_ELEMENT_IMPORT;
    
    // 检查[Template] @Element
    if (position >= tokens.size() || tokens[position].value != "[Template]") {
        addError("元素模板导入缺少[Template]");
        return import_info;
    }
    position++;
    
    pImpl->skipWhitespace(tokens, position);
    
    if (position >= tokens.size() || tokens[position].value != "@Element") {
        addError("元素模板导入缺少@Element");
        return import_info;
    }
    position++;
    
    pImpl->skipWhitespace(tokens, position);
    
    // 解析目标名称
    if (position < tokens.size()) {
        import_info.target_name = tokens[position].value;
        position++;
    }
    
    pImpl->skipWhitespace(tokens, position);
    
    // 解析from语法
    if (position < tokens.size() && tokens[position].value == "from") {
        import_info.syntax_type = ImportSyntaxType::FROM_ONLY;
        position++;
        pImpl->skipWhitespace(tokens, position);
        
        // 解析文件路径
        if (position < tokens.size()) {
            import_info.file_path = tokens[position].value;
            position++;
        }
        
        // 检查是否有as
        pImpl->skipWhitespace(tokens, position);
        if (position < tokens.size() && tokens[position].value == "as") {
            import_info.syntax_type = ImportSyntaxType::FROM_AS;
            position++;
            pImpl->skipWhitespace(tokens, position);
            
            // 解析命名
            if (position < tokens.size()) {
                import_info.name = tokens[position].value;
                position++;
            }
        } else {
            // 没有as，使用目标名称作为名称
            import_info.name = import_info.target_name;
        }
    } else {
        addError("元素模板导入必须使用from语法");
    }
    
    return import_info;
}

ImportInfo ImportSystem::parseTemplateStyleImport(const std::vector<Token>& tokens, size_t& position) {
    ImportInfo import_info;
    import_info.type = ImportType::TEMPLATE_STYLE_IMPORT;
    
    // 检查[Template] @Style
    if (position >= tokens.size() || tokens[position].value != "[Template]") {
        addError("样式组模板导入缺少[Template]");
        return import_info;
    }
    position++;
    
    pImpl->skipWhitespace(tokens, position);
    
    if (position >= tokens.size() || tokens[position].value != "@Style") {
        addError("样式组模板导入缺少@Style");
        return import_info;
    }
    position++;
    
    pImpl->skipWhitespace(tokens, position);
    
    // 解析目标名称
    if (position < tokens.size()) {
        import_info.target_name = tokens[position].value;
        position++;
    }
    
    pImpl->skipWhitespace(tokens, position);
    
    // 解析from语法
    if (position < tokens.size() && tokens[position].value == "from") {
        import_info.syntax_type = ImportSyntaxType::FROM_ONLY;
        position++;
        pImpl->skipWhitespace(tokens, position);
        
        // 解析文件路径
        if (position < tokens.size()) {
            import_info.file_path = tokens[position].value;
            position++;
        }
        
        // 检查是否有as
        pImpl->skipWhitespace(tokens, position);
        if (position < tokens.size() && tokens[position].value == "as") {
            import_info.syntax_type = ImportSyntaxType::FROM_AS;
            position++;
            pImpl->skipWhitespace(tokens, position);
            
            // 解析命名
            if (position < tokens.size()) {
                import_info.name = tokens[position].value;
                position++;
            }
        } else {
            // 没有as，使用目标名称作为名称
            import_info.name = import_info.target_name;
        }
    } else {
        addError("样式组模板导入必须使用from语法");
    }
    
    return import_info;
}

ImportInfo ImportSystem::parseTemplateVarImport(const std::vector<Token>& tokens, size_t& position) {
    ImportInfo import_info;
    import_info.type = ImportType::TEMPLATE_VAR_IMPORT;
    
    // 检查[Template] @Var
    if (position >= tokens.size() || tokens[position].value != "[Template]") {
        addError("变量组模板导入缺少[Template]");
        return import_info;
    }
    position++;
    
    pImpl->skipWhitespace(tokens, position);
    
    if (position >= tokens.size() || tokens[position].value != "@Var") {
        addError("变量组模板导入缺少@Var");
        return import_info;
    }
    position++;
    
    pImpl->skipWhitespace(tokens, position);
    
    // 解析目标名称
    if (position < tokens.size()) {
        import_info.target_name = tokens[position].value;
        position++;
    }
    
    pImpl->skipWhitespace(tokens, position);
    
    // 解析from语法
    if (position < tokens.size() && tokens[position].value == "from") {
        import_info.syntax_type = ImportSyntaxType::FROM_ONLY;
        position++;
        pImpl->skipWhitespace(tokens, position);
        
        // 解析文件路径
        if (position < tokens.size()) {
            import_info.file_path = tokens[position].value;
            position++;
        }
        
        // 检查是否有as
        pImpl->skipWhitespace(tokens, position);
        if (position < tokens.size() && tokens[position].value == "as") {
            import_info.syntax_type = ImportSyntaxType::FROM_AS;
            position++;
            pImpl->skipWhitespace(tokens, position);
            
            // 解析命名
            if (position < tokens.size()) {
                import_info.name = tokens[position].value;
                position++;
            }
        } else {
            // 没有as，使用目标名称作为名称
            import_info.name = import_info.target_name;
        }
    } else {
        addError("变量组模板导入必须使用from语法");
    }
    
    return import_info;
}

void ImportSystem::addImport(const ImportInfo& import_info) {
    if (import_info.name.empty()) {
        addError("导入缺少名称");
        return;
    }
    
    pImpl->imports[import_info.name] = import_info;
    
    if (pImpl->debug_mode) {
        pImpl->logDebug("添加导入: " + import_info.name);
    }
}

void ImportSystem::removeImport(const std::string& name) {
    pImpl->imports.erase(name);
    
    if (pImpl->debug_mode) {
        pImpl->logDebug("删除导入: " + name);
    }
}

ImportInfo* ImportSystem::getImport(const std::string& name) {
    auto it = pImpl->imports.find(name);
    if (it != pImpl->imports.end()) {
        return &(it->second);
    }
    return nullptr;
}

std::vector<ImportInfo> ImportSystem::getAllImports() const {
    std::vector<ImportInfo> result;
    for (const auto& [name, info] : pImpl->imports) {
        result.push_back(info);
    }
    return result;
}

std::vector<ImportInfo> ImportSystem::getImportsByType(ImportType type) const {
    std::vector<ImportInfo> result;
    for (const auto& [name, info] : pImpl->imports) {
        if (info.type == type) {
            result.push_back(info);
        }
    }
    return result;
}

bool ImportSystem::hasImport(const std::string& name) const {
    return pImpl->imports.find(name) != pImpl->imports.end();
}

void ImportSystem::addModule(const ModuleInfo& module_info) {
    if (module_info.name.empty()) {
        addError("模块缺少名称");
        return;
    }
    
    pImpl->modules[module_info.name] = module_info;
    
    if (pImpl->debug_mode) {
        pImpl->logDebug("添加模块: " + module_info.name);
    }
}

void ImportSystem::removeModule(const std::string& name) {
    pImpl->modules.erase(name);
    
    if (pImpl->debug_mode) {
        pImpl->logDebug("删除模块: " + name);
    }
}

ModuleInfo* ImportSystem::getModule(const std::string& name) {
    auto it = pImpl->modules.find(name);
    if (it != pImpl->modules.end()) {
        return &(it->second);
    }
    return nullptr;
}

std::vector<ModuleInfo> ImportSystem::getAllModules() const {
    std::vector<ModuleInfo> result;
    for (const auto& [name, info] : pImpl->modules) {
        result.push_back(info);
    }
    return result;
}

bool ImportSystem::hasModule(const std::string& name) const {
    return pImpl->modules.find(name) != pImpl->modules.end();
}

bool ImportSystem::validateImport(const ImportInfo& import_info) {
    // 验证导入名称
    if (!pImpl->isValidImportName(import_info.name)) {
        addError("无效的导入名称: " + import_info.name);
        return false;
    }
    
    // 验证文件路径
    if (!pImpl->isValidFilePath(import_info.file_path)) {
        addError("无效的文件路径: " + import_info.file_path);
        return false;
    }
    
    // 验证文件扩展名
    if (!pImpl->hasValidFileExtension(import_info.file_path, import_info.type)) {
        addError("无效的文件扩展名: " + import_info.file_path);
        return false;
    }
    
    return true;
}

std::vector<std::string> ImportSystem::validateImportList(const std::vector<ImportInfo>& imports) {
    std::vector<std::string> errors;
    
    for (const auto& import_info : imports) {
        if (!validateImport(import_info)) {
            auto import_errors = getErrors();
            errors.insert(errors.end(), import_errors.begin(), import_errors.end());
        }
    }
    
    return errors;
}

bool ImportSystem::validateImportPath(const std::string& file_path) {
    return pImpl->isValidFilePath(file_path);
}

bool ImportSystem::validateImportName(const std::string& name) {
    return pImpl->isValidImportName(name);
}

bool ImportSystem::processImport(const std::string& import_name) {
    auto* import_info = getImport(import_name);
    if (!import_info) {
        addError("导入不存在: " + import_name);
        return false;
    }
    
    return processImportByType(import_info->type, import_info->file_path);
}

bool ImportSystem::processImportByType(ImportType type, const std::string& file_path) {
    // 这里实现具体的导入处理逻辑
    if (pImpl->debug_mode) {
        pImpl->logDebug("处理导入: " + file_path + " 类型: " + std::to_string(static_cast<int>(type)));
    }
    
    return true;
}

std::string ImportSystem::resolveImportPath(const std::string& relative_path) {
    return pImpl->normalizeFilePath(relative_path);
}

bool ImportSystem::checkCircularDependencies(const std::string& module_name) {
    // 这里实现循环依赖检测逻辑
    if (pImpl->debug_mode) {
        pImpl->logDebug("检查循环依赖: " + module_name);
    }
    
    return false; // 暂时返回false
}

std::vector<std::string> ImportSystem::getDependencyChain(const std::string& module_name) {
    // 这里实现依赖链获取逻辑
    if (pImpl->debug_mode) {
        pImpl->logDebug("获取依赖链: " + module_name);
    }
    
    return {};
}

bool ImportSystem::resolveDependencies(const std::string& module_name) {
    // 这里实现依赖解析逻辑
    if (pImpl->debug_mode) {
        pImpl->logDebug("解析依赖: " + module_name);
    }
    
    return true;
}

void ImportSystem::setConfig(const ImportSystemConfig& config) {
    pImpl->config = config;
}

ImportSystemConfig ImportSystem::getConfig() const {
    return pImpl->config;
}

bool ImportSystem::hasErrors() const {
    return !pImpl->errors.empty();
}

std::vector<std::string> ImportSystem::getErrors() const {
    return pImpl->errors;
}

void ImportSystem::clearErrors() {
    pImpl->errors.clear();
}

void ImportSystem::addError(const std::string& error_message) {
    pImpl->errors.push_back(error_message);
    
    if (pImpl->debug_mode) {
        std::cerr << "导入系统错误: " << error_message << std::endl;
    }
}

std::string ImportSystem::getDebugInfo() const {
    std::ostringstream oss;
    oss << "导入系统调试信息:\n";
    oss << "导入总数: " << pImpl->imports.size() << "\n";
    oss << "模块总数: " << pImpl->modules.size() << "\n";
    oss << "HTML导入: " << (pImpl->config.enable_html_import ? "启用" : "禁用") << "\n";
    oss << "CSS导入: " << (pImpl->config.enable_style_import ? "启用" : "禁用") << "\n";
    oss << "JavaScript导入: " << (pImpl->config.enable_javascript_import ? "启用" : "禁用") << "\n";
    oss << "CHTL导入: " << (pImpl->config.enable_chtl_import ? "启用" : "禁用") << "\n";
    oss << "CJmod导入: " << (pImpl->config.enable_cjmod_import ? "启用" : "禁用") << "\n";
    oss << "自定义导入: " << (pImpl->config.enable_custom_import ? "启用" : "禁用") << "\n";
    oss << "模板导入: " << (pImpl->config.enable_template_import ? "启用" : "禁用") << "\n";
    oss << "错误数量: " << pImpl->errors.size() << "\n";
    
    for (const auto& [name, import] : pImpl->imports) {
        oss << "  导入: " << name << " (类型: " << static_cast<int>(import.type) 
            << ", 文件: " << import.file_path << ")\n";
    }
    
    return oss.str();
}

void ImportSystem::enableDebugMode(bool enable) {
    pImpl->debug_mode = enable;
}

void ImportSystem::setStateMachine(std::shared_ptr<CHTLStateMachine> sm) {
    state_machine = sm;
}

// Impl类方法实现
bool ImportSystem::Impl::isImportDeclaration(const std::vector<Token>& tokens, size_t position) {
    if (position >= tokens.size()) {
        return false;
    }
    
    const Token& token = tokens[position];
    return token.value == "[Import]";
}

ImportType ImportSystem::Impl::detectImportType(const std::vector<Token>& tokens, size_t position) {
    if (position >= tokens.size()) {
        return ImportType::HTML_IMPORT;
    }
    
    const Token& token = tokens[position];
    
    // 检查是否为自定义导入
    if (token.value == "[Custom]") {
        if (position + 1 < tokens.size()) {
            const Token& next_token = tokens[position + 1];
            if (next_token.value == "@Element") return ImportType::CUSTOM_ELEMENT_IMPORT;
            if (next_token.value == "@Style") return ImportType::CUSTOM_STYLE_IMPORT;
            if (next_token.value == "@Var") return ImportType::CUSTOM_VAR_IMPORT;
        }
    }
    
    // 检查是否为模板导入
    if (token.value == "[Template]") {
        if (position + 1 < tokens.size()) {
            const Token& next_token = tokens[position + 1];
            if (next_token.value == "@Element") return ImportType::TEMPLATE_ELEMENT_IMPORT;
            if (next_token.value == "@Style") return ImportType::TEMPLATE_STYLE_IMPORT;
            if (next_token.value == "@Var") return ImportType::TEMPLATE_VAR_IMPORT;
        }
    }
    
    // 检查基本导入类型
    if (token.value == "@Html") return ImportType::HTML_IMPORT;
    if (token.value == "@Style") return ImportType::STYLE_IMPORT;
    if (token.value == "@JavaScript") return ImportType::JAVASCRIPT_IMPORT;
    if (token.value == "@Chtl") return ImportType::CHTL_IMPORT;
    if (token.value == "@CJmod") return ImportType::CJMOD_IMPORT;
    
    return ImportType::HTML_IMPORT; // 默认类型
}

void ImportSystem::Impl::skipWhitespace(const std::vector<Token>& tokens, size_t& position) {
    while (position < tokens.size() && 
           (tokens[position].value == " " || tokens[position].value == "\t" || 
            tokens[position].value == "\n" || tokens[position].value == "\r")) {
        position++;
    }
}

bool ImportSystem::Impl::isEndOfTokens(const std::vector<Token>& tokens, size_t position) {
    return position >= tokens.size();
}

bool ImportSystem::Impl::isValidImportName(const std::string& name) {
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

bool ImportSystem::Impl::isValidFilePath(const std::string& file_path) {
    if (file_path.empty()) {
        return false;
    }
    
    // 检查是否包含有效的路径字符
    for (char c : file_path) {
        if (!std::isalnum(c) && c != '/' && c != '\\' && c != '.' && c != '_' && c != '-') {
            return false;
        }
    }
    
    return true;
}

bool ImportSystem::Impl::hasValidFileExtension(const std::string& file_path, ImportType type) {
    std::string extension = extractFileExtension(file_path);
    
    switch (type) {
        case ImportType::HTML_IMPORT:
            return extension == "html" || extension == "htm";
        case ImportType::STYLE_IMPORT:
            return extension == "css";
        case ImportType::JAVASCRIPT_IMPORT:
            return extension == "js";
        case ImportType::CHTL_IMPORT:
            return extension == "chtl";
        case ImportType::CJMOD_IMPORT:
            return extension == "cjmod";
        case ImportType::CUSTOM_ELEMENT_IMPORT:
        case ImportType::CUSTOM_STYLE_IMPORT:
        case ImportType::CUSTOM_VAR_IMPORT:
        case ImportType::TEMPLATE_ELEMENT_IMPORT:
        case ImportType::TEMPLATE_STYLE_IMPORT:
        case ImportType::TEMPLATE_VAR_IMPORT:
            return extension == "chtl";
        default:
            return true;
    }
}

std::string ImportSystem::Impl::normalizeFilePath(const std::string& file_path) {
    std::string normalized = file_path;
    
    // 将'.'替换为'/'
    std::replace(normalized.begin(), normalized.end(), '.', '/');
    
    return normalized;
}

std::string ImportSystem::Impl::extractFileExtension(const std::string& file_path) {
    size_t last_dot = file_path.find_last_of('.');
    if (last_dot != std::string::npos) {
        return file_path.substr(last_dot + 1);
    }
    return "";
}

bool ImportSystem::Impl::fileExists(const std::string& file_path) {
    return std::filesystem::exists(file_path);
}

std::string ImportSystem::Impl::extractImportContent(const std::vector<Token>& tokens, size_t& position) {
    std::string content;
    
    while (position < tokens.size()) {
        content += tokens[position].value;
        position++;
    }
    
    return content;
}

std::string ImportSystem::Impl::parseImportIdentifier(const std::vector<Token>& tokens, size_t& position) {
    std::string identifier;
    
    if (position < tokens.size() && tokens[position].type == TokenType::IDENTIFIER) {
        identifier = tokens[position].value;
        position++;
    }
    
    return identifier;
}

void ImportSystem::Impl::logDebug(const std::string& message) {
    if (debug_mode) {
        std::cout << "[DEBUG] " << message << std::endl;
    }
}

} // namespace chtl