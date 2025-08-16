#include <iostream>
#include <string>
#include "src/compilers/basic_chtl_compiler.cpp"

using namespace chtl;

void testBasicElement() {
    std::cout << "=== 测试基本元素 ===" << std::endl;
    
    BasicCHTLCompiler compiler;
    compiler.initialize();
    
    CodeFragment fragment;
    fragment.content = R"(div {
        id: box;
        class: container;
        
        text {
            "Hello, CHTL!"
        }
    })";
    
    fragment.type = CodeFragmentType::CHTL_FRAGMENT;
    
    auto result = compiler.compile(fragment);
    
    std::cout << "输入: " << fragment.content << std::endl;
    std::cout << "输出: " << result.output << std::endl;
    std::cout << "成功: " << (result.success ? "是" : "否") << std::endl;
    
    if (!result.errors.empty()) {
        std::cout << "错误:" << std::endl;
        for (const auto& error : result.errors) {
            std::cout << "  - " << error << std::endl;
        }
    }
    
    std::cout << std::endl;
}

void testTextNode() {
    std::cout << "=== 测试文本节点 ===" << std::endl;
    
    BasicCHTLCompiler compiler;
    compiler.initialize();
    
    CodeFragment fragment;
    fragment.content = R"(text {
        "这是一段文本"
    })";
    
    fragment.type = CodeFragmentType::CHTL_FRAGMENT;
    
    auto result = compiler.compile(fragment);
    
    std::cout << "输入: " << fragment.content << std::endl;
    std::cout << "输出: " << result.output << std::endl;
    std::cout << "成功: " << (result.success ? "是" : "否") << std::endl;
    std::cout << std::endl;
}

void testComment() {
    std::cout << "=== 测试注释 ===" << std::endl;
    
    BasicCHTLCompiler compiler;
    compiler.initialize();
    
    CodeFragment fragment;
    fragment.content = "-- 这是生成器注释";
    fragment.type = CodeFragmentType::CHTL_FRAGMENT;
    
    auto result = compiler.compile(fragment);
    
    std::cout << "输入: " << fragment.content << std::endl;
    std::cout << "输出: " << result.output << std::endl;
    std::cout << "成功: " << (result.success ? "是" : "否") << std::endl;
    std::cout << std::endl;
}

void testTemplateDefinition() {
    std::cout << "=== 测试模板定义 ===" << std::endl;
    
    BasicCHTLCompiler compiler;
    compiler.initialize();
    
    CodeFragment fragment;
    fragment.content = R"([Template] @Style DefaultText {
        color: black;
        font-size: 16px;
    })";
    
    fragment.type = CodeFragmentType::CHTL_FRAGMENT;
    
    auto result = compiler.compile(fragment);
    
    std::cout << "输入: " << fragment.content << std::endl;
    std::cout << "输出: " << result.output << std::endl;
    std::cout << "成功: " << (result.success ? "是" : "否") << std::endl;
    std::cout << std::endl;
}

void testOriginEmbedding() {
    std::cout << "=== 测试原始嵌入 ===" << std::endl;
    
    BasicCHTLCompiler compiler;
    compiler.initialize();
    
    CodeFragment fragment;
    fragment.content = R"([Origin] @Html {
        <p>这是原始HTML内容</p>
    })";
    
    fragment.type = CodeFragmentType::CHTL_FRAGMENT;
    
    auto result = compiler.compile(fragment);
    
    std::cout << "输入: " << fragment.content << std::endl;
    std::cout << "输出: " << result.output << std::endl;
    std::cout << "成功: " << (result.success ? "是" : "否") << std::endl;
    std::cout << std::endl;
}

int main() {
    std::cout << "CHTL基础编译器测试" << std::endl;
    std::cout << "===================" << std::endl;
    std::cout << std::endl;
    
    try {
        testComment();
        testTextNode();
        testBasicElement();
        testTemplateDefinition();
        testOriginEmbedding();
        
        std::cout << "所有测试完成！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "测试过程中发生错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}