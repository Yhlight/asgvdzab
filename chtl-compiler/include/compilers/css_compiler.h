#ifndef CSS_COMPILER_H
#define CSS_COMPILER_H

#include "base_compiler.h"
#include <memory>

namespace chtl {

// CSS编译器 - 使用ANTLR处理全局CSS
class CSSCompiler : public BaseCompiler {
public:
    CSSCompiler();
    ~CSSCompiler() override;
    
    // 实现基类接口
    bool compile(const std::string& input, 
                std::string& output,
                std::vector<CompileError>& errors) override;
    
    void setOptions(const CompileOptions& options) override;
    std::string getName() const override { return "CSS Compiler (ANTLR)"; }
    std::string getVersion() const override { return "1.0.0"; }
    bool validate(const std::string& input) const override;
    
    // CSS特有配置
    struct CSSConfig {
        // CSS级别
        enum CSSLevel {
            CSS1,
            CSS2,
            CSS21,
            CSS3,
            CSS4
        };
        CSSLevel level = CSS3;
        
        // 是否启用CSS变量
        bool enableCSSVariables = true;
        // 是否启用嵌套规则
        bool enableNesting = false;
        // 是否启用厂商前缀
        bool enableVendorPrefixes = true;
        // 是否压缩输出
        bool compressOutput = false;
        // 是否保留注释
        bool preserveComments = false;
    };
    
    void setCSSConfig(const CSSConfig& config);
    
    // 获取使用的CSS特性
    struct CSSFeatures {
        bool usesFlexbox = false;
        bool usesGrid = false;
        bool usesVariables = false;
        bool usesCalc = false;
        bool usesTransforms = false;
        bool usesAnimations = false;
        std::vector<std::string> unknownProperties;
    };
    CSSFeatures getUsedFeatures() const;
    
private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace chtl

#endif // CSS_COMPILER_H