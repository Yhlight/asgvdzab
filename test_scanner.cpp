#include "src/Scanner/UnifiedScanner.cpp"
#include "src/Core/Token.cpp"
#include "src/Core/Context.cpp"
#include <iostream>

int main() {
    using namespace CHTL;
    
    // 创建扫描器配置
    Scanner::ScannerConfig config;
    config.enableDebug = true;
    config.generateTokens = true;
    
    // 创建扫描器
    auto scanner = std::make_unique<Scanner::CHTLUnifiedScanner>(config);
    
    // 测试CHTL代码
    std::string testCode = R"(
html
{
    head
    {
        
    }
    
    body
    {
        div
        {
            style
            {
                width: 100px;
                height: 100px;
                background-color: red;
                
                .box
                {
                    color: blue;
                }
            }
            
            script
            {
                {{box}}.addEventListener('click', () => {
                    console.log('Box clicked!');
                });
                
                {{box}}->listen({
                    click: () => {
                        console.log('CHTL JS click!');
                    }
                });
            }
            
            text
            {
                Hello World
            }
        }
    }
}

[Template] @Style DefaultStyle
{
    color: black;
    font-size: 16px;
}

[Custom] @Element Box
{
    div
    {
        @Style DefaultStyle;
        
        text
        {
            Box Content
        }
    }
}
)";
    
    std::cout << "测试CHTL统一扫描器..." << std::endl;
    std::cout << "源码长度: " << testCode.size() << " 字节" << std::endl;
    
    try {
        // 扫描源码
        auto fragments = scanner->scanSource(testCode, "test.chtl");
        
        std::cout << "\n扫描结果:" << std::endl;
        std::cout << "共生成 " << fragments.size() << " 个代码片段" << std::endl;
        
        for (size_t i = 0; i < fragments.size(); ++i) {
            const auto& fragment = fragments[i];
            
            std::string typeStr;
            switch (fragment->type) {
                case Core::FragmentType::CHTL: typeStr = "CHTL"; break;
                case Core::FragmentType::CHTL_JS: typeStr = "CHTL_JS"; break;
                case Core::FragmentType::CSS: typeStr = "CSS"; break;
                case Core::FragmentType::JAVASCRIPT: typeStr = "JAVASCRIPT"; break;
                case Core::FragmentType::HTML: typeStr = "HTML"; break;
                default: typeStr = "UNKNOWN"; break;
            }
            
            std::cout << "\n片段 " << i + 1 << ":" << std::endl;
            std::cout << "  类型: " << typeStr << std::endl;
            std::cout << "  位置: " << fragment->lineNumber << ":" << fragment->columnNumber << std::endl;
            std::cout << "  大小: " << fragment->content.size() << " 字节" << std::endl;
            std::cout << "  内容预览: ";
            
            // 显示内容的前50个字符
            std::string preview = fragment->content;
            if (preview.size() > 50) {
                preview = preview.substr(0, 50) + "...";
            }
            
            // 替换换行符为空格以便显示
            for (char& ch : preview) {
                if (ch == '\n' || ch == '\r') {
                    ch = ' ';
                }
            }
            
            std::cout << "\"" << preview << "\"" << std::endl;
        }
        
        // 检查错误
        const auto& errors = scanner->getErrors();
        if (!errors.empty()) {
            std::cout << "\n扫描错误:" << std::endl;
            for (const auto& error : errors) {
                std::cout << "  " << error->line << ":" << error->column 
                         << " " << error->message << std::endl;
            }
        } else {
            std::cout << "\n扫描完成，无错误。" << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "扫描失败: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}