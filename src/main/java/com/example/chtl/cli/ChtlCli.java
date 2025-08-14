package com.example.chtl.cli;

import com.example.chtl.ast.json.AstJsonSerializer;
import com.example.chtl.compilers.ChtlCompiler;
import com.example.chtl.core.CHTLUnifiedScanner;
import com.example.chtl.core.ScanResult;
import com.example.chtl.dispatch.CompilerDispatcher;
import com.example.chtl.merge.ResultMerger;
import picocli.CommandLine;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.concurrent.Callable;

@CommandLine.Command(name = "chtl", mixinStandardHelpOptions = true, version = "0.1.0",
        description = "CHTL -> HTML 编译器 (Scanner + Dispatcher + Compilers + Merger)")
public class ChtlCli implements Callable<Integer> {
    private static final Logger log = LoggerFactory.getLogger(ChtlCli.class);

    @CommandLine.Parameters(index = "0", description = "输入 CHTL 源文件路径")
    private Path input;

    @CommandLine.Option(names = {"-o", "--output"}, description = "输出 HTML 文件路径", required = true)
    private Path output;

    @CommandLine.Option(names = "--dump-fragments", description = "打印扫描片段列表")
    private boolean dumpFragments;

    @CommandLine.Option(names = "--dump-ast", description = "打印 CHTL AST JSON")
    private boolean dumpAst;

    @CommandLine.Option(names = "--strict", description = "严格模式，解析或约束错误立即失败")
    private boolean strict;

    @Override
    public Integer call() {
        try {
            String source = Files.readString(input, StandardCharsets.UTF_8);
            CHTLUnifiedScanner scanner = new CHTLUnifiedScanner();
            ScanResult scanResult = scanner.scan(source);

            if (dumpFragments) {
                System.out.println("{\n  \"fragments\": [");
                for (int i=0;i<scanResult.getFragments().size();i++) {
                    var f = scanResult.getFragments().get(i);
                    System.out.printf("    {\"type\":\"%s\",\"start\":%d,\"end\":%d,\"origin\":\"%s\"}%s\n",
                            f.type(), f.startOffset(), f.endOffset(), escape(f.origin()), (i==scanResult.getFragments().size()-1?"":" ,"));
                }
                System.out.println("  ]\n}");
            }

            if (dumpAst || strict) {
                var lexer = new com.example.chtl.compilers.chtl.ChtlLexer(source);
                var tokens = lexer.lex();
                try {
                    var parser = new com.example.chtl.parsers.chtl.ChtlParser(source, tokens, new com.example.chtl.compilers.chtl.ChtlState(), input.getParent(), strict);
                    var doc = parser.parseDocument();
                    if (dumpAst) System.out.println(AstJsonSerializer.toJson(doc));
                } catch (RuntimeException ex) {
                    if (strict) {
                        System.err.println("解析错误: " + ex.getMessage());
                        return 2;
                    } else {
                        log.warn("解析警告: {}", ex.getMessage());
                    }
                }
            }

            CompilerDispatcher dispatcher = new CompilerDispatcher();
            var compileResult = dispatcher.dispatch(scanResult);

            String html = ResultMerger.mergeToHtmlDocument(compileResult);
            if (output.getParent() != null) {
                Files.createDirectories(output.getParent());
            }
            Files.writeString(output, html, StandardCharsets.UTF_8);
            log.info("编译完成: {} -> {}", input, output);
            return 0;
        } catch (IOException e) {
            throw new RuntimeException("IO 错误: " + e.getMessage(), e);
        }
    }

    private static String escape(String s){ return s==null?"":s.replace("\\","\\\\").replace("\"","\\\""); }

    public static void main(String[] args) {
        int exit = new CommandLine(new ChtlCli()).execute(args);
        System.exit(exit);
    }
}