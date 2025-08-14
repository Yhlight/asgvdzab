package com.example.chtl.compilers.chtl;

import java.util.ArrayDeque;
import java.util.Deque;

public class ChtlState {
	public final Deque<String> elementStack = new ArrayDeque<>();
	public String currentNamespace = "";
	public boolean debugMode = false;
}