package com.chtl.compiler.chtl;import com.chtl.compiler.style.LocalStyleProcessor;


import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * 局部样式处理器
 * 处理CHTL的局部样式块
 */
public class LocalStyleProcessor {
    private static final Logger logger = LoggerFactory.getLogger(LocalStyleProcessor.class);
    
    // 正则表达式模式
    private static final Pattern CLASS_SELECTOR_PATTERN = Pattern.compile(
        "\\.(\\w+)\\s*\\{([^}]*)\\}", Pattern.DOTALL
    );
    
    private static final Pattern ID_SELECTOR_PATTERN = Pattern.compile(
        "#(\\w+)\\s*\\{([^}]*)\\}", Pattern.DOTALL
    );
    
    private static final Pattern CONTEXT_SELECTOR_PATTERN = Pattern.compile(
        "&([:\\w-]+)\\s*\\{([^}]*)\\}", Pattern.DOTALL
    );
    
    private static final Pattern INLINE_STYLE_PATTERN = Pattern.compile(
        "([\\w-]+)\\s*:\\s*([^;]+);", Pattern.MULTILINE
    );
    
    /**
     * 处理局部样式
     */
    public LocalStyleResult process(String styleContent) {
        logger.debug("处理局部样式内容");
        
        LocalStyleResult result = new LocalStyleResult();
        
        // 1. 提取类选择器
        Matcher classMatcher = CLASS_SELECTOR_PATTERN.matcher(styleContent);
        while (classMatcher.find()) {
            String className = classMatcher.group(1);
            String properties = classMatcher.group(2);
            
            result.addGlobalStyle("." + className + " {\n" + properties + "\n}\n");
            result.addAutoClass(className);
            
            // 从原内容中移除已处理的部分
            styleContent = styleContent.replace(classMatcher.group(), "");
        }
        
        // 2. 提取ID选择器
        Matcher idMatcher = ID_SELECTOR_PATTERN.matcher(styleContent);
        while (idMatcher.find()) {
            String idName = idMatcher.group(1);
            String properties = idMatcher.group(2);
            
            result.addGlobalStyle("#" + idName + " {\n" + properties + "\n}\n");
            result.setAutoId(idName);
            
            styleContent = styleContent.replace(idMatcher.group(), "");
        }
        
        // 3. 提取上下文选择器(&)
        Matcher contextMatcher = CONTEXT_SELECTOR_PATTERN.matcher(styleContent);
        while (contextMatcher.find()) {
            String pseudo = contextMatcher.group(1);
            String properties = contextMatcher.group(2);
            
            // 这部分需要在代码生成时处理，因为需要知道实际的类名
            result.addContextSelector("&" + pseudo, properties);
            
            styleContent = styleContent.replace(contextMatcher.group(), "");
        }
        
        // 4. 剩余的都是内联样式
        Matcher inlineMatcher = INLINE_STYLE_PATTERN.matcher(styleContent);
        StringBuilder inlineStyles = new StringBuilder();
        
        while (inlineMatcher.find()) {
            String property = inlineMatcher.group(1);
            String value = inlineMatcher.group(2).trim();
            
            inlineStyles.append(property).append(": ").append(value).append("; ");
        }
        
        result.setInlineStyles(inlineStyles.toString());
        
        return result;
    }
}