from __future__ import annotations

from chtl_compiler.types import CompilationUnit


class ChtlJsCompiler:
	def __init__(self) -> None:
		pass

	def compile(self, content: str) -> CompilationUnit:
		# Very small CHTL JS -> JS lowering
		# 1) '->' to '.'
		js = content.replace('->', '.')
		# 2) Transform {{selector}} into __CHTL__sel("selector")
		js = self._transform_enhanced_selectors(js)
		# 3) Inject runtime prelude if any usage found
		prelude = self._runtime_prelude() if "__CHTL__sel(" in js else ""
		return CompilationUnit(js=(prelude + js))

	def _transform_enhanced_selectors(self, js: str) -> str:
		out = []
		i = 0
		n = len(js)
		while i < n:
			if i + 1 < n and js[i] == '{' and js[i + 1] == '{':
				# find closing '}}'
				j = i + 2
				while j + 1 < n and not (js[j] == '}' and js[j + 1] == '}'):
					j += 1
				if j + 1 >= n:
					out.append(js[i:])
					break
				selector = js[i + 2:j].strip()
				out.append(f'__CHTL__sel("{selector}")')
				i = j + 2
				continue
			out.append(js[i])
			i += 1
		return "".join(out)

	def _runtime_prelude(self) -> str:
		return (
			"(function(){\n"
			"  function __CHTL__sel(q){\n"
			"    // very naive selector handling: #id, .class, tag, or space-descendant\n"
			"    var s = q.trim();\n"
			"    try { return document.querySelector(s); } catch(e){ return null; }\n"
			"  }\n"
			"  window.__CHTL__sel = __CHTL__sel;\n"
			"})();\n"
		)