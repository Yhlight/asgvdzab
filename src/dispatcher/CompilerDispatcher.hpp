#pragma once

#include <string>
#include <memory>
#include <vector>
#include "scanner/CHTLUnifiedScanner.hpp"
#include "common/CodeFragment.hpp"

class CHTLCompiler;
class CHTLJSCompiler;
class CSSCompiler;
class JavaScriptCompiler;

/**
 * CompilerDispatcher - 编译器调度器
 * 
 * 负责协调各个编译器的工作：
 * - 使用CHTLUnifiedScanner切割代码
 * - 将不同类型的片段分发给对应的编译器
 * - 合并编译结果生成最终的HTML
 */
class CompilerDispatcher {
public:
    CompilerDispatcher(bool debugMode = false);
    ~CompilerDispatcher();
    
    /**
     * 编译CHTL文件
     * @param inputFile 输入文件路径
     * @param outputDir 输出目录
     * @return 是否编译成功
     */
    bool compile(const std::string& inputFile, const std::string& outputDir);
    
private:
    bool debugMode_;
    
    std::unique_ptr<CHTLUnifiedScanner> scanner_;
    std::unique_ptr<CHTLCompiler> chtlCompiler_;
    std::unique_ptr<CHTLJSCompiler> chtljsCompiler_;
    std::unique_ptr<CSSCompiler> cssCompiler_;
    std::unique_ptr<JavaScriptCompiler> jsCompiler_;
    
    /**
     * 读取文件内容
     */
    std::string readFile(const std::string& path);
    
    /**
     * 写入文件
     */
    bool writeFile(const std::string& path, const std::string& content);
    
    /**
     * 处理代码片段
     */
    std::string processFragment(const CodeFragment& fragment);
    
    /**
     * 合并编译结果
     */
    std::string mergeResults(const std::vector<std::string>& results);
    
    /**
     * 生成HTML输出
     */
    std::string generateHTML(const std::string& content);
};