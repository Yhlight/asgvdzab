#include "Common/Context.h"
#include "Common/ImportResolver.h"
#include <iostream>
#include <algorithm>
#include <filesystem>

namespace Chtl {

Context::Context() 
    : currentLine_(1),
      currentColumn_(1) {
    globalMap_ = std::make_unique<GlobalMap>();
    state_ = std::make_unique<State>();
    importResolver_ = std::make_unique<ImportResolver>();
    
    // 添加默认包含路径
    includePaths_.push_back(".");
    includePaths_.push_back("./modules");
}

Context::~Context() {
}

void Context::addDiagnostic(const Diagnostic& diag) {
    diagnostics_.push_back(diag);
}

void Context::addInfo(const std::string& msg, size_t line, size_t col) {
    Diagnostic diag;
    diag.level = Diagnostic::INFO;
    diag.message = msg;
    diag.file = sourceInfo_.fileName;
    diag.line = line ? line : currentLine_;
    diag.column = col ? col : currentColumn_;
    addDiagnostic(diag);
}

void Context::addWarning(const std::string& msg, size_t line, size_t col) {
    Diagnostic diag;
    diag.level = Diagnostic::WARNING;
    diag.message = msg;
    diag.file = sourceInfo_.fileName;
    diag.line = line ? line : currentLine_;
    diag.column = col ? col : currentColumn_;
    addDiagnostic(diag);
}

void Context::addError(const std::string& msg, size_t line, size_t col) {
    Diagnostic diag;
    diag.level = Diagnostic::ERROR;
    diag.message = msg;
    diag.file = sourceInfo_.fileName;
    diag.line = line ? line : currentLine_;
    diag.column = col ? col : currentColumn_;
    addDiagnostic(diag);
    
    // 同时设置State的错误状态
    state_->setError(msg);
}

bool Context::hasErrors() const {
    return std::any_of(diagnostics_.begin(), diagnostics_.end(),
        [](const Diagnostic& d) { return d.level == Diagnostic::ERROR; });
}

bool Context::hasWarnings() const {
    return std::any_of(diagnostics_.begin(), diagnostics_.end(),
        [](const Diagnostic& d) { return d.level == Diagnostic::WARNING; });
}

void Context::enterScope(const std::string& name) {
    scopeStack_.push_back(name.empty() ? "anonymous" : name);
}

void Context::exitScope() {
    if (!scopeStack_.empty()) {
        scopeStack_.pop_back();
    }
}

std::string Context::getCurrentScope() const {
    if (scopeStack_.empty()) {
        return "global";
    }
    
    std::string scope;
    for (size_t i = 0; i < scopeStack_.size(); ++i) {
        if (i > 0) scope += "::";
        scope += scopeStack_[i];
    }
    return scope;
}

void Context::setTempData(const std::string& key, const std::string& value) {
    tempData_[key] = value;
}

std::string Context::getTempData(const std::string& key) const {
    auto it = tempData_.find(key);
    return (it != tempData_.end()) ? it->second : "";
}

bool Context::hasTempData(const std::string& key) const {
    return tempData_.find(key) != tempData_.end();
}

void Context::addIncludePath(const std::string& path) {
    includePaths_.push_back(path);
}

std::string Context::resolveFilePath(const std::string& path) const {
    namespace fs = std::filesystem;
    
    // 如果是绝对路径，直接返回
    if (fs::path(path).is_absolute()) {
        return path;
    }
    
    // 尝试相对于当前文件的目录
    if (!sourceInfo_.directory.empty()) {
        fs::path candidate = fs::path(sourceInfo_.directory) / path;
        if (fs::exists(candidate)) {
            return candidate.string();
        }
    }
    
    // 尝试包含路径
    for (const auto& includePath : includePaths_) {
        fs::path candidate = fs::path(includePath) / path;
        if (fs::exists(candidate)) {
            return candidate.string();
        }
    }
    
    // 返回原始路径
    return path;
}

void Context::reset() {
    // 清空输出
    clearOutput();
    
    // 重置状态
    state_->reset();
    globalMap_->clear();
    
    // 清空诊断信息
    diagnostics_.clear();
    
    // 重置位置
    currentLine_ = 1;
    currentColumn_ = 1;
    
    // 清空作用域
    scopeStack_.clear();
    
    // 清空临时数据
    tempData_.clear();
}

CompileResult Context::generateResult() {
    CompileResult result;
    
    result.success = !hasErrors();
    
    // 合并输出
    if (!htmlOutput_.empty()) {
        result.output = htmlOutput_;
    }
    
    // 如果有CSS，添加style标签
    if (!cssOutput_.empty()) {
        result.output = "<style>\n" + cssOutput_ + "</style>\n" + result.output;
    }
    
    // 如果有JS，添加script标签
    if (!jsOutput_.empty()) {
        result.output += "\n<script>\n" + jsOutput_ + "</script>";
    }
    
    // 如果有通用输出，直接添加
    if (!output_.empty()) {
        result.output = output_;
    }
    
    // 收集错误信息
    for (const auto& diag : diagnostics_) {
        if (diag.level == Diagnostic::ERROR) {
            if (!result.errorMsg.empty()) {
                result.errorMsg += "\n";
            }
            result.errorMsg += diag.file + ":" + std::to_string(diag.line) + 
                              ":" + std::to_string(diag.column) + ": " + diag.message;
        } else if (diag.level == Diagnostic::WARNING) {
            result.warnings.push_back(diag.file + ":" + std::to_string(diag.line) + 
                                     ":" + std::to_string(diag.column) + ": " + diag.message);
        }
    }
    
    return result;
}

void Context::dump() const {
    std::cout << "=== Context Dump ===\n";
    
    std::cout << "Source: " << sourceInfo_.fileName << "\n";
    std::cout << "Position: " << currentLine_ << ":" << currentColumn_ << "\n";
    std::cout << "Current Scope: " << getCurrentScope() << "\n";
    
    std::cout << "\nDiagnostics (" << diagnostics_.size() << "):\n";
    for (const auto& diag : diagnostics_) {
        const char* level = "";
        switch (diag.level) {
            case Diagnostic::INFO: level = "INFO"; break;
            case Diagnostic::WARNING: level = "WARNING"; break;
            case Diagnostic::ERROR: level = "ERROR"; break;
        }
        std::cout << "  [" << level << "] " << diag.line << ":" << diag.column 
                  << " " << diag.message << "\n";
    }
    
    std::cout << "\nOutput Sizes:\n";
    std::cout << "  HTML: " << htmlOutput_.size() << " bytes\n";
    std::cout << "  CSS: " << cssOutput_.size() << " bytes\n";
    std::cout << "  JS: " << jsOutput_.size() << " bytes\n";
    std::cout << "  General: " << output_.size() << " bytes\n";
    
    std::cout << "\nInclude Paths:\n";
    for (const auto& path : includePaths_) {
        std::cout << "  " << path << "\n";
    }
    
    std::cout << "\nTemp Data (" << tempData_.size() << " entries)\n";
    
    std::cout << "\n--- GlobalMap ---\n";
    globalMap_->dump();
    
    std::cout << "\n--- State ---\n";
    state_->dump();
    
    std::cout << "====================\n";
}

} // namespace Chtl