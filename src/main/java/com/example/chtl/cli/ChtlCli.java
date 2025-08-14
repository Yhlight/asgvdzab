package com.example.chtl.cli;

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

@CommandLine.Command(name = "chtl", mixinStandardHelpOptions = true, version = "0.1.0",
        description = "CHTL -> HTML 编译器 (Scanner + Dispatcher + Compilers + Merger)")
public class ChtlCli implements Runnable {
    private static final Logger log = LoggerFactory.getLogger(ChtlCli.class);

    @CommandLine.Parameters(index = "0", description = "输入 CHTL 源文件路径")
    private Path input;

    @CommandLine.Option(names = {"-o", "--output"}, description = "输出 HTML 文件路径", required = true)
    private Path output;

    @Override
    public void run() {
        try {
            String source = Files.readString(input, StandardCharsets.UTF_8);
            CHTLUnifiedScanner scanner = new CHTLUnifiedScanner();
            ScanResult scanResult = scanner.scan(source);

            CompilerDispatcher dispatcher = new CompilerDispatcher();
            var compileResult = dispatcher.dispatch(scanResult);

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

    public static void main(String[] args) {
        int exit = new CommandLine(new ChtlCli()).execute(args);
        System.exit(exit);
    }
}