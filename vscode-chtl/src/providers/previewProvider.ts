import * as vscode from 'vscode';

export class CHTLPreviewProvider {
    public static currentPanel: vscode.WebviewPanel | undefined;
    
    private static readonly viewType = 'chtlPreview';
    
    public static createOrShow(extensionUri: vscode.Uri) {
        const column = vscode.window.activeTextEditor
            ? vscode.window.activeTextEditor.viewColumn
            : undefined;
        
        // If we already have a panel, show it
        if (CHTLPreviewProvider.currentPanel) {
            CHTLPreviewProvider.currentPanel.reveal(column);
            return;
        }
        
        // Otherwise, create a new panel
        const panel = vscode.window.createWebviewPanel(
            CHTLPreviewProvider.viewType,
            'CHTL Preview',
            column || vscode.ViewColumn.One,
            {
                enableScripts: true,
                localResourceRoots: [extensionUri]
            }
        );
        
        CHTLPreviewProvider.currentPanel = panel;
        
        // Set content
        CHTLPreviewProvider.updateContent();
        
        // Listen for disposal
        panel.onDidDispose(() => {
            CHTLPreviewProvider.currentPanel = undefined;
        });
        
        // Update on active editor change
        vscode.window.onDidChangeActiveTextEditor(() => {
            if (panel.visible) {
                CHTLPreviewProvider.updateContent();
            }
        });
    }
    
    private static updateContent() {
        if (!CHTLPreviewProvider.currentPanel) {
            return;
        }
        
        const panel = CHTLPreviewProvider.currentPanel;
        const webview = panel.webview;
        
        // Get compiled HTML content
        const htmlContent = CHTLPreviewProvider.getCompiledContent();
        
        webview.html = `<!DOCTYPE html>
            <html lang="en">
            <head>
                <meta charset="UTF-8">
                <meta name="viewport" content="width=device-width, initial-scale=1.0">
                <title>CHTL Preview</title>
                <style>
                    body {
                        font-family: Arial, sans-serif;
                        padding: 20px;
                        background: #f5f5f5;
                    }
                    .preview-container {
                        background: white;
                        border-radius: 8px;
                        box-shadow: 0 2px 8px rgba(0,0,0,0.1);
                        padding: 20px;
                        margin: 0 auto;
                        max-width: 1200px;
                    }
                    .preview-header {
                        border-bottom: 1px solid #e0e0e0;
                        padding-bottom: 10px;
                        margin-bottom: 20px;
                    }
                    .preview-header h1 {
                        margin: 0;
                        color: #333;
                        font-size: 18px;
                    }
                    .preview-content {
                        min-height: 300px;
                    }
                    .no-content {
                        color: #666;
                        text-align: center;
                        padding: 50px;
                    }
                </style>
            </head>
            <body>
                <div class="preview-container">
                    <div class="preview-header">
                        <h1>CHTL Preview</h1>
                    </div>
                    <div class="preview-content">
                        ${htmlContent || '<div class="no-content">No compiled output available. Compile your CHTL file first.</div>'}
                    </div>
                </div>
            </body>
            </html>`;
    }
    
    private static getCompiledContent(): string {
        // TODO: Get actual compiled content from the output file
        // For now, return placeholder
        return '';
    }
}