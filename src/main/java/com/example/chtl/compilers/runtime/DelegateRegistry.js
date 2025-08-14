(function(global){
  if (!global.__CHTL_DELEGATE__) {
    const store = new WeakMap(); // element -> Map(event -> Array<{selector, handler}>)
    function getEventMap(el){
      let m = store.get(el); if (!m){ m = new Map(); store.set(el, m); }
      return m;
    }
    function ensure(el, event){
      const m = getEventMap(el);
      let arr = m.get(event);
      if (!arr){ arr = []; m.set(event, arr); }
      return arr;
    }
    function register(el, event, selector, handler){
      const regs = ensure(el, event);
      // 去重：同一 selector+handler 不重复注册
      for (let i=0;i<regs.length;i++){
        const r = regs[i];
        if (r.selector === selector && r.handler === handler) return regs;
      }
      regs.push({ selector, handler });
      // 懒绑定事件一次
      if (!el.__chtl_bound__) el.__chtl_bound__ = new Set();
      const key = event+"#";
      if (!el.__chtl_bound__.has(key)){
        el.addEventListener(event, function(ev){
          const entries = ensure(el, event).slice();
          for (const r of entries){
            try {
              if (matches(ev.target, r.selector, el)) r.handler.call(ev.target, ev);
            } catch (_) {}
          }
        });
        el.__chtl_bound__.add(key);
      }
      return regs;
    }
    function matches(node, selector, root){
      if (!node || !(node instanceof Element)) return false;
      try { return node.matches(selector); } catch (_){ return false; }
    }
    function dispose(el){ store.delete(el); if (el && el.__chtl_bound__) el.__chtl_bound__.clear(); }
    global.__CHTL_DELEGATE__ = { ensure, register, dispose };
  }
})(window);