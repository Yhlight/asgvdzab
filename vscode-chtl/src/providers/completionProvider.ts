import * as vscode from 'vscode';

export class CHTLCompletionProvider implements vscode.CompletionItemProvider {
    
    provideCompletionItems(
        document: vscode.TextDocument,
        position: vscode.Position,
        token: vscode.CancellationToken,
        context: vscode.CompletionContext
    ): vscode.ProviderResult<vscode.CompletionItem[] | vscode.CompletionList> {
        
        const line = document.lineAt(position).text;
        const linePrefix = line.substring(0, position.character);
        
        // Template type completions
        if (linePrefix.endsWith('@')) {
            return this.getTemplateTypeCompletions();
        }
        
        // HTML element completions
        if (this.isElementContext(document, position)) {
            return this.getElementCompletions();
        }
        
        // Attribute completions
        if (this.isAttributeContext(document, position)) {
            return this.getAttributeCompletions();
        }
        
        // CSS property completions
        if (this.isStyleContext(document, position)) {
            return this.getCSSPropertyCompletions();
        }
        
        // CHTL JS completions
        if (linePrefix.includes('->')) {
            return this.getCHTLJSMethodCompletions();
        }
        
        // Keyword completions
        return this.getKeywordCompletions();
    }
    
    private getTemplateTypeCompletions(): vscode.CompletionItem[] {
        const items: vscode.CompletionItem[] = [];
        
        const types = ['Element', 'Style', 'Var', 'Html', 'Chtl', 'JavaScript'];
        
        types.forEach(type => {
            const item = new vscode.CompletionItem(type, vscode.CompletionItemKind.Keyword);
            item.insertText = type;
            item.documentation = new vscode.MarkdownString(`CHTL ${type} type`);
            items.push(item);
        });
        
        return items;
    }
    
    private getElementCompletions(): vscode.CompletionItem[] {
        const htmlElements = [
            'div', 'span', 'p', 'a', 'h1', 'h2', 'h3', 'h4', 'h5', 'h6',
            'ul', 'ol', 'li', 'table', 'tr', 'td', 'th', 'thead', 'tbody',
            'img', 'input', 'button', 'form', 'label', 'select', 'option',
            'textarea', 'header', 'footer', 'nav', 'main', 'section', 'article',
            'aside', 'figure', 'figcaption', 'video', 'audio', 'canvas'
        ];
        
        return htmlElements.map(elem => {
            const item = new vscode.CompletionItem(elem, vscode.CompletionItemKind.Property);
            item.insertText = new vscode.SnippetString(`${elem} {\n\t$0\n}`);
            item.documentation = new vscode.MarkdownString(`HTML \`<${elem}>\` element`);
            return item;
        });
    }
    
    private getAttributeCompletions(): vscode.CompletionItem[] {
        const attributes = [
            { name: 'id', snippet: 'id: "$1";' },
            { name: 'class', snippet: 'class: "$1";' },
            { name: 'style', snippet: 'style {\n\t$0\n}' },
            { name: 'text', snippet: 'text { $0 }' },
            { name: 'script', snippet: 'script {\n\t$0\n}' },
            { name: 'href', snippet: 'href: "$1";' },
            { name: 'src', snippet: 'src: "$1";' },
            { name: 'alt', snippet: 'alt: "$1";' },
            { name: 'type', snippet: 'type: "$1";' },
            { name: 'name', snippet: 'name: "$1";' },
            { name: 'value', snippet: 'value: "$1";' },
            { name: 'placeholder', snippet: 'placeholder: "$1";' }
        ];
        
        return attributes.map(attr => {
            const item = new vscode.CompletionItem(attr.name, vscode.CompletionItemKind.Property);
            item.insertText = new vscode.SnippetString(attr.snippet);
            item.documentation = new vscode.MarkdownString(`CHTL attribute: ${attr.name}`);
            return item;
        });
    }
    
    private getCSSPropertyCompletions(): vscode.CompletionItem[] {
        const properties = [
            { name: 'color', value: '#$1' },
            { name: 'background', value: '#$1' },
            { name: 'background-color', value: '#$1' },
            { name: 'font-size', value: '${1:16}px' },
            { name: 'font-family', value: '"$1"' },
            { name: 'font-weight', value: '${1|normal,bold,100,200,300,400,500,600,700,800,900|}' },
            { name: 'margin', value: '${1:0}' },
            { name: 'padding', value: '${1:0}' },
            { name: 'width', value: '${1:100}%' },
            { name: 'height', value: '${1:auto}' },
            { name: 'display', value: '${1|block,inline,inline-block,flex,grid,none|}' },
            { name: 'position', value: '${1|static,relative,absolute,fixed,sticky|}' },
            { name: 'border', value: '1px solid #$1' },
            { name: 'border-radius', value: '${1:4}px' },
            { name: 'box-shadow', value: '0 2px 4px rgba(0,0,0,0.1)' },
            { name: 'text-align', value: '${1|left,center,right,justify|}' },
            { name: 'flex-direction', value: '${1|row,column,row-reverse,column-reverse|}' },
            { name: 'justify-content', value: '${1|flex-start,center,flex-end,space-between,space-around|}' },
            { name: 'align-items', value: '${1|flex-start,center,flex-end,stretch,baseline|}' }
        ];
        
        return properties.map(prop => {
            const item = new vscode.CompletionItem(prop.name, vscode.CompletionItemKind.Property);
            item.insertText = new vscode.SnippetString(`${prop.name}: ${prop.value};`);
            item.documentation = new vscode.MarkdownString(`CSS property: ${prop.name}`);
            return item;
        });
    }
    
    private getCHTLJSMethodCompletions(): vscode.CompletionItem[] {
        const methods = [
            {
                name: 'listen',
                snippet: 'listen("${1:click}", (${2:e}) => {\n\t$0\n})',
                doc: 'Add event listener to element'
            },
            {
                name: 'delegate',
                snippet: 'delegate("${1:click}", "${2:.selector}", (${3:e}) => {\n\t$0\n})',
                doc: 'Add delegated event listener'
            },
            {
                name: 'animate',
                snippet: 'animate({\n\tduration: ${1:1000},\n\tfrom: { ${2:opacity}: ${3:0} },\n\tto: { ${2:opacity}: ${4:1} }\n})',
                doc: 'Animate element properties'
            }
        ];
        
        return methods.map(method => {
            const item = new vscode.CompletionItem(method.name, vscode.CompletionItemKind.Method);
            item.insertText = new vscode.SnippetString(method.snippet);
            item.documentation = new vscode.MarkdownString(method.doc);
            return item;
        });
    }
    
    private getKeywordCompletions(): vscode.CompletionItem[] {
        const keywords = [
            { name: '[Template]', snippet: '[Template] @${1|Element,Style,Var|} ${2:Name} {\n\t$0\n}', kind: vscode.CompletionItemKind.Keyword },
            { name: '[Custom]', snippet: '[Custom] @${1|Element,Style,Var|} ${2:Name} {\n\t$0\n}', kind: vscode.CompletionItemKind.Keyword },
            { name: '[Namespace]', snippet: '[Namespace] ${1:name} {\n\t$0\n}', kind: vscode.CompletionItemKind.Module },
            { name: '[Import]', snippet: '[Import] @${1|Chtl,Style,JavaScript|} from "${2:path}"${3: as ${4:alias}};', kind: vscode.CompletionItemKind.Keyword },
            { name: 'except', snippet: 'except ${1:constraint};', kind: vscode.CompletionItemKind.Keyword },
            { name: 'inherit', snippet: 'inherit ${1:BaseStyle};', kind: vscode.CompletionItemKind.Keyword },
            { name: 'from', snippet: 'from ${1:namespace}', kind: vscode.CompletionItemKind.Keyword },
            { name: 'as', snippet: 'as ${1:alias}', kind: vscode.CompletionItemKind.Keyword }
        ];
        
        return keywords.map(kw => {
            const item = new vscode.CompletionItem(kw.name, kw.kind);
            item.insertText = new vscode.SnippetString(kw.snippet);
            return item;
        });
    }
    
    private isElementContext(document: vscode.TextDocument, position: vscode.Position): boolean {
        const text = document.getText();
        const offset = document.offsetAt(position);
        
        // Simple heuristic: check if we're at the start of a line or after a closing brace
        const lineText = document.lineAt(position).text;
        const trimmed = lineText.substring(0, position.character).trim();
        
        return trimmed === '' || trimmed.endsWith('}');
    }
    
    private isAttributeContext(document: vscode.TextDocument, position: vscode.Position): boolean {
        // Check if we're inside an element block
        const text = document.getText();
        const offset = document.offsetAt(position);
        
        let braceCount = 0;
        for (let i = offset - 1; i >= 0; i--) {
            if (text[i] === '}') braceCount++;
            if (text[i] === '{') {
                braceCount--;
                if (braceCount < 0) {
                    // We're inside a block
                    return true;
                }
            }
        }
        
        return false;
    }
    
    private isStyleContext(document: vscode.TextDocument, position: vscode.Position): boolean {
        const text = document.getText();
        const offset = document.offsetAt(position);
        
        // Look backwards for 'style {'
        const searchText = text.substring(Math.max(0, offset - 100), offset);
        return searchText.includes('style {') && !searchText.includes('}');
    }
}