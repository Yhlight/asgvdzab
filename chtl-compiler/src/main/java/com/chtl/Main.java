package com.chtl;

import com.chtl.api.CHTLCompilerService;

public class Main {
    public static void main(String[] args) {
        String source = "<style scoped>h1{color:red;}</style>\n" +
                        "<div>Hello CHTL</div>\n" +
                        "<style>body{margin:0}</style>\n" +
                        "<script>console.log('hi')</script>\n";
        String html = new CHTLCompilerService().compileToHtml(source);
        System.out.println(html);
    }
}