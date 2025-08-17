#pragma once

#include "../scanner/CodeFragment.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace CHTL {

// 前向声明
class CHTLCompiler;
class CHTLJSCompiler;
class CSSCompiler;
class JSCompiler;
class ResultMerger;

/**
 * 编译结果结构
 */
struct CompilationResult {
    bool success;                           // 编译是否成功
    std::string output;                     // 编译输出
    std::vector<std::string> errors;        // 错误信息
    std::vector<std::string> warnings;      // 警告信息
    FragmentType sourceType;                // 源片段类型
    std::string context;                    // 编译上下文
    
    CompilationResult() : success(false), sourceType(FragmentType::UNKNOWN) {}
    
    CompilationResult(bool s, const std::string& out, FragmentType type) 
        : success(s), output(out), sourceType(type) {}
};

using CompilationResultPtr = std::shared_ptr<CompilationResult>;
using CompilationResults = std::vector<CompilationResultPtr>;

/**
 * 编译器调度器
 * 负责根据代码片段类型调度相应的编译器进行编译
 */
class CompilerDispatcher {
public:
    CompilerDispatcher();
    ~CompilerDispatcher();

    /**
     * 编译代码片段列表
     * @param fragments 代码片段列表
     * @param outputFilename 输出文件名
     * @return 编译是否成功
     */
    bool compile(const FragmentList& fragments, const std::string& outputFilename);

    /**
     * 获取编译结果
     * @return 编译结果列表
     */
    const CompilationResults& getResults() const { return results_; }

    /**
     * 获取最终HTML输出
     * @return HTML字符串
     */
    const std::string& getFinalOutput() const { return finalOutput_; }

    /**
     * 获取所有错误信息
     * @return 错误信息列表
     */
    std::vector<std::string> getAllErrors() const;

    /**
     * 获取所有警告信息
     * @return 警告信息列表
     */
    std::vector<std::string> getAllWarnings() const;

    /**
     * 设置调试模式
     * @param debug 是否启用调试模式
     */
    void setDebugMode(bool debug);

    /**
     * 清除之前的编译结果
     */
    void clearResults();

    /**
     * 设置编译选项
     * @param options 编译选项映射
     */
    void setCompilerOptions(const std::unordered_map<std::string, std::string>& options);

private:
    // 各个编译器实例
    std::unique_ptr<CHTLCompiler> chtlCompiler_;
    std::unique_ptr<CHTLJSCompiler> chtljsCompiler_;
    std::unique_ptr<CSSCompiler> cssCompiler_;
    std::unique_ptr<JSCompiler> jsCompiler_;
    std::unique_ptr<ResultMerger> resultMerger_;
    
    // 编译结果
    CompilationResults results_;
    std::string finalOutput_;
    
    // 配置
    bool debugMode_;
    std::unordered_map<std::string, std::string> compilerOptions_;
    
    // 分类的片段
    FragmentList chtlFragments_;
    FragmentList chtljsFragments_;
    FragmentList cssFragments_;
    FragmentList jsFragments_;
    FragmentList htmlFragments_;

    /**
     * 初始化编译器
     */
    void initializeCompilers();

    /**
     * 按类型分类代码片段
     * @param fragments 输入的代码片段列表
     */
    void categorizeFragments(const FragmentList& fragments);

    /**
     * 编译CHTL片段
     * @return 编译是否成功
     */
    bool compileCHTLFragments();

    /**
     * 编译CHTL JS片段
     * @return 编译是否成功
     */
    bool compileCHTLJSFragments();

    /**
     * 编译CSS片段
     * @return 编译是否成功
     */
    bool compileCSSFragments();

    /**
     * 编译JavaScript片段
     * @return 编译是否成功
     */
    bool compileJSFragments();

    /**
     * 处理HTML片段（原始嵌入）
     * @return 处理是否成功
     */
    bool processHTMLFragments();

    /**
     * 合并编译结果
     * @param outputFilename 输出文件名
     * @return 合并是否成功
     */
    bool mergeResults(const std::string& outputFilename);

    /**
     * 添加编译结果
     * @param result 编译结果
     */
    void addResult(CompilationResultPtr result);

    /**
     * 记录错误
     * @param message 错误信息
     * @param context 上下文
     */
    void recordError(const std::string& message, const std::string& context = "");

    /**
     * 记录警告
     * @param message 警告信息
     * @param context 上下文
     */
    void recordWarning(const std::string& message, const std::string& context = "");

    /**
     * 调试输出
     * @param message 调试信息
     */
    void debugOutput(const std::string& message);

    /**
     * 验证片段依赖关系
     * @param fragments 片段列表
     * @return 验证是否通过
     */
    bool validateFragmentDependencies(const FragmentList& fragments);

    /**
     * 优化编译顺序
     * @param fragments 片段列表
     * @return 优化后的片段列表
     */
    FragmentList optimizeCompilationOrder(const FragmentList& fragments);
};

} // namespace CHTL