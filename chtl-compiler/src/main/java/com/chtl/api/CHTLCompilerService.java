package com.chtl.api;

import com.chtl.model.CodeFragment;
import com.chtl.pipeline.CompilerDispatcher;
import com.chtl.pipeline.CompilationResultMerger;
import com.chtl.scanner.CHTLUnifiedScanner;

import java.util.List;

public class CHTLCompilerService {
    private final CHTLUnifiedScanner scanner;
    private final CompilerDispatcher dispatcher;
    private final CompilationResultMerger merger;

    public CHTLCompilerService() {
        this.scanner = new CHTLUnifiedScanner();
        this.dispatcher = new CompilerDispatcher();
        this.merger = new CompilationResultMerger();
    }

    public String compileToHtml(String chtlSource) {
        List<CodeFragment> fragments = scanner.scan(chtlSource);
        List<String> compiled = dispatcher.compileAll(fragments);
        return merger.merge(chtlSource, fragments, compiled);
    }
}