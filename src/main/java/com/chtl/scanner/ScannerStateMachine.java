package com.chtl.scanner;

public class ScannerStateMachine {
	public enum State { INITIAL, CHTL, CSS, JS, CHTL_JS }
	private State currentState = State.INITIAL;
	public State getState() { return currentState; }
	public void setState(State state) { this.currentState = state; }

	public ScannerStateMachine() {
	}

	public ScannerStateMachine(com.chtl.context.CompilationContext context) {
		// 保留以兼容调用方，目前不依赖 context
	}
}
