package com.example.chtl.compilers.chtl;

import com.example.chtl.core.config.Configuration;

import java.util.ArrayDeque;
import java.util.Deque;

public class ChtlState {
	public final Deque<String> elementStack = new ArrayDeque<>();
	public final Deque<String> namespaceStack = new ArrayDeque<>();
	public String currentNamespace = "";
	public boolean debugMode = false;

	public final Configuration configuration;
	private int autoClassCounter;
	private int autoIdCounter;

	public ChtlGlobalMap globals;

	public ChtlState(){
		this(Configuration.defaults());
	}

	public ChtlState(Configuration configuration){
		this.configuration = configuration;
		this.autoClassCounter = configuration.INDEX_INITIAL_COUNT;
		this.autoIdCounter = configuration.INDEX_INITIAL_COUNT;
		this.debugMode = configuration.DEBUG_MODE;
		this.globals = new ChtlGlobalMap();
	}

	public ChtlState(ChtlGlobalMap globals){
		this(Configuration.defaults());
		this.globals = globals;
	}

	public String nextAutoClass(){ return "auto-c" + (autoClassCounter++); }
	public String nextAutoId(){ return "auto-i" + (autoIdCounter++); }
}