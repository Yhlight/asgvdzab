#pragma once

#include "../common/Types.h"
#include <string>
#include <vector>
#include <memory>

namespace CHTL {

/**
 * CSS编译器 - 基于ANTLR实现
 * 负责编译CSS代码片段，处理全局style相关的内容
 * 这是一个接口类，实际的ANTLR实现将在后续集成
 */
class CSSCompiler {
private:
    Configuration config;
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
    bool debug_mode;
    
    // CSS解析相关
    std::string global_css;
    std::vector<std::string> css_rules;
    
    // 内部方法
    void addError(const std::string& message);
    void addWarning(const std::string& message);
    
    // CSS验证
    bool validateCSS(const std::string& css_code) const;
    std::string normalizeCSSCode(const std::string& css_code) const;
    
    // CSS优化
    std::string optimizeCSS(const std::string& css_code) const;
    std::string minifyCSS(const std::string& css_code) const;
    
public:
    CSSCompiler();
    explicit CSSCompiler(const Configuration& cfg);
    
    // 设置配置
    void setConfiguration(const Configuration& cfg);
    
    // 主要编译方法
    CompileResultPtr compile(const std::string& css_code);
    
    // CSS规则管理
    void addCSSRule(const std::string& rule);
    void addGlobalCSS(const std::string& css);
    std::string getCompiledCSS() const;
    
    // CSS合并和优化
    std::string mergeCSS(const std::vector<std::string>& css_parts) const;
    std::string optimizeAndMinify(const std::string& css) const;
    
    // 错误和警告
    std::vector<std::string> getErrors() const { return errors; }
    std::vector<std::string> getWarnings() const { return warnings; }
    void clearErrors() { errors.clear(); }
    void clearWarnings() { warnings.clear(); }
    
    // 重置编译器状态
    void reset();
    
    // 调试方法
    void setDebugMode(bool enabled) { debug_mode = enabled; }
    void dumpCSS() const;
};

} // namespace CHTL