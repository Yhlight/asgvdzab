package com.chtl.ast;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public class CHTLJsNodes {
	public interface JsNode extends AstNode {}

	// 增强选择器 {{...}}
	public static class Selector implements JsNode {
		public enum Kind { AUTO, CLASS, ID, TAG, DESCENDANT, INDEXED }
		private final String raw; // 文档限制的几种
		public Selector(String raw) { this.raw = raw; }
		public String getRaw() { return raw; }
	}

	// 链式调用 {{selector}}->method() / .method()
	public static class Chain implements JsNode {
		private final Selector selector; // 可为 null（例如变量场景）
		private final List<Invocation> invocations = new ArrayList<>();
		public Chain(Selector selector) { this.selector = selector; }
		public Selector getSelector() { return selector; }
		public List<Invocation> getInvocations() { return invocations; }
	}

	public static class Invocation implements JsNode {
		private final String name; // 方法或属性名
		private final List<String> args = new ArrayList<>();
		public Invocation(String name) { this.name = name; }
		public String getName() { return name; }
		public List<String> getArgs() { return args; }
	}

	// listen 增强监听器：{{selector}}->listen({ event: handler, ... })
	public static class Listen implements JsNode {
		public final Selector selector;
		public final Map<String, String> handlers; // 事件名 -> 回调（函数名或内联函数文本）
		public Listen(Selector selector, Map<String, String> handlers) { this.selector = selector; this.handlers = handlers; }
	}

	// delegate 事件委托
	public static class Delegate implements JsNode {
		public final Selector parent;
		public final List<Selector> targets; // target: 单个或数组
		public final Map<String, String> handlers;
		public Delegate(Selector parent, List<Selector> targets, Map<String, String> handlers) { this.parent = parent; this.targets = targets; this.handlers = handlers; }
	}

	// animate 动画
	public static class Animate implements JsNode {
		public final int duration;
		public final String easing;
		public final Map<String, String> begin;
		public final List<When> whenList;
		public final Map<String, String> end;
		public final int loop;
		public final String direction;
		public final Integer delay;
		public final String callback;
		public Animate(int duration, String easing, Map<String, String> begin, List<When> whenList, Map<String, String> end, int loop, String direction, Integer delay, String callback) {
			this.duration = duration; this.easing = easing; this.begin = begin; this.whenList = whenList; this.end = end; this.loop = loop; this.direction = direction; this.delay = delay; this.callback = callback;
		}
	}

	public static class When implements JsNode {
		public final double at;
		public final Map<String, String> styles;
		public When(double at, Map<String, String> styles) { this.at = at; this.styles = styles; }
	}
}