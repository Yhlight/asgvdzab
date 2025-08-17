#include "antlr4-runtime.h"
#include "CSS3Lexer.h"
#include "CSS3Parser.h"
#include "CSS3BaseListener.h"
#include <iostream>

using namespace antlr4;
using namespace antlr4::tree;

class SimpleCSSListener : public CSS3BaseListener {
public:
    void enterStylesheet(CSS3Parser::StylesheetContext *ctx) override {
        std::cout << "进入样式表" << std::endl;
    }
    
    void exitStylesheet(CSS3Parser::StylesheetContext *ctx) override {
        std::cout << "退出样式表" << std::endl;
    }
};

int main() {
    try {
        std::cout << "=== 监听器测试 ===" << std::endl;
        
        std::string simpleCSS = ".test { color: red; }";
        std::cout << "测试CSS: " << simpleCSS << std::endl;
        
        ANTLRInputStream input(simpleCSS);
        CSS3Lexer lexer(&input);
        CommonTokenStream tokens(&lexer);
        CSS3Parser parser(&tokens);
        
        auto tree = parser.stylesheet();
        std::cout << "✓ 解析树创建成功" << std::endl;
        
        SimpleCSSListener listener;
        ParseTreeWalker walker;
        
        std::cout << "开始遍历解析树..." << std::endl;
        walker.walk(&listener, tree);
        std::cout << "✓ 遍历完成!" << std::endl;
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
}