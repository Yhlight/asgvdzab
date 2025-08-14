from __future__ import annotations

import argparse
import sys
from pathlib import Path

from chtl_compiler.scanner.unified_scanner import CHTLUnifiedScanner
from chtl_compiler.dispatcher.compiler_dispatcher import CompilerDispatcher
from chtl_compiler.merger.result_merger import ResultMerger


def main(argv: list[str] | None = None) -> int:
	parser = argparse.ArgumentParser(description="CHTL pipeline CLI (scaffold)")
	parser.add_argument("source", help="Path to .chtl source")
	parser.add_argument("-o", "--out", help="Output HTML path; omit to print to stdout")
	args = parser.parse_args(argv)

	src_path = Path(args.source)
	if not src_path.exists():
		print(f"Source not found: {src_path}", file=sys.stderr)
		return 1

	source_text = src_path.read_text(encoding="utf-8")
	scanner = CHTLUnifiedScanner()
	dispatcher = CompilerDispatcher()
	merger = ResultMerger()

	fragments = scanner.scan(source_text)
	bundle = dispatcher.dispatch(fragments)
	html_doc = merger.merge_to_html_document(bundle)

	if args.out:
		Path(args.out).write_text(html_doc, encoding="utf-8")
	else:
		print(html_doc)
	return 0


if __name__ == "__main__":
	sys.exit(main())