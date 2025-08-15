#include "chtl_compiler.h"
#include <fstream>
#include <sstream>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <ctime>

namespace chtl {

// CHTLCompilerSystem实现类
class CHTLCompilerSystem::Impl {
public:
    explicit Impl(const CHTLSystemConfig& config) 
        : config_(config),
          scanner_(config.scannerConfig),
          dispatcher_(config.dispatcherConfig),
          merger_(config.mergerConfig) {
        
        // 设置进度回调
        dispatcher_.setProgressCallback([this](size_t current, size_t total, const std::string& message) {
            if (logCallback_) {
                std::stringstream ss;
                ss << "进度: " << current << "/" << total << " - " << message;
                logCallback_("INFO", ss.str());
            }
        });
    }
    
    CHTLCompileResult compileFile(const std::string& inputPath) {
        auto startTime = std::chrono::high_resolution_clock::now();
        CHTLCompileResult result;
        result.outputPath = config_.outputPath;
        
        log("INFO", "开始编译文件: " + inputPath);
        
        try {
            // 读取文件
            std::ifstream file(inputPath);
            if (!file.is_open()) {
                result.success = false;
                result.errors.push_back("无法打开文件: " + inputPath);
                log("ERROR", "无法打开文件: " + inputPath);
                return result;
            }
            
            std::stringstream buffer;
            buffer << file.rdbuf();
            std::string sourceCode = buffer.str();
            result.stats.inputSize = sourceCode.size();
            
            // 编译
            return compileInternal(sourceCode, inputPath, startTime);
            
        } catch (const std::exception& e) {
            result.success = false;
            result.errors.push_back("编译异常: " + std::string(e.what()));
            log("ERROR", "编译异常: " + std::string(e.what()));
            return result;
        }
    }
    
    CHTLCompileResult compileString(const std::string& chtlCode, 
                                   const std::string& sourceName) {
        auto startTime = std::chrono::high_resolution_clock::now();
        CHTLCompileResult result;
        result.stats.inputSize = chtlCode.size();
        
        log("INFO", "开始编译字符串: " + sourceName);
        
        return compileInternal(chtlCode, sourceName, startTime);
    }
    
    std::vector<CHTLCompileResult> compileFiles(const std::vector<std::string>& inputPaths) {
        std::vector<CHTLCompileResult> results;
        
        for (const auto& path : inputPaths) {
            results.push_back(compileFile(path));
        }
        
        return results;
    }
    
    void setLogCallback(LogCallback callback) {
        logCallback_ = callback;
    }
    
    CHTLUnifiedScanner& getScanner() { return scanner_; }
    CompilerDispatcher& getDispatcher() { return dispatcher_; }
    OutputMerger& getMerger() { return merger_; }

private:
    CHTLCompileResult compileInternal(const std::string& sourceCode,
                                     const std::string& sourceName,
                                     std::chrono::high_resolution_clock::time_point startTime) {
        CHTLCompileResult result;
        
        // 1. 扫描阶段
        auto scanStart = std::chrono::high_resolution_clock::now();
        log("INFO", "开始扫描代码...");
        
        auto fragments = scanner_.scan(sourceCode, sourceName);
        if (fragments.empty() && !scanner_.getLastError().empty()) {
            result.success = false;
            result.errors.push_back("扫描错误: " + scanner_.getLastError());
            log("ERROR", "扫描错误: " + scanner_.getLastError());
            return result;
        }
        
        auto scanEnd = std::chrono::high_resolution_clock::now();
        result.stats.scanTime = std::chrono::duration_cast<std::chrono::milliseconds>(scanEnd - scanStart);
        result.stats.fragmentCount = fragments.size();
        
        log("INFO", "扫描完成，共 " + std::to_string(fragments.size()) + " 个片段");
        
        // 2. 编译阶段
        auto compileStart = std::chrono::high_resolution_clock::now();
        log("INFO", "开始编译片段...");
        
        auto compileResults = dispatcher_.dispatch(fragments);
        
        auto compileEnd = std::chrono::high_resolution_clock::now();
        result.stats.compileTime = std::chrono::duration_cast<std::chrono::milliseconds>(compileEnd - compileStart);
        
        // 收集编译错误和警告
        for (const auto& cr : compileResults) {
            if (!cr.success) {
                result.errors.push_back(cr.errorMessage);
                result.stats.errorCount++;
            }
            
            result.stats.warningCount += cr.info.warningCount;
            for (const auto& warning : cr.info.warnings) {
                result.warnings.push_back(warning);
            }
        }
        
        if (!result.errors.empty()) {
            result.success = false;
            log("ERROR", "编译失败，共 " + std::to_string(result.errors.size()) + " 个错误");
            return result;
        }
        
        log("INFO", "编译完成");
        
        // 3. 合并阶段
        auto mergeStart = std::chrono::high_resolution_clock::now();
        log("INFO", "开始合并输出...");
        
        result.outputHTML = merger_.merge(compileResults);
        
        auto mergeEnd = std::chrono::high_resolution_clock::now();
        result.stats.mergeTime = std::chrono::duration_cast<std::chrono::milliseconds>(mergeEnd - mergeStart);
        result.stats.outputSize = result.outputHTML.size();
        
        // 验证输出
        std::vector<std::string> validationErrors;
        if (!merger_.validateOutput(result.outputHTML, validationErrors)) {
            for (const auto& error : validationErrors) {
                result.warnings.push_back("HTML验证警告: " + error);
            }
        }
        
        log("INFO", "合并完成，输出大小: " + std::to_string(result.stats.outputSize) + " 字节");
        
        // 4. 写入文件（如果配置）
        if (config_.writeToFile && !config_.outputPath.empty()) {
            std::ofstream outFile(config_.outputPath);
            if (outFile.is_open()) {
                outFile << result.outputHTML;
                outFile.close();
                result.outputPath = config_.outputPath;
                log("INFO", "已写入文件: " + config_.outputPath);
            } else {
                result.warnings.push_back("无法写入输出文件: " + config_.outputPath);
                log("WARNING", "无法写入输出文件: " + config_.outputPath);
            }
        }
        
        // 计算总时间
        auto endTime = std::chrono::high_resolution_clock::now();
        result.stats.totalTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        
        result.success = true;
        log("INFO", "编译成功，总用时: " + std::to_string(result.stats.totalTime.count()) + " ms");
        
        return result;
    }
    
    void log(const std::string& level, const std::string& message) {
        // 检查日志级别
        CHTLSystemConfig::LogLevel currentLevel = CHTLSystemConfig::INFO;
        if (level == "ERROR") currentLevel = CHTLSystemConfig::ERROR;
        else if (level == "WARNING") currentLevel = CHTLSystemConfig::WARNING;
        else if (level == "DEBUG") currentLevel = CHTLSystemConfig::DEBUG;
        else if (level == "TRACE") currentLevel = CHTLSystemConfig::TRACE;
        
        if (currentLevel > config_.logLevel) {
            return;
        }
        
        // 输出到回调
        if (logCallback_) {
            logCallback_(level, message);
        }
        
        // 输出到文件
        if (!config_.logFile.empty()) {
            std::ofstream logFile(config_.logFile, std::ios::app);
            if (logFile.is_open()) {
                auto now = std::chrono::system_clock::now();
                auto time_t = std::chrono::system_clock::to_time_t(now);
                logFile << "[" << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S") 
                       << "] [" << level << "] " << message << "\n";
            }
        }
    }

private:
    CHTLSystemConfig config_;
    LogCallback logCallback_;
    
    CHTLUnifiedScanner scanner_;
    CompilerDispatcher dispatcher_;
    OutputMerger merger_;
};

// CHTLCompilerSystem 实现
CHTLCompilerSystem::CHTLCompilerSystem(const CHTLSystemConfig& config)
    : pImpl(std::make_unique<Impl>(config)) {}

CHTLCompilerSystem::~CHTLCompilerSystem() = default;

CHTLCompileResult CHTLCompilerSystem::compileFile(const std::string& inputPath) {
    return pImpl->compileFile(inputPath);
}

CHTLCompileResult CHTLCompilerSystem::compileString(const std::string& chtlCode, 
                                                   const std::string& sourceName) {
    return pImpl->compileString(chtlCode, sourceName);
}

std::vector<CHTLCompileResult> CHTLCompilerSystem::compileFiles(const std::vector<std::string>& inputPaths) {
    return pImpl->compileFiles(inputPaths);
}

void CHTLCompilerSystem::setLogCallback(LogCallback callback) {
    pImpl->setLogCallback(callback);
}

CHTLUnifiedScanner& CHTLCompilerSystem::getScanner() {
    return pImpl->getScanner();
}

CompilerDispatcher& CHTLCompilerSystem::getDispatcher() {
    return pImpl->getDispatcher();
}

OutputMerger& CHTLCompilerSystem::getMerger() {
    return pImpl->getMerger();
}

std::string CHTLCompilerSystem::getVersion() {
    return "1.0.0";
}

std::string CHTLCompilerSystem::getBuildInfo() {
    return "CHTL Compiler v1.0.0 - Built with C++17";
}

// 便捷函数实现
CHTLCompileResult quickCompile(const std::string& inputPath) {
    CHTLSystemConfig config;
    config.outputPath = inputPath + ".html";
    
    CHTLCompilerSystem compiler(config);
    return compiler.compileFile(inputPath);
}

} // namespace chtl