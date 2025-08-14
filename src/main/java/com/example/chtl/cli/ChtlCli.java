package com.example.chtl.cli;

import com.example.chtl.core.CHTLUnifiedScanner;
import com.example.chtl.core.Fragment;
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

@CommandLine.Command(name = "chtl", mixinStandardHelpOptions = true, version = "0.1.0",
        description = "CHTL -> HTML 编译器 (Scanner + Dispatcher + Compilers + Merger)")
public class ChtlCli implements Runnable {
    private static final Logger log = LoggerFactory.getLogger(ChtlCli.class);

    @CommandLine.Parameters(index = "0", description = "输入 CHTL 源文件路径")
    private Path input;

    @CommandLine.Option(names = {"-o", "--output"}, description = "输出 HTML 文件路径", required = true)
    private Path output;

    @CommandLine.Option(names = "--dump-fragments", description = "打印扫描片段列表")
    private boolean dumpFragments;

    @CommandLine.Option(names = "--dump-ast", description = "打印 AST/Fragments 的 JSON 概要")
    private boolean dumpAst;

    @Override
    public void run() {
        try {
            String source = Files.readString(input, StandardCharsets.UTF_8);
            CHTLUnifiedScanner scanner = new CHTLUnifiedScanner();
            ScanResult scanResult = scanner.scan(source);

            if (dumpFragments) {
                System.out.println("== Fragments ==");
                for (var f : scanResult.getFragments()) {
                    System.out.printf("%-10s %6d-%-6d (%s)\n", f.type(), f.startOffset(), f.endOffset(), f.origin());
                }
            }

            CompilerDispatcher dispatcher = new CompilerDispatcher();
            var compileResult = dispatcher.dispatch(scanResult);

            if (dumpAst) {
                StringBuilder json = new StringBuilder();
                json.append("{\n");
                json.append("  \"fragments\": [\n");
                int idx = 0; for (Fragment f : scanResult.getFragments()) {
                    json.append("    {")
                        .append("\"type\": \"").append(f.type()).append("\",")
                        .append(" \"start\": ").append(f.startOffset()).append(',')
                        .append(" \"end\": ").append(f.endOffset()).append(',')
                        .append(" \"origin\": \"").append(escapeJson(f.origin())).append("\"")
                        .append("}");
                    if (++idx < scanResult.getFragments().size()) json.append(',');
                    json.append("\n");
                }
                json.append("  ],\n");
                json.append("  \"htmlLength\": ").append(compileResult.getHtmlBody().length()).append(',').append('\n');
                json.append("  \"cssLength\": ").append(compileResult.getGlobalCss().length()).append(',').append('\n');
                json.append("  \"jsLength\": ").append(compileResult.getGlobalJs().length()).append('\n');
                json.append("}\n");
                System.out.print(json);
            }

            String html = ResultMerger.mergeToHtmlDocument(compileResult);
            if (output.getParent() != null) {
                Files.createDirectories(output.getParent());
            }
            Files.writeString(output, html, StandardCharsets.UTF_8);
            log.info("编译完成: {} -> {}", input, output);
        } catch (IOException e) {
            throw new RuntimeException("IO 错误: " + e.getMessage(), e);
        }
    }

    private static String escapeJson(String s){ if (s==null) return ""; return s.replace("\\","\\\\").replace("\"","\\\"").replace("\n","\\n"); }

    public static void main(String[] args) {
        int exit = new CommandLine(new ChtlCli()).execute(args);
        System.exit(exit);
    }
}