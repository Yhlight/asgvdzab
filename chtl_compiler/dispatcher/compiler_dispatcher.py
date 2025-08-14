from __future__ import annotations

from typing import List

from chtl_compiler.types import CompilationBundle, CompilationUnit, Fragment, FragmentKind
from chtl_compiler.compilers.chtl_compiler import ChtlCompiler
from chtl_compiler.compilers.chtl_js_compiler import ChtlJsCompiler
from chtl_compiler.compilers.css_compiler import CssCompiler
from chtl_compiler.compilers.js_compiler import JsCompiler


class CompilerDispatcher:
	def __init__(self) -> None:
		self._chtl = ChtlCompiler()
		self._chtl_js = ChtlJsCompiler()
		self._css = CssCompiler()
		self._js = JsCompiler()

	def dispatch(self, fragments: List[Fragment]) -> CompilationBundle:
		units: List[CompilationUnit] = []
		for fragment in fragments:
			if fragment.kind == FragmentKind.CHTL:
				units.append(self._chtl.compile(fragment.content))
			elif fragment.kind == FragmentKind.CHTL_JS:
				units.append(self._chtl_js.compile(fragment.content))
			elif fragment.kind == FragmentKind.CSS:
				units.append(self._css.compile(fragment.content))
			elif fragment.kind == FragmentKind.JS:
				units.append(self._js.compile(fragment.content))
		return CompilationBundle(units=units)