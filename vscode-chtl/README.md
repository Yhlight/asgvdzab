# CHTL Language Support for Visual Studio Code

Complete language support for CHTL (Custom HyperText Language) in Visual Studio Code.

![CHTL Logo](images/logo.png)

## Features

### 🎨 Syntax Highlighting
- Full syntax highlighting for CHTL language constructs
- Support for templates, custom elements, namespaces, and imports
- CHTL JS enhanced syntax highlighting
- Embedded CSS and JavaScript highlighting

### 🚀 IntelliSense & Auto-completion
- Smart completions for CHTL keywords and structures
- HTML element suggestions
- CSS property completions
- CHTL JS method completions (`->listen`, `->delegate`, `->animate`)
- Template and variable references

### 🔍 Real-time Error Checking
- Syntax error detection as you type
- Undefined reference warnings
- Import validation
- Constraint violation detection

### 🛠️ Built-in Compiler
- No external dependencies - compiler included
- Compile on save option
- Batch compilation support
- Development and production modes

### 📦 Code Snippets
- Quick snippets for common patterns
- Template creation helpers
- Complete file templates

## Installation

1. Install from VSCode Marketplace: Search for "CHTL Language Support"
2. Or install manually: `code --install-extension chtl-1.0.0.vsix`

## Quick Start

1. Create a new file with `.chtl` extension
2. Start typing - IntelliSense will guide you
3. Use `Ctrl+Shift+P` and search for "CHTL" commands

### Example CHTL File

```chtl
// Import UI components
[Import] @Chtl from chtl::ui as ui;

// Define custom styles
[Template] @Style Theme {
    primaryColor: "#007bff";
    fontFamily: "Arial, sans-serif";
}

// Main content
body {
    style {
        margin: 0;
        padding: 20px;
        font-family: @Var Theme.fontFamily;
    }
    
    @Element Container from ui {
        h1 {
            style {
                color: @Var Theme.primaryColor;
            }
            text { Welcome to CHTL }
        }
        
        @Element Button from ui {
            style {
                @Style PrimaryButton from ui;
            }
            text { Get Started }
            
            script {
                let btn = {{#button}};
                btn->listen("click", () => {
                    alert("Hello CHTL!");
                });
            }
        }
    }
}
```

## Commands

Access commands via Command Palette (`Ctrl+Shift+P`):

- **CHTL: Compile Current File** - Compile the active CHTL file
- **CHTL: Compile All Files** - Compile all CHTL files in workspace
- **CHTL: Show Preview** - Preview compiled output
- **CHTL: Create New Project** - Generate a new CHTL project structure

## Configuration

Configure CHTL in VSCode settings:

```json
{
    // Compiler settings
    "chtl.compiler.path": "",  // Path to custom compiler (leave empty for built-in)
    "chtl.compiler.javaPath": "java",  // Path to Java executable
    "chtl.compiler.autoSave": true,  // Compile on save
    "chtl.compiler.outputPath": "./dist",  // Output directory
    "chtl.compiler.mode": "development",  // "development" or "production"
    
    // Diagnostics
    "chtl.diagnostics.enable": true,  // Enable error checking
    "chtl.diagnostics.delay": 500  // Delay before checking (ms)
}
```

## Official Modules

The extension includes official CHTL modules:

### UI Module (`chtl::ui`)
- Button, Card, Container components
- Grid and Flex layouts
- Color and spacing variables

### Form Module (`chtl::form`)
- Form components and validation
- Input, Select, Checkbox, Radio
- Built-in validation helpers

## Requirements

- Visual Studio Code 1.74.0 or higher
- Java 17 or higher (for compiler)

## Known Issues

- First compilation may be slow while Java runtime initializes
- Large files may have delayed error checking

## Contributing

Found a bug or have a feature request? Please open an issue on our [GitHub repository](https://github.com/chtl-lang/vscode-chtl).

## License

MIT License - see LICENSE file for details.

---

**Enjoy coding with CHTL!** 🚀