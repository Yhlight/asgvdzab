# Changelog

All notable changes to the "CHTL Language Support" extension will be documented in this file.

## [1.0.0] - 2024-01-15

### Added
- Initial release of CHTL Language Support for Visual Studio Code
- Complete syntax highlighting for CHTL language
- IntelliSense and auto-completion support
- Built-in CHTL compiler integration
- Real-time error checking and diagnostics
- Code snippets for common patterns
- Module management commands
- Compile on save functionality
- Preview command for compiled output
- Support for official CHTL modules (Chtholly UI framework)

### Features
- **Language Support**
  - Syntax highlighting for CHTL keywords, templates, and constructs
  - Embedded CSS and JavaScript highlighting
  - CHTL JS enhanced syntax support
  - Smart bracket completion for `[]` keywords

- **Developer Experience**
  - Auto-completion for keywords, templates, and functions
  - Real-time syntax error detection
  - Go to definition and find references
  - Symbol navigation

- **Compilation**
  - Built-in compiler (no external dependencies)
  - Configurable output directory
  - Development and production modes
  - Source map generation
  - Optimization options

- **Module System**
  - Install and manage CHTL modules
  - Create new modules with templates
  - Official module support

### Known Issues
- First compilation may be slower while Java runtime initializes
- Large files may experience delayed error checking

### Requirements
- Visual Studio Code 1.74.0 or higher
- Java 17 or higher for compiler functionality