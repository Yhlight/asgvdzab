package com.example.chtl.compilers.chtljs;

public class ChtlJsContext {
	public final ChtlJsGlobalMap globals;
	public final ChtlJsState state;

	public ChtlJsContext(ChtlJsGlobalMap globals, ChtlJsState state){
		this.globals = globals;
		this.state = state;
	}

	public void emitJs(String js){ globals.emitJs(js); }
}