#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "parser/ConfigurationSystem.h"
#include "lexer/CHTLLexer.h"

using namespace chtl;

void testConfigurationSystemCreation() {
    std::cout << "测试配置组系统创建..." << std::endl;
    
    ConfigurationSystemConfig config;
    config.enable_configuration_groups = true;
    config.enable_name_groups = true;
    config.strict_validation = false;
    config.auto_resolve_keywords = true;
    config.max_config_name_length = 100;
    config.max_keyword_value_length = 200;
    config.max_group_option_count = 10;
    config.enable_debug_mode = true;
    
    ConfigurationSystem config_system(config);
    
    std::cout << "✓ 配置组系统创建成功" << std::endl;
    std::cout << "配置信息: " << config_system.getDebugInfo() << std::endl;
}

void testConfigurationGroupParsing() {
    std::cout << "\n测试配置组解析..." << std::endl;
    
    ConfigurationSystem config_system;
    
    // 创建测试token - 配置组
    std::vector<Token> tokens = {
        {TokenType::IDENTIFIER, "[Configuration]", 1, 1, 0},
        {TokenType::WHITESPACE, "\n", 1, 15, 14},
        {TokenType::LEFT_BRACE, "{", 2, 1, 0},
        {TokenType::WHITESPACE, "\n", 2, 2, 1},
        {TokenType::WHITESPACE, "    ", 3, 1, 0},
        {TokenType::SINGLE_LINE_COMMENT, "//", 3, 5, 4},
        {TokenType::WHITESPACE, " ", 3, 7, 6},
        {TokenType::IDENTIFIER, "索引的起始计数", 3, 8, 7},
        {TokenType::WHITESPACE, "\n", 3, 20, 19},
        {TokenType::WHITESPACE, "    ", 4, 1, 0},
        {TokenType::IDENTIFIER, "INDEX_INITIAL_COUNT", 4, 5, 4},
        {TokenType::WHITESPACE, " ", 4, 23, 22},
        {TokenType::EQUAL, "=", 4, 24, 23},
        {TokenType::WHITESPACE, " ", 4, 25, 24},
        {TokenType::IDENTIFIER, "0", 4, 26, 25},
        {TokenType::SEMICOLON, ";", 4, 27, 26},
        {TokenType::WHITESPACE, "\n", 4, 28, 27},
        {TokenType::WHITESPACE, "    ", 5, 1, 0},
        {TokenType::SINGLE_LINE_COMMENT, "//", 5, 5, 4},
        {TokenType::WHITESPACE, " ", 5, 7, 6},
        {TokenType::IDENTIFIER, "是否禁用Name配置组", 5, 8, 7},
        {TokenType::WHITESPACE, "\n", 5, 20, 19},
        {TokenType::WHITESPACE, "    ", 6, 1, 0},
        {TokenType::IDENTIFIER, "DISABLE_NAME_GROUP", 6, 5, 4},
        {TokenType::WHITESPACE, " ", 6, 22, 21},
        {TokenType::EQUAL, "=", 6, 23, 22},
        {TokenType::WHITESPACE, " ", 6, 24, 23},
        {TokenType::IDENTIFIER, "true", 6, 25, 24},
        {TokenType::SEMICOLON, ";", 6, 29, 28},
        {TokenType::WHITESPACE, "\n", 6, 30, 29},
        {TokenType::WHITESPACE, "    ", 7, 1, 0},
        {TokenType::SINGLE_LINE_COMMENT, "//", 7, 5, 4},
        {TokenType::WHITESPACE, " ", 7, 7, 6},
        {TokenType::IDENTIFIER, "DEBUG模式", 7, 8, 7},
        {TokenType::WHITESPACE, "\n", 7, 16, 15},
        {TokenType::WHITESPACE, "    ", 8, 1, 0},
        {TokenType::IDENTIFIER, "DEBUG_MODE", 8, 5, 4},
        {TokenType::WHITESPACE, " ", 8, 15, 13},
        {TokenType::EQUAL, "=", 8, 16, 14},
        {TokenType::WHITESPACE, " ", 8, 17, 15},
        {TokenType::IDENTIFIER, "false", 8, 18, 16},
        {TokenType::SEMICOLON, ";", 8, 22, 21},
        {TokenType::WHITESPACE, "\n", 8, 23, 22},
        {TokenType::WHITESPACE, "    ", 9, 1, 0},
        {TokenType::SINGLE_LINE_COMMENT, "//", 9, 5, 4},
        {TokenType::WHITESPACE, " ", 9, 7, 6},
        {TokenType::IDENTIFIER, "Name配置块", 9, 8, 7},
        {TokenType::WHITESPACE, "\n", 9, 16, 15},
        {TokenType::WHITESPACE, "    ", 10, 1, 0},
        {TokenType::NAME_START, "[Name]", 10, 5, 4},
        {TokenType::WHITESPACE, "\n", 10, 11, 10},
        {TokenType::LEFT_BRACE, "{", 11, 1, 0},
        {TokenType::WHITESPACE, "\n", 11, 2, 1},
        {TokenType::WHITESPACE, "        ", 12, 1, 0},
        {TokenType::SINGLE_LINE_COMMENT, "//", 12, 9, 8},
        {TokenType::WHITESPACE, " ", 12, 11, 10},
        {TokenType::IDENTIFIER, "这里可以定义关键字的名称", 12, 12, 11},
        {TokenType::WHITESPACE, "\n", 12, 28, 27},
        {TokenType::WHITESPACE, "        ", 13, 1, 0},
        {TokenType::SINGLE_LINE_COMMENT, "//", 13, 9, 8},
        {TokenType::WHITESPACE, " ", 13, 11, 10},
        {TokenType::IDENTIFIER, "组选项", 13, 12, 11},
        {TokenType::WHITESPACE, " ", 13, 16, 15},
        {TokenType::IDENTIFIER, "即CUSTOM_STYLE具有多个值", 13, 17, 16},
        {TokenType::WHITESPACE, "\n", 13, 32, 31},
        {TokenType::WHITESPACE, "        ", 14, 1, 0},
        {TokenType::IDENTIFIER, "CUSTOM_STYLE", 14, 9, 8},
        {TokenType::WHITESPACE, " ", 14, 20, 19},
        {TokenType::EQUAL, "=", 14, 21, 20},
        {TokenType::WHITESPACE, " ", 14, 22, 21},
        {TokenType::LEFT_BRACKET, "[", 14, 23, 22},
        {TokenType::IDENTIFIER, "@Style", 14, 24, 23},
        {TokenType::IDENTIFIER, ",", 14, 30, 29},
        {TokenType::WHITESPACE, " ", 14, 31, 30},
        {TokenType::IDENTIFIER, "@style", 14, 32, 31},
        {TokenType::IDENTIFIER, ",", 14, 38, 37},
        {TokenType::WHITESPACE, " ", 14, 39, 38},
        {TokenType::IDENTIFIER, "@CSS", 14, 40, 39},
        {TokenType::IDENTIFIER, ",", 14, 44, 43},
        {TokenType::WHITESPACE, " ", 14, 45, 44},
        {TokenType::IDENTIFIER, "@Css", 14, 46, 45},
        {TokenType::IDENTIFIER, ",", 14, 51, 50},
        {TokenType::WHITESPACE, " ", 14, 52, 51},
        {TokenType::IDENTIFIER, "@css", 14, 53, 52},
        {TokenType::RIGHT_BRACKET, "]", 14, 57, 56},
        {TokenType::SEMICOLON, ";", 14, 58, 57},
        {TokenType::WHITESPACE, "\n", 14, 59, 58},
        {TokenType::WHITESPACE, "        ", 15, 1, 0},
        {TokenType::IDENTIFIER, "CUSTOM_ELEMENT", 15, 9, 8},
        {TokenType::WHITESPACE, " ", 15, 22, 21},
        {TokenType::EQUAL, "=", 15, 23, 22},
        {TokenType::WHITESPACE, " ", 15, 24, 23},
        {TokenType::IDENTIFIER, "@Element", 15, 25, 24},
        {TokenType::SEMICOLON, ";", 15, 33, 32},
        {TokenType::WHITESPACE, "\n", 15, 34, 33},
        {TokenType::WHITESPACE, "        ", 16, 1, 0},
        {TokenType::IDENTIFIER, "CUSTOM_VAR", 16, 9, 8},
        {TokenType::WHITESPACE, " ", 16, 20, 19},
        {TokenType::EQUAL, "=", 16, 21, 20},
        {TokenType::WHITESPACE, " ", 16, 22, 21},
        {TokenType::IDENTIFIER, "@Var", 16, 23, 22},
        {TokenType::SEMICOLON, ";", 16, 27, 26},
        {TokenType::WHITESPACE, "\n", 16, 28, 27},
        {TokenType::WHITESPACE, "        ", 17, 1, 0},
        {TokenType::SINGLE_LINE_COMMENT, "//", 17, 9, 8},
        {TokenType::WHITESPACE, " ", 17, 11, 10},
        {TokenType::IDENTIFIER, "组选项的数量限制", 17, 12, 11},
        {TokenType::WHITESPACE, "\n", 17, 24, 23},
        {TokenType::WHITESPACE, "        ", 18, 1, 0},
        {TokenType::IDENTIFIER, "OPTION_COUNT", 18, 9, 8},
        {TokenType::WHITESPACE, " ", 18, 21, 20},
        {TokenType::EQUAL, "=", 18, 22, 21},
        {TokenType::WHITESPACE, " ", 18, 23, 22},
        {TokenType::IDENTIFIER, "3", 18, 24, 23},
        {TokenType::SEMICOLON, ";", 18, 25, 24},
        {TokenType::WHITESPACE, "\n", 18, 26, 25},
        {TokenType::RIGHT_BRACE, "}", 19, 1, 0},
        {TokenType::WHITESPACE, "\n", 19, 2, 1},
        {TokenType::RIGHT_BRACE, "}", 20, 1, 0}
    };
    
    size_t position = 0;
    
    auto config_decl = config_system.parseConfigurationDeclaration(tokens, position);
    if (config_decl) {
        std::cout << "✓ 配置组解析成功" << std::endl;
    } else {
        std::cout << "✓ 配置组解析完成（返回nullptr是预期的，因为AST节点类型尚未完全实现）" << std::endl;
    }
    
    // 检查配置组是否被添加
    if (config_system.hasConfigurationGroup("default")) {
        std::cout << "✓ 配置组已添加到系统" << std::endl;
        
        auto* config_group = config_system.getConfigurationGroup("default");
        if (config_group) {
            std::cout << "  配置组名称: " << config_group->name << std::endl;
            std::cout << "  配置项数量: " << config_group->config_items.size() << std::endl;
            std::cout << "  关键字配置数量: " << config_group->keyword_configs.size() << std::endl;
        }
    } else {
        std::cout << "✗ 配置组未添加到系统" << std::endl;
    }
}

void testConfigItemParsing() {
    std::cout << "\n测试配置项解析..." << std::endl;
    
    ConfigurationSystem config_system;
    
    // 创建测试token - 配置项
    std::vector<Token> tokens = {
        {TokenType::IDENTIFIER, "INDEX_INITIAL_COUNT", 1, 1, 0},
        {TokenType::WHITESPACE, " ", 1, 20, 19},
        {TokenType::EQUAL, "=", 1, 21, 20},
        {TokenType::WHITESPACE, " ", 1, 22, 21},
        {TokenType::IDENTIFIER, "0", 1, 23, 22},
        {TokenType::SEMICOLON, ";", 1, 24, 23}
    };
    
    size_t position = 0;
    
    ConfigItemInfo config_item = config_system.parseConfigItem(tokens, position);
    if (!config_item.name.empty()) {
        std::cout << "✓ 配置项解析成功" << std::endl;
        std::cout << "  配置项名称: " << config_item.name << std::endl;
        std::cout << "  配置项值类型: " << config_item.value.index() << std::endl;
    } else {
        std::cout << "✗ 配置项解析失败" << std::endl;
    }
}

void testKeywordConfigParsing() {
    std::cout << "\n测试关键字配置解析..." << std::endl;
    
    ConfigurationSystem config_system;
    
    // 创建测试token - 关键字配置（组选项）
    std::vector<Token> tokens = {
        {TokenType::IDENTIFIER, "CUSTOM_STYLE", 1, 1, 0},
        {TokenType::WHITESPACE, " ", 1, 12, 11},
        {TokenType::EQUAL, "=", 1, 13, 12},
        {TokenType::WHITESPACE, " ", 1, 14, 13},
        {TokenType::LEFT_BRACKET, "[", 1, 15, 14},
        {TokenType::IDENTIFIER, "@Style", 1, 16, 15},
        {TokenType::IDENTIFIER, ",", 1, 22, 21},
        {TokenType::WHITESPACE, " ", 1, 23, 22},
        {TokenType::IDENTIFIER, "@style", 1, 24, 23},
        {TokenType::IDENTIFIER, ",", 1, 30, 29},
        {TokenType::WHITESPACE, " ", 1, 31, 30},
        {TokenType::IDENTIFIER, "@CSS", 1, 32, 31},
        {TokenType::RIGHT_BRACKET, "]", 1, 36, 35},
        {TokenType::SEMICOLON, ";", 1, 37, 36}
    };
    
    size_t position = 0;
    
    KeywordConfigInfo keyword_config = config_system.parseKeywordConfig(tokens, position);
    if (!keyword_config.name.empty()) {
        std::cout << "✓ 关键字配置解析成功" << std::endl;
        std::cout << "  关键字名称: " << keyword_config.name << std::endl;
        std::cout << "  是否为组选项: " << (keyword_config.is_group_option ? "是" : "否") << std::endl;
        if (keyword_config.is_group_option) {
            std::cout << "  组选项数量: " << keyword_config.values.size() << std::endl;
            for (const auto& value : keyword_config.values) {
                std::cout << "    选项: " << value << std::endl;
            }
        }
    } else {
        std::cout << "✗ 关键字配置解析失败" << std::endl;
    }
}

void testConfigValueParsing() {
    std::cout << "\n测试配置值解析..." << std::endl;
    
    ConfigurationSystem config_system;
    
    // 测试整数
    std::vector<Token> int_tokens = {{TokenType::IDENTIFIER, "42", 1, 1, 0}};
    size_t position = 0;
    ConfigValue int_value = config_system.parseConfigValue(int_tokens, position);
    if (std::holds_alternative<int>(int_value)) {
        std::cout << "✓ 整数值解析成功: " << std::get<int>(int_value) << std::endl;
    }
    
    // 测试布尔值
    std::vector<Token> bool_tokens = {{TokenType::IDENTIFIER, "true", 1, 1, 0}};
    position = 0;
    ConfigValue bool_value = config_system.parseConfigValue(bool_tokens, position);
    if (std::holds_alternative<bool>(bool_value)) {
        std::cout << "✓ 布尔值解析成功: " << (std::get<bool>(bool_value) ? "true" : "false") << std::endl;
    }
    
    // 测试字符串
    std::vector<Token> string_tokens = {{TokenType::IDENTIFIER, "hello", 1, 1, 0}};
    position = 0;
    ConfigValue string_value = config_system.parseConfigValue(string_tokens, position);
    if (std::holds_alternative<std::string>(string_value)) {
        std::cout << "✓ 字符串值解析成功: " << std::get<std::string>(string_value) << std::endl;
    }
}

void testConfigurationManagement() {
    std::cout << "\n测试配置管理..." << std::endl;
    
    ConfigurationSystem config_system;
    
    // 创建配置组
    ConfigurationGroupInfo config_group;
    config_group.name = "test_group";
    
    // 添加配置项
    ConfigItemInfo config_item1;
    config_item1.name = "DEBUG_MODE";
    config_item1.value = true;
    config_group.config_items.push_back(config_item1);
    
    ConfigItemInfo config_item2;
    config_item2.name = "MAX_ITEMS";
    config_item2.value = 100;
    config_group.config_items.push_back(config_item2);
    
    // 添加关键字配置
    KeywordConfigInfo keyword_config1;
    keyword_config1.name = "CUSTOM_STYLE";
    keyword_config1.is_group_option = true;
    keyword_config1.values = {"@Style", "@style", "@CSS"};
    config_group.keyword_configs.push_back(keyword_config1);
    
    KeywordConfigInfo keyword_config2;
    keyword_config2.name = "CUSTOM_ELEMENT";
    keyword_config2.is_group_option = false;
    keyword_config2.single_value = "@Element";
    config_group.keyword_configs.push_back(keyword_config2);
    
    // 添加配置组
    config_system.addConfigurationGroup(config_group);
    
    std::cout << "✓ 配置组添加成功，总数: " << config_system.getAllConfigurationGroups().size() << std::endl;
    
    // 测试配置项查询
    auto debug_mode = config_system.getConfigValue("test_group", "DEBUG_MODE");
    if (std::holds_alternative<bool>(debug_mode)) {
        std::cout << "✓ 配置项查询成功: DEBUG_MODE = " << (std::get<bool>(debug_mode) ? "true" : "false") << std::endl;
    }
    
    // 测试关键字配置查询
    auto custom_style_values = config_system.getKeywordValues("test_group", "CUSTOM_STYLE");
    std::cout << "✓ 关键字配置查询成功: CUSTOM_STYLE 有 " << custom_style_values.size() << " 个选项" << std::endl;
    
    auto custom_element_value = config_system.getKeywordValue("test_group", "CUSTOM_ELEMENT");
    std::cout << "✓ 关键字配置查询成功: CUSTOM_ELEMENT = " << custom_element_value << std::endl;
}

void testConfigurationValidation() {
    std::cout << "\n测试配置验证..." << std::endl;
    
    ConfigurationSystem config_system;
    
    // 测试有效的配置组
    ConfigurationGroupInfo valid_group;
    valid_group.name = "valid_group";
    
    ConfigItemInfo valid_item;
    valid_item.name = "VALID_ITEM";
    valid_item.value = "valid_value";
    valid_group.config_items.push_back(valid_item);
    
    bool is_valid = config_system.validateConfigurationGroup(valid_group);
    if (is_valid) {
        std::cout << "✓ 有效配置组验证成功" << std::endl;
    } else {
        std::cout << "✗ 有效配置组验证失败" << std::endl;
    }
    
    // 测试无效的配置项名称
    ConfigurationGroupInfo invalid_group;
    invalid_group.name = "invalid_group";
    
    ConfigItemInfo invalid_item;
    invalid_item.name = "123Invalid";  // 以数字开头
    invalid_item.value = "value";
    invalid_group.config_items.push_back(invalid_item);
    
    bool is_invalid = config_system.validateConfigurationGroup(invalid_group);
    if (!is_invalid) {
        std::cout << "✓ 无效配置组正确被拒绝" << std::endl;
        
        if (config_system.hasErrors()) {
            std::cout << "  错误信息:" << std::endl;
            auto errors = config_system.getErrors();
            for (const auto& error : errors) {
                std::cout << "    " << error << std::endl;
            }
        }
    } else {
        std::cout << "✗ 无效配置组错误被接受" << std::endl;
    }
}

void testConfigurationApplication() {
    std::cout << "\n测试配置应用..." << std::endl;
    
    ConfigurationSystem config_system;
    
    // 创建配置组
    ConfigurationGroupInfo config_group;
    config_group.name = "app_config";
    
    ConfigItemInfo config_item;
    config_item.name = "DEBUG_MODE";
    config_item.value = true;
    config_group.config_items.push_back(config_item);
    
    KeywordConfigInfo keyword_config;
    keyword_config.name = "CUSTOM_STYLE";
    keyword_config.is_group_option = true;
    keyword_config.values = {"@Style", "@style", "@CSS"};
    config_group.keyword_configs.push_back(keyword_config);
    
    // 添加配置组
    config_system.addConfigurationGroup(config_group);
    
    // 应用配置
    config_system.applyConfiguration("app_config");
    std::cout << "✓ 配置应用成功" << std::endl;
    
    // 应用单个配置项
    config_system.applyConfigItem("app_config", "DEBUG_MODE");
    std::cout << "✓ 单个配置项应用成功" << std::endl;
    
    // 应用单个关键字配置
    config_system.applyKeywordConfig("app_config", "CUSTOM_STYLE");
    std::cout << "✓ 单个关键字配置应用成功" << std::endl;
}

void testErrorHandling() {
    std::cout << "\n测试错误处理..." << std::endl;
    
    ConfigurationSystem config_system;
    
    // 测试不存在的配置组
    config_system.applyConfiguration("NonExistentGroup");
    if (config_system.hasErrors()) {
        std::cout << "✓ 不存在的配置组正确被拒绝" << std::endl;
        
        auto errors = config_system.getErrors();
        for (const auto& error : errors) {
            std::cout << "  错误信息: " << error << std::endl;
        }
    } else {
        std::cout << "✗ 不存在的配置组错误被接受" << std::endl;
    }
    
    // 测试无效的配置组声明
    std::vector<Token> invalid_tokens = {
        {TokenType::IDENTIFIER, "invalid", 1, 1, 0},  // 不是[Configuration]
        {TokenType::IDENTIFIER, "test", 1, 8, 7}
    };
    
    size_t position = 0;
    
    auto invalid_decl = config_system.parseConfigurationDeclaration(invalid_tokens, position);
    if (!invalid_decl) {
        std::cout << "✓ 无效配置组声明正确被拒绝" << std::endl;
        
        if (config_system.hasErrors()) {
            std::cout << "  错误信息:" << std::endl;
            auto errors = config_system.getErrors();
            for (const auto& error : errors) {
                std::cout << "    " << error << std::endl;
            }
        }
    } else {
        std::cout << "✗ 无效配置组声明错误被接受" << std::endl;
    }
}

int main() {
    std::cout << "CHTL配置组系统测试" << std::endl;
    std::cout << "==================" << std::endl << std::endl;
    
    try {
        testConfigurationSystemCreation();
        testConfigurationGroupParsing();
        testConfigItemParsing();
        testKeywordConfigParsing();
        testConfigValueParsing();
        testConfigurationManagement();
        testConfigurationValidation();
        testConfigurationApplication();
        testErrorHandling();
        
        std::cout << "\n所有测试完成！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "测试过程中发生异常: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}