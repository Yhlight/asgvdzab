(function(){
	if (!window.__CHTL_DELEGATE__) window.__CHTL_DELEGATE__ = { map: new Map() };
	const REG = window.__CHTL_DELEGATE__;
	REG.ensure = function(root, type){
		let key = root.__chtl_delegate_key || (root.__chtl_delegate_key = Math.random().toString(36).slice(2));
		let entry = this.map.get(key);
		if (!entry) { entry = { root, events: {} }; this.map.set(key, entry); }
		if (!entry.events[type]) {
			entry.events[type] = [];
			root.addEventListener(type, function(evt){
				let regs = entry.events[type]; if (!regs) return;
				let t = evt.target;
				while (t && t !== root) {
					for (const r of regs) { if (t.matches && t.matches(r.selector)) r.handler.call(t, evt); }
					t = t.parentNode;
				}
			});
		}
		return entry.events[type];
	};
	REG.add = function(root, type, selector, handler){
		let regs = this.ensure(root, type);
		for (const r of regs) if (r.selector===selector && r.handler===handler) return; // 去重
		regs.push({ selector, handler });
	};
	REG.remove = function(root, type, selector, handler){
		let key = root.__chtl_delegate_key; if (!key) return;
		let entry = this.map.get(key); if (!entry) return;
		let regs = entry.events[type]; if (!regs) return;
		entry.events[type] = regs.filter(r => !(r.selector===selector && r.handler===handler));
	};
})();