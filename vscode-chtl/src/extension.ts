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