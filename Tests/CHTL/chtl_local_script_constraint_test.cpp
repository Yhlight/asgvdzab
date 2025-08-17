#include "constraints/chtl_local_script_constraint.hpp"
#include <iostream>

using namespace chtl;

void testValidLocalScriptSyntax() {
    std::cout << "=== CHTL局部script块允许语法测试 ===" << std::endl;
    
    CHtlLocalScriptConstraint constraint;
    
    // Test valid local script syntax
    std::string validLocalScript = R"(
        /* 允许的CHTL语法 */
        
        // 模板变量
        const primaryColor = ThemeColor(primary);
        const bgColor = @Var ColorPalette(secondary);
        
        // 自定义变量组特例化
        const customColor = ThemeColor(tableColor = rgb(255, 192, 203));
        
        // 命名空间from
        ConfigData from config/settings.chtl;
        
        // 生成器注释
        -- 局部script配置信息
        -- 组件特定的JavaScript逻辑
        
        // 原始嵌入
        [Origin] @JavaScript {
            window.localConfig = { mode: 'local' };
        }
        
        [Origin] @Html {
            <div id="local-marker"></div>
        }
        
        // JavaScript模板语法（特供语法，不应被误禁）
        const userName = {{user.name}};
        const content = {{{htmlContent}}};
        const safeText = {{&unescapedText}};
        
        // 纯JavaScript代码
        function setupLocalScript() {
            console.log('Local script initialized');
            
            const config = {
                debug: true,
                features: ['template', 'variables']
            };
            
            // 使用JavaScript模板语法
            document.getElementById('user-name').textContent = {{user.displayName}};
            
            return config;
        }
        
        // 事件监听器
        document.addEventListener('DOMContentLoaded', function() {
            setupLocalScript();
            
            // 更多JavaScript模板语法
            const messages = [
                {{#each messages}}
                    '{{message}}',
                {{/each}}
            ];
        });
        
        // 异步函数
        async function loadData() {
            try {
                const response = await fetch('/api/data');
                const data = await response.json();
                
                // 模板变量在JavaScript中的使用
                data.theme = ThemeColor(accent);
                
                return data;
            } catch (error) {
                console.error('Load failed:', error);
            }
        }
    )";
    
    auto result = constraint.validateCHtlLocalScriptBlock(validLocalScript);
    
    std::cout << "有效局部script语法测试: " << (result.isValid ? "✓ 通过" : "✗ 失败") << std::endl;
    if (!result.isValid) {
        for (const auto& violation : result.violations) {
            std::cout << "  错误: " << violation.message << " (" << violation.element << ")" << std::endl;
        }
    }
    
    std::cout << "模板变量: " << result.templateVariables.size() << std::endl;
    std::cout << "自定义变量: " << result.customVariables.size() << std::endl;
    std::cout << "生成器注释: " << result.foundGeneratorComments.size() << std::endl;
    std::cout << "原始嵌入: " << result.foundRawEmbeddings.size() << std::endl;
    std::cout << "from子句: " << result.fromClauses.size() << std::endl;
    std::cout << "JavaScript模板语法: " << result.jsTemplateSyntax.size() << std::endl;
    std::cout << "纯JavaScript行数: " << result.pureJavaScriptLines << std::endl;
}

void testForbiddenLocalScriptSyntax() {
    std::cout << "\n=== CHTL局部script块禁止语法测试 ===" << std::endl;
    
    CHtlLocalScriptConstraint constraint;
    
    // Test forbidden script syntax (style and element related)
    std::string forbiddenScript = R"(
        // ❌ 禁止的CHTL语法
        
        // 禁止样式相关语法
        @Style DefaultButton;
        inherit @Style BaseTheme;
        delete @Style OldTheme;
        [Custom] @Style MyButton;
        [Template] @Style ButtonBase;
        
        // 禁止元素相关语法
        @Element CustomDiv;
        text {
            "这是文本内容"
        }
        [Custom] @Element MyDiv;
        [Template] @Element BaseElement;
        
        // 禁止样式块
        style {
            color: red;
            background: blue;
        }
        
        // 但允许的语法应该正常工作
        const color = ThemeColor(primary);
        -- 这是允许的生成器注释
        
        [Origin] @JavaScript {
            console.log('允许的原始嵌入');
        }
        
        // JavaScript模板语法应该被保护
        const template = {{variable}};
        const html = {{{content}}};
        const safe = {{&text}};
    )";
    
    auto result = constraint.validateCHtlLocalScriptBlock(forbiddenScript);
    
    std::cout << "禁止语法测试: " << (result.isValid ? "✗ 应该失败但通过了" : "✓ 正确检测到错误") << std::endl;
    if (!result.isValid) {
        std::cout << "检测到的禁止语法违规:" << std::endl;
        for (const auto& violation : result.violations) {
            std::cout << "  - " << violation.message << std::endl;
        }
    }
    
    std::cout << "模板变量: " << result.templateVariables.size() << " 个" << std::endl;
    std::cout << "JavaScript模板语法: " << result.jsTemplateSyntax.size() << " 个" << std::endl;
    std::cout << "原始嵌入: " << result.foundRawEmbeddings.size() << " 个" << std::endl;
}

void testJavaScriptTemplateSyntaxProtection() {
    std::cout << "\n=== JavaScript模板语法保护测试 ===" << std::endl;
    
    CHtlLocalScriptConstraint constraint;
    
    // Test JavaScript template syntax protection
    std::cout << "\nJavaScript模板语法验证:" << std::endl;
    std::cout << "简单变量 '{{name}}': " 
              << (constraint.isJavaScriptTemplateSyntax("const name = {{user.name}};") ? "✓" : "✗") << std::endl;
    std::cout << "HTML内容 '{{{content}}}': " 
              << (constraint.isJavaScriptTemplateSyntax("div.innerHTML = {{{htmlContent}}};") ? "✓" : "✗") << std::endl;
    std::cout << "无转义 '{{&text}}': " 
              << (constraint.isJavaScriptTemplateSyntax("output.value = {{&rawText}};") ? "✓" : "✗") << std::endl;
    std::cout << "非模板语法 'function()': " 
              << (constraint.isJavaScriptTemplateSyntax("function test() { return true; }") ? "✗" : "✓") << std::endl;
    
    // Test comprehensive template syntax
    std::string templateScript = R"(
        // JavaScript模板语法保护测试
        
        // 各种模板语法
        const userName = {{user.name}};
        const userEmail = {{user.email}};
        const htmlContent = {{{article.content}}};
        const safeOutput = {{&message.text}};
        
        // 复杂模板表达式
        const computed = {{user.firstName + ' ' + user.lastName}};
        const conditional = {{user.isActive ? 'active' : 'inactive'}};
        
        // 在函数中使用
        function displayUser() {
            document.getElementById('name').textContent = {{user.displayName}};
            document.getElementById('avatar').src = {{user.avatar.url}};
        }
        
        // 数组和对象
        const data = {
            name: {{profile.name}},
            settings: {{{profile.settingsJson}}},
            preferences: {{&profile.preferencesString}}
        };
        
        // 允许的CHTL变量（应该与模板语法区分）
        const themeColor = ThemeColor(primary);
        DataConfig from config/data.chtl;
        
        -- 生成器注释：模板语法和CHTL语法可以共存
    )";
    
    auto result = constraint.validateCHtlLocalScriptBlock(templateScript);
    
    std::cout << "\n模板语法保护测试: " << (result.isValid ? "✓ 通过" : "✗ 失败") << std::endl;
    if (!result.isValid) {
        for (const auto& violation : result.violations) {
            std::cout << "  错误: " << violation.message << " (" << violation.element << ")" << std::endl;
        }
    }
    
    std::cout << "检测到的JavaScript模板语法: " << result.jsTemplateSyntax.size() << " 个" << std::endl;
    std::cout << "模板变量: " << result.templateVariables.size() << " 个" << std::endl;
    std::cout << "from子句: " << result.fromClauses.size() << " 个" << std::endl;
    std::cout << "生成器注释: " << result.foundGeneratorComments.size() << " 个" << std::endl;
}

void testLocalScriptSpecificConstraints() {
    std::cout << "\n=== 局部script特定约束测试 ===" << std::endl;
    
    CHtlLocalScriptConstraint constraint;
    
    // 测试生成器注释验证
    std::cout << "\n生成器注释验证:" << std::endl;
    std::cout << "有效注释 '-- 局部script配置': " 
              << (constraint.validateGeneratorComment("-- 局部script配置") ? "✓" : "✗") << std::endl;
    std::cout << "无效注释 '<!-- HTML注释 -->': " 
              << (constraint.validateGeneratorComment("<!-- HTML注释 -->") ? "✗" : "✓") << std::endl;
    
    // 测试原始嵌入验证
    std::cout << "\n原始嵌入验证:" << std::endl;
    std::cout << "HTML嵌入 '[Origin] @Html {': " 
              << (constraint.validateRawEmbedding("[Origin] @Html {") ? "✓" : "✗") << std::endl;
    std::cout << "CSS嵌入 '[Origin] @Style {': " 
              << (constraint.validateRawEmbedding("[Origin] @Style {") ? "✓" : "✗") << std::endl;
    std::cout << "JS嵌入 '[Origin] @JavaScript {': " 
              << (constraint.validateRawEmbedding("[Origin] @JavaScript {") ? "✓" : "✗") << std::endl;
    
    // 测试变量引用验证
    std::cout << "\n变量引用验证:" << std::endl;
    std::cout << "模板变量 'ThemeColor(primary)': " 
              << (constraint.validateTemplateVariableReference("const color = ThemeColor(primary);") ? "✓" : "✗") << std::endl;
    std::cout << "自定义变量 '@Var ColorSet(accent)': " 
              << (constraint.validateCustomVariableReference("const accent = @Var ColorSet(accent);") ? "✓" : "✗") << std::endl;
    std::cout << "变量特例化 'ColorSet(custom = red)': " 
              << (constraint.validateVariableSpecialization("const red = ColorSet(custom = red);") ? "✓" : "✗") << std::endl;
    
    // 测试from子句验证
    std::cout << "\nfrom子句验证:" << std::endl;
    std::cout << "导入语句 'Config from settings/app.chtl': " 
              << (constraint.validateFromClause("Config from settings/app.chtl") ? "✓" : "✗") << std::endl;
    std::cout << "无效导入 'Config import settings': " 
              << (constraint.validateFromClause("Config import settings") ? "✗" : "✓") << std::endl;
}

void testCompleteLocalScriptBlock() {
    std::cout << "\n=== 完整局部script块测试 ===" << std::endl;
    
    CHtlLocalScriptConstraint constraint;
    
    // 测试完整的局部script块
    std::string completeLocalScript = R"(
        /* 完整的CHTL局部script块示例 */
        
        -- 局部script配置
        -- 支持模板变量、自定义变量组、变量组特例化、命名空间from
        -- 生成器注释和原始嵌入是特殊存在
        -- JavaScript模板语法{{&}}等不应被误禁
        
        // CHTL变量系统
        const primaryColor = ThemeColor(primary);
        const secondaryColor = @Var ColorPalette(secondary);
        const customColor = ThemeColor(highlight = #ff6b6b);
        
        // 命名空间导入
        AppConfig from config/application.chtl;
        UserData from data/users.chtl;
        
        // JavaScript模板语法
        const userName = {{user.name}};
        const profile = {{{user.profileHtml}}};
        const message = {{&notification.text}};
        
        // 原始嵌入
        [Origin] @JavaScript {
            window.localScriptConfig = {
                version: '1.0',
                features: ['variables', 'templates']
            };
        }
        
        [Origin] @Html {
            <div id="local-script-marker" style="display: none;"></div>
        }
        
        // 纯JavaScript逻辑
        function initializeLocalScript() {
            console.log('Initializing local script with config:', AppConfig);
            
            // 使用CHTL变量
            document.body.style.setProperty('--primary-color', primaryColor);
            document.body.style.setProperty('--secondary-color', secondaryColor);
            
            // 使用JavaScript模板
            const welcomeMsg = {{translations.welcome}};
            console.log('Welcome message:', welcomeMsg);
            
            // 事件处理
            document.addEventListener('click', function(e) {
                if (e.target.matches('.interactive')) {
                    const elementData = {{&e.target.dataset.info}};
                    processInteraction(elementData);
                }
            });
        }
        
        function processInteraction(data) {
            // 处理交互数据
            const processedData = {
                timestamp: Date.now(),
                userId: {{user.id}},
                action: data.action,
                theme: customColor
            };
            
            return processedData;
        }
        
        // 异步数据加载
        async function loadUserPreferences() {
            try {
                const response = await fetch('/api/preferences');
                const preferences = await response.json();
                
                // 应用用户偏好
                preferences.theme = primaryColor;
                preferences.locale = {{user.locale}};
                
                return preferences;
            } catch (error) {
                console.error('Failed to load preferences:', error);
                return null;
            }
        }
        
        // 初始化
        document.addEventListener('DOMContentLoaded', function() {
            initializeLocalScript();
            loadUserPreferences().then(prefs => {
                if (prefs) {
                    console.log('User preferences loaded:', prefs);
                }
            });
        });
    )";
    
    auto result = constraint.validateCHtlLocalScriptBlock(completeLocalScript);
    
    std::cout << "完整局部script块验证: " << (result.isValid ? "✓ 通过" : "✗ 有问题") << std::endl;
    if (!result.isValid) {
        std::cout << "发现 " << result.violations.size() << " 个验证问题:" << std::endl;
        for (const auto& violation : result.violations) {
            std::cout << "  - " << violation.message << " (" << violation.element << ")" << std::endl;
        }
    }
    
    std::cout << "\n统计信息:" << std::endl;
    std::cout << "  模板变量: " << result.templateVariables.size() << " 个" << std::endl;
    std::cout << "  自定义变量: " << result.customVariables.size() << " 个" << std::endl;
    std::cout << "  生成器注释: " << result.foundGeneratorComments.size() << " 个" << std::endl;
    std::cout << "  原始嵌入: " << result.foundRawEmbeddings.size() << " 个" << std::endl;
    std::cout << "  from子句: " << result.fromClauses.size() << " 个" << std::endl;
    std::cout << "  JavaScript模板语法: " << result.jsTemplateSyntax.size() << " 个" << std::endl;
    std::cout << "  纯JavaScript行数: " << result.pureJavaScriptLines << " 行" << std::endl;
}

int main() {
    try {
        std::cout << "🚀 CHTL局部script块约束器测试" << std::endl;
        std::cout << "验证CHTL局部script块的语法约束：允许特定CHTL语法，保护JavaScript模板语法\n" << std::endl;
        
        testValidLocalScriptSyntax();
        testForbiddenLocalScriptSyntax();
        testJavaScriptTemplateSyntaxProtection();
        testLocalScriptSpecificConstraints();
        testCompleteLocalScriptBlock();
        
        std::cout << "\n🎉 局部script约束器测试完成!" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "测试异常: " << e.what() << std::endl;
        return 1;
    }
}