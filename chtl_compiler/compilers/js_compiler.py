from __future__ import annotations

from chtl_compiler.types import CompilationUnit


class JsCompiler:
	def __init__(self) -> None:
		pass

	def compile(self, content: str) -> CompilationUnit:
		# Placeholder: passthrough
		return CompilationUnit(js=content)