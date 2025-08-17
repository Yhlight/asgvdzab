#include "compiler/compiler_dispatcher.hpp"
#include <chrono>
#include <algorithm>
#include <sstream>

namespace CHTL {

// CompilerDispatcher 实现
CompilerDispatcher::CompilerDispatcher(const CompilerConfig& config) 
    : config_(config) {
}

void CompilerDispatcher::registerCHTLCompiler(std::unique_ptr<ICHTLCompiler> compiler) {
    chtlCompiler_ = std::move(compiler);
}

void CompilerDispatcher::registerCHTLJSCompiler(std::unique_ptr<ICHTLJSCompiler> compiler) {
    chtlJSCompiler_ = std::move(compiler);
}

void CompilerDispatcher::registerCSSCompiler(std::unique_ptr<ICSSCompiler> compiler) {
    cssCompiler_ = std::move(compiler);
}

void CompilerDispatcher::registerJavaScriptCompiler(std::unique_ptr<IJavaScriptCompiler> compiler) {
    jsCompiler_ = std::move(compiler);
}

CompileResult CompilerDispatcher::compile(const std::vector<CodeFragment>& fragments, const std::string& filename) {
    auto start = std::chrono::high_resolution_clock::now();
    
    // 创建编译管道
    CompilationPipeline pipeline = createPipeline(fragments, filename);
    
    // 预处理片段
    preprocessFragments(pipeline);
    
    // 分发片段到对应编译器
    dispatchFragments(pipeline);
    
    // 后处理结果
    postprocessResults(pipeline);
    
    // 生成最终结果
    CompileResult result = generateFinalResult(pipeline);
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    if (config_.debugMode) {
        result.output += "\n<!-- 编译时间: " + std::to_string(duration.count()) + "ms -->";
    }
    
    return result;
}

void CompilerDispatcher::setConfig(const CompilerConfig& config) {
    config_ = config;
    
    // 更新所有编译器的配置
    if (chtlCompiler_) chtlCompiler_->setConfig(config);
    if (chtlJSCompiler_) chtlJSCompiler_->setConfig(config);
    if (cssCompiler_) cssCompiler_->setConfig(config);
    if (jsCompiler_) jsCompiler_->setConfig(config);
}

CompilationPipeline CompilerDispatcher::createPipeline(const std::vector<CodeFragment>& fragments, const std::string& filename) {
    CompilationPipeline pipeline;
    pipeline.fragments = fragments;
    pipeline.config = config_;
    pipeline.sourceFilename = filename;
    
    return pipeline;
}

void CompilerDispatcher::preprocessFragments(CompilationPipeline& pipeline) {
    // 对片段进行排序和分组
    sortFragmentsByLocation(pipeline.fragments);
    
    // 更新全局上下文
    updateGlobalContext(pipeline);
    
    // 验证片段顺序
    validateFragmentOrder(pipeline.fragments);
}

void CompilerDispatcher::dispatchFragments(CompilationPipeline& pipeline) {
    // 分组片段
    FragmentGroup groups = groupFragments(pipeline.fragments);
    
    // 编译各类型片段
    if (!groups.chtlFragments.empty()) {
        CompileResult result = compileCHTLFragments(groups.chtlFragments, pipeline);
        mergeCompileResult(result, pipeline);
    }
    
    if (!groups.chtlJSFragments.empty()) {
        CompileResult result = compileCHTLJSFragments(groups.chtlJSFragments, pipeline);
        mergeCompileResult(result, pipeline);
    }
    
    if (!groups.cssFragments.empty()) {
        CompileResult result = compileCSSFragments(groups.cssFragments, pipeline);
        mergeCompileResult(result, pipeline);
    }
    
    if (!groups.jsFragments.empty()) {
        CompileResult result = compileJavaScriptFragments(groups.jsFragments, pipeline);
        mergeCompileResult(result, pipeline);
    }
    
    // HTML片段直接添加到输出
    for (const auto& fragment : groups.htmlFragments) {
        pipeline.htmlOutput += fragment.content;
    }
}

void CompilerDispatcher::postprocessResults(CompilationPipeline& pipeline) {
    // 优化输出
    optimizeOutput(pipeline);
    
    // 解析依赖
    resolveDependencies(pipeline);
    
    if (config_.debugMode) {
        dumpPipelineState(pipeline, "postprocess");
        validatePipelineIntegrity(pipeline);
    }
}

CompileResult CompilerDispatcher::generateFinalResult(const CompilationPipeline& pipeline) {
    CompileResult result;
    
    // 合并所有错误和警告
    result.errors.insert(result.errors.end(), pipeline.errors.begin(), pipeline.errors.end());
    result.warnings.insert(result.warnings.end(), pipeline.warnings.begin(), pipeline.warnings.end());
    
    // 生成HTML输出
    result.output = generateHTMLOutput(pipeline);
    
    // 设置成功状态
    result.success = !pipeline.hasErrors;
    
    return result;
}

CompilerDispatcher::FragmentGroup CompilerDispatcher::groupFragments(const std::vector<CodeFragment>& fragments) {
    FragmentGroup group;
    
    for (const auto& fragment : fragments) {
        switch (fragment.type) {
            case FragmentType::CHTL:
                group.chtlFragments.push_back(fragment);
                break;
            case FragmentType::CHTL_JS:
                group.chtlJSFragments.push_back(fragment);
                break;
            case FragmentType::CSS:
                group.cssFragments.push_back(fragment);
                break;
            case FragmentType::JAVASCRIPT:
                group.jsFragments.push_back(fragment);
                break;
            case FragmentType::HTML:
                group.htmlFragments.push_back(fragment);
                break;
            case FragmentType::RAW:
                // 原始片段作为HTML处理
                group.htmlFragments.push_back(fragment);
                break;
        }
    }
    
    return group;
}

void CompilerDispatcher::sortFragmentsByLocation(std::vector<CodeFragment>& fragments) {
    std::sort(fragments.begin(), fragments.end(), 
        [](const CodeFragment& a, const CodeFragment& b) {
            if (a.location.line != b.location.line) {
                return a.location.line < b.location.line;
            }
            return a.location.column < b.location.column;
        });
}

CompileResult CompilerDispatcher::compileCHTLFragments(const std::vector<CodeFragment>& fragments, CompilationPipeline& pipeline) {
    CompileResult result;
    
    if (!chtlCompiler_) {
        reportError("CHTL编译器未注册", SourceLocation{});
        result.success = false;
        result.addError(CompileError("CHTL编译器未注册", SourceLocation{}));
        
        // 临时实现：即使没有编译器，也尝试生成基本输出
        for (const auto& fragment : fragments) {
            result.output += "<!-- CHTL Fragment (Basic Conversion) -->\n";
            result.output += "<div class=\"chtl-fragment\">\n";
            result.output += "  <pre>" + fragment.content + "</pre>\n";
            result.output += "</div>\n";
            result.output += "<!-- End CHTL Fragment -->\n\n";
        }
        
        return result;
    }
    
    // 临时实现：将CHTL片段转换为基本HTML
    for (const auto& fragment : fragments) {
        result.output += "<!-- CHTL Fragment -->\n";
        result.output += fragment.content;
        result.output += "\n<!-- End CHTL Fragment -->\n";
    }
    
    return result;
}

CompileResult CompilerDispatcher::compileCHTLJSFragments(const std::vector<CodeFragment>& fragments, CompilationPipeline& pipeline) {
    CompileResult result;
    
    if (!chtlJSCompiler_) {
        reportError("CHTL JS编译器未注册", SourceLocation{});
        result.success = false;
        result.addError(CompileError("CHTL JS编译器未注册", SourceLocation{}));
        
        // 临时实现：即使没有编译器，也生成基本JavaScript代码
        pipeline.jsOutput += "<script>\n";
        pipeline.jsOutput += "// CHTL JS Fragment (Basic Conversion)\n";
        for (const auto& fragment : fragments) {
            pipeline.jsOutput += "console.log('CHTL JS Fragment:', '" + fragment.content.substr(0, 50) + "...');\n";
        }
        pipeline.jsOutput += "</script>\n";
        
        return result;
    }
    
    // 临时实现：将CHTL JS片段转换为基本JavaScript
    pipeline.jsOutput += "<script>\n";
    for (const auto& fragment : fragments) {
        pipeline.jsOutput += "// CHTL JS Fragment\n";
        pipeline.jsOutput += fragment.content;
        pipeline.jsOutput += "\n// End CHTL JS Fragment\n";
    }
    pipeline.jsOutput += "</script>\n";
    
    return result;
}

CompileResult CompilerDispatcher::compileCSSFragments(const std::vector<CodeFragment>& fragments, CompilationPipeline& pipeline) {
    CompileResult result;
    
    // 简单实现：直接将CSS片段添加到样式输出
    pipeline.cssOutput += "<style>\n";
    for (const auto& fragment : fragments) {
        pipeline.cssOutput += "/* CSS Fragment */\n";
        pipeline.cssOutput += fragment.content;
        pipeline.cssOutput += "\n/* End CSS Fragment */\n";
    }
    pipeline.cssOutput += "</style>\n";
    
    return result;
}

CompileResult CompilerDispatcher::compileJavaScriptFragments(const std::vector<CodeFragment>& fragments, CompilationPipeline& pipeline) {
    CompileResult result;
    
    // 简单实现：直接将JavaScript片段添加到脚本输出
    pipeline.jsOutput += "<script>\n";
    for (const auto& fragment : fragments) {
        pipeline.jsOutput += "// JavaScript Fragment\n";
        pipeline.jsOutput += fragment.content;
        pipeline.jsOutput += "\n// End JavaScript Fragment\n";
    }
    pipeline.jsOutput += "</script>\n";
    
    return result;
}

void CompilerDispatcher::mergeCompileResult(const CompileResult& result, CompilationPipeline& pipeline) {
    // 合并错误和警告
    pipeline.errors.insert(pipeline.errors.end(), result.errors.begin(), result.errors.end());
    pipeline.warnings.insert(pipeline.warnings.end(), result.warnings.begin(), result.warnings.end());
    
    // 更新错误状态
    if (!result.success) {
        pipeline.hasErrors = true;
    }
    
    // 合并HTML输出
    pipeline.htmlOutput += result.output;
}

std::string CompilerDispatcher::generateHTMLOutput(const CompilationPipeline& pipeline) {
    std::string html;
    
    // 基本HTML结构
    html += "<!DOCTYPE html>\n";
    html += "<html>\n";
    html += "<head>\n";
    html += "    <meta charset=\"UTF-8\">\n";
    html += "    <title>CHTL Generated</title>\n";
    
    // 添加CSS
    if (!pipeline.cssOutput.empty()) {
        html += "    " + pipeline.cssOutput + "\n";
    }
    
    html += "</head>\n";
    html += "<body>\n";
    
    // 添加HTML内容
    if (!pipeline.htmlOutput.empty()) {
        html += "    " + pipeline.htmlOutput + "\n";
    }
    
    // 添加JavaScript
    if (!pipeline.jsOutput.empty()) {
        html += "    " + pipeline.jsOutput + "\n";
    }
    
    html += "</body>\n";
    html += "</html>\n";
    
    return html;
}

void CompilerDispatcher::resolveDependencies(CompilationPipeline& pipeline) {
    // 简单实现：暂时不处理依赖
}

void CompilerDispatcher::validateFragmentOrder(const std::vector<CodeFragment>& fragments) {
    // 简单实现：暂时不验证顺序
}

void CompilerDispatcher::reportError(const std::string& message, const SourceLocation& location) {
    errors_.emplace_back(message, location, "error");
}

void CompilerDispatcher::reportWarning(const std::string& message, const SourceLocation& location) {
    warnings_.emplace_back(message, location, "warning");
}

void CompilerDispatcher::updateGlobalContext(CompilationPipeline& pipeline) {
    // 简单实现：设置基本上下文
    pipeline.globalContext["filename"] = pipeline.sourceFilename;
    pipeline.globalContext["fragmentCount"] = std::to_string(pipeline.fragments.size());
}

std::unordered_map<std::string, std::string> CompilerDispatcher::extractContextFromFragment(const CodeFragment& fragment) {
    return fragment.context;
}

void CompilerDispatcher::optimizeOutput(CompilationPipeline& pipeline) {
    // 简单实现：移除多余的空行
    removeRedundantCSS(pipeline.cssOutput);
    removeRedundantJS(pipeline.jsOutput);
}

void CompilerDispatcher::removeRedundantCSS(std::string& css) {
    // 简单实现：移除连续的空行
    std::string result;
    bool lastWasEmpty = false;
    
    std::istringstream stream(css);
    std::string line;
    while (std::getline(stream, line)) {
        bool isEmpty = line.empty() || line.find_first_not_of(" \t") == std::string::npos;
        if (!isEmpty || !lastWasEmpty) {
            result += line + "\n";
        }
        lastWasEmpty = isEmpty;
    }
    
    css = result;
}

void CompilerDispatcher::removeRedundantJS(std::string& js) {
    // 与CSS相同的处理
    removeRedundantCSS(js);
}

void CompilerDispatcher::minifyOutput(CompilationPipeline& pipeline) {
    // 简单实现：暂时不进行压缩
}

void CompilerDispatcher::dumpPipelineState(const CompilationPipeline& pipeline, const std::string& stage) {
    if (!config_.debugMode) return;
    
    // 调试输出管道状态
    // 这里可以添加详细的调试信息
}

void CompilerDispatcher::validatePipelineIntegrity(const CompilationPipeline& pipeline) {
    // 简单实现：检查基本完整性
    if (pipeline.fragments.empty()) {
        reportWarning("编译管道中没有代码片段", SourceLocation{});
    }
}

// CompilerFactory 实现
std::unique_ptr<ICHTLCompiler> CompilerFactory::createCHTLCompiler(const CompilerConfig& config) {
    // 临时实现：返回空指针
    return nullptr;
}

std::unique_ptr<ICHTLJSCompiler> CompilerFactory::createCHTLJSCompiler(const CompilerConfig& config) {
    // 临时实现：返回空指针
    return nullptr;
}

std::unique_ptr<ICSSCompiler> CompilerFactory::createCSSCompiler(const CompilerConfig& config) {
    // 临时实现：返回空指针
    return nullptr;
}

std::unique_ptr<IJavaScriptCompiler> CompilerFactory::createJavaScriptCompiler(const CompilerConfig& config) {
    // 临时实现：返回空指针
    return nullptr;
}

std::unique_ptr<CompilerDispatcher> CompilerFactory::createFullDispatcher(const CompilerConfig& config) {
    auto dispatcher = std::make_unique<CompilerDispatcher>(config);
    
    // 注册编译器（当前为空实现）
    // dispatcher->registerCHTLCompiler(createCHTLCompiler(config));
    // dispatcher->registerCHTLJSCompiler(createCHTLJSCompiler(config));
    // dispatcher->registerCSSCompiler(createCSSCompiler(config));
    // dispatcher->registerJavaScriptCompiler(createJavaScriptCompiler(config));
    
    return dispatcher;
}

// 静态成员定义
CompilerFactory::CHTLCompilerFactory CompilerFactory::chtlCompilerFactory_ = nullptr;
CompilerFactory::CHTLJSCompilerFactory CompilerFactory::chtlJSCompilerFactory_ = nullptr;
CompilerFactory::CSSCompilerFactory CompilerFactory::cssCompilerFactory_ = nullptr;
CompilerFactory::JavaScriptCompilerFactory CompilerFactory::jsCompilerFactory_ = nullptr;

void CompilerFactory::registerCHTLCompilerFactory(CHTLCompilerFactory factory) {
    chtlCompilerFactory_ = factory;
}

void CompilerFactory::registerCHTLJSCompilerFactory(CHTLJSCompilerFactory factory) {
    chtlJSCompilerFactory_ = factory;
}

void CompilerFactory::registerCSSCompilerFactory(CSSCompilerFactory factory) {
    cssCompilerFactory_ = factory;
}

void CompilerFactory::registerJavaScriptCompilerFactory(JavaScriptCompilerFactory factory) {
    jsCompilerFactory_ = factory;
}

} // namespace CHTL