package com.chtl.compiler.chtl.lexer;

import com.chtl.compiler.chtl.token.CHTLToken;
import com.chtl.compiler.chtl.token.CHTLTokenType;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.BeforeEach;
import static org.junit.jupiter.api.Assertions.*;

import java.util.List;

/**
 * CHTL词法分析器测试
 */
public class CHTLLexerTest {
    
    private CHTLLexer lexer;
    
    @Test
    public void testBasicTokens() {
        String code = "div { id: myDiv; }";
        lexer = new CHTLLexer(code);
        List<CHTLToken> tokens = lexer.tokenize();
        
        assertEquals(CHTLTokenType.IDENTIFIER, tokens.get(0).getType());
        assertEquals("div", tokens.get(0).getValue());
        assertEquals(CHTLTokenType.LEFT_BRACE, tokens.get(1).getType());
        assertEquals(CHTLTokenType.IDENTIFIER, tokens.get(2).getType());
        assertEquals("id", tokens.get(2).getValue());
        assertEquals(CHTLTokenType.COLON, tokens.get(3).getType());
        assertEquals(CHTLTokenType.IDENTIFIER, tokens.get(4).getType());
        assertEquals("myDiv", tokens.get(4).getValue());
        assertEquals(CHTLTokenType.SEMICOLON, tokens.get(5).getType());
        assertEquals(CHTLTokenType.RIGHT_BRACE, tokens.get(6).getType());
        assertEquals(CHTLTokenType.EOF, tokens.get(7).getType());
    }
    
    @Test
    public void testCEEquivalence() {
        // 测试CE对等式：冒号和等号等价
        String code1 = "id: test;";
        String code2 = "id= test;";
        
        lexer = new CHTLLexer(code1);
        List<CHTLToken> tokens1 = lexer.tokenize();
        
        lexer = new CHTLLexer(code2);
        List<CHTLToken> tokens2 = lexer.tokenize();
        
        assertEquals(CHTLTokenType.COLON, tokens1.get(1).getType());
        assertEquals(CHTLTokenType.EQUALS, tokens2.get(1).getType());
    }
    
    @Test
    public void testTextNode() {
        String code = "text { Hello World }";
        lexer = new CHTLLexer(code);
        List<CHTLToken> tokens = lexer.tokenize();
        
        assertEquals(CHTLTokenType.TEXT, tokens.get(0).getType());
        assertEquals(CHTLTokenType.LEFT_BRACE, tokens.get(1).getType());
        assertEquals(CHTLTokenType.IDENTIFIER, tokens.get(2).getType());
        assertEquals("Hello", tokens.get(2).getValue());
        assertEquals(CHTLTokenType.IDENTIFIER, tokens.get(3).getType());
        assertEquals("World", tokens.get(3).getValue());
        assertEquals(CHTLTokenType.RIGHT_BRACE, tokens.get(4).getType());
    }
    
    @Test
    public void testStringLiterals() {
        String code = "title: \"测试标题\"; content: '单引号内容';";
        lexer = new CHTLLexer(code);
        List<CHTLToken> tokens = lexer.tokenize();
        
        assertEquals(CHTLTokenType.STRING_LITERAL, tokens.get(2).getType());
        assertEquals("测试标题", tokens.get(2).getValue());
        
        assertEquals(CHTLTokenType.STRING_LITERAL, tokens.get(6).getType());
        assertEquals("单引号内容", tokens.get(6).getValue());
    }
    
    @Test
    public void testStyleAndScript() {
        String code = """
            div {
                style {
                    width: 100px;
                }
                script {
                    console.log("test");
                }
            }
            """;
        
        lexer = new CHTLLexer(code);
        List<CHTLToken> tokens = lexer.tokenize();
        
        // 验证style和script关键字被正确识别
        boolean hasStyle = tokens.stream()
            .anyMatch(t -> t.getType() == CHTLTokenType.STYLE);
        boolean hasScript = tokens.stream()
            .anyMatch(t -> t.getType() == CHTLTokenType.SCRIPT);
        
        assertTrue(hasStyle);
        assertTrue(hasScript);
    }
    
    @Test
    public void testComments() {
        String code = """
            // 这是行注释
            /* 这是块注释 */
            -- 这是生成器注释
            div { }
            """;
        
        lexer = new CHTLLexer(code);
        List<CHTLToken> tokens = lexer.tokenize();
        
        assertEquals(CHTLTokenType.LINE_COMMENT, tokens.get(0).getType());
        assertEquals("// 这是行注释", tokens.get(0).getValue());
        
        assertEquals(CHTLTokenType.BLOCK_COMMENT, tokens.get(1).getType());
        assertEquals("/* 这是块注释 */", tokens.get(1).getValue());
        
        assertEquals(CHTLTokenType.GENERATOR_COMMENT, tokens.get(2).getType());
        assertEquals("-- 这是生成器注释", tokens.get(2).getValue());
    }
    
    @Test
    public void testTemplateAndCustom() {
        String code = """
            [Template] @Element Card {
                div { }
            }
            
            [Custom] @Style primary from theme {
                color: blue;
            }
            """;
        
        lexer = new CHTLLexer(code);
        List<CHTLToken> tokens = lexer.tokenize();
        
        // 验证模板和自定义关键字
        assertEquals(CHTLTokenType.TEMPLATE, tokens.get(0).getType());
        assertEquals(CHTLTokenType.AT_ELEMENT, tokens.get(1).getType());
        
        boolean hasCustom = tokens.stream()
            .anyMatch(t -> t.getType() == CHTLTokenType.CUSTOM);
        boolean hasAtStyle = tokens.stream()
            .anyMatch(t -> t.getType() == CHTLTokenType.AT_STYLE);
        
        assertTrue(hasCustom);
        assertTrue(hasAtStyle);
    }
    
    @Test
    public void testLocalStyleSelectors() {
        String code = """
            style {
                .box { }
                #container { }
                &:hover { }
            }
            """;
        
        lexer = new CHTLLexer(code);
        List<CHTLToken> tokens = lexer.tokenize();
        
        // 验证选择器符号
        boolean hasDot = tokens.stream()
            .anyMatch(t -> t.getType() == CHTLTokenType.DOT);
        boolean hasHash = tokens.stream()
            .anyMatch(t -> t.getType() == CHTLTokenType.HASH);
        boolean hasAmpersand = tokens.stream()
            .anyMatch(t -> t.getType() == CHTLTokenType.AMPERSAND);
        
        assertTrue(hasDot);
        assertTrue(hasHash);
        assertTrue(hasAmpersand);
    }
    
    @Test
    public void testNumbers() {
        String code = "width: 100; height: 50.5;";
        lexer = new CHTLLexer(code);
        List<CHTLToken> tokens = lexer.tokenize();
        
        assertEquals(CHTLTokenType.NUMBER, tokens.get(2).getType());
        assertEquals("100", tokens.get(2).getValue());
        
        assertEquals(CHTLTokenType.NUMBER, tokens.get(6).getType());
        assertEquals("50.5", tokens.get(6).getValue());
    }
}