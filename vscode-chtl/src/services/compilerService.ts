import * as vscode from 'vscode';
import * as path from 'path';
import * as fs from 'fs';
import * as child_process from 'child_process';

export interface CompilerError {
    type: 'error' | 'warning' | 'info' | 'hint';
    message: string;
    line?: number;
    column?: number;
    length?: number;
    code?: string;
    relatedInfo?: CompilerError[];
}

export interface CompilerResult {
    success: boolean;
    output?: string;
    errors: CompilerError[];
    warnings: CompilerError[];
}

export class CHTLCompilerService {
    private context: vscode.ExtensionContext;
    private compilerPath: string;
    private modulePath: string;
    private outputChannel: vscode.OutputChannel;
    
    constructor(context: vscode.ExtensionContext) {
        this.context = context;
        this.outputChannel = vscode.window.createOutputChannel('CHTL Compiler');
        
        // Set compiler and module paths
        this.compilerPath = this.getCompilerPath();
        this.modulePath = this.getModulePath();
        
        // Verify compiler exists
        this.verifyCompiler();
    }
    
    private getCompilerPath(): string {
        // Check user configuration first
        const config = vscode.workspace.getConfiguration('chtl');
        const userPath = config.get<string>('compiler.path');
        
        if (userPath && fs.existsSync(userPath)) {
            return userPath;
        }
        
        // Use built-in compiler
        return this.context.asAbsolutePath(path.join('compiler', 'chtl-compiler.jar'));
    }
    
    private getModulePath(): string {
        return this.context.asAbsolutePath(path.join('compiler', 'modules'));
    }
    
    private verifyCompiler() {
        if (!fs.existsSync(this.compilerPath)) {
            vscode.window.showErrorMessage(
                'CHTL compiler not found. Please check the compiler path in settings.',
                'Open Settings'
            ).then(selection => {
                if (selection === 'Open Settings') {
                    vscode.commands.executeCommand('workbench.action.openSettings', 'chtl.compiler.path');
                }
            });
        }
    }
    
    public async validateDocument(document: vscode.TextDocument): Promise<CompilerError[]> {
        // Save document content to temporary file
        const tempFile = path.join(this.context.globalStorageUri.fsPath, `temp_${Date.now()}.chtl`);
        
        try {
            // Ensure directory exists
            await vscode.workspace.fs.createDirectory(this.context.globalStorageUri);
            
            // Write content
            await vscode.workspace.fs.writeFile(
                vscode.Uri.file(tempFile),
                Buffer.from(document.getText())
            );
            
            // Run compiler in validation mode
            const result = await this.runCompiler([
                '--validate-only',
                '--json-output',
                tempFile
            ]);
            
            // Parse errors from result
            return this.parseValidationResult(result);
            
        } catch (error) {
            console.error('Validation error:', error);
            return [];
        } finally {
            // Clean up temp file
            try {
                await vscode.workspace.fs.delete(vscode.Uri.file(tempFile));
            } catch {}
        }
    }
    
    public async compileFile(uri: vscode.Uri): Promise<CompilerResult> {
        this.outputChannel.show();
        this.outputChannel.appendLine(`Compiling ${uri.fsPath}...`);
        
        try {
            const config = vscode.workspace.getConfiguration('chtl');
            const mode = config.get<string>('compiler.mode', 'development');
            const outputPath = config.get<string>('compiler.outputPath', './dist');
            
            // Prepare output directory
            const workspaceFolder = vscode.workspace.getWorkspaceFolder(uri);
            const outputDir = workspaceFolder 
                ? path.join(workspaceFolder.uri.fsPath, outputPath)
                : path.dirname(uri.fsPath);
            
            // Create output directory if needed
            if (!fs.existsSync(outputDir)) {
                fs.mkdirSync(outputDir, { recursive: true });
            }
            
            // Determine output file name
            const inputName = path.basename(uri.fsPath, '.chtl');
            const outputFile = path.join(outputDir, `${inputName}.html`);
            
            // Run compiler
            const args = [
                `--${mode}`,
                '--module-path', this.modulePath,
                '-o', outputFile,
                uri.fsPath
            ];
            
            const result = await this.runCompiler(args);
            
            if (result.success) {
                this.outputChannel.appendLine(`✓ Compilation successful: ${outputFile}`);
                vscode.window.showInformationMessage(`CHTL compiled successfully: ${path.basename(outputFile)}`);
            } else {
                this.outputChannel.appendLine(`✗ Compilation failed`);
                this.showErrors(result.errors);
            }
            
            return result;
            
        } catch (error) {
            this.outputChannel.appendLine(`Error: ${error}`);
            return {
                success: false,
                errors: [{
                    type: 'error',
                    message: String(error)
                }],
                warnings: []
            };
        }
    }
    
    public async compileAll(): Promise<void> {
        const files = await vscode.workspace.findFiles('**/*.chtl', '**/node_modules/**');
        
        if (files.length === 0) {
            vscode.window.showInformationMessage('No CHTL files found in workspace');
            return;
        }
        
        vscode.window.withProgress({
            location: vscode.ProgressLocation.Notification,
            title: 'Compiling CHTL files',
            cancellable: true
        }, async (progress, token) => {
            let compiled = 0;
            let errors = 0;
            
            for (const file of files) {
                if (token.isCancellationRequested) {
                    break;
                }
                
                progress.report({
                    increment: 100 / files.length,
                    message: `${compiled + 1}/${files.length} - ${path.basename(file.fsPath)}`
                });
                
                const result = await this.compileFile(file);
                if (result.success) {
                    compiled++;
                } else {
                    errors++;
                }
            }
            
            const message = errors > 0
                ? `Compiled ${compiled} files with ${errors} errors`
                : `Successfully compiled ${compiled} files`;
            
            vscode.window.showInformationMessage(message);
        });
    }
    
    private async runCompiler(args: string[]): Promise<CompilerResult> {
        const config = vscode.workspace.getConfiguration('chtl');
        const javaPath = config.get<string>('compiler.javaPath', 'java');
        
        const fullArgs = ['-jar', this.compilerPath, ...args];
        
        return new Promise((resolve) => {
            const process = child_process.spawn(javaPath, fullArgs);
            
            let stdout = '';
            let stderr = '';
            
            process.stdout.on('data', (data) => {
                stdout += data.toString();
            });
            
            process.stderr.on('data', (data) => {
                stderr += data.toString();
            });
            
            process.on('close', (code) => {
                if (code === 0) {
                    resolve({
                        success: true,
                        output: stdout,
                        errors: [],
                        warnings: []
                    });
                } else {
                    // Parse errors from stderr or stdout
                    const errors = this.parseCompilerOutput(stderr || stdout);
                    resolve({
                        success: false,
                        errors: errors.filter(e => e.type === 'error'),
                        warnings: errors.filter(e => e.type === 'warning')
                    });
                }
            });
            
            process.on('error', (error) => {
                resolve({
                    success: false,
                    errors: [{
                        type: 'error',
                        message: `Failed to run compiler: ${error.message}`
                    }],
                    warnings: []
                });
            });
        });
    }
    
    private parseValidationResult(result: CompilerResult): CompilerError[] {
        if (result.output) {
            try {
                // Try to parse JSON output
                const data = JSON.parse(result.output);
                if (data.errors && Array.isArray(data.errors)) {
                    return data.errors;
                }
            } catch {}
        }
        
        return [...result.errors, ...result.warnings];
    }
    
    private parseCompilerOutput(output: string): CompilerError[] {
        const errors: CompilerError[] = [];
        const lines = output.split('\n');
        
        for (const line of lines) {
            // Parse error format: filename:line:column: error: message
            const errorMatch = line.match(/^(.+):(\d+):(\d+):\s*(error|warning):\s*(.+)$/);
            if (errorMatch) {
                errors.push({
                    type: errorMatch[4] as 'error' | 'warning',
                    message: errorMatch[5],
                    line: parseInt(errorMatch[2]),
                    column: parseInt(errorMatch[3])
                });
            }
            
            // Parse simple error format: Error: message
            const simpleMatch = line.match(/^(Error|Warning):\s*(.+)$/i);
            if (simpleMatch) {
                errors.push({
                    type: simpleMatch[1].toLowerCase() as 'error' | 'warning',
                    message: simpleMatch[2]
                });
            }
        }
        
        // If no specific errors found, treat entire output as error
        if (errors.length === 0 && output.trim()) {
            errors.push({
                type: 'error',
                message: output.trim()
            });
        }
        
        return errors;
    }
    
    private showErrors(errors: CompilerError[]) {
        for (const error of errors) {
            const location = error.line ? ` at line ${error.line}` : '';
            this.outputChannel.appendLine(`${error.type.toUpperCase()}: ${error.message}${location}`);
        }
    }
}