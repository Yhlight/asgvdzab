package com.chtl.scanner;

import com.chtl.model.CodeFragment;
import com.chtl.model.FragmentType;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.BeforeEach;
import static org.junit.jupiter.api.Assertions.*;

import java.util.List;

/**
 * CHTL统一扫描器测试
 * 验证宽判严判机制的正确性
 */
public class CHTLUnifiedScannerTest {
    
    private CHTLUnifiedScanner scanner;
    
    @BeforeEach
    public void setUp() {
        scanner = new CHTLUnifiedScanner();
    }
    
    @Test
    public void testStrictModeForCHTL() {
        // 测试CHTL严判模式：最小单元切割
        String code = """
            div {
                id: myDiv;
                class: container;
            }
            span {
                text { Hello }
            }
            """;
        
        List<CodeFragment> fragments = scanner.scan(code);
        
        // CHTL代码应该保持连续，不会过度切割
        assertTrue(fragments.size() >= 1);
        assertEquals(FragmentType.CHTL, fragments.get(0).getType());
    }
    
    @Test
    public void testStrictModeForCHTLJS() {
        // 测试CHTL JS严判模式：精确切割特征
        String code = """
            script {
                {{#button}}->listen({
                    click: () => {
                        console.log('clicked');
                        {{.box}}->style.color = 'red';
                    }
                });
            }
            """;
        
        List<CodeFragment> fragments = scanner.scan(code);
        
        // 应该识别出CHTL JS片段
        boolean hasCHTLJS = fragments.stream()
            .anyMatch(f -> f.getType() == FragmentType.CHTL_JS);
        assertTrue(hasCHTLJS);
        
        // 验证CHTL JS特征被正确识别
        String jsContent = fragments.stream()
            .filter(f -> f.getType() == FragmentType.CHTL_JS)
            .map(CodeFragment::getContent)
            .findFirst()
            .orElse("");
        
        assertTrue(jsContent.contains("{{#button}}"));
        assertTrue(jsContent.contains("->listen"));
        assertTrue(jsContent.contains("{{.box}}"));
    }
    
    @Test
    public void testRelaxedModeForCSS() {
        // 测试CSS宽判模式：整体处理直到遇到CHTL特征
        String code = """
            [Origin] @Style {
                body {
                    margin: 0;
                    padding: 0;
                }
                .container {
                    width: 100%;
                }
            }
            """;
        
        List<CodeFragment> fragments = scanner.scan(code);
        
        // CSS应该作为一个整体片段
        assertEquals(1, fragments.size());
        assertEquals(FragmentType.CSS, fragments.get(0).getType());
    }
    
    @Test
    public void testRelaxedModeForJS() {
        // 测试JS宽判模式：整体处理直到遇到CHTL JS特征
        String code = """
            script {
                function normalFunction() {
                    console.log('This is normal JS');
                    return true;
                }
                
                const value = 42;
            }
            """;
        
        List<CodeFragment> fragments = scanner.scan(code);
        
        // 普通JS应该作为一个整体片段
        boolean hasPlainJS = fragments.stream()
            .anyMatch(f -> f.getType() == FragmentType.JAVASCRIPT);
        assertTrue(hasPlainJS);
    }
    
    @Test
    public void testMixedCHTLAndCHTLJS() {
        // 测试混合CHTL和CHTL JS的切割
        String code = """
            div {
                style {
                    .box {
                        color: red;
                    }
                    &:hover {
                        color: blue;
                    }
                }
                
                script {
                    {{.box}}->addEventListener('click', () => {
                        console.log('Box clicked');
                    });
                }
            }
            """;
        
        List<CodeFragment> fragments = scanner.scan(code);
        
        // 应该有多种类型的片段
        boolean hasCHTL = fragments.stream()
            .anyMatch(f -> f.getType() == FragmentType.CHTL);
        boolean hasCHTLStyle = fragments.stream()
            .anyMatch(f -> f.getType() == FragmentType.CHTL_LOCAL_STYLE);
        boolean hasCHTLJS = fragments.stream()
            .anyMatch(f -> f.getType() == FragmentType.CHTL_JS);
        
        assertTrue(hasCHTL);
        assertTrue(hasCHTLStyle);
        assertTrue(hasCHTLJS);
    }
    
    @Test
    public void testCHTLJSFeatureSplitting() {
        // 测试CHTL JS特征的精确切割
        String jsCode = """
            const box = {{#myBox}};
            box->style.color = 'red';
            {{button[0]}}->listen({
                click: handleClick
            });
            """;
        
        // 在script块中的处理
        String code = "script { " + jsCode + " }";
        List<CodeFragment> fragments = scanner.scan(code);
        
        // 验证CHTL JS特征被正确切割
        String content = fragments.stream()
            .filter(f -> f.getType() == FragmentType.CHTL_JS)
            .map(CodeFragment::getContent)
            .reduce("", String::concat);
        
        // 所有CHTL JS特征都应该被保留
        assertTrue(content.contains("{{#myBox}}"));
        assertTrue(content.contains("->style"));
        assertTrue(content.contains("{{button[0]}}"));
        assertTrue(content.contains("->listen"));
    }
    
    @Test
    public void testLocalStyleProcessing() {
        // 测试局部样式的处理
        String code = """
            div {
                style {
                    width: 100px;
                    height: 100px;
                    
                    .active {
                        background: yellow;
                    }
                    
                    &:hover {
                        transform: scale(1.1);
                    }
                }
            }
            """;
        
        List<CodeFragment> fragments = scanner.scan(code);
        
        // 应该同时有CHTL和CHTL样式片段
        boolean hasCHTL = fragments.stream()
            .anyMatch(f -> f.getType() == FragmentType.CHTL);
        boolean hasLocalStyle = fragments.stream()
            .anyMatch(f -> f.getType() == FragmentType.CHTL_LOCAL_STYLE || 
                         f.getType() == FragmentType.CSS);
        
        assertTrue(hasCHTL);
        assertTrue(hasLocalStyle);
    }
    
    @Test
    public void testFragmentBoundaries() {
        // 测试片段边界的正确性
        String code = """
            div {
                id: container;
            }
            
            [Origin] @Style {
                body { margin: 0; }
            }
            
            script {
                {{#container}}->onclick = function() {
                    alert('clicked');
                };
            }
            """;
        
        List<CodeFragment> fragments = scanner.scan(code);
        
        // 验证片段数量和类型
        assertTrue(fragments.size() >= 3, "应该至少有3个片段：CHTL、CSS、CHTL JS");
        
        // 验证片段顺序和内容
        for (CodeFragment fragment : fragments) {
            assertNotNull(fragment.getContent());
            assertFalse(fragment.getContent().isEmpty());
        }
    }
}