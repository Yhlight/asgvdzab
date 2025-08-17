#pragma once

#include "../dispatcher/CompilerDispatcher.h"
#include "HTMLOutput.h"
#include <string>
#include <vector>
#include <memory>

namespace CHTL {

/**
 * 编译结果合并器
 * 负责将各个编译器的输出合并成最终的HTML文件
 */
class ResultMerger {
public:
    ResultMerger();
    ~ResultMerger() = default;

    /**
     * 合并编译结果
     * @param results 编译结果列表
     * @param outputFilename 输出文件名
     * @return 合并后的HTML内容
     */
    std::string merge(const CompilationResults& results, const std::string& outputFilename);

    /**
     * 设置调试模式
     * @param debug 是否启用调试模式
     */
    void setDebugMode(bool debug) { debugMode_ = debug; }

    /**
     * 获取合并过程中的错误
     * @return 错误信息列表
     */
    const std::vector<std::string>& getErrors() const { return errors_; }

    /**
     * 获取合并过程中的警告
     * @return 警告信息列表
     */
    const std::vector<std::string>& getWarnings() const { return warnings_; }

private:
    bool debugMode_;
    std::vector<std::string> errors_;
    std::vector<std::string> warnings_;

    // HTML输出构建器
    std::unique_ptr<HTMLOutput> htmlOutput_;

    // 合并不同类型的结果
    void mergeHTMLContent(const CompilationResults& results);
    void mergeCSSContent(const CompilationResults& results);
    void mergeJSContent(const CompilationResults& results);

    // 错误处理
    void recordError(const std::string& message);
    void recordWarning(const std::string& message);
    void debugOutput(const std::string& message);
};

} // namespace CHTL