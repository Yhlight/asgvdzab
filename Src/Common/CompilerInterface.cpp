#include "Common/CompilerInterface.h"
#include <sstream>

namespace Chtl {

// === CompileResult实现 ===

void CompileResult::AddError(const std::string& message, const SourceLocation& location) {
    std::ostringstream oss;
    if (!location.FileName.empty()) {
        oss << location.FileName << ":" << location.Line << ":" << location.Column << ": ";
    }
    oss << message;
    Errors_.push_back(oss.str());
    Success_ = false;
}

void CompileResult::AddWarning(const std::string& message, const SourceLocation& location) {
    std::ostringstream oss;
    if (!location.FileName.empty()) {
        oss << location.FileName << ":" << location.Line << ":" << location.Column << ": ";
    }
    oss << message;
    Warnings_.push_back(oss.str());
}

// === AntlrCompiler实现 ===

bool AntlrCompiler::Initialize() {
    if (Initialized_) return true;
    
    bool result = InitializeAntlr();
    if (result) {
        Initialized_ = true;
    }
    return result;
}

void AntlrCompiler::Cleanup() {
    if (Initialized_) {
        CleanupAntlr();
        Initialized_ = false;
    }
}

// === CompilerRegistry实现 ===

CompilerRegistry& CompilerRegistry::GetInstance() {
    static CompilerRegistry instance;
    return instance;
}

void CompilerRegistry::RegisterCompiler(FragmentType type, 
    std::function<std::unique_ptr<ICompiler>()> creator) {
    Creators_[type] = std::move(creator);
}

std::unique_ptr<ICompiler> CompilerRegistry::CreateCompiler(FragmentType type) {
    auto it = Creators_.find(type);
    if (it != Creators_.end()) {
        return it->second();
    }
    return nullptr;
}

std::vector<FragmentType> CompilerRegistry::GetSupportedTypes() const {
    std::vector<FragmentType> types;
    for (const auto& pair : Creators_) {
        types.push_back(pair.first);
    }
    return types;
}

bool CompilerRegistry::IsSupported(FragmentType type) const {
    return Creators_.find(type) != Creators_.end();
}

// === CompilerConfig实现 ===

std::string CompilerConfig::ToString() const {
    std::ostringstream oss;
    oss << "CompilerConfig{";
    oss << "DebugMode=" << (DebugMode ? "true" : "false") << ", ";
    oss << "Optimize=" << (Optimize ? "true" : "false") << ", ";
    oss << "OutputFormat=" << OutputFormat << ", ";
    oss << "IndexInitialCount=" << IndexInitialCount << ", ";
    oss << "MinifyCss=" << (MinifyCss ? "true" : "false") << ", ";
    oss << "MinifyJs=" << (MinifyJs ? "true" : "false");
    oss << "}";
    return oss.str();
}

bool CompilerConfig::FromString(const std::string& configStr) {
    // 简单的配置字符串解析 - 这里可以根据需要扩展
    // 格式: key1=value1;key2=value2;...
    
    std::istringstream iss(configStr);
    std::string item;
    
    while (std::getline(iss, item, ';')) {
        size_t equalPos = item.find('=');
        if (equalPos != std::string::npos) {
            std::string key = item.substr(0, equalPos);
            std::string value = item.substr(equalPos + 1);
            
            // 去除空白字符
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            
            // 设置配置值
            if (key == "DebugMode") {
                DebugMode = (value == "true");
            } else if (key == "Optimize") {
                Optimize = (value == "true");
            } else if (key == "OutputFormat") {
                OutputFormat = value;
            } else if (key == "IndexInitialCount") {
                IndexInitialCount = std::stoi(value);
            } else if (key == "MinifyCss") {
                MinifyCss = (value == "true");
            } else if (key == "MinifyJs") {
                MinifyJs = (value == "true");
            }
        }
    }
    
    return true;
}

} // namespace Chtl