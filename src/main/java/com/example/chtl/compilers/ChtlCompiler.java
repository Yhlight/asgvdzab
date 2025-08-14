package com.example.chtl.compilers;

import com.example.chtl.ast.chtl.*;
import com.example.chtl.compilers.chtl.ChtlContext;
import com.example.chtl.compilers.chtl.ChtlGlobalMap;
import com.example.chtl.compilers.chtl.ChtlLexer;
import com.example.chtl.compilers.chtl.ChtlState;
import com.example.chtl.parsers.chtl.ChtlParser;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class ChtlCompiler {
	private static final Logger log = LoggerFactory.getLogger(ChtlCompiler.class);

	public static class Output {
		public final String htmlBody;
		public final String emittedGlobalCss;
		public final String emittedGlobalJs;

		public Output(String htmlBody, String emittedGlobalCss, String emittedGlobalJs) {
			this.htmlBody = htmlBody;
			this.emittedGlobalCss = emittedGlobalCss;
			this.emittedGlobalJs = emittedGlobalJs;
		}
	}

	public Output compile(String chtlSource) {
		log.debug("开始编译 CHTL 源，长度={} 字符", chtlSource.length());
		var lexer = new ChtlLexer(chtlSource);
		var tokens = lexer.lex();
		var parser = new ChtlParser(chtlSource, tokens);
		ChtlContext ctx = new ChtlContext(new ChtlGlobalMap(), new ChtlState());
		try {
			ChtlDocument doc = parser.parseDocument();
			StringBuilder html = new StringBuilder();
			StringBuilder globalCss = new StringBuilder();
			for (ChtlNode n : doc.items()) {
				renderNode(n, html, globalCss);
			}
			return new Output(html.toString(), globalCss.toString(), ctx.globals.getGlobalJs());
		} catch (RuntimeException ex) {
			// 解析失败时回退到占位输出，避免影响整体管线
			log.warn("CHTL 解析失败，使用占位输出: {}", ex.getMessage());
			String htmlBody = "<!-- CHTL 编译占位输出（解析失败回退） -->";
			return new Output(htmlBody, "", ctx.globals.getGlobalJs());
		}
	}

	private void renderNode(ChtlNode node, StringBuilder html, StringBuilder globalCss) {
		if (node instanceof ElementNode el) {
			html.append('<').append(el.tagName());
			for (AttributeNode a : el.attributes()) {
				html.append(' ').append(a.name()).append("=\"").append(escapeHtml(a.value())).append("\"");
			}
			html.append('>').append('\n');
			for (ChtlNode c : el.children()) renderNode(c, html, globalCss);
			html.append("</").append(el.tagName()).append('>').append('\n');
		} else if (node instanceof TextNode t) {
			html.append(escapeHtml(t.text())).append('\n');
		} else if (node instanceof StyleBlockNode s) {
			// 内联作为 style 属性已在 AST 层表现，这里简单拼接到 style 标签（简化）
			if (!s.inlineStyles().isEmpty()) {
				StringBuilder inline = new StringBuilder();
				for (AttributeNode a : s.inlineStyles()) inline.append(a.name()).append(':').append(a.value()).append(';');
				html.append("<style>").append(inline).append("</style>\n");
			}
			for (StyleRuleNode r : s.globalRules()) {
				globalCss.append(r.selector()).append("{\n");
				for (AttributeNode a : r.declarations()) globalCss.append(a.name()).append(':').append(a.value()).append(';').append('\n');
				globalCss.append("}\n");
			}
		} else if (node instanceof ScriptBlockNode sb) {
			html.append("<script>\n").append(sb.code()).append("\n</script>\n");
		}
	}

	private static String escapeHtml(String s) {
		return s.replace("&","&amp;").replace("<","&lt;").replace(">","&gt;").replace("\"","&quot;");
	}
}