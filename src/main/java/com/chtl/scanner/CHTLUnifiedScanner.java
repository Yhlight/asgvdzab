package com.chtl.scanner;

import com.chtl.model.CodeFragment;
import com.chtl.model.FragmentType;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * CHTL统一扫描器 - 精准代码切割器
 * 负责将CHTL源代码切割成不同类型的片段
 */
public class CHTLUnifiedScanner {
    private static final Logger logger = LoggerFactory.getLogger(CHTLUnifiedScanner.class);
    
    // 正则表达式模式
    private static final Pattern STYLE_BLOCK_PATTERN = Pattern.compile(
        "style\\s*\\{([^{}]*(?:\\{[^{}]*\\}[^{}]*)*)\\}", 
        Pattern.DOTALL | Pattern.MULTILINE
    );
    
    private static final Pattern SCRIPT_BLOCK_PATTERN = Pattern.compile(
        "script\\s*\\{([^{}]*(?:\\{[^{}]*\\}[^{}]*)*)\\}", 
        Pattern.DOTALL | Pattern.MULTILINE
    );
    
    private static final Pattern GLOBAL_STYLE_PATTERN = Pattern.compile(
        "\\[Origin\\]\\s*@Style\\s*\\{([^}]*)\\}", 
        Pattern.DOTALL | Pattern.MULTILINE
    );
    
    private static final Pattern COMMENT_PATTERN = Pattern.compile(
        "//.*?$|/\\*.*?\\*/", 
        Pattern.MULTILINE | Pattern.DOTALL
    );
    
    public CHTLUnifiedScanner() {
        logger.info("初始化CHTL统一扫描器");
    }
    
    /**
     * 扫描并切割CHTL源代码
     * @param sourceCode CHTL源代码
     * @return 代码片段列表
     */
    public List<CodeFragment> scan(String sourceCode) {
        logger.debug("开始扫描CHTL源代码，长度: {} 字符", sourceCode.length());
        List<CodeFragment> fragments = new ArrayList<>();
        
        // 创建源代码的可变副本
        StringBuilder mutableSource = new StringBuilder(sourceCode);
        
        // 1. 提取全局样式块
        extractGlobalStyles(mutableSource, fragments);
        
        // 2. 提取script块（包括局部script）
        extractScriptBlocks(mutableSource, fragments);
        
        // 3. 提取局部style块
        extractLocalStyles(mutableSource, fragments);
        
        // 4. 剩余的都是CHTL主体代码
        String remainingCode = mutableSource.toString().trim();
        if (!remainingCode.isEmpty()) {
            fragments.add(new CodeFragment(FragmentType.CHTL, remainingCode, 0));
        }
        
        logger.info("扫描完成，共生成 {} 个代码片段", fragments.size());
        return fragments;
    }
    
    /**
     * 提取全局样式块
     */
    private void extractGlobalStyles(StringBuilder source, List<CodeFragment> fragments) {
        Matcher matcher = GLOBAL_STYLE_PATTERN.matcher(source);
        int offset = 0;
        
        while (matcher.find()) {
            String styleContent = matcher.group(1);
            int startPos = matcher.start() - offset;
            
            fragments.add(new CodeFragment(
                FragmentType.CSS, 
                styleContent.trim(), 
                startPos
            ));
            
            // 从源代码中移除已提取的部分
            source.delete(startPos, matcher.end() - offset);
            offset += matcher.end() - matcher.start();
            
            logger.debug("提取全局样式块，位置: {}", startPos);
        }
    }
    
    /**
     * 提取脚本块
     */
    private void extractScriptBlocks(StringBuilder source, List<CodeFragment> fragments) {
        Matcher matcher = SCRIPT_BLOCK_PATTERN.matcher(source);
        int offset = 0;
        
        while (matcher.find()) {
            String scriptContent = matcher.group(1);
            int startPos = matcher.start() - offset;
            
            // 判断是否包含CHTL JS特有语法
            FragmentType type = containsCHTLJSSyntax(scriptContent) 
                ? FragmentType.CHTL_JS 
                : FragmentType.JAVASCRIPT;
            
            fragments.add(new CodeFragment(
                type, 
                scriptContent.trim(), 
                startPos
            ));
            
            // 从源代码中移除已提取的部分
            source.delete(startPos, matcher.end() - offset);
            offset += matcher.end() - matcher.start();
            
            logger.debug("提取{}块，位置: {}", type, startPos);
        }
    }
    
    /**
     * 提取局部样式块
     */
    private void extractLocalStyles(StringBuilder source, List<CodeFragment> fragments) {
        Matcher matcher = STYLE_BLOCK_PATTERN.matcher(source);
        int offset = 0;
        
        while (matcher.find()) {
            String styleContent = matcher.group(1);
            int startPos = matcher.start() - offset;
            
            // 局部样式块由CHTL编译器处理
            fragments.add(new CodeFragment(
                FragmentType.CHTL_LOCAL_STYLE, 
                styleContent.trim(), 
                startPos
            ));
            
            // 保留style块的标记，但清空内容
            String replacement = "style { /*processed*/ }";
            source.replace(startPos, matcher.end() - offset, replacement);
            offset += (matcher.end() - matcher.start()) - replacement.length();
            
            logger.debug("提取局部样式块，位置: {}", startPos);
        }
    }
    
    /**
     * 检查是否包含CHTL JS特有语法
     */
    private boolean containsCHTLJSSyntax(String scriptContent) {
        // 检查增强选择器 {{}}
        if (scriptContent.contains("{{") && scriptContent.contains("}}")) {
            return true;
        }
        
        // 检查箭头操作符 ->
        if (scriptContent.contains("->")) {
            return true;
        }
        
        // 检查特殊方法调用
        if (scriptContent.contains("->listen(") || 
            scriptContent.contains("->delegate(") ||
            scriptContent.contains("animate(")) {
            return true;
        }
        
        return false;
    }
    
    /**
     * 移除注释（保留--注释）
     */
    private String removeComments(String code) {
        // 保留--注释，移除//和/**/注释
        return code.replaceAll("(?<!-)//(?!-).*?$", "")
                   .replaceAll("/\\*(?!\\*/).*?\\*/", "");
    }
}