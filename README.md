# CHTL Compiler Pipeline (Scaffold)

This project scaffolds the requested CHTL compilation pipeline:

- CHTL source → CHTLUnifiedScanner (精准代码切割器)
- Fragments: CHTL 片段, CHTL JS 片段, CSS 片段, JS 片段
- CompilerDispatcher (编译器调度器)
- Compilers:
  - CHTL Compiler (手写, minimal subset)
  - CHTL JS Compiler (手写, passthrough + TODO for扩展语法)
  - CSS Compiler (ANTLR placeholder, passthrough)
  - JavaScript Compiler (ANTLR placeholder, passthrough)
- 编译结果合并 → HTML 输出

Note: This is a functional scaffold. It parses a small, well-defined subset of CHTL sufficient to demonstrate the pipeline. Many advanced features outlined in the CHTL syntax document are left as TODOs.

## Usage

- Run the CLI on a `.chtl` file and get HTML output:

```bash
python -m chtl_compiler.cli /workspace/examples/sample.chtl -o /workspace/out.html
```

- Print to stdout:

```bash
python -m chtl_compiler.cli /workspace/examples/sample.chtl
```

## Layout

- `chtl_compiler/`
  - `scanner/unified_scanner.py`: CHTLUnifiedScanner
  - `dispatcher/compiler_dispatcher.py`: CompilerDispatcher
  - `compilers/`: CHTL, CHTL JS (handwritten), CSS & JS (ANTLR placeholders)
  - `merger/result_merger.py`: 编译结果合并
  - `types.py`: Shared types
  - `cli.py`: CLI entrypoint
- `examples/sample.chtl`: Minimal sample

## Roadmap (selected)

- Full CHTL AST & semantics
- Complete CHTL JS transformations (增强选择器, listen/delegate/animate)
- Integrate real ANTLR grammars for CSS/JS
- Source maps and diagnostics