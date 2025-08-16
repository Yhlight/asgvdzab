#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "parser/CHTLParser.h"
#include "lexer/CHTLToken.h"

using namespace chtl;

void testParserCreation() {
    std::cout << "=== 测试解析器创建 ===" << std::endl;
    
    // 测试默认配置
    ParserConfig default_config;
    auto parser1 = std::make_unique<CHTLParser>(default_config);
    
    std::cout << "默认配置解析器创建成功" << std::endl;
    std::cout << "错误恢复: " << (default_config.enable_error_recovery ? "启用" : "禁用") << std::endl;
    std::cout << "严格模式: " << (default_config.strict_mode ? "启用" : "禁用") << std::endl;
    std::cout << "保留注释: " << (default_config.preserve_comments ? "启用" : "禁用") << std::endl;
    
    // 测试自定义配置
    ParserConfig custom_config;
    custom_config.enable_error_recovery = false;
    custom_config.strict_mode = false;
    custom_config.max_error_count = 50;
    custom_config.max_nesting_depth = 50;
    
    auto parser2 = std::make_unique<CHTLParser>(custom_config);
    
    std::cout << "自定义配置解析器创建成功" << std::endl;
    std::cout << "错误恢复: " << (custom_config.enable_error_recovery ? "启用" : "禁用") << std::endl;
    std::cout << "严格模式: " << (custom_config.strict_mode ? "启用" : "禁用") << std::endl;
    std::cout << "最大错误数: " << custom_config.max_error_count << std::endl;
    std::cout << "最大嵌套深度: " << custom_config.max_nesting_depth << std::endl;
    
    std::cout << std::endl;
}

void testParserState() {
    std::cout << "=== 测试解析器状态管理 ===" << std::endl;
    
    ParserConfig config;
    auto parser = std::make_unique<CHTLParser>(config);
    
    // 测试初始状态
    std::cout << "初始状态: " << static_cast<int>(parser->getCurrentState()) << std::endl;
    std::cout << "是否在初始状态: " << (parser->isInState(ParserState::INITIAL) ? "是" : "否") << std::endl;
    
    // 测试状态切换
    parser->setState(ParserState::EXPECTING_ELEMENT);
    std::cout << "切换后状态: " << static_cast<int>(parser->getCurrentState()) << std::endl;
    std::cout << "是否在期望元素状态: " << (parser->isInState(ParserState::EXPECTING_ELEMENT) ? "是" : "否") << std::endl;
    
    parser->setState(ParserState::EXPECTING_ATTRIBUTE);
    std::cout << "再次切换后状态: " << static_cast<int>(parser->getCurrentState()) << std::endl;
    
    // 恢复初始状态
    parser->setState(ParserState::INITIAL);
    std::cout << "恢复后状态: " << static_cast<int>(parser->getCurrentState()) << std::endl;
    
    std::cout << std::endl;
}

void testParserErrors() {
    std::cout << "=== 测试解析器错误处理 ===" << std::endl;
    
    ParserConfig config;
    auto parser = std::make_unique<CHTLParser>(config);
    
    // 测试初始状态
    std::cout << "初始错误状态: " << (parser->hasErrors() ? "有错误" : "无错误") << std::endl;
    std::cout << "错误数量: " << parser->getErrors().size() << std::endl;
    
    // 添加一些测试错误
    parser->addError(ParserErrorType::LEXICAL_ERROR, "测试词法错误", 1, 1, 0);
    parser->addError(ParserErrorType::SYNTAX_ERROR, "测试语法错误", 2, 5, 10);
    parser->addError(ParserErrorType::SEMANTIC_ERROR, "测试语义错误", 3, 10, 20);
    
    std::cout << "添加错误后状态: " << (parser->hasErrors() ? "有错误" : "无错误") << std::endl;
    std::cout << "错误数量: " << parser->getErrors().size() << std::endl;
    
    // 显示错误详情
    auto errors = parser->getErrors();
    for (size_t i = 0; i < errors.size(); i++) {
        const auto& error = errors[i];
        std::cout << "错误 " << (i + 1) << ":" << std::endl;
        std::cout << "  类型: " << static_cast<int>(error.type) << std::endl;
        std::cout << "  消息: " << error.message << std::endl;
        std::cout << "  行号: " << error.line << std::endl;
        std::cout << "  列号: " << error.column << std::endl;
        std::cout << "  位置: " << error.position << std::endl;
        std::cout << "  可恢复: " << (error.is_recoverable ? "是" : "否") << std::endl;
    }
    
    // 测试错误清除
    parser->clearErrors();
    std::cout << "清除错误后状态: " << (parser->hasErrors() ? "有错误" : "无错误") << std::endl;
    std::cout << "错误数量: " << parser->getErrors().size() << std::endl;
    
    std::cout << std::endl;
}

void testParserStats() {
    std::cout << "=== 测试解析器统计信息 ===" << std::endl;
    
    ParserConfig config;
    auto parser = std::make_unique<CHTLParser>(config);
    
    // 获取初始统计信息
    auto stats = parser->getStats();
    std::cout << "初始统计信息:" << std::endl;
    std::cout << "  总Token数: " << stats.total_tokens << std::endl;
    std::cout << "  总节点数: " << stats.total_nodes << std::endl;
    std::cout << "  错误数量: " << stats.error_count << std::endl;
    std::cout << "  恢复次数: " << stats.recovery_count << std::endl;
    std::cout << "  解析时间: " << stats.parse_time_ms << "ms" << std::endl;
    
    std::cout << std::endl;
}

void testParserDebug() {
    std::cout << "=== 测试解析器调试功能 ===" << std::endl;
    
    ParserConfig config;
    config.enable_debug = true;
    auto parser = std::make_unique<CHTLParser>(config);
    
    // 获取调试信息
    std::string debug_info = parser->getDebugInfo();
    std::cout << "调试信息:" << std::endl;
    std::cout << debug_info << std::endl;
    
    // 测试调试模式切换
    parser->enableDebugMode(false);
    std::cout << "禁用调试模式后:" << std::endl;
    std::cout << "调试模式: " << (parser->getConfig().enable_debug ? "启用" : "禁用") << std::endl;
    
    parser->enableDebugMode(true);
    std::cout << "启用调试模式后:" << std::endl;
    std::cout << "调试模式: " << (parser->getConfig().enable_debug ? "启用" : "禁用") << std::endl;
    
    std::cout << std::endl;
}

void testParserConfiguration() {
    std::cout << "=== 测试解析器配置管理 ===" << std::endl;
    
    ParserConfig config;
    auto parser = std::make_unique<CHTLParser>(config);
    
    // 获取当前配置
    auto current_config = parser->getConfig();
    std::cout << "当前配置:" << std::endl;
    std::cout << "  错误恢复: " << (current_config.enable_error_recovery ? "启用" : "禁用") << std::endl;
    std::cout << "  严格模式: " << (current_config.strict_mode ? "启用" : "禁用") << std::endl;
    std::cout << "  保留注释: " << (current_config.preserve_comments ? "启用" : "禁用") << std::endl;
    std::cout << "  最大错误数: " << current_config.max_error_count << std::endl;
    std::cout << "  最大嵌套深度: " << current_config.max_nesting_depth << std::endl;
    
    // 修改配置
    ParserConfig new_config;
    new_config.enable_error_recovery = false;
    new_config.strict_mode = false;
    new_config.max_error_count = 200;
    new_config.max_nesting_depth = 200;
    
    parser->setConfig(new_config);
    
    // 验证配置已更新
    auto updated_config = parser->getConfig();
    std::cout << "更新后配置:" << std::endl;
    std::cout << "  错误恢复: " << (updated_config.enable_error_recovery ? "启用" : "禁用") << std::endl;
    std::cout << "  严格模式: " << (updated_config.strict_mode ? "启用" : "禁用") << std::endl;
    std::cout << "  最大错误数: " << updated_config.max_error_count << std::endl;
    std::cout << "  最大嵌套深度: " << updated_config.max_nesting_depth << std::endl;
    
    std::cout << std::endl;
}

void testParserFileOperations() {
    std::cout << "=== 测试解析器文件操作 ===" << std::endl;
    
    ParserConfig config;
    auto parser = std::make_unique<CHTLParser>(config);
    
    // 测试解析不存在的文件
    std::string non_existent_file = "non_existent_file.chtl";
    auto result = parser->parseFile(non_existent_file);
    
    if (!result) {
        std::cout << "解析不存在的文件失败，符合预期" << std::endl;
        std::cout << "错误状态: " << (parser->hasErrors() ? "有错误" : "无错误") << std::endl;
        
        auto errors = parser->getErrors();
        if (!errors.empty()) {
            std::cout << "错误消息: " << errors[0].message << std::endl;
        }
    } else {
        std::cout << "警告：解析不存在的文件意外成功" << std::endl;
    }
    
    // 测试解析空字符串
    auto string_result = parser->parseString("");
    if (!string_result) {
        std::cout << "解析空字符串失败，符合预期" << std::endl;
    } else {
        std::cout << "警告：解析空字符串意外成功" << std::endl;
    }
    
    std::cout << std::endl;
}

void testParserTokenHandling() {
    std::cout << "=== 测试解析器Token处理 ===" << std::endl;
    
    ParserConfig config;
    auto parser = std::make_unique<CHTLParser>(config);
    
    // 创建测试tokens
    std::vector<Token> test_tokens = {
        {TokenType::IDENTIFIER, "html", 1, 1, 0},
        {TokenType::LEFT_BRACE, "{", 1, 6, 5},
        {TokenType::IDENTIFIER, "head", 2, 5, 10},
        {TokenType::LEFT_BRACE, "{", 2, 10, 15},
        {TokenType::IDENTIFIER, "title", 3, 9, 20},
        {TokenType::LEFT_BRACE, "{", 3, 15, 25},
        {TokenType::TEXT, "测试页面", 4, 13, 30},
        {TokenType::RIGHT_BRACE, "}", 5, 9, 35},
        {TokenType::RIGHT_BRACE, "}", 6, 5, 40},
        {TokenType::RIGHT_BRACE, "}", 7, 1, 45}
    };
    
    // 测试解析tokens
    auto result = parser->parse(test_tokens);
    
    if (result) {
        std::cout << "成功解析测试tokens" << std::endl;
        std::cout << "生成的AST节点数: " << result->getTotalNodeCount() << std::endl;
        
        // 获取统计信息
        auto stats = parser->getStats();
        std::cout << "解析统计:" << std::endl;
        std::cout << "  总Token数: " << stats.total_tokens << std::endl;
        std::cout << "  总节点数: " << stats.total_nodes << std::endl;
        std::cout << "  解析时间: " << stats.parse_time_ms << "ms" << std::endl;
    } else {
        std::cout << "解析测试tokens失败" << std::endl;
        if (parser->hasErrors()) {
            auto errors = parser->getErrors();
            for (const auto& error : errors) {
                std::cout << "错误: " << error.message << std::endl;
            }
        }
    }
    
    std::cout << std::endl;
}

int main() {
    std::cout << "CHTL解析器系统测试" << std::endl;
    std::cout << "==================" << std::endl << std::endl;
    
    try {
        testParserCreation();
        testParserState();
        testParserErrors();
        testParserStats();
        testParserDebug();
        testParserConfiguration();
        testParserFileOperations();
        testParserTokenHandling();
        
        std::cout << "所有测试完成！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "测试过程中发生异常: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}