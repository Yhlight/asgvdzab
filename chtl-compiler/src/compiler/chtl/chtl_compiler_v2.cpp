#include "compiler/chtl/chtl_compiler_v2.h"
#include "compiler/chtl/chtl_lexer.h"
#include "compiler/chtl/chtl_parser.h"
#include "compiler/chtl/chtl_generator.h"
#include <sstream>

namespace chtl {

CHTLCompilerV2::CHTLCompilerV2() 
    : global_map_(std::make_unique<GlobalMap>()) {
}

CompilationResult CHTLCompilerV2::compile(const CodeFragment& fragment,
                                         const CompilerOptions& options) {
    CompilationResult result;
    result.success = true;
    
    try {
        // 1. 词法分析
        CHTLLexer lexer(fragment.content, fragment.location.file_path);
        
        // 2. 语法分析
        CHTLParser parser(lexer, *global_map_);
        auto ast = parser.parse();
        
        // 检查解析错误
        if (parser.hasErrors()) {
            result.success = false;
            result.errors = parser.getErrors();
            result.warnings = parser.getWarnings();
            return result;
        }
        
        // 3. 代码生成
        CompilerState state(*global_map_);
        CHTLGenerator generator(state);
        
        std::string html_output = generator.generate(ast.get());
        
        // 构建最终输出
        std::stringstream output;
        
        // 如果是完整的HTML文档（包含html标签），直接使用
        bool has_html_tag = fragment.content.find("html") != std::string::npos &&
                           fragment.content.find("{") != std::string::npos;
        
        if (has_html_tag) {
            // 插入全局样式和脚本到适当位置
            std::string global_styles = generator.getGlobalStyles();
            std::string global_scripts = generator.getGlobalScripts();
            
            // 查找</head>标签位置，插入样式
            size_t head_end = html_output.find("</head>");
            if (head_end != std::string::npos && !global_styles.empty()) {
                html_output.insert(head_end, "\n" + global_styles);
            } else if (head_end == std::string::npos && !global_styles.empty()) {
                // 如果没有head标签，在body前插入
                size_t body_start = html_output.find("<body>");
                if (body_start != std::string::npos) {
                    html_output.insert(body_start, "<head>\n" + global_styles + "</head>\n");
                }
            }
            
            // 查找</body>标签位置，插入脚本
            size_t body_end = html_output.find("</body>");
            if (body_end != std::string::npos && !global_scripts.empty()) {
                html_output.insert(body_end, global_scripts + "\n");
            }
            
            output << html_output;
        } else {
            // 片段模式，只输出内容和样式
            output << generator.getGlobalStyles();
            output << html_output;
            output << generator.getGlobalScripts();
        }
        
        result.output = output.str();
        
        // 添加编译状态的错误和警告
        if (state.hasErrors()) {
            result.success = false;
            for (const auto& error : state.getErrors()) {
                result.errors.push_back(error.message);
            }
        }
        
        for (const auto& warning : state.getWarnings()) {
            result.warnings.push_back(warning.message);
        }
        
    } catch (const std::exception& e) {
        result.success = false;
        result.errors.push_back(std::string("Compilation error: ") + e.what());
    }
    
    return result;
}

} // namespace chtl