package com.chtl.compiler.chtljs.lexer;

import com.chtl.compiler.chtljs.token.CHTLJSToken;
import com.chtl.compiler.chtljs.token.CHTLJSTokenType;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

import java.util.List;

/**
 * CHTL JS词法分析器测试
 */
public class CHTLJSLexerTest {
    
    @Test
    public void testEnhancedSelector() {
        String code = "{{#myButton}}->addEventListener('click', handler);";
        CHTLJSLexer lexer = new CHTLJSLexer(code);
        List<CHTLJSToken> tokens = lexer.tokenize();
        
        assertEquals(CHTLJSTokenType.DOUBLE_LEFT_BRACE, tokens.get(0).getType());
        assertEquals(CHTLJSTokenType.HASH, tokens.get(1).getType());
        assertEquals(CHTLJSTokenType.IDENTIFIER, tokens.get(2).getType());
        assertEquals("myButton", tokens.get(2).getValue());
        assertEquals(CHTLJSTokenType.DOUBLE_RIGHT_BRACE, tokens.get(3).getType());
        assertEquals(CHTLJSTokenType.ARROW, tokens.get(4).getType());
    }
    
    @Test
    public void testMultipleSelectors() {
        String code = """
            {{box}}->textContent;
            {{.box}}->style.color = 'red';
            {{button[0]}}->click();
            """;
        
        CHTLJSLexer lexer = new CHTLJSLexer(code);
        List<CHTLJSToken> tokens = lexer.tokenize();
        
        // 验证三种选择器模式
        // {{box}} - 标签/标识符选择器
        assertEquals(CHTLJSTokenType.DOUBLE_LEFT_BRACE, tokens.get(0).getType());
        assertEquals(CHTLJSTokenType.IDENTIFIER, tokens.get(1).getType());
        assertEquals("box", tokens.get(1).getValue());
        
        // {{.box}} - 类选择器
        boolean hasClassSelector = false;
        for (int i = 0; i < tokens.size() - 1; i++) {
            if (tokens.get(i).getType() == CHTLJSTokenType.DOUBLE_LEFT_BRACE &&
                tokens.get(i + 1).getType() == CHTLJSTokenType.DOT) {
                hasClassSelector = true;
                break;
            }
        }
        assertTrue(hasClassSelector);
        
        // {{button[0]}} - 索引访问
        boolean hasIndexAccess = tokens.stream()
            .anyMatch(t -> t.getType() == CHTLJSTokenType.LEFT_BRACKET);
        assertTrue(hasIndexAccess);
    }
    
    @Test
    public void testListenMethod() {
        String code = """
            {{#submitBtn}}->listen({
                click: () => {
                    console.log('Clicked');
                },
                mouseenter: handleMouseEnter
            });
            """;
        
        CHTLJSLexer lexer = new CHTLJSLexer(code);
        List<CHTLJSToken> tokens = lexer.tokenize();
        
        // 验证listen方法被识别
        boolean hasListen = tokens.stream()
            .anyMatch(t -> t.getType() == CHTLJSTokenType.LISTEN);
        assertTrue(hasListen);
        
        // 验证箭头操作符
        boolean hasArrow = tokens.stream()
            .anyMatch(t -> t.getType() == CHTLJSTokenType.ARROW);
        assertTrue(hasArrow);
    }
    
    @Test
    public void testDelegateMethod() {
        String code = """
            {{document.body}}->delegate({
                target: {{.btn}},
                click: handleClick
            });
            """;
        
        CHTLJSLexer lexer = new CHTLJSLexer(code);
        List<CHTLJSToken> tokens = lexer.tokenize();
        
        // 验证delegate方法
        boolean hasDelegate = tokens.stream()
            .anyMatch(t -> t.getType() == CHTLJSTokenType.DELEGATE);
        assertTrue(hasDelegate);
        
        // 验证target关键字
        boolean hasTarget = tokens.stream()
            .anyMatch(t -> t.getType() == CHTLJSTokenType.TARGET);
        assertTrue(hasTarget);
    }
    
    @Test
    public void testAnimateFunction() {
        String code = """
            animate({
                duration: 1000,
                easing: 'ease-in-out',
                begin: {
                    opacity: 0
                },
                end: {
                    opacity: 1
                },
                loop: -1,
                callback: onComplete
            });
            """;
        
        CHTLJSLexer lexer = new CHTLJSLexer(code);
        List<CHTLJSToken> tokens = lexer.tokenize();
        
        // 验证animate函数和所有动画关键字
        assertEquals(CHTLJSTokenType.ANIMATE, tokens.get(0).getType());
        
        boolean hasDuration = tokens.stream()
            .anyMatch(t -> t.getType() == CHTLJSTokenType.DURATION);
        boolean hasEasing = tokens.stream()
            .anyMatch(t -> t.getType() == CHTLJSTokenType.EASING);
        boolean hasBegin = tokens.stream()
            .anyMatch(t -> t.getType() == CHTLJSTokenType.BEGIN);
        boolean hasEnd = tokens.stream()
            .anyMatch(t -> t.getType() == CHTLJSTokenType.END);
        boolean hasLoop = tokens.stream()
            .anyMatch(t -> t.getType() == CHTLJSTokenType.LOOP);
        boolean hasCallback = tokens.stream()
            .anyMatch(t -> t.getType() == CHTLJSTokenType.CALLBACK);
        
        assertTrue(hasDuration);
        assertTrue(hasEasing);
        assertTrue(hasBegin);
        assertTrue(hasEnd);
        assertTrue(hasLoop);
        assertTrue(hasCallback);
    }
    
    @Test
    public void testArrowChaining() {
        String code = "{{#element}}->style->color = 'blue';";
        
        CHTLJSLexer lexer = new CHTLJSLexer(code);
        List<CHTLJSToken> tokens = lexer.tokenize();
        
        // 统计箭头操作符数量
        long arrowCount = tokens.stream()
            .filter(t -> t.getType() == CHTLJSTokenType.ARROW)
            .count();
        
        assertEquals(2, arrowCount); // 应该有两个箭头
    }
    
    @Test
    public void testJavaScriptKeywords() {
        String code = """
            const box = {{.box}};
            function handleClick() {
                let value = 10;
                if (value > 5) {
                    return true;
                }
            }
            """;
        
        CHTLJSLexer lexer = new CHTLJSLexer(code);
        List<CHTLJSToken> tokens = lexer.tokenize();
        
        // 验证JavaScript关键字
        assertEquals(CHTLJSTokenType.CONST, tokens.get(0).getType());
        
        boolean hasFunction = tokens.stream()
            .anyMatch(t -> t.getType() == CHTLJSTokenType.FUNCTION);
        boolean hasLet = tokens.stream()
            .anyMatch(t -> t.getType() == CHTLJSTokenType.LET);
        boolean hasIf = tokens.stream()
            .anyMatch(t -> t.getType() == CHTLJSTokenType.IF);
        boolean hasReturn = tokens.stream()
            .anyMatch(t -> t.getType() == CHTLJSTokenType.RETURN);
        
        assertTrue(hasFunction);
        assertTrue(hasLet);
        assertTrue(hasIf);
        assertTrue(hasReturn);
    }
}