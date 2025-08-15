package com.chtl.scanner;

import com.chtl.model.CodeFragment;
import java.util.List;

/**
 * CHTL统一扫描器
 * 使用新的块级扫描架构
 */
public class CHTLUnifiedScanner {
    
    private final UnifiedScanner scanner;
    
    public CHTLUnifiedScanner() {
        this.scanner = new UnifiedScanner();
    }
    
    /**
     * 扫描CHTL源代码
     * @param sourceCode CHTL源代码
     * @return 代码片段列表
     */
    public List<CodeFragment> scan(String sourceCode) {
        return scanner.scan(sourceCode);
    }
}