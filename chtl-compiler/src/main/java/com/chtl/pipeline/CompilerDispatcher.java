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
            case HTML_TEXT:
                return chtlCompiler.compileTemplateSegment(code);
            case LOCAL_STYLE_BLOCK:
                return chtlCompiler.compileLocalStyleBlock(code);
            case CSS_CODE:
                return cssCompiler.compileStyleInline(code);
            case JS_CODE:
                // JS 宽判，仅在有 CHTL/CHTL JS token 的邻接处才已在扫描器中分割
                return jsCompiler.compileScriptInline(code);
            case CHTL_TOKEN:
                // CHTL token 在 HTML/CSS 中：交给 CHTL 编译器处理
                return chtlCompiler.compileChtlToken(code);
            case CHTL_JS_TOKEN:
                // 脚本内 CHTL/CHTL JS token：按“CHTL → CHTL JS → JS”链处理，但 token 本身是最小单元
                String s1 = chtlCompiler.compileScriptSegment(code);
                String s2 = chtlJsCompiler.compileScriptSegment(s1);
                return jsCompiler.compileScriptInline(s2);
            default:
                return code;
        }
    }
}