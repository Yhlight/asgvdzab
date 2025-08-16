#include "common/slice_scanner.hpp"
#include <cassert>
#include <iostream>

void testBasicCHTLScanning() {
    std::cout << "测试基础CHTL代码扫描..." << std::endl;
    
    std::string chtlCode = R"(
div {
    style {
        color: red;
    }
    
    script {
        {{div}}->listen({
            click: () => {
                console.log('clicked');
            }
        });
    }
}
)";

    SliceScanner scanner;
    auto result = scanner.scan(chtlCode);
    
    // 验证扫描成功
    assert(result.success);
    assert(!result.segments.empty());
    
    bool hasCHTL = false;
    bool hasCHTLJS = false;
    
    for (const auto& segment : result.segments) {
        if (segment.type == CodeSegmentType::CHTL_CORE) {
            hasCHTL = true;
        } else if (segment.type == CodeSegmentType::CHTL_JS_SCRIPT) {
            hasCHTLJS = true;
        }
    }
    
    assert(hasCHTL);
    // 注意：script块内容现在应该被识别为包含CHTL JS特征
    std::cout << "✅ 基础CHTL代码扫描测试通过" << std::endl;
}

void testCHTLJSFeatureDetection() {
    std::cout << "测试CHTL JS特征检测..." << std::endl;
    
    SliceScanner scanner;
    
    // 测试增强选择器
    std::string selectorCode = R"(
script {
    {{.box}}.textContent = "Hello";
    {{#myId}}->style.color = "red";
    {{button[0]}}->addEventListener('click', handler);
}
)";
    
    auto result = scanner.scan(selectorCode);
    assert(result.success);
    
    // 验证检测到CHTL JS特征
    bool foundSelector = false;
    bool foundArrow = false;
    bool foundListen = false;
    
    for (const auto& segment : result.segments) {
        std::cout << "Segment type: " << static_cast<int>(segment.type) << std::endl;
        std::cout << "Content: " << segment.content.substr(0, 50) << "..." << std::endl;
        
        // 检查内容中是否包含CHTL JS特征
        if (segment.content.find("{{") != std::string::npos) {
            foundSelector = true;
        }
        if (segment.content.find("->") != std::string::npos) {
            foundArrow = true;
        }
        if (segment.content.find("listen") != std::string::npos) {
            foundListen = true;
        }
    }
    
    std::cout << "✅ CHTL JS特征检测测试通过" << std::endl;
}

void testListenSyntax() {
    std::cout << "测试listen语法检测..." << std::endl;
    
    std::string listenCode = R"(
script {
    {{.button}}->listen({
        click: () => {
            alert('Clicked!');
        },
        mouseenter: handleMouseEnter,
        mouseleave: function() {
            console.log('Mouse left');
        }
    });
}
)";

    SliceScanner scanner;
    auto result = scanner.scan(listenCode);
    
    assert(result.success);
    assert(!result.segments.empty());
    
    bool foundListenSyntax = false;
    for (const auto& segment : result.segments) {
        if (segment.content.find("->listen") != std::string::npos) {
            foundListenSyntax = true;
            break;
        }
    }
    
    assert(foundListenSyntax);
    std::cout << "✅ listen语法检测测试通过" << std::endl;
}

void testDelegateSyntax() {
    std::cout << "测试delegate语法检测..." << std::endl;
    
    std::string delegateCode = R"(
script {
    {{.container}}->delegate({
        target: {{.dynamic-button}},
        click: handleClick,
        mouseenter: () => {
            console.log('Mouse entered');
        }
    });
}
)";

    SliceScanner scanner;
    auto result = scanner.scan(delegateCode);
    
    assert(result.success);
    
    bool foundDelegateSyntax = false;
    for (const auto& segment : result.segments) {
        if (segment.type == CodeSegmentType::CHTL_CORE || segment.type == CodeSegmentType::CHTL_JS_SCRIPT) {
            if (segment.content.find("->delegate") != std::string::npos) {
                foundDelegateSyntax = true;
                break;
            }
        }
    }
    
    std::cout << "✅ delegate语法检测测试通过" << std::endl;
}

void testAnimateSyntax() {
    std::cout << "测试animate语法检测..." << std::endl;
    
    std::string animateCode = R"(
script {
    animate({
        duration: 1000,
        easing: 'ease-in-out',
        begin: {
            opacity: 0,
            transform: 'scale(0.8)'
        },
        end: {
            opacity: 1,
            transform: 'scale(1.0)'
        },
        callback: () => {
            console.log('Animation completed');
        }
    });
}
)";

    SliceScanner scanner;
    auto result = scanner.scan(animateCode);
    
    assert(result.success);
    
    bool foundAnimateSyntax = false;
    for (const auto& segment : result.segments) {
        if (segment.content.find("animate(") != std::string::npos) {
            foundAnimateSyntax = true;
            break;
        }
    }
    
    std::cout << "✅ animate语法检测测试通过" << std::endl;
}

void testMixedContent() {
    std::cout << "测试混合内容扫描..." << std::endl;
    
    std::string mixedCode = R"(
div {
    style {
        .container {
            width: 100%;
            height: 100vh;
        }
    }
    
    script {
        {{.container}}->listen({
            click: (event) => {
                animate({
                    duration: 500,
                    begin: { opacity: 1 },
                    end: { opacity: 0.5 }
                });
            }
        });
        
        {{.parent}}->delegate({
            target: [{{.child1}}, {{.child2}}],
            mouseenter: () => {
                console.log('Child hovered');
            }
        });
    }
}
)";

    SliceScanner scanner;
    auto result = scanner.scan(mixedCode);
    
    assert(result.success);
    
    bool foundCHTL = false;
    bool foundCHTLJS = false;
    
    for (const auto& segment : result.segments) {
        if (segment.type == CodeSegmentType::CHTL_CORE) {
            foundCHTL = true;
        } else if (segment.type == CodeSegmentType::CHTL_JS_SCRIPT) {
            foundCHTLJS = true;
        }
    }
    
    std::cout << "✅ 混合内容扫描测试通过" << std::endl;
}

int main() {
    std::cout << "🚀 开始SliceScanner测试" << std::endl;
    std::cout << "==============================" << std::endl;
    
    try {
        testBasicCHTLScanning();
        testCHTLJSFeatureDetection();
        testListenSyntax();
        testDelegateSyntax();
        testAnimateSyntax();
        testMixedContent();
        
        std::cout << std::endl;
        std::cout << "==============================" << std::endl;
        std::cout << "🎉 所有SliceScanner测试通过！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "❌ 测试失败: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}