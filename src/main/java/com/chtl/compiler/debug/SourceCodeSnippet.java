package com.chtl.compiler.debug;
import java.util.ArrayList;
import java.util.List;



import java.io.PrintStream;
import java.util.*;

/**
 * 源代码片段
 * 显示错误位置附近的代码
 */
public class SourceCodeSnippet {
    private final String sourcePath;
    private final int errorLine;
    private final int errorColumn;
    private final List<CodeLine> lines = new ArrayList<Object>();
    
    public SourceCodeSnippet(String sourcePath, int errorLine, int errorColumn) {
        this.sourcePath = sourcePath;
        this.errorLine = errorLine;
        this.errorColumn = errorColumn;
    }
    
    public void addLine(int lineNumber, String content) {
        lines.add(new CodeLine(lineNumber, content));
    }
    
    /**
     * 打印代码片段
     */
    public void print(PrintStream out) {
        out.println();
        
        int maxLineNumWidth = String.valueOf(
            lines.stream().mapToInt(l -> l.lineNumber).max().orElse(0)
        ).length();
        
        for (CodeLine line : lines) {
            // 行号
            String lineNum = String.format("%" + maxLineNumWidth + "d", line.lineNumber);
            
            if (line.lineNumber == errorLine) {
                // 错误行高亮
                out.print(ColorUtil.red(lineNum + " |>"));
            } else {
                out.print(ColorUtil.gray(lineNum + " | "));
            }
            
            // 代码内容
            out.println(" " + line.content);
            
            // 错误指示器
            if (line.lineNumber == errorLine && errorColumn > 0) {
                // 打印指向错误位置的箭头
                StringBuilder pointer = new StringBuilder();
                pointer.append(" ".repeat(maxLineNumWidth + 3)); // 行号和分隔符的空格
                
                // 添加前导空格
                for (int i = 1; i < errorColumn && i <= line.content.length(); i++) {
                    char ch = line.content.charAt(i - 1);
                    if (ch == '\t') {
                        pointer.append('\t');
                    } else {
                        pointer.append(' ');
                    }
                }
                
                pointer.append(ColorUtil.red("^"));
                out.println(pointer.toString());
            }
        }
    }
    
    /**
     * 代码行
     */
    private static class CodeLine {
        final int lineNumber;
        final String content;
        
        CodeLine(int lineNumber, String content) {
            this.lineNumber = lineNumber;
            this.content = content;
        }
    }
}
