package com.chtl.compiler;

public class CHTLCompiler {
    public String compileTemplateSegment(String templateSegment) {
        // 直接透传模板内容（未来可在此做语法转换）
        return templateSegment;
    }

    public String compileLocalStyleBlock(String styleBlock) {
        // 局部 style：按规则交给 CHTL 编译器管理，当前简单地为其增加一个 data-scope 属性示意
        if (!styleBlock.toLowerCase().startsWith("<style")) {
            return styleBlock;
        }
        int close = styleBlock.indexOf('>');
        if (close < 0) return styleBlock;
        String start = styleBlock.substring(0, close);
        String rest = styleBlock.substring(close);
        if (!start.toLowerCase().contains("scoped")) {
            return styleBlock; // 容错
        }
        String startWithScope = start.replace("scoped", "scoped data-scope=\"component\"");
        return startWithScope + rest;
    }

    public String compileScriptSegment(String scriptBlock) {
        // CHTL/模板相关的脚本预处理：此处暂不做转换
        return scriptBlock;
    }

    public String compileChtlToken(String token) {
        // 严判最小单元：例如 "{{box}}->" 或 "{{id}}"。这里暂时透传，后续结合 CHTL 语义生成目标代码。
        return token;
    }
}