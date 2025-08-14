package com.example.chtl.compilers.chtl;

public class ChtlContext {
	public final ChtlGlobalMap globals;
	public final ChtlState state;

	public ChtlContext(ChtlGlobalMap globals, ChtlState state) {
		this.globals = globals;
		this.state = state;
	}

	public void emitCss(String css) { globals.emitGlobalCss(css); }
	public void emitJs(String js) { globals.emitGlobalJs(js); }
}