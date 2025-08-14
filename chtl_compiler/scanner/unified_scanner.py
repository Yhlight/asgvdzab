from __future__ import annotations

from typing import List, Tuple

from chtl_compiler.types import Fragment, FragmentKind


class CHTLUnifiedScanner:
	def __init__(self) -> None:
		pass

	def scan(self, source: str) -> List[Fragment]:
		fragments: List[Fragment] = []
		# Always include a primary CHTL fragment for the whole source
		fragments.append(Fragment(kind=FragmentKind.CHTL, content=source, start=0, end=len(source)))

		# Extract style{} and script{} blocks as fragments (shallow scan)
		for kind, start, end, content in self._extract_block_pairs(source, ["style", "script"]):
			if kind == "style":
				fragments.append(Fragment(kind=FragmentKind.CSS, content=content, start=start, end=end))
			elif kind == "script":
				fragments.append(Fragment(kind=FragmentKind.CHTL_JS, content=content, start=start, end=end))

		# Extract [Origin] blocks
		for origin_kind, start, end, content in self._extract_origin_blocks(source):
			if origin_kind == "@Style":
				fragments.append(Fragment(kind=FragmentKind.CSS, content=content, start=start, end=end, metadata={"origin": "style"}))
			elif origin_kind == "@JavaScript":
				fragments.append(Fragment(kind=FragmentKind.JS, content=content, start=start, end=end, metadata={"origin": "javascript"}))
			elif origin_kind == "@Html":
				# Origin HTML will be handled by CHTL compiler; we do not split it out here to avoid duplication
				pass

		return fragments

	def _extract_block_pairs(self, source: str, keywords: List[str]) -> List[Tuple[str, int, int, str]]:
		results: List[Tuple[str, int, int, str]] = []
		idx = 0
		length = len(source)
		while idx < length:
			for key in keywords:
				if source.startswith(key, idx):
					j = idx + len(key)
					while j < length and source[j].isspace():
						j += 1
					if j < length and source[j] == '{':
						block_start = j
						block_end = self._find_matching_brace(source, block_start)
						if block_end != -1:
							content = source[block_start + 1:block_end]
							results.append((key, idx, block_end + 1, content))
							idx = block_end + 1
							break
					else:
						continue
			else:
				idx += 1
		return results

	def _extract_origin_blocks(self, source: str) -> List[Tuple[str, int, int, str]]:
		results: List[Tuple[str, int, int, str]] = []
		idx = 0
		length = len(source)
		prefix = "[Origin]"
		while idx < length:
			if source.startswith(prefix, idx):
				j = idx + len(prefix)
				while j < length and source[j].isspace():
					j += 1
				if j < length and source[j] == '@':
					k = j
					while k < length and (source[k].isalnum() or source[k] in ['@']):
						k += 1
					kind = source[j:k]
					while k < length and source[k].isspace():
						k += 1
					if k < length and source[k] == '{':
						block_start = k
						block_end = self._find_matching_brace(source, block_start)
						if block_end != -1:
							content = source[block_start + 1:block_end]
							results.append((kind, idx, block_end + 1, content))
							idx = block_end + 1
							continue
			idx += 1
		return results

	def _find_matching_brace(self, source: str, open_index: int) -> int:
		depth = 0
		i = open_index
		length = len(source)
		while i < length:
			ch = source[i]
			if ch == '{':
				depth += 1
			elif ch == '}':
				depth -= 1
				if depth == 0:
					return i
			i += 1
		return -1