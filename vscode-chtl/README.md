# CHTL Language Support for Visual Studio Code

This extension provides comprehensive language support for CHTL (Custom HyperText Language) in Visual Studio Code.

## Features

### Syntax Highlighting
- Full CHTL syntax highlighting
- Embedded CSS and JavaScript support
- CHTL-specific keywords and constructs

### IntelliSense
- Auto-completion for CHTL keywords
- Template and function suggestions
- Smart bracket completion

### Built-in Compiler
- No separate installation required
- Compile on save
- Real-time error checking

### Code Navigation
- Go to definition
- Find all references
- Symbol search

### Code Snippets
- Common CHTL patterns
- Template snippets
- Event handler snippets

## Requirements

- Visual Studio Code 1.74.0 or higher
- Java 17 or higher (for the built-in compiler)

## Installation

1. Open Visual Studio Code
2. Press `Ctrl+P` / `Cmd+P` to open Quick Open
3. Type `ext install chtl` and press Enter
4. Or install from the [Visual Studio Marketplace](https://marketplace.visualstudio.com/items?itemName=chtl-team.chtl)

## Usage

### Creating a CHTL File

1. Create a new file with `.chtl` extension
2. Start typing - the extension will provide syntax highlighting and auto-completion

### Compiling

- **Manual Compile**: Right-click on a `.chtl` file and select "CHTL: Compile Current File"
- **Auto Compile**: Enable auto-compile in settings to compile on save
- **Compile All**: Use command palette (`Ctrl+Shift+P`) and run "CHTL: Compile All Files"

### Configuration

Open VS Code settings and search for "CHTL" to configure:

- `chtl.compiler.autoSave`: Enable/disable auto-compile on save
- `chtl.compiler.outputPath`: Set output directory for compiled files
- `chtl.compiler.mode`: Choose between development and production mode
- `chtl.compiler.optimize`: Enable compilation optimization
- `chtl.compiler.sourcemap`: Generate source maps

## Example

```chtl
[Template] @Element Card {
    div {
        class = "card";
        
        h2 {
            slot { }
        }
        
        div {
            class = "content";
            slot { }
        }
        
        style {
            & {
                border: 1px solid #ddd;
                border-radius: 8px;
                padding: 16px;
                margin: 8px;
            }
            
            & h2 {
                margin-top: 0;
                color: #333;
            }
        }
    }
}

// 使用示例
body {
    @Element Card {
        text { Welcome to CHTL }
        text { This is a simple card component. }
    }
}
```

## Module Support

The extension includes support for CHTL modules:

- **Install Module**: `Ctrl+Shift+P` → "CHTL: Install Module"
- **Create Module**: `Ctrl+Shift+P` → "CHTL: Create New Module"
- **Official Modules**: Includes the Chtholly UI framework

## Troubleshooting

### Java Not Found

The extension requires Java 17+ to run the built-in compiler. Make sure:

1. Java 17 or higher is installed
2. `JAVA_HOME` environment variable is set
3. Or configure `chtl.compiler.javaPath` in settings

### Compilation Errors

1. Check the Output panel (View → Output → CHTL)
2. Ensure your CHTL syntax is correct
3. Check file permissions

## Contributing

Found a bug or have a feature request? Please visit our [GitHub repository](https://github.com/your-org/vscode-chtl).

## License

MIT

## Release Notes

### 1.0.0

- Initial release
- Full CHTL syntax highlighting
- Built-in compiler integration
- Auto-completion and IntelliSense
- Real-time error checking
- Module support