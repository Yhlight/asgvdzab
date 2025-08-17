#include "parsers/chtl_js_parser.hpp"
#include <iostream>

using namespace chtl;

int main() {
    CHTLJSScanner scanner;
    
    // 测试委托语法
    std::string content = R"(
        {{.container}}->delegate({
            target: {{.dynamic-button}},
            click: handleClick,
            mouseenter: () => {
                console.log('Mouse entered');
            }
        });
        
        {{.parent}}->delegate({
            target: [{{.child1}}, {{.child2}}],
            click: () => console.log('Child clicked')
        });
    )";
    
    std::cout << "测试内容: " << content << std::endl;
    
    auto delegates = scanner.scanDelegates(content);
    
    std::cout << "找到委托数量: " << delegates.size() << std::endl;
    
    for (size_t d = 0; d < delegates.size(); ++d) {
        const auto& delegate = delegates[d];
        std::cout << "\n委托 " << d << ":" << std::endl;
        std::cout << "  目标数量: " << delegate.targets.size() << std::endl;
        std::cout << "  监听器数量: " << delegate.listeners.size() << std::endl;
        
        for (size_t i = 0; i < delegate.targets.size(); ++i) {
            std::cout << "  目标 " << i << ": " << delegate.targets[i].selector << std::endl;
        }
        
        for (size_t i = 0; i < delegate.listeners.size(); ++i) {
            std::cout << "  监听器 " << i << ": " << delegate.listeners[i].eventType 
                      << " -> " << delegate.listeners[i].handlerCode << std::endl;
        }
    }
    
    return 0;
}