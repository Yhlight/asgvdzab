import * as vscode from 'vscode';
import * as path from 'path';
import { LanguageClient, LanguageClientOptions, ServerOptions } from 'vscode-languageclient/node';
import { CHTLCompletionProvider } from './providers/completionProvider';
import { CHTLHoverProvider } from './providers/hoverProvider';
import { CHTLDiagnosticProvider } from './providers/diagnosticProvider';
import { CHTLCompilerService } from './services/compilerService';
import { CHTLPreviewProvider } from './providers/previewProvider';
import { CHTLProjectCreator } from './commands/projectCreator';

let client: LanguageClient;
let diagnosticProvider: CHTLDiagnosticProvider;
let compilerService: CHTLCompilerService;

export function activate(context: vscode.ExtensionContext) {
    console.log('CHTL Language Support is now active!');

    // Initialize compiler service
    compilerService = new CHTLCompilerService(context);
    
    // Register language features
    registerLanguageFeatures(context);
    
    // Register commands
    registerCommands(context);
    
    // Start language server
    startLanguageServer(context);
    
    // Register file watchers
    registerFileWatchers(context);
}

function registerBracketBehavior(context: vscode.ExtensionContext) {
    // Override default bracket auto-closing for CHTL keywords
    context.subscriptions.push(
        vscode.workspace.onDidChangeTextDocument(event => {
            if (event.document.languageId !== 'chtl') return;
            
            for (const change of event.contentChanges) {
                // Check if user typed '[' 
                if (change.text === '[' && change.rangeLength === 0) {
                    const position = change.range.start;
                    const line = event.document.lineAt(position.line);
                    const textBefore = line.text.substring(0, position.character);
                    const textAfter = line.text.substring(position.character);
                    
                    // Check if this might be a CHTL keyword
                    const keywordPattern = /^\s*$/;
                    const afterPattern = /^[A-Z][a-zA-Z]*/;
                    
                    if (keywordPattern.test(textBefore) && afterPattern.test(textAfter)) {
                        // This might be a keyword like [Template], don't auto-close
                        const editor = vscode.window.activeTextEditor;
                        if (editor && editor.document === event.document) {
                            // Remove the auto-inserted ']' if it exists
                            const nextChar = event.document.getText(new vscode.Range(
                                position.translate(0, 1),
                                position.translate(0, 2)
                            ));
                            
                            if (nextChar === ']') {
                                editor.edit(editBuilder => {
                                    editBuilder.delete(new vscode.Range(
                                        position.translate(0, 1),
                                        position.translate(0, 2)
                                    ));
                                }, { undoStopBefore: false, undoStopAfter: false });
                            }
                        }
                    }
                }
            }
        })
    );
    
    // Add smart typing for CHTL keywords
    context.subscriptions.push(
        vscode.languages.registerCompletionItemProvider('chtl', {
            provideCompletionItems(document: vscode.TextDocument, position: vscode.Position) {
                const linePrefix = document.lineAt(position).text.substring(0, position.character);
                
                // If user typed '[', suggest CHTL keywords
                if (linePrefix.endsWith('[')) {
                    const keywords = ['Template', 'Custom', 'Origin', 'Import', 'Namespace', 'Configuration', 'Info', 'Export'];
                    
                    return keywords.map(keyword => {
                        const item = new vscode.CompletionItem(keyword, vscode.CompletionItemKind.Keyword);
                        item.insertText = keyword + ']';
                        item.detail = `CHTL ${keyword} declaration`;
                        item.documentation = new vscode.MarkdownString(`Creates a **[${keyword}]** block in CHTL`);
                        
                        // Move cursor after the closing bracket
                        item.command = {
                            command: 'cursorMove',
                            arguments: [{
                                to: 'right',
                                by: 'character',
                                value: 1
                            }]
                        };
                        
                        return item;
                    });
                }
                
                return undefined;
            }
        }, '[')
    );
}

function registerLanguageFeatures(context: vscode.ExtensionContext) {
    // Register completion provider
    const completionProvider = new CHTLCompletionProvider();
    context.subscriptions.push(
        vscode.languages.registerCompletionItemProvider(
            'chtl',
            completionProvider,
            '.', '@', ':', '-', '#', '{'
        )
    );
    
    // Register hover provider
    const hoverProvider = new CHTLHoverProvider();
    context.subscriptions.push(
        vscode.languages.registerHoverProvider('chtl', hoverProvider)
    );
    
    // Register custom bracket behavior
    registerBracketBehavior(context);
    
    // Register diagnostic provider
    diagnosticProvider = new CHTLDiagnosticProvider(compilerService);
    diagnosticProvider.activate(context.subscriptions);
}

function registerCommands(context: vscode.ExtensionContext) {
    // Compile current file
    context.subscriptions.push(
        vscode.commands.registerCommand('chtl.compile', async () => {
            const editor = vscode.window.activeTextEditor;
            if (!editor || editor.document.languageId !== 'chtl') {
                vscode.window.showErrorMessage('No CHTL file is currently open');
                return;
            }
            
            await compilerService.compileFile(editor.document.uri);
        })
    );
    
    // Compile all files
    context.subscriptions.push(
        vscode.commands.registerCommand('chtl.compileAll', async () => {
            await compilerService.compileAll();
        })
    );
    
    // Show preview
    context.subscriptions.push(
        vscode.commands.registerCommand('chtl.showPreview', () => {
            CHTLPreviewProvider.createOrShow(context.extensionUri);
        })
    );
    
    // Create new project
    context.subscriptions.push(
        vscode.commands.registerCommand('chtl.createProject', async () => {
            await CHTLProjectCreator.create();
        })
    );
}

function startLanguageServer(context: vscode.ExtensionContext) {
    // Server module path
    const serverModule = context.asAbsolutePath(
        path.join('out', 'server', 'server.js')
    );
    
    // Server options
    const serverOptions: ServerOptions = {
        run: { module: serverModule },
        debug: {
            module: serverModule,
            options: { execArgv: ['--nolazy', '--inspect=6009'] }
        }
    };
    
    // Client options
    const clientOptions: LanguageClientOptions = {
        documentSelector: [{ scheme: 'file', language: 'chtl' }],
        synchronize: {
            fileEvents: vscode.workspace.createFileSystemWatcher('**/*.chtl')
        }
    };
    
    // Create and start the language client
    client = new LanguageClient(
        'chtlLanguageServer',
        'CHTL Language Server',
        serverOptions,
        clientOptions
    );
    
    // Start the client
    client.start();
}

function registerFileWatchers(context: vscode.ExtensionContext) {
    // Watch for CHTL file changes
    const watcher = vscode.workspace.createFileSystemWatcher('**/*.chtl');
    
    // Handle file creation
    watcher.onDidCreate(uri => {
        console.log(`CHTL file created: ${uri.fsPath}`);
    });
    
    // Handle file changes
    watcher.onDidChange(uri => {
        // Trigger diagnostics update
        if (diagnosticProvider) {
            diagnosticProvider.updateDiagnostics(uri);
        }
        
        // Auto-compile if enabled
        const config = vscode.workspace.getConfiguration('chtl');
        if (config.get('compiler.autoSave')) {
            compilerService.compileFile(uri);
        }
    });
    
    // Handle file deletion
    watcher.onDidDelete(uri => {
        if (diagnosticProvider) {
            diagnosticProvider.deleteDiagnostics(uri);
        }
    });
    
    context.subscriptions.push(watcher);
}

export function deactivate(): Thenable<void> | undefined {
    if (!client) {
        return undefined;
    }
    return client.stop();
}