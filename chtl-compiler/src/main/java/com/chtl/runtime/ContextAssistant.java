package com.chtl.runtime;

import com.chtl.lexer.State;

import java.util.ArrayDeque;
import java.util.Deque;
import java.util.HashMap;
import java.util.Map;

public class ContextAssistant {
	private final StateMachine stateMachine = new StateMachine();
	private final Deque<Map<String, Object>> frames = new ArrayDeque<>();

	public ContextAssistant() { frames.push(new HashMap<>()); }

	public State currentState() { return stateMachine.current(); }

	public StateMachine.Guard enter(State s) { return stateMachine.enter(s); }

	public void pushFrame() { frames.push(new HashMap<>()); }
	public void popFrame() { if (!frames.isEmpty()) frames.pop(); }

	public void set(String key, Object value) { frames.peek().put(key, value); }
	public Object get(String key) {
		for (Map<String, Object> f : frames) {
			if (f.containsKey(key)) return f.get(key);
		}
		return null;
	}
}