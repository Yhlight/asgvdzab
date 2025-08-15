import * as vscode from 'vscode';
import { CHTLCompilerService, CompilerError } from '../services/compilerService';

export class CHTLDiagnosticProvider {
    private diagnosticCollection: vscode.DiagnosticCollection;
    private compilerService: CHTLCompilerService;
    private documentVersions: Map<string, number> = new Map();
    private pendingValidation: Map<string, NodeJS.Timeout> = new Map();
    
    constructor(compilerService: CHTLCompilerService) {
        this.compilerService = compilerService;
        this.diagnosticCollection = vscode.languages.createDiagnosticCollection('chtl');
    }
    
    public activate(subscriptions: vscode.Disposable[]) {
        // Validate all open documents
        vscode.workspace.textDocuments.forEach(doc => {
            if (doc.languageId === 'chtl') {
                this.validateDocument(doc);
            }
        });
        
        // Validate on open
        subscriptions.push(
            vscode.workspace.onDidOpenTextDocument(doc => {
                if (doc.languageId === 'chtl') {
                    this.validateDocument(doc);
                }
            })
        );
        
        // Validate on change
        subscriptions.push(
            vscode.workspace.onDidChangeTextDocument(event => {
                if (event.document.languageId === 'chtl') {
                    this.triggerValidation(event.document);
                }
            })
        );
        
        // Clear diagnostics on close
        subscriptions.push(
            vscode.workspace.onDidCloseTextDocument(doc => {
                this.diagnosticCollection.delete(doc.uri);
                this.documentVersions.delete(doc.uri.toString());
            })
        );
        
        subscriptions.push(this.diagnosticCollection);
    }
    
    public updateDiagnostics(uri: vscode.Uri) {
        const document = vscode.workspace.textDocuments.find(doc => doc.uri.toString() === uri.toString());
        if (document && document.languageId === 'chtl') {
            this.validateDocument(document);
        }
    }
    
    public deleteDiagnostics(uri: vscode.Uri) {
        this.diagnosticCollection.delete(uri);
        this.documentVersions.delete(uri.toString());
    }
    
    private triggerValidation(document: vscode.TextDocument) {
        const uri = document.uri.toString();
        
        // Cancel pending validation
        const pending = this.pendingValidation.get(uri);
        if (pending) {
            clearTimeout(pending);
        }
        
        // Get delay from configuration
        const config = vscode.workspace.getConfiguration('chtl');
        const delay = config.get<number>('diagnostics.delay', 500);
        
        // Schedule new validation
        const timeout = setTimeout(() => {
            this.pendingValidation.delete(uri);
            this.validateDocument(document);
        }, delay);
        
        this.pendingValidation.set(uri, timeout);
    }
    
    private async validateDocument(document: vscode.TextDocument) {
        if (!this.isEnabled()) {
            return;
        }
        
        const version = document.version;
        const uri = document.uri.toString();
        
        // Store document version
        this.documentVersions.set(uri, version);
        
        try {
            // Run validation through compiler service
            const errors = await this.compilerService.validateDocument(document);
            
            // Check if document version is still current
            if (this.documentVersions.get(uri) !== version) {
                return;
            }
            
            // Convert compiler errors to diagnostics
            const diagnostics = this.convertToDiagnostics(errors, document);
            
            // Update diagnostics
            this.diagnosticCollection.set(document.uri, diagnostics);
            
        } catch (error) {
            console.error('Error validating CHTL document:', error);
        }
    }
    
    private convertToDiagnostics(errors: CompilerError[], document: vscode.TextDocument): vscode.Diagnostic[] {
        return errors.map(error => {
            const range = this.getErrorRange(error, document);
            const severity = this.getSeverity(error.type);
            
            const diagnostic = new vscode.Diagnostic(range, error.message, severity);
            diagnostic.source = 'chtl';
            diagnostic.code = error.code;
            
            // Add related information if available
            if (error.relatedInfo) {
                diagnostic.relatedInformation = error.relatedInfo.map(info => {
                    return new vscode.DiagnosticRelatedInformation(
                        new vscode.Location(document.uri, this.getErrorRange(info, document)),
                        info.message
                    );
                });
            }
            
            return diagnostic;
        });
    }
    
    private getErrorRange(error: CompilerError, document: vscode.TextDocument): vscode.Range {
        // If we have specific position information
        if (error.line !== undefined && error.column !== undefined) {
            const line = Math.max(0, error.line - 1);
            const column = Math.max(0, error.column - 1);
            
            // Try to find the end of the error token
            let endColumn = column;
            if (error.length) {
                endColumn = column + error.length;
            } else {
                // Try to find word boundary
                const lineText = document.lineAt(line).text;
                const wordMatch = /\w+/.exec(lineText.substring(column));
                if (wordMatch) {
                    endColumn = column + wordMatch[0].length;
                } else {
                    endColumn = column + 1;
                }
            }
            
            return new vscode.Range(
                new vscode.Position(line, column),
                new vscode.Position(line, endColumn)
            );
        }
        
        // Default to first line
        return new vscode.Range(0, 0, 0, 1);
    }
    
    private getSeverity(type: string): vscode.DiagnosticSeverity {
        switch (type) {
            case 'error':
                return vscode.DiagnosticSeverity.Error;
            case 'warning':
                return vscode.DiagnosticSeverity.Warning;
            case 'info':
                return vscode.DiagnosticSeverity.Information;
            case 'hint':
                return vscode.DiagnosticSeverity.Hint;
            default:
                return vscode.DiagnosticSeverity.Error;
        }
    }
    
    private isEnabled(): boolean {
        const config = vscode.workspace.getConfiguration('chtl');
        return config.get<boolean>('diagnostics.enable', true);
    }
}