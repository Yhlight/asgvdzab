package com.chtl.lexer;

import java.util.ArrayDeque;
import java.util.Deque;

public class Context {
	private final GlobalMap globalMap;
	private final Deque<State> stateStack = new ArrayDeque<>();

	public Context(GlobalMap globalMap) {
		this.globalMap = globalMap;
		stateStack.push(State.DEFAULT);
	}

	public GlobalMap getGlobalMap() { return globalMap; }

	public State currentState() { return stateStack.peek(); }

	public void push(State s) { stateStack.push(s); }

	public State pop() { return stateStack.pop(); }
}