#ifndef JAVASCRIPT_COMPILER_H
#define JAVASCRIPT_COMPILER_H

#include "base_compiler.h"
#include <memory>

namespace chtl {

// JavaScript编译器 - 使用ANTLR处理纯JavaScript
class JavaScriptCompiler : public BaseCompiler {
public:
    JavaScriptCompiler();
    ~JavaScriptCompiler() override;
    
    // 实现基类接口
    bool compile(const std::string& input, 
                std::string& output,
                std::vector<CompileError>& errors) override;
    
    void setOptions(const CompileOptions& options) override;
    std::string getName() const override { return "JavaScript Compiler (ANTLR)"; }
    std::string getVersion() const override { return "1.0.0"; }
    bool validate(const std::string& input) const override;
    
    // JavaScript特有配置
    struct JSConfig {
        // ECMAScript版本
        enum ECMAVersion {
            ES5,
            ES6,
            ES2017,
            ES2018,
            ES2019,
            ES2020,
            ES2021,
            ES2022,
            ESNext
        };
        ECMAVersion version = ES2022;
        
        // 是否启用严格模式
        bool strictMode = true;
        // 是否转译到ES5
        bool transpileToES5 = false;
        // 是否启用类型检查（基础）
        bool enableTypeChecking = false;
        // 是否压缩输出
        bool minifyOutput = false;
        // 是否生成source map
        bool generateSourceMap = false;
    };
    
    void setJSConfig(const JSConfig& config);
    
    // 获取使用的JavaScript特性
    struct JSFeatures {
        bool usesAsync = false;
        bool usesGenerators = false;
        bool usesClasses = false;
        bool usesArrowFunctions = false;
        bool usesDestructuring = false;
        bool usesModules = false;
        bool usesPromises = false;
        std::vector<std::string> globalVariables;
    };
    JSFeatures getUsedFeatures() const;
    
private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace chtl

#endif // JAVASCRIPT_COMPILER_H