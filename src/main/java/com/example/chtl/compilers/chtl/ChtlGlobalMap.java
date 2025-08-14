package com.example.chtl.compilers.chtl;

import java.util.HashMap;
import java.util.Map;

public class ChtlGlobalMap {
	public final Map<String, Object> styleTemplates = new HashMap<>();
	public final Map<String, Object> elementTemplates = new HashMap<>();
	public final Map<String, Object> varTemplates = new HashMap<>();
	public final Map<String, Object> customStyles = new HashMap<>();
	public final Map<String, Object> customElements = new HashMap<>();
	public final Map<String, Object> customVars = new HashMap<>();

	private final StringBuilder globalCss = new StringBuilder();
	private final StringBuilder globalJs = new StringBuilder();

	public void emitGlobalCss(String css) { if (css != null && !css.isBlank()) globalCss.append(css).append('\n'); }
	public void emitGlobalJs(String js) { if (js != null && !js.isBlank()) globalJs.append(js).append('\n'); }

	public String getGlobalCss() { return globalCss.toString(); }
	public String getGlobalJs() { return globalJs.toString(); }
}