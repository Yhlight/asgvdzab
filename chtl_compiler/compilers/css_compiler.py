from __future__ import annotations

from chtl_compiler.types import CompilationUnit


class CssCompiler:
	def __init__(self) -> None:
		pass

	def compile(self, content: str) -> CompilationUnit:
		# Placeholder: passthrough
		return CompilationUnit(css=content)