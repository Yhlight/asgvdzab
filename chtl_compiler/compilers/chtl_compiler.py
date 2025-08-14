from __future__ import annotations

from dataclasses import dataclass
from typing import List, Tuple

from chtl_compiler.types import CompilationUnit


@dataclass
class Token:
	kind: str
	value: str


class ChtlCompiler:
	def __init__(self) -> None:
		pass

	def compile(self, source: str) -> CompilationUnit:
		# Very small subset parser to demonstrate the pipeline.
		# Supports:
		# - element { ... }
		# - attributes: key : value; (value may be quoted or bare)
		# - text { ... }
		# - CE ':' '=' equivalence
		# - [Origin] @Html { ... } passthrough
		parser = _Parser(source)
		html = parser.parse_document()
		return CompilationUnit(html=html)


class _Parser:
	def __init__(self, source: str) -> None:
		self.s = source
		self.i = 0
		self.n = len(source)

	def parse_document(self) -> str:
		parts: List[str] = []
		while self._skip_ws():
			if self._peek_str("[Origin]"):
				parts.append(self._parse_origin_html())
				continue
			if self._peek_str("text"):
				parts.append(self._parse_text_block())
				continue
			tag = self._read_ident()
			if not tag:
				break
			self._skip_ws()
			if not self._consume('{'):
				# not a block; ignore unknown text
				break
			parts.append(self._parse_element(tag))
		return "".join(parts)

	def _parse_origin_html(self) -> str:
		self._consume_str("[Origin]")
		self._skip_ws()
		if self._consume('@'):
			kw = self._read_ident()
			if kw != 'Html':
				# skip non-html origin in CHTL compiler
				self._skip_until_block_skipped()
				return ""
		self._skip_ws()
		if self._consume('{'):
			content = self._read_balanced()
			return content
		return ""

	def _parse_text_block(self) -> str:
		self._consume_str('text')
		self._skip_ws()
		assert self._consume('{')
		text = self._read_balanced()
		return self._escape_html(self._strip_quotes(text.strip()))

	def _parse_element(self, tag: str) -> str:
		attrs: List[Tuple[str, str]] = []
		children: List[str] = []
		while True:
			self._skip_ws()
			if self._peek('}'):
				self._consume('}')
				break
			if self._peek_str('text'):
				children.append(self._parse_text_block())
				continue
			if self._peek_str('[Origin]'):
				children.append(self._parse_origin_html())
				continue
			# attribute or child element
			pos_before = self.i
			ident = self._read_ident()
			if not ident:
				# skip garbage
				self.i += 1
				continue
			self._skip_ws()
			if self._peek('{'):
				# child element or special local blocks
				self._consume('{')
				if ident in ('style', 'script'):
					# skip local style/script; handled by scanner and dedicated compilers
					_ = self._read_balanced()
					continue
				children.append(self._parse_element(ident))
				continue
			# attribute: key : value;
			if self._consume(':') or self._consume('='):
				self._skip_ws()
				value = self._read_attr_value()
				attrs.append((ident, value))
				self._skip_ws()
				self._consume(';')
				continue
			# not attribute; revert and ignore
			self.i = pos_before
			self.i += 1
			continue
		attr_str = self._render_attrs(attrs)
		if children:
			return f"<{tag}{attr_str}>" + "".join(children) + f"</{tag}>"
		return f"<{tag}{attr_str}></{tag}>"

	def _render_attrs(self, attrs: List[Tuple[str, str]]) -> str:
		parts: List[str] = []
		for k, v in attrs:
			if k == 'style':
				# allow raw CSS within style attr
				parts.append(f" style=\"{v}\"")
			else:
				parts.append(f" {k}=\"{self._escape_html(v)}\"")
			# Auto-class/id handling in CHTL style{} is not implemented here
		return "".join(parts)

	def _read_ident(self) -> str:
		self._skip_ws()
		start = self.i
		while self.i < self.n and (self.s[self.i].isalnum() or self.s[self.i] in ['-', '_']):
			self.i += 1
		return self.s[start:self.i]

	def _read_attr_value(self) -> str:
		self._skip_ws()
		if self._peek('"') or self._peek("'"):
			quote = self.s[self.i]
			self.i += 1
			start = self.i
			while self.i < self.n and self.s[self.i] != quote:
				self.i += 1
			val = self.s[start:self.i]
			self._consume(quote)
			return val
		# bare literal until ; or newline or } or whitespace
		start = self.i
		while self.i < self.n and self.s[self.i] not in [';', '\n', '\r', '}']:
			self.i += 1
		return self.s[start:self.i].strip()

	def _read_balanced(self) -> str:
		depth = 1
		start = self.i
		while self.i < self.n:
			ch = self.s[self.i]
			if ch == '{':
				depth += 1
			elif ch == '}':
				depth -= 1
				if depth == 0:
					content = self.s[start:self.i]
					self.i += 1
					return content
			self.i += 1
		return ""

	def _skip_until_block_skipped(self) -> None:
		self._skip_ws()
		if self._consume('{'):
			_ = self._read_balanced()

	def _skip_ws(self) -> bool:
		moved = False
		while self.i < self.n and self.s[self.i].isspace():
			self.i += 1
			moved = True
		return self.i < self.n or moved

	def _peek(self, ch: str) -> bool:
		return self.i < self.n and self.s[self.i] == ch

	def _peek_str(self, text: str) -> bool:
		return self.s.startswith(text, self.i)

	def _consume(self, ch: str) -> bool:
		if self._peek(ch):
			self.i += 1
			return True
		return False

	def _consume_str(self, text: str) -> bool:
		if self._peek_str(text):
			self.i += len(text)
			return True
		return False

	def _escape_html(self, s: str) -> str:
		return (
			s.replace('&', '&amp;')
			.replace('<', '&lt;')
			.replace('>', '&gt;')
			.replace('"', '&quot;')
		)

	def _strip_quotes(self, s: str) -> str:
		if len(s) >= 2 and s[0] == s[-1] and s[0] in ['"', "'"]:
			return s[1:-1]
		return s