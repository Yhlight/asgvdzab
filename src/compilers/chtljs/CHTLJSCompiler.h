#pragma once

#include "../../scanner/CodeFragment.h"
#include "../../dispatcher/CompilerDispatcher.h"
#include <string>
#include <vector>
#include <memory>

namespace CHTL {

/**
 * CHTL JS编译器 - 手写实现
 * 负责编译CHTL JS语法到JavaScript
 */
class CHTLJSCompiler {
public:
    CHTLJSCompiler();
    ~CHTLJSCompiler() = default;

    /**
     * 编译CHTL JS片段
     * @param fragment CHTL JS代码片段
     * @return 编译结果
     */
    CompilationResultPtr compile(const FragmentPtr& fragment);

    /**
     * 批量编译CHTL JS片段
     * @param fragments CHTL JS代码片段列表
     * @return 编译结果列表
     */
    CompilationResults compileAll(const FragmentList& fragments);

    /**
     * 设置调试模式
     * @param debug 是否启用调试模式
     */
    void setDebugMode(bool debug) { debugMode_ = debug; }

    /**
     * 设置编译选项
     * @param options 编译选项
     */
    void setOptions(const std::unordered_map<std::string, std::string>& options);

private:
    bool debugMode_;
    std::unordered_map<std::string, std::string> options_;
    std::vector<std::string> errors_;
    std::vector<std::string> warnings_;

    // 编译单个CHTL JS片段的具体实现
    std::string compileFragment(const std::string& content, const std::string& context);
    
    // 错误处理
    void recordError(const std::string& message);
    void recordWarning(const std::string& message);
    void debugOutput(const std::string& message);
};

} // namespace CHTL