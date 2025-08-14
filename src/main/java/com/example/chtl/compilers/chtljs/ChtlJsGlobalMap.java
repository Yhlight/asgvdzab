package com.example.chtl.compilers.chtljs;

public class ChtlJsGlobalMap {
	private final StringBuilder globalJs = new StringBuilder();
	public void emitJs(String js){ if(js!=null && !js.isBlank()) globalJs.append(js).append('\n'); }
	public String getGlobalJs(){ return globalJs.toString(); }
}