#pragma once

#include <string>
#include <memory>
#include <vector>
#include <stack>
#include "CHTLGlobalMap.hpp"
#include "CHTLState.hpp"

namespace chtl {

/**
 * 编译上下文
 * 管理编译过程中的所有状态和信息
 */
class CHTLContext {
public:
    CHTLContext();
    ~CHTLContext();
    
    // 文件管理
    void setCurrentFile(const std::string& filePath);
    const std::string& getCurrentFile() const { return currentFile_; }
    
    // 位置管理
    void setCurrentPosition(size_t line, size_t column);
    size_t getCurrentLine() const { return currentLine_; }
    size_t getCurrentColumn() const { return currentColumn_; }
    
    // 错误管理
    void addError(const std::string& message);
    void addWarning(const std::string& message);
    void addError(const std::string& message, size_t line, size_t column);
    void addWarning(const std::string& message, size_t line, size_t column);
    bool hasErrors() const { return !errors_.empty(); }
    const std::vector<std::string>& getErrors() const { return errors_; }
    const std::vector<std::string>& getWarnings() const { return warnings_; }
    
    // 获取全局映射
    CHTLGlobalMap& getGlobalMap() { return *globalMap_; }
    const CHTLGlobalMap& getGlobalMap() const { return *globalMap_; }
    
    // 获取状态管理器
    CHTLStateManager& getStateManager() { return *stateManager_; }
    const CHTLStateManager& getStateManager() const { return *stateManager_; }
    
    // 生成唯一ID（用于自动化类名等）
    std::string generateUniqueId(const std::string& prefix = "chtl");
    
    // 缩进管理（用于代码生成）
    void increaseIndent() { indentLevel_++; }
    void decreaseIndent() { if (indentLevel_ > 0) indentLevel_--; }
    int getIndentLevel() const { return indentLevel_; }
    std::string getIndentString() const;
    
    // 局部样式块管理
    void enterStyleBlock();
    void exitStyleBlock();
    bool isInStyleBlock() const { return styleBlockDepth_ > 0; }
    
    // 局部脚本块管理
    void enterScriptBlock();
    void exitScriptBlock();
    bool isInScriptBlock() const { return scriptBlockDepth_ > 0; }
    
    // 当前元素上下文
    void pushElement(const std::string& elementName);
    void popElement();
    const std::string& getCurrentElement() const;
    
    // 生成的CSS类管理
    void addGeneratedClass(const std::string& className);
    const std::vector<std::string>& getGeneratedClasses() const { return generatedClasses_; }
    
    // 调试模式
    void setDebugMode(bool debug) { debugMode_ = debug; }
    bool isDebugMode() const { return debugMode_; }
    
    // 重置上下文
    void reset();
    
private:
    std::string currentFile_;
    size_t currentLine_;
    size_t currentColumn_;
    
    std::vector<std::string> errors_;
    std::vector<std::string> warnings_;
    
    std::unique_ptr<CHTLGlobalMap> globalMap_;
    std::unique_ptr<CHTLStateManager> stateManager_;
    
    size_t uniqueIdCounter_;
    int indentLevel_;
    int styleBlockDepth_;
    int scriptBlockDepth_;
    
    std::stack<std::string> elementStack_;
    std::vector<std::string> generatedClasses_;
    
    bool debugMode_;
};

/**
 * RAII风格的作用域守卫
 */
class ScopeGuard {
public:
    ScopeGuard(CHTLContext& context, ScopeType type, const std::string& name = "")
        : context_(context) {
        context_.getGlobalMap().enterScope(type, name);
    }
    
    ~ScopeGuard() {
        context_.getGlobalMap().exitScope();
    }
    
private:
    CHTLContext& context_;
};

/**
 * RAII风格的状态守卫
 */
class StateGuard {
public:
    StateGuard(CHTLContext& context, LexerState state)
        : context_(context), isLexer_(true) {
        context_.getStateManager().pushLexerState(state);
    }
    
    StateGuard(CHTLContext& context, ParserState state)
        : context_(context), isLexer_(false) {
        context_.getStateManager().pushParserState(state);
    }
    
    ~StateGuard() {
        if (isLexer_) {
            context_.getStateManager().popLexerState();
        } else {
            context_.getStateManager().popParserState();
        }
    }
    
private:
    CHTLContext& context_;
    bool isLexer_;
};

} // namespace chtl