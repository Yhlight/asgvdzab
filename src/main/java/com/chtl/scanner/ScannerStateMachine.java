package com.chtl.scanner;

public class ScannerStateMachine {
    public enum State { INITIAL, CHTL, CSS, JS, CHTL_JS }
    private State currentState = State.INITIAL;
    public State getState() { return currentState; }
    public void setState(State state) { this.currentState = state; }
}
