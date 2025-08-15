package com.chtl.pipeline;

import com.chtl.compiler.CHTLCompiler;
import com.chtl.compiler.CHTLJsCompiler;
import com.chtl.compiler.CssCompiler;
import com.chtl.compiler.JavaScriptCompiler;
import com.chtl.model.CodeFragment;
import com.chtl.model.CodeFragmentType;

import java.util.ArrayList;
import java.util.List;

public class CompilerDispatcher {
    private final CHTLCompiler chtlCompiler;
    private final CHTLJsCompiler chtlJsCompiler;
    private final CssCompiler cssCompiler;
    private final JavaScriptCompiler jsCompiler;

    public CompilerDispatcher() {
        this.chtlCompiler = new CHTLCompiler();
        this.chtlJsCompiler = new CHTLJsCompiler();
        this.cssCompiler = new CssCompiler();
        this.jsCompiler = new JavaScriptCompiler();
    }

    public List<String> compileAll(List<CodeFragment> fragments) {
        List<String> outputs = new ArrayList<>();
        for (CodeFragment fragment : fragments) {
            outputs.add(compile(fragment));
        }
        return outputs;
    }

    private String compile(CodeFragment fragment) {
        String code = fragment.getSourceSlice();
        CodeFragmentType type = fragment.getType();
        switch (type) {
            case TEMPLATE:
                return chtlCompiler.compileTemplateSegment(code);
            case LOCAL_STYLE_BLOCK:
                return chtlCompiler.compileLocalStyleBlock(code);
            case GLOBAL_STYLE_BLOCK:
                return cssCompiler.compileStyleBlock(code);
            case SCRIPT_BLOCK:
                String s1 = chtlCompiler.compileScriptSegment(code);
                String s2 = chtlJsCompiler.compileScriptSegment(s1);
                return jsCompiler.compileScriptBlock(s2);
            default:
                return code;
        }
    }
}