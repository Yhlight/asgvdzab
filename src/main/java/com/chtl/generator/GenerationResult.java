package com.chtl.generator;

/**
 * 代码生成结果
 */
public class GenerationResult {
    private String html;
    private String css;
    private String javaScript;
    
    public GenerationResult(String html, String css, String javaScript) {
        this.html = html;
        this.css = css;
        this.javaScript = javaScript;
    }
    
    public String getHtml() {
        return html;
    }
    
    public void setHtml(String html) {
        this.html = html;
    }
    
    public String getCss() {
        return css;
    }
    
    public void setCss(String css) {
        this.css = css;
    }
    
    public String getJavaScript() {
        return javaScript;
    }
    
    public void setJavaScript(String javaScript) {
        this.javaScript = javaScript;
    }
    
    /**
     * 获取完整的HTML页面
     */
    public String getCompleteHTML() {
        StringBuilder sb = new StringBuilder();
        sb.append("<!DOCTYPE html>\n");
        sb.append("<html>\n");
        sb.append("<head>\n");
        
        if (css != null && !css.trim().isEmpty()) {
            sb.append("<style>\n");
            sb.append(css);
            sb.append("</style>\n");
        }
        
        sb.append("</head>\n");
        sb.append("<body>\n");
        
        if (html != null) {
            sb.append(html);
        }
        
        if (javaScript != null && !javaScript.trim().isEmpty()) {
            sb.append("<script>\n");
            sb.append(javaScript);
            sb.append("</script>\n");
        }
        
        sb.append("</body>\n");
        sb.append("</html>");
        
        return sb.toString();
    }
}