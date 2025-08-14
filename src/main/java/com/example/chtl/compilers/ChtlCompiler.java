package com.example.chtl.compilers;

import com.example.chtl.ast.chtl.*;
import com.example.chtl.compilers.chtl.ChtlContext;
import com.example.chtl.compilers.chtl.ChtlGlobalMap;
import com.example.chtl.compilers.chtl.ChtlLexer;
import com.example.chtl.compilers.chtl.ChtlState;
import com.example.chtl.parsers.chtl.ChtlParser;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.nio.file.Path;
import java.util.HashSet;
import java.util.Set;

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
		return compile(chtlSource, null);
	}

	public Output compile(String chtlSource, Path baseDir) {
		var lexer = new ChtlLexer(chtlSource);
		var tokens = lexer.lex();
		var parser = new ChtlParser(chtlSource, tokens, new ChtlState());
		ChtlContext ctx = new ChtlContext(new ChtlGlobalMap(), new ChtlState());
		ChtlDocument doc = parser.parseDocument();
		StringBuilder html = new StringBuilder();
		StringBuilder globalCss = new StringBuilder();
		for (ChtlNode n : doc.items()) { renderNode(n, html, globalCss, new HashSet<>()); }
		return new Output(html.toString(), globalCss.toString(), ctx.globals.getGlobalJs());
	}

	private void renderNode(ChtlNode node, StringBuilder html, StringBuilder globalCss, Set<String> except) {
		if (node instanceof ImportNode.NamespaceNode ns) {
			for (var item : ns.body()) renderNode(item, html, globalCss, new HashSet<>(except));
			return;
		}
		if (node instanceof ElementNode el) {
			// except 静态检查：如果当前元素或其子元素在父 except 集合内，跳过
			for (var c : el.constraints()) for (var t : c.targets()) except.add(t);
			if (except.contains(el.tagName()) || except.contains("@Html") || except.contains("[Template]") || except.contains("[Custom]")) return;
			html.append('<').append(el.tagName());
			for (AttributeNode a : el.attributes()) {
				html.append(' ').append(a.name()).append("=\"").append(escapeHtml(a.value())).append("\"");
			}
			html.append('>').append('\n');
			for (ChtlNode c : el.children()) renderNode(c, html, globalCss, new HashSet<>(except));
			html.append("</").append(el.tagName()).append('>').append('\n');
		} else if (node instanceof TextNode t) {
			html.append(escapeHtml(t.text())).append('\n');
		} else if (node instanceof StyleBlockNode s) {
			// inline 去重并渲染
			Set<String> seen = new HashSet<>();
			StringBuilder inline = new StringBuilder();
			for (AttributeNode a : s.inlineStyles()) {
				if (seen.add(a.name())) inline.append(a.name()).append(':').append(a.value()).append(';');
			}
			if (inline.length() > 0) html.append("<style>").append(inline).append("</style>\n");
			for (StyleRuleNode r : s.globalRules()) {
				globalCss.append(r.selector()).append("{\n");
				for (AttributeNode a : r.declarations()) globalCss.append(a.name()).append(':').append(a.value()).append(';').append('\n');
				globalCss.append("}\n");
			}
		} else if (node instanceof ScriptBlockNode sb) {
			html.append("<script>\n").append(sb.code()).append("\n</script>\n");
		} else if (node instanceof OriginNodes.OriginHtmlNode oh) {
			html.append(oh.html()).append('\n');
		} else if (node instanceof OriginNodes.OriginStyleNode os) {
			globalCss.append(os.css()).append('\n');
		} else if (node instanceof OriginNodes.OriginJavaScriptNode oj) {
			// 作为全局 JS 注入
			html.append("<script>\n").append(oj.js()).append("\n</script>\n");
		}
	}

	private static String escapeHtml(String s) {
		return s.replace("&","&amp;").replace("<","&lt;").replace(">","&gt;").replace("\"","&quot;");
	}
}