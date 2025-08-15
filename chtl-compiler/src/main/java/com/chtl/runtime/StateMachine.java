package com.chtl.runtime;

import com.chtl.lexer.State;

import java.util.ArrayDeque;
import java.util.Deque;

public class StateMachine {
	private final Deque<State> stack = new ArrayDeque<>();

	public StateMachine() {
		stack.push(State.DEFAULT);
	}

	public State current() { return stack.peek(); }

	public Guard enter(State state) {
		stack.push(state);
		return new Guard(this, state);
	}

	private void exit(State expected) {
		State top = stack.peek();
		if (top == expected) {
			stack.pop();
		} else {
			// 保护性弹栈：直到遇到 expected 或回到 DEFAULT
			while (!stack.isEmpty() && stack.peek() != expected && stack.peek() != State.DEFAULT) {
				stack.pop();
			}
			if (!stack.isEmpty() && stack.peek() == expected) stack.pop();
		}
	}

	public static class Guard implements AutoCloseable {
		private final StateMachine machine;
		private final State state;
		private boolean closed;
		private Guard(StateMachine machine, State state) { this.machine = machine; this.state = state; }
		@Override public void close() { if (!closed) { closed = true; machine.exit(state); } }
	}
}