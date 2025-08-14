from __future__ import annotations

from chtl_compiler.types import CompilationBundle


class ResultMerger:
	def __init__(self) -> None:
		pass

	def merge_to_html_document(self, bundle: CompilationBundle) -> str:
		unit = bundle.merge()
		css_block = f"<style>\n{unit.css}\n</style>\n" if unit.css.strip() else ""
		js_block = f"<script>\n{unit.js}\n</script>\n" if unit.js.strip() else ""
		body = unit.html
		# If result already includes <html>, assume full doc
		if '<html' in body or '<!DOCTYPE' in body:
			# Append assets if not already present (naive)
			return f"{body}\n{css_block}{js_block}".strip()
		# Minimal HTML doc scaffold
		return (
			"<!DOCTYPE html>\n"
			"<html lang=\"en\">\n"
			"<head>\n<meta charset=\"utf-8\">\n" + css_block + "</head>\n"
			"<body>\n" + body + "\n" + js_block + "</body>\n"
			"</html>\n"
		)