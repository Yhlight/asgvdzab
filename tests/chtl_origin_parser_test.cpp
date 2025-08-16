#include "parsers/chtl_origin_parser.hpp"
#include <iostream>
#include <string>
#include <cassert>

using namespace chtl;

void testHtmlOriginDeclaration() {
    std::cout << "🧪 测试HTML原始嵌入声明..." << std::endl;
    
    CHTLOriginParser parser;
    
    // 测试内联HTML原始嵌入
    std::string source = "[Origin] @Html { <div>原始HTML内容</div><span>不经过CHTL处理</span> }";
    
    auto result = parser.parseOriginDeclaration(source, "html_origin.chtl");
    
    assert(result.success);
    assert(result.rootNode);
    assert(result.rootNode->type == CHTLASTNodeType::ORIGIN_DECLARATION);
    
    auto originNode = std::dynamic_pointer_cast<OriginEmbedNode>(result.rootNode);
    assert(originNode);
    assert(originNode->originType == "@Html");
    assert(!originNode->content.empty());
    assert(originNode->content.find("原始HTML内容") != std::string::npos);
    
    std::cout << "✅ HTML原始嵌入声明测试通过" << std::endl;
}

void testCssOriginDeclaration() {
    std::cout << "🧪 测试CSS原始嵌入声明..." << std::endl;
    
    CHTLOriginParser parser;
    
    // 测试内联CSS原始嵌入
    std::string source = "[Origin] @Style { .custom-class { background-color: #ff0000; border: 1px solid black; } }";
    
    auto result = parser.parseOriginDeclaration(source, "css_origin.chtl");
    
    assert(result.success);
    assert(result.rootNode);
    assert(result.rootNode->type == CHTLASTNodeType::ORIGIN_DECLARATION);
    
    auto originNode = std::dynamic_pointer_cast<OriginEmbedNode>(result.rootNode);
    assert(originNode);
    assert(originNode->originType == "@Style");
    assert(!originNode->content.empty());
    assert(originNode->content.find("custom-class") != std::string::npos);
    
    std::cout << "✅ CSS原始嵌入声明测试通过" << std::endl;
}

void testJavaScriptOriginDeclaration() {
    std::cout << "🧪 测试JavaScript原始嵌入声明..." << std::endl;
    
    CHTLOriginParser parser;
    
    // 测试内联JavaScript原始嵌入
    std::string source = "[Origin] @JavaScript { function customFunction() { console.log('原始JavaScript代码'); return true; } }";
    
    auto result = parser.parseOriginDeclaration(source, "js_origin.chtl");
    
    assert(result.success);
    assert(result.rootNode);
    assert(result.rootNode->type == CHTLASTNodeType::ORIGIN_DECLARATION);
    
    auto originNode = std::dynamic_pointer_cast<OriginEmbedNode>(result.rootNode);
    assert(originNode);
    assert(originNode->originType == "@JavaScript");
    assert(!originNode->content.empty());
    assert(originNode->content.find("customFunction") != std::string::npos);
    
    std::cout << "✅ JavaScript原始嵌入声明测试通过" << std::endl;
}

void testEnhancedOriginDeclaration() {
    std::cout << "🧪 测试增强原始嵌入声明..." << std::endl;
    
    CHTLOriginParser parser;
    
    // 测试增强HTML原始嵌入声明
    std::string source = "[Origin] @Html customHeader { <header><h1>自定义头部</h1><nav>导航栏</nav></header> }";
    
    auto result = parser.parseOriginDeclaration(source, "enhanced_origin.chtl");
    
    assert(result.success);
    assert(result.rootNode);
    assert(result.rootNode->type == CHTLASTNodeType::ORIGIN_DECLARATION);
    
    auto originNode = std::dynamic_pointer_cast<OriginEmbedNode>(result.rootNode);
    assert(originNode);
    assert(originNode->originType == "@Html");
    assert(originNode->name == "customHeader");
    assert(!originNode->content.empty());
    assert(originNode->content.find("自定义头部") != std::string::npos);
    
    // 检查是否注册到原始嵌入注册表
    auto registry = parser.getOriginRegistry();
    const auto* originBlock = registry->findOriginBlock(OriginType::HTML_ORIGIN, "customHeader");
    assert(originBlock != nullptr);
    assert(originBlock->name == "customHeader");
    assert(originBlock->type == OriginType::HTML_ORIGIN);
    assert(originBlock->mode == OriginMode::ENHANCED_DECLARE);
    
    std::cout << "✅ 增强原始嵌入声明测试通过" << std::endl;
}

void testOriginUsage() {
    std::cout << "🧪 测试原始嵌入使用..." << std::endl;
    
    CHTLOriginParser parser;
    
    // 先注册一个增强原始嵌入
    std::string declaration = "[Origin] @Html navigation { <nav><ul><li>首页</li><li>关于</li></ul></nav> }";
    auto declResult = parser.parseOriginDeclaration(declaration, "origin_decl.chtl");
    assert(declResult.success);
    
    // 测试原始嵌入使用
    std::string usage = "[Origin] @Html navigation;";
    auto originRef = parser.parseOriginUsage(usage);
    
    assert(originRef.type == OriginType::HTML_ORIGIN);
    assert(originRef.name == "navigation");
    
    // 测试原始嵌入展开
    auto expandedNode = parser.expandOriginReference(originRef);
    assert(expandedNode != nullptr);
    
    auto expandedOrigin = std::dynamic_pointer_cast<OriginEmbedNode>(expandedNode);
    assert(expandedOrigin);
    assert(expandedOrigin->originType == "@Html");
    assert(expandedOrigin->content.find("首页") != std::string::npos);
    
    std::cout << "✅ 原始嵌入使用测试通过" << std::endl;
}

void testOriginScanner() {
    std::cout << "🧪 测试原始嵌入扫描器..." << std::endl;
    
    OriginScanner scanner;
    OriginContext context;
    context.currentType = OriginType::HTML_ORIGIN;
    context.currentMode = OriginMode::INLINE_EMBED;
    
    // 测试扫描HTML内容
    std::string htmlContent = "<div class=\"test\">\n  <p>段落内容</p>\n  <span>行内元素</span>\n</div>";
    auto scannedHtml = scanner.scanOriginContent(htmlContent, context);
    
    assert(!scannedHtml.empty());
    assert(scannedHtml.find("test") != std::string::npos);
    assert(scannedHtml.find("段落内容") != std::string::npos);
    
    // 测试验证HTML内容
    assert(scanner.validateOriginContent(htmlContent, OriginType::HTML_ORIGIN));
    
    // 测试扫描CSS内容
    context.currentType = OriginType::STYLE_ORIGIN;
    std::string cssContent = ".button { color: blue; background: white; }";
    auto scannedCss = scanner.scanOriginContent(cssContent, context);
    
    assert(!scannedCss.empty());
    assert(scannedCss.find("button") != std::string::npos);
    assert(scanner.validateOriginContent(cssContent, OriginType::STYLE_ORIGIN));
    
    // 测试扫描JavaScript内容
    context.currentType = OriginType::JAVASCRIPT_ORIGIN;
    std::string jsContent = "function onClick() { alert('点击事件'); }";
    auto scannedJs = scanner.scanOriginContent(jsContent, context);
    
    assert(!scannedJs.empty());
    assert(scannedJs.find("onClick") != std::string::npos);
    assert(scanner.validateOriginContent(jsContent, OriginType::JAVASCRIPT_ORIGIN));
    
    std::cout << "✅ 原始嵌入扫描器测试通过" << std::endl;
}

void testOriginStateMachine() {
    std::cout << "🧪 测试原始嵌入状态机..." << std::endl;
    
    OriginStateMachine stateMachine;
    OriginContext context;
    
    // 测试处理原始嵌入声明
    Token originToken(TokenType::LEFT_BRACKET, "[", Position{});
    Token keywordToken(TokenType::IDENTIFIER, "Origin", Position{});
    Token rightBracketToken(TokenType::RIGHT_BRACKET, "]", Position{});
    Token atToken(TokenType::OPERATOR, "@", Position{});
    Token typeToken(TokenType::IDENTIFIER, "Html", Position{});
    Token nameToken(TokenType::IDENTIFIER, "header", Position{});
    Token leftBraceToken(TokenType::LEFT_BRACE, "{", Position{});
    Token contentToken(TokenType::IDENTIFIER, "content", Position{});
    Token rightBraceToken(TokenType::RIGHT_BRACE, "}", Position{});
    
    stateMachine.processToken(originToken, context);
    stateMachine.processToken(keywordToken, context);
    stateMachine.processToken(rightBracketToken, context);
    stateMachine.processToken(atToken, context);
    stateMachine.processToken(typeToken, context);
    stateMachine.processToken(nameToken, context);
    stateMachine.processToken(leftBraceToken, context);
    stateMachine.processToken(contentToken, context);
    stateMachine.processToken(rightBraceToken, context);
    
    auto originBlocks = stateMachine.getOriginBlocks();
    assert(!originBlocks.empty());
    assert(originBlocks[0].type == OriginType::HTML_ORIGIN);
    assert(originBlocks[0].name == "header");
    assert(originBlocks[0].mode == OriginMode::ENHANCED_DECLARE);
    
    std::cout << "✅ 原始嵌入状态机测试通过" << std::endl;
}

void testOriginRegistry() {
    std::cout << "🧪 测试原始嵌入注册表..." << std::endl;
    
    OriginRegistry registry;
    
    // 创建测试原始嵌入块
    OriginBlock htmlBlock(OriginType::HTML_ORIGIN, OriginMode::ENHANCED_DECLARE, "testHtml");
    htmlBlock.content = "<div>测试HTML</div>";
    
    OriginBlock cssBlock(OriginType::STYLE_ORIGIN, OriginMode::ENHANCED_DECLARE, "testCss");
    cssBlock.content = ".test { color: red; }";
    
    OriginBlock jsBlock(OriginType::JAVASCRIPT_ORIGIN, OriginMode::ENHANCED_DECLARE, "testJs");
    jsBlock.content = "function test() {}";
    
    // 注册原始嵌入块
    assert(registry.registerOriginBlock(htmlBlock));
    assert(registry.registerOriginBlock(cssBlock));
    assert(registry.registerOriginBlock(jsBlock));
    
    // 查找原始嵌入块
    const auto* foundHtml = registry.findOriginBlock(OriginType::HTML_ORIGIN, "testHtml");
    const auto* foundCss = registry.findOriginBlock(OriginType::STYLE_ORIGIN, "testCss");
    const auto* foundJs = registry.findOriginBlock(OriginType::JAVASCRIPT_ORIGIN, "testJs");
    
    assert(foundHtml != nullptr);
    assert(foundCss != nullptr);
    assert(foundJs != nullptr);
    
    assert(foundHtml->content.find("测试HTML") != std::string::npos);
    assert(foundCss->content.find("test") != std::string::npos);
    assert(foundJs->content.find("function") != std::string::npos);
    
    // 验证原始嵌入引用
    OriginReference htmlRef(OriginType::HTML_ORIGIN, "testHtml");
    OriginReference cssRef(OriginType::STYLE_ORIGIN, "testCss");
    OriginReference jsRef(OriginType::JAVASCRIPT_ORIGIN, "testJs");
    
    assert(registry.validateOriginReference(htmlRef));
    assert(registry.validateOriginReference(cssRef));
    assert(registry.validateOriginReference(jsRef));
    
    // 测试重复注册
    assert(!registry.registerOriginBlock(htmlBlock)); // 应该失败，已存在
    
    std::cout << "✅ 原始嵌入注册表测试通过" << std::endl;
}

void testOriginUtils() {
    std::cout << "🧪 测试原始嵌入工具类..." << std::endl;
    
    // 测试原始嵌入声明识别
    std::string originDecl = "[Origin] @Html { <div>内容</div> }";
    assert(OriginUtils::isOriginDeclaration(originDecl));
    
    std::string enhancedDecl = "[Origin] @Style customStyle { .class { color: red; } }";
    assert(OriginUtils::isOriginDeclaration(enhancedDecl));
    
    // 测试原始嵌入使用识别
    std::string originUsage = "[Origin] @Html customHtml;";
    assert(OriginUtils::isOriginUsage(originUsage));
    
    // 测试类型提取
    assert(OriginUtils::extractOriginTypeString(originDecl) == "Html");
    assert(OriginUtils::extractOriginTypeString(enhancedDecl) == "Style");
    
    // 测试名称提取
    assert(OriginUtils::extractOriginName(enhancedDecl) == "customStyle");
    assert(OriginUtils::extractOriginName(originUsage) == "customHtml");
    
    // 测试内容类型检测
    std::string htmlContent = "<div><span>HTML内容</span></div>";
    assert(OriginUtils::detectOriginType(htmlContent) == OriginType::HTML_ORIGIN);
    
    std::string cssContent = ".button { background: blue; }";
    assert(OriginUtils::detectOriginType(cssContent) == OriginType::STYLE_ORIGIN);
    
    std::string jsContent = "function test() { var x = 10; }";
    assert(OriginUtils::detectOriginType(jsContent) == OriginType::JAVASCRIPT_ORIGIN);
    
    // 测试语法验证
    assert(OriginUtils::validateOriginSyntax(originDecl));
    assert(OriginUtils::validateOriginSyntax(enhancedDecl));
    assert(OriginUtils::validateOriginSyntax(originUsage));
    
    // 测试内容验证
    assert(OriginUtils::isValidHtml(htmlContent));
    assert(OriginUtils::isValidCss(cssContent));
    assert(OriginUtils::isValidJavaScript(jsContent));
    
    std::cout << "✅ 原始嵌入工具类测试通过" << std::endl;
}

void testComplexOriginContent() {
    std::cout << "🧪 测试复杂原始嵌入内容..." << std::endl;
    
    CHTLOriginParser parser;
    
    // 测试包含嵌套结构的HTML原始嵌入
    std::string complexHtml = R"([Origin] @Html complexLayout {
        <div class="container">
            <header>
                <h1>页面标题</h1>
                <nav>
                    <ul>
                        <li><a href="#home">首页</a></li>
                        <li><a href="#about">关于</a></li>
                    </ul>
                </nav>
            </header>
            <main>
                <section>
                    <p>主要内容区域</p>
                </section>
            </main>
        </div>
    })";
    
    auto result = parser.parseOriginDeclaration(complexHtml, "complex.chtl");
    
    assert(result.success);
    auto originNode = std::dynamic_pointer_cast<OriginEmbedNode>(result.rootNode);
    assert(originNode);
    assert(originNode->name == "complexLayout");
    assert(originNode->content.find("container") != std::string::npos);
    assert(originNode->content.find("页面标题") != std::string::npos);
    
    std::cout << "✅ 复杂原始嵌入内容测试通过" << std::endl;
}

void testErrorHandling() {
    std::cout << "🧪 测试错误处理..." << std::endl;
    
    CHTLOriginParser parser;
    
    // 测试无效的原始嵌入声明
    std::string invalidOrigin = "[Origin] @InvalidType { content }";
    auto result = parser.parseOriginDeclaration(invalidOrigin, "invalid.chtl");
    assert(!result.success);
    assert(!parser.getErrors().empty());
    
    // 测试无效的原始嵌入使用
    std::string invalidUsage = "[Origin] @InvalidType name;";
    auto originRef = parser.parseOriginUsage(invalidUsage);
    assert(!parser.getErrors().empty());
    
    // 测试未定义的原始嵌入引用
    OriginReference undefinedRef(OriginType::HTML_ORIGIN, "undefined");
    auto expandedNode = parser.expandOriginReference(undefinedRef);
    assert(expandedNode == nullptr);
    assert(!parser.getErrors().empty());
    
    std::cout << "✅ 错误处理测试通过" << std::endl;
}

int main() {
    std::cout << "🚀 开始CHTL原始嵌入系统解析器测试" << std::endl;
    std::cout << "========================================" << std::endl;
    
    try {
        testHtmlOriginDeclaration();
        testCssOriginDeclaration();
        testJavaScriptOriginDeclaration();
        testEnhancedOriginDeclaration();
        testOriginUsage();
        testOriginScanner();
        testOriginStateMachine();
        testOriginRegistry();
        testOriginUtils();
        testComplexOriginContent();
        testErrorHandling();
        
        std::cout << "\n========================================" << std::endl;
        std::cout << "🎉 所有测试通过！" << std::endl;
        std::cout << "✅ 原始嵌入系统解析器实现完成" << std::endl;
        std::cout << "   • HTML原始嵌入 ([Origin] @Html { HTML代码 })" << std::endl;
        std::cout << "   • CSS原始嵌入 ([Origin] @Style { CSS代码 })" << std::endl;
        std::cout << "   • JavaScript原始嵌入 ([Origin] @JavaScript { JS代码 })" << std::endl;
        std::cout << "   • 增强原始嵌入 ([Origin] @Type name { 代码 })" << std::endl;
        std::cout << "   • 原始嵌入使用 ([Origin] @Type name;)" << std::endl;
        std::cout << "   • 内联嵌入模式 (直接嵌入原始代码)" << std::endl;
        std::cout << "   • 增强声明模式 (命名的原始嵌入)" << std::endl;
        std::cout << "   • 增强使用模式 (引用已声明的原始嵌入)" << std::endl;
        std::cout << "   • 原始嵌入注册表管理" << std::endl;
        std::cout << "   • 格式保持和内容验证" << std::endl;
        std::cout << "   • 状态机和扫描器架构" << std::endl;
        std::cout << "   • 上下文感知解析" << std::endl;
        std::cout << "   • 严格按照CHTL语法文档实现" << std::endl;
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "❌ 测试失败: " << e.what() << std::endl;
        return 1;
    }
}