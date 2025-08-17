#include "../../include/merger/ResultMerger.h"
#include "../../include/common/Utils.h"
#include <iostream>
#include <sstream>
#include <regex>

namespace CHTL {

ResultMerger::ResultMerger() : debug_mode(false) {
    // 设置默认的HTML模板
    html_template = R"(<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>CHTL Generated Page</title>
    <style>
        {{CSS_CONTENT}}
    </style>
</head>
<body>
    {{HTML_CONTENT}}
    <script>
        {{JS_CONTENT}}
    </script>
</body>
</html>)";
}

ResultMerger::ResultMerger(const Configuration& cfg) : config(cfg), debug_mode(false) {
    // 设置默认的HTML模板
    html_template = R"(<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>CHTL Generated Page</title>
    <style>
        {{CSS_CONTENT}}
    </style>
</head>
<body>
    {{HTML_CONTENT}}
    <script>
        {{JS_CONTENT}}
    </script>
</body>
</html>)";
}

void ResultMerger::setConfiguration(const Configuration& cfg) {
    config = cfg;
}

void ResultMerger::setHTMLTemplate(const std::string& template_html) {
    html_template = template_html;
}

CompileResultPtr ResultMerger::mergeResults(const std::vector<CompileResultPtr>& results) {
    auto merged_result = std::make_shared<CompileResult>();
    
    try {
        // 收集所有错误和警告
        collectErrors(results);
        
        // 按类型分类结果
        std::vector<CompileResultPtr> chtl_results;
        std::vector<CompileResultPtr> chtl_js_results;
        std::vector<CompileResultPtr> css_results;
        std::vector<CompileResultPtr> js_results;
        
        for (const auto& result : results) {
            if (!result || !result->success) {
                continue; // 跳过失败的编译结果
            }
            
            // 根据输出内容判断类型（简化判断）
            std::string output = result->output;
            if (output.find("<") != std::string::npos && output.find(">") != std::string::npos) {
                chtl_results.push_back(result);
            } else if (output.find("{{") != std::string::npos || output.find("->") != std::string::npos) {
                chtl_js_results.push_back(result);
            } else if (output.find("{") != std::string::npos && output.find(":") != std::string::npos) {
                css_results.push_back(result);
            } else {
                js_results.push_back(result);
            }
        }
        
        // 分类合并
        merged_result = mergeByType(chtl_results, chtl_js_results, css_results, js_results);
        
    } catch (const std::exception& e) {
        merged_result->success = false;
        merged_result->errors.push_back("合并异常: " + std::string(e.what()));
    }
    
    return merged_result;
}

CompileResultPtr ResultMerger::mergeByType(
    const std::vector<CompileResultPtr>& chtl_results,
    const std::vector<CompileResultPtr>& chtl_js_results,
    const std::vector<CompileResultPtr>& css_results,
    const std::vector<CompileResultPtr>& js_results) {
    
    auto result = std::make_shared<CompileResult>();
    
    try {
        // 合并HTML内容
        merged_html = mergeHTMLContent(chtl_results);
        
        // 合并CSS内容
        merged_css = mergeCSSContent(css_results);
        
        // 合并JavaScript内容
        std::vector<CompileResultPtr> all_js_results = chtl_js_results;
        all_js_results.insert(all_js_results.end(), js_results.begin(), js_results.end());
        merged_js = mergeJSContent(all_js_results);
        
        // 构建最终的HTML结构
        std::string final_html = buildHTMLStructure(merged_html);
        final_html = injectCSS(final_html, merged_css);
        final_html = injectJS(final_html, merged_js);
        
        // 优化最终输出
        final_html = optimizeHTML(final_html);
        
        result->success = errors.empty();
        result->output = final_html;
        result->errors = errors;
        result->warnings = warnings;
        
        if (debug_mode) {
            Utils::debugLog("RESULT_MERGER", "合并完成，最终HTML长度: " + std::to_string(final_html.length()));
        }
        
    } catch (const std::exception& e) {
        result->success = false;
        result->errors.push_back("分类合并异常: " + std::string(e.what()));
    }
    
    return result;
}

std::string ResultMerger::mergeHTMLContent(const std::vector<CompileResultPtr>& chtl_results) {
    std::stringstream html;
    
    for (const auto& result : chtl_results) {
        if (result && result->success && !result->output.empty()) {
            html << result->output << "\n";
        }
    }
    
    std::string merged = html.str();
    
    if (debug_mode) {
        Utils::debugLog("RESULT_MERGER", "合并HTML内容，长度: " + std::to_string(merged.length()));
    }
    
    return merged;
}

std::string ResultMerger::mergeCSSContent(const std::vector<CompileResultPtr>& css_results) {
    std::stringstream css;
    
    for (const auto& result : css_results) {
        if (result && result->success && !result->output.empty()) {
            css << result->output << "\n";
        }
    }
    
    std::string merged = css.str();
    
    if (debug_mode) {
        Utils::debugLog("RESULT_MERGER", "合并CSS内容，长度: " + std::to_string(merged.length()));
    }
    
    return merged;
}

std::string ResultMerger::mergeJSContent(const std::vector<CompileResultPtr>& js_results) {
    std::stringstream js;
    
    // 添加CHTL JS运行时支持代码
    js << "// CHTL JS运行时支持\n";
    js << "(function() {\n";
    js << "  'use strict';\n";
    js << "  \n";
    js << "  // CHTL增强选择器支持\n";
    js << "  window.CHTL = window.CHTL || {};\n";
    js << "  \n";
    js << "  // 事件委托管理器\n";
    js << "  CHTL.delegationManager = {\n";
    js << "    delegates: {},\n";
    js << "    register: function(parent, child, event, handler) {\n";
    js << "      const key = parent + '::' + event;\n";
    js << "      if (!this.delegates[key]) {\n";
    js << "        this.delegates[key] = [];\n";
    js << "        document.addEventListener(event, (e) => {\n";
    js << "          const parentEl = e.target.closest(parent);\n";
    js << "          if (parentEl) {\n";
    js << "            this.delegates[key].forEach(delegate => {\n";
    js << "              if (e.target.matches(delegate.child)) {\n";
    js << "                delegate.handler(e);\n";
    js << "              }\n";
    js << "            });\n";
    js << "          }\n";
    js << "        });\n";
    js << "      }\n";
    js << "      this.delegates[key].push({ child, handler });\n";
    js << "    }\n";
    js << "  };\n";
    js << "  \n";
    js << "  // 动画管理器\n";
    js << "  CHTL.animationManager = {\n";
    js << "    animations: [],\n";
    js << "    create: function(config) {\n";
    js << "      return new Promise((resolve) => {\n";
    js << "        // TODO: 实现完整的动画逻辑\n";
    js << "        resolve();\n";
    js << "      });\n";
    js << "    }\n";
    js << "  };\n";
    js << "})();\n\n";
    
    // 合并用户的JS代码
    for (const auto& result : js_results) {
        if (result && result->success && !result->output.empty()) {
            js << result->output << "\n";
        }
    }
    
    std::string merged = js.str();
    
    if (debug_mode) {
        Utils::debugLog("RESULT_MERGER", "合并JS内容，长度: " + std::to_string(merged.length()));
    }
    
    return merged;
}

std::string ResultMerger::buildHTMLStructure(const std::string& body_content) {
    std::string html = html_template;
    
    // 替换HTML内容占位符
    html = Utils::replace(html, "{{HTML_CONTENT}}", body_content);
    
    return html;
}

std::string ResultMerger::injectCSS(const std::string& html, const std::string& css) {
    std::string result = html;
    
    // 替换CSS内容占位符
    result = Utils::replace(result, "{{CSS_CONTENT}}", css);
    
    return result;
}

std::string ResultMerger::injectJS(const std::string& html, const std::string& js) {
    std::string result = html;
    
    // 替换JS内容占位符
    result = Utils::replace(result, "{{JS_CONTENT}}", js);
    
    return result;
}

std::string ResultMerger::optimizeHTML(const std::string& html) const {
    std::string optimized = html;
    
    // 移除HTML注释（保留CHTL生成器注释）
    optimized = std::regex_replace(optimized, std::regex(R"(<!--(?!.*CHTL).*?-->)"), "");
    
    // 移除多余的空行
    optimized = std::regex_replace(optimized, std::regex(R"(\n\s*\n)"), "\n");
    
    return Utils::trim(optimized);
}

std::string ResultMerger::formatHTML(const std::string& html) const {
    std::string formatted = html;
    
    // 基本的HTML格式化
    // 在标签后添加换行
    formatted = std::regex_replace(formatted, std::regex(R"(>\s*<)"), ">\n<");
    
    // 简单的缩进处理
    std::vector<std::string> lines = Utils::split(formatted, '\n');
    std::stringstream result;
    int indent_level = 0;
    
    for (const auto& line : lines) {
        std::string trimmed_line = Utils::trim(line);
        if (trimmed_line.empty()) continue;
        
        // 检查是否为结束标签
        if (trimmed_line.find("</") == 0) {
            indent_level--;
        }
        
        // 添加缩进
        for (int i = 0; i < indent_level; ++i) {
            result << "  ";
        }
        result << trimmed_line << "\n";
        
        // 检查是否为开始标签
        if (trimmed_line.find("<") == 0 && trimmed_line.find("</") != 0 && 
            trimmed_line.find("/>") == std::string::npos) {
            indent_level++;
        }
    }
    
    return result.str();
}

void ResultMerger::collectErrors(const std::vector<CompileResultPtr>& results) {
    for (const auto& result : results) {
        if (result) {
            errors.insert(errors.end(), result->errors.begin(), result->errors.end());
            warnings.insert(warnings.end(), result->warnings.begin(), result->warnings.end());
        }
    }
}

void ResultMerger::addError(const std::string& message) {
    errors.push_back("结果合并器错误: " + message);
    
    if (debug_mode) {
        Utils::debugLog("RESULT_MERGER_ERROR", message);
    }
}

void ResultMerger::addWarning(const std::string& message) {
    warnings.push_back("结果合并器警告: " + message);
    
    if (debug_mode) {
        Utils::debugLog("RESULT_MERGER_WARNING", message);
    }
}

void ResultMerger::setMinifyOutput(bool minify) {
    config.debug_mode = !minify; // 简化处理
}

void ResultMerger::setFormatOutput(bool format) {
    // 在配置中记录格式化选项
    // 这里可以扩展配置结构来支持更多选项
}

void ResultMerger::setIncludeComments(bool include) {
    // 在配置中记录注释选项
}

void ResultMerger::reset() {
    merged_html.clear();
    merged_css.clear();
    merged_js.clear();
    errors.clear();
    warnings.clear();
}

void ResultMerger::dumpMergeResults() const {
    std::cout << "=== 合并结果统计 ===" << std::endl;
    std::cout << "HTML长度: " << merged_html.length() << std::endl;
    std::cout << "CSS长度: " << merged_css.length() << std::endl;
    std::cout << "JS长度: " << merged_js.length() << std::endl;
    std::cout << "错误数量: " << errors.size() << std::endl;
    std::cout << "警告数量: " << warnings.size() << std::endl;
    
    if (!errors.empty()) {
        std::cout << "\n错误信息:" << std::endl;
        for (const auto& error : errors) {
            std::cout << "  " << error << std::endl;
        }
    }
    
    if (!warnings.empty()) {
        std::cout << "\n警告信息:" << std::endl;
        for (const auto& warning : warnings) {
            std::cout << "  " << warning << std::endl;
        }
    }
    
    if (debug_mode) {
        std::cout << "\n=== 详细内容 ===" << std::endl;
        
        if (!merged_html.empty()) {
            std::cout << "\nHTML内容:\n" << merged_html << std::endl;
        }
        
        if (!merged_css.empty()) {
            std::cout << "\nCSS内容:\n" << merged_css << std::endl;
        }
        
        if (!merged_js.empty()) {
            std::cout << "\nJS内容:\n" << merged_js << std::endl;
        }
    }
    
    std::cout << "==================" << std::endl;
}

void ResultMerger::setDebugMode(bool enabled) {
    debug_mode = enabled;
}

} // namespace CHTL