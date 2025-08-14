(function(){
	if (!window.__CHTL_DELEGATE__) window.__CHTL_DELEGATE__ = { map: new Map() };
	window.__CHTL_DELEGATE__.ensure = function(root, type){
		let key = root.__chtl_delegate_key || (root.__chtl_delegate_key = Math.random().toString(36).slice(2));
		let m = this.map.get(key);
		if (!m) { m = {}; this.map.set(key, m); }
		if (!m[type]) {
			m[type] = [];
			root.addEventListener(type, function(evt){
				let regs = m[type]; if (!regs) return;
				let t = evt.target;
				while (t && t !== root) {
					for (const r of regs) {
						if (t.matches && t.matches(r.selector)) r.handler.call(t, evt);
					}
					t = t.parentNode;
				}
			});
		}
		return m[type];
	};
})();