#ifndef CHTL_COMPILER_SYSTEM_H
#define CHTL_COMPILER_SYSTEM_H

#include <string>
#include <memory>
#include <functional>
#include <chrono>

// 包含所有组件头文件
#include "scanner/chtl_unified_scanner.h"
#include "dispatcher/compiler_dispatcher.h"
#include "compilers/chtl_compiler.h"
#include "compilers/chtl_js_compiler.h"
#include "compilers/css_compiler.h"
#include "compilers/javascript_compiler.h"
#include "merger/output_merger.h"

namespace chtl {

// CHTL编译系统配置
struct CHTLSystemConfig {
    ScannerConfig scannerConfig;
    DispatcherConfig dispatcherConfig;
    MergerConfig mergerConfig;
    
    // 输出配置
    bool writeToFile = true;
    std::string outputPath = "output.html";
    
    // 日志配置
    enum LogLevel {
        NONE,
        ERROR,
        WARNING,
        INFO,
        DEBUG,
        TRACE
    };
    LogLevel logLevel = INFO;
    std::string logFile;
};

// 编译结果
struct CHTLCompileResult {
    bool success;
    std::string outputHTML;
    std::string outputPath;
    
    // 错误和警告
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
    
    // 编译统计
    struct Stats {
        std::chrono::milliseconds totalTime;
        std::chrono::milliseconds scanTime;
        std::chrono::milliseconds compileTime;
        std::chrono::milliseconds mergeTime;
        
        size_t inputSize;
        size_t outputSize;
        size_t fragmentCount;
        size_t errorCount;
        size_t warningCount;
    } stats;
};

// 日志回调
using LogCallback = std::function<void(const std::string& level, const std::string& message)>;

// CHTL编译器系统主类
class CHTLCompilerSystem {
public:
    explicit CHTLCompilerSystem(const CHTLSystemConfig& config = CHTLSystemConfig());
    ~CHTLCompilerSystem();
    
    // 编译CHTL文件
    CHTLCompileResult compileFile(const std::string& inputPath);
    
    // 编译CHTL字符串
    CHTLCompileResult compileString(const std::string& chtlCode, 
                                   const std::string& sourceName = "<input>");
    
    // 批量编译
    std::vector<CHTLCompileResult> compileFiles(const std::vector<std::string>& inputPaths);
    
    // 设置日志回调
    void setLogCallback(LogCallback callback);
    
    // 获取各个组件的引用（用于详细配置）
    CHTLUnifiedScanner& getScanner();
    CompilerDispatcher& getDispatcher();
    OutputMerger& getMerger();
    
    // 获取版本信息
    static std::string getVersion();
    static std::string getBuildInfo();
    
private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

// 便捷函数：快速编译
CHTLCompileResult quickCompile(const std::string& inputPath);

} // namespace chtl

#endif // CHTL_COMPILER_SYSTEM_H