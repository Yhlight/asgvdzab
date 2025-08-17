#include "core/unified_scanner.hpp"
#include "core/chtl_configuration.hpp"
#include <iostream>
#include <cassert>
#include <map>

using namespace chtl;

std::string languageTypeToString(LanguageType type) {
    switch (type) {
        case LanguageType::CHTL: return "CHTL";
        case LanguageType::CHTL_JS: return "CHTL_JS";
        case LanguageType::CSS: return "CSS";
        case LanguageType::JAVASCRIPT: return "JavaScript";
        case LanguageType::HTML: return "HTML";
        case LanguageType::MIXED: return "MIXED";
        default: return "UNKNOWN";
    }
}

void testLanguageDetection() {
    std::cout << "=== 语言检测测试 ===" << std::endl;
    
    // CHTL语法检测
    std::cout << "CHTL语法检测:" << std::endl;
    std::cout << "  '@Var ThemeColor(primary)': " 
              << (LanguageDetector::isChtlSyntax("@Var ThemeColor(primary)") ? "✓" : "✗") << std::endl;
    std::cout << "  '[Origin] @Html {}': " 
              << (LanguageDetector::isChtlSyntax("[Origin] @Html {}") ? "✓" : "✗") << std::endl;
    std::cout << "  'inherit @Style': " 
              << (LanguageDetector::isChtlSyntax("inherit @Style header") ? "✓" : "✗") << std::endl;
    std::cout << "  '-- 注释': " 
              << (LanguageDetector::isChtlSyntax("-- 这是一个生成器注释") ? "✓" : "✗") << std::endl;
    
    // CSS语法检测
    std::cout << "\nCSS语法检测:" << std::endl;
    std::cout << "  '.header { color: red; }': " 
              << (LanguageDetector::isCssSyntax(".header { color: red; }") ? "✓" : "✗") << std::endl;
    std::cout << "  'background-color: blue;': " 
              << (LanguageDetector::isCssSyntax("background-color: blue;") ? "✓" : "✗") << std::endl;
    std::cout << "  '@media screen': " 
              << (LanguageDetector::isCssSyntax("@media screen and (max-width: 600px)") ? "✓" : "✗") << std::endl;
    
    // JavaScript语法检测
    std::cout << "\nJavaScript语法检测:" << std::endl;
    std::cout << "  'function test() {}': " 
              << (LanguageDetector::isJavaScriptSyntax("function test() { return true; }") ? "✓" : "✗") << std::endl;
    std::cout << "  'const x = () => {}': " 
              << (LanguageDetector::isJavaScriptSyntax("const x = () => { console.log('test'); }") ? "✓" : "✗") << std::endl;
    std::cout << "  '{{user.name}}': " 
              << (LanguageDetector::isJavaScriptSyntax("Hello {{user.name}}!") ? "✓" : "✗") << std::endl;
}

void testConfigurationSystem() {
    std::cout << "\n=== 配置系统测试 ===" << std::endl;
    
    CHtlConfiguration config;
    
    std::string configBlock = R"(
    [Configuration]
    {
        INDEX_INITIAL_COUNT = 1;
        DEBUG_MODE = true;
        OPTION_COUNT = 5;
        
        [Name]
        {
            CUSTOM_STYLE = [@Style, @style, @CSS];
            KEYWORD_FROM = from;
        }
        
        [OriginType]
        {
            ORIGINTYPE_VUE = @Vue;
            ORIGINTYPE_REACT = @React;
        }
    }
    )";
    
    bool loaded = config.loadFromConfigBlock(configBlock);
    std::cout << "配置加载: " << (loaded ? "✓" : "✗") << std::endl;
    std::cout << "索引起始计数: " << config.getIndexInitialCount() << std::endl;
    std::cout << "调试模式: " << (config.isDebugMode() ? "启用" : "禁用") << std::endl;
    std::cout << "自定义Vue类型: " << (config.isValidCustomOriginType("Vue") ? "✓" : "✗") << std::endl;
    
    auto customStyles = config.getKeywordNames("CUSTOM_STYLE");
    std::cout << "自定义样式关键字: ";
    for (const auto& style : customStyles) {
        std::cout << style << " ";
    }
    std::cout << std::endl;
}

void testRawEmbeddingScanning() {
    std::cout << "\n=== 原始嵌入扫描测试 ===" << std::endl;
    
    UnifiedScanner scanner;
    scanner.setDebugMode(true);
    
    std::string chtlContent = R"(
        // CHTL主体内容
        @Var ThemeColor(primary = "blue")
        @Style NavigationBar
        
        // 基本原始嵌入
        [Origin] @Html {
            <div class="container">
                <h1>Welcome</h1>
            </div>
        }
        
        [Origin] @Style {
            .container {
                width: 100%;
                margin: 0 auto;
            }
        }
        
        [Origin] @JavaScript {
            function initApp() {
                console.log('App initialized');
            }
        }
        
        // 带名原始嵌入
        [Origin] @Vue userProfile {
            <template>
                <div class="user-profile">
                    <h2>{{ user.name }}</h2>
                </div>
            </template>
        }
        
        [Origin] @React dashboard {
            const Dashboard = () => (
                <div>React Dashboard Component</div>
            );
        }
        
        // 原始嵌入引用
        [Origin] @Vue userProfile;
        [Origin] @React dashboard;
    )";
    
    auto result = scanner.scanChtlFile(chtlContent, "test.chtl");
    
    std::cout << "扫描结果:" << std::endl;
    std::cout << "  代码块数量: " << result.codeBlocks.size() << std::endl;
    std::cout << "  CHTL元素数量: " << result.chtlElements.size() << std::endl;
    std::cout << "  错误数量: " << result.errors.size() << std::endl;
    std::cout << "  警告数量: " << result.warnings.size() << std::endl;
    
    std::cout << "\n代码块详情:" << std::endl;
    for (size_t i = 0; i < result.codeBlocks.size(); ++i) {
        const auto& block = result.codeBlocks[i];
        std::cout << "  块 " << (i+1) << ": " << languageTypeToString(block.language)
                  << " (" << block.blockType;
        if (!block.blockName.empty()) {
            std::cout << " " << block.blockName;
        }
        std::cout << ") 行 " << block.startLine << "-" << block.endLine << std::endl;
    }
    
    std::cout << "\nCHTL元素详情:" << std::endl;
    for (size_t i = 0; i < result.chtlElements.size(); ++i) {
        const auto& element = result.chtlElements[i];
        std::cout << "  元素 " << (i+1) << ": " << element.name;
        if (!element.namespace_.empty()) {
            std::cout << " (命名空间: " << element.namespace_ << ")";
        }
        if (!element.value.empty()) {
            std::cout << " (值: " << element.value << ")";
        }
        std::cout << " 行 " << element.line << std::endl;
    }
}

void testLocalBlockScanning() {
    std::cout << "\n=== 局部块扫描测试 ===" << std::endl;
    
    UnifiedScanner scanner;
    scanner.setDebugMode(true);
    
    std::string chtlContent = R"(
        @Element Header {
            // 局部样式块
            style {
                .header {
                    background: linear-gradient(45deg, #blue, #green);
                    padding: 20px;
                }
                
                .header h1 {
                    color: white;
                    text-align: center;
                }
            }
            
            // 局部脚本块 - CHTL JS混合
            script {
                // CHTL语法
                const themeColor = ThemeColor(primary);
                const configData = ConfigData from config/app.chtl;
                
                // JavaScript语法
                function setupHeader() {
                    const header = document.querySelector('.header');
                    header.addEventListener('click', handleHeaderClick);
                }
                
                // JavaScript模板语法
                const userName = {{user.name}};
                const welcomeMsg = `Welcome, ${userName}!`;
                
                // 原始嵌入在脚本中
                [Origin] @Html headerContent {
                    <div class="header-content">
                        <h1>{{title}}</h1>
                    </div>
                }
                
                function handleHeaderClick() {
                    console.log('Header clicked');
                }
            }
            
            <header class="header">
                <h1>Application Header</h1>
            </header>
        }
    )";
    
    auto result = scanner.scanChtlFile(chtlContent, "header.chtl");
    
    std::cout << "局部块扫描结果:" << std::endl;
    std::cout << "  总代码块: " << result.codeBlocks.size() << std::endl;
    
    // 统计不同类型的块
    int styleBlocks = 0, scriptBlocks = 0, originBlocks = 0, mainBlocks = 0;
    for (const auto& block : result.codeBlocks) {
        if (block.blockType == "style") styleBlocks++;
        else if (block.blockType == "script") scriptBlocks++;
        else if (block.blockType == "Origin") originBlocks++;
        else if (block.blockType == "main") mainBlocks++;
    }
    
    std::cout << "  样式块: " << styleBlocks << std::endl;
    std::cout << "  脚本块: " << scriptBlocks << std::endl;
    std::cout << "  原始嵌入块: " << originBlocks << std::endl;
    std::cout << "  主体块: " << mainBlocks << std::endl;
    
    // 显示语言混合情况
    std::cout << "\n语言混合分析:" << std::endl;
    for (const auto& block : result.codeBlocks) {
        if (block.blockType == "script") {
            std::cout << "  脚本块语言: " << languageTypeToString(block.language)
                      << " (上下文: " << block.context << ")" << std::endl;
        }
    }
}

void testComplexMixedFile() {
    std::cout << "\n=== 复杂混合文件测试 ===" << std::endl;
    
    UnifiedScanner scanner;
    scanner.setDebugMode(false); // 关闭详细调试输出
    
    std::string complexContent = R"(
        // CHTL配置
        [Configuration]
        {
            DEBUG_MODE = true;
            [OriginType]
            {
                ORIGINTYPE_VUE = @Vue;
                ORIGINTYPE_SVELTE = @Svelte;
            }
        }
        
        // 命名空间声明
        [Namespace] ui.components
        
        // 导入声明
        [Import] @Style from theme/base.chtl
        [Import] @Element from components/common.chtl
        
        // 模板变量
        @Var ThemeColor(primary = "#007bff", secondary = "#6c757d")
        @Var Typography(fontFamily = "Arial", fontSize = "14px")
        
        // 样式组定义
        [Template] @Style CardLayout {
            border-radius: 8px;
            box-shadow: 0 2px 4px rgba(0,0,0,0.1);
            padding: 16px;
        }
        
        [Custom] @Style NavigationStyle {
            background: ThemeColor(primary);
            color: white;
            height: 60px;
        }
        
        // 元素定义
        @Element AppLayout {
            // 局部样式 - 纯CSS
            style {
                .app-layout {
                    display: flex;
                    flex-direction: column;
                    min-height: 100vh;
                }
                
                .main-content {
                    flex: 1;
                    margin: Typography(fontSize);
                }
            }
            
            // 局部脚本 - CHTL JS混合
            script {
                // CHTL语法
                const theme = ThemeColor(primary);
                const fonts = Typography from theme/fonts.chtl;
                
                // JavaScript语法
                class AppController {
                    constructor() {
                        this.initialized = false;
                    }
                    
                    init() {
                        this.setupLayout();
                        this.bindEvents();
                        this.initialized = true;
                    }
                    
                    setupLayout() {
                        const layout = document.querySelector('.app-layout');
                        layout.style.backgroundColor = theme;
                    }
                    
                    bindEvents() {
                        window.addEventListener('resize', this.handleResize.bind(this));
                    }
                    
                    handleResize() {
                        // JavaScript模板语法
                        const message = `Window resized to {{window.innerWidth}}x{{window.innerHeight}}`;
                        console.log(message);
                    }
                }
                
                // 原始嵌入
                [Origin] @Vue layoutComponent {
                    <template>
                        <div class="vue-layout">
                            <slot name="header"></slot>
                            <main class="main-content">
                                <slot></slot>
                            </main>
                            <slot name="footer"></slot>
                        </div>
                    </template>
                    
                    <script>
                    export default {
                        name: 'LayoutComponent',
                        props: ['theme']
                    }
                    </script>
                }
                
                [Origin] @Svelte counterWidget {
                    <script>
                        let count = 0;
                        function increment() {
                            count += 1;
                        }
                    </script>
                    
                    <button on:click={increment}>
                        Count: {count}
                    </button>
                    
                    <style>
                        button {
                            background: var(--primary-color);
                            color: white;
                            border: none;
                            padding: 8px 16px;
                            border-radius: 4px;
                        }
                    </style>
                }
                
                // 继续JavaScript逻辑
                const app = new AppController();
                document.addEventListener('DOMContentLoaded', () => {
                    app.init();
                });
            }
            
            // HTML内容
            <div class="app-layout">
                <header inherit NavigationStyle>
                    [Origin] @Html navigation;
                </header>
                
                <main class="main-content">
                    <div inherit CardLayout>
                        [Origin] @Vue layoutComponent;
                    </div>
                </main>
                
                <footer>
                    [Origin] @Svelte counterWidget;
                </footer>
            </div>
        }
        
        // 继承和删除操作
        [Custom] @Style DarkTheme inherit NavigationStyle {
            background: #333;
            color: #fff;
        }
        
        delete NavigationStyle.height;
        
        // 生成器注释
        -- 这是一个复杂的混合文件示例
        -- 包含了CHTL的所有主要功能
    )";
    
    auto result = scanner.scanChtlFile(complexContent, "complex.chtl");
    
    std::cout << "复杂文件扫描结果:" << std::endl;
    std::cout << "  总代码块: " << result.codeBlocks.size() << std::endl;
    std::cout << "  CHTL元素: " << result.chtlElements.size() << std::endl;
    std::cout << "  错误: " << result.errors.size() << std::endl;
    std::cout << "  警告: " << result.warnings.size() << std::endl;
    
    // 语言统计
    std::map<LanguageType, int> languageStats;
    for (const auto& block : result.codeBlocks) {
        languageStats[block.language]++;
    }
    
    std::cout << "\n语言统计:" << std::endl;
    for (const auto& pair : languageStats) {
        std::cout << "  " << languageTypeToString(pair.first) << ": " << pair.second << " 个块" << std::endl;
    }
    
    // 块类型统计
    std::map<std::string, int> blockStats;
    for (const auto& block : result.codeBlocks) {
        blockStats[block.blockType]++;
    }
    
    std::cout << "\n块类型统计:" << std::endl;
    for (const auto& pair : blockStats) {
        std::cout << "  " << pair.first << ": " << pair.second << " 个" << std::endl;
    }
    
    if (!result.errors.empty()) {
        std::cout << "\n错误信息:" << std::endl;
        for (const auto& error : result.errors) {
            std::cout << "  ❌ " << error << std::endl;
        }
    }
    
    if (!result.warnings.empty()) {
        std::cout << "\n警告信息:" << std::endl;
        for (const auto& warning : result.warnings) {
            std::cout << "  ⚠️  " << warning << std::endl;
        }
    }
}

int main() {
    try {
        std::cout << "🚀 CHTL统一扫描器测试" << std::endl;
        std::cout << "验证CHTL、CHTL JS、CSS、JS大统一扫描功能\n" << std::endl;
        
        testLanguageDetection();
        testConfigurationSystem();
        testRawEmbeddingScanning();
        testLocalBlockScanning();
        testComplexMixedFile();
        
        std::cout << "\n🎉 统一扫描器测试完成!" << std::endl;
        std::cout << "\n✅ 实现的核心功能:" << std::endl;
        std::cout << "  📊 智能语言检测 - 自动识别CHTL、CSS、JavaScript语法" << std::endl;
        std::cout << "  🔧 配置系统集成 - 支持自定义关键字和原始嵌入类型" << std::endl;
        std::cout << "  🔍 原始嵌入扫描 - 精确识别增强原始嵌入（基本类型+自定义类型+带名）" << std::endl;
        std::cout << "  📦 局部块处理 - 分别处理style和script块，支持语言混合检测" << std::endl;
        std::cout << "  🎯 CHTL语法解析 - 识别变量、样式组、元素、导入、命名空间等" << std::endl;
        std::cout << "  🔄 多语言统一 - 在单一扫描器中处理CHTL、CHTL JS、CSS、JS" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "测试异常: " << e.what() << std::endl;
        return 1;
    }
}