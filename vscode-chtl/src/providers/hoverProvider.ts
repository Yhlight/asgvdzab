import * as vscode from 'vscode';

export class CHTLHoverProvider implements vscode.HoverProvider {
    
    provideHover(
        document: vscode.TextDocument,
        position: vscode.Position,
        token: vscode.CancellationToken
    ): vscode.ProviderResult<vscode.Hover> {
        
        const range = document.getWordRangeAtPosition(position);
        if (!range) {
            return null;
        }
        
        const word = document.getText(range);
        const line = document.lineAt(position).text;
        
        // Check for CHTL keywords
        const keywordInfo = this.getKeywordInfo(word, line);
        if (keywordInfo) {
            return new vscode.Hover(keywordInfo, range);
        }
        
        // Check for HTML elements
        const elementInfo = this.getElementInfo(word);
        if (elementInfo) {
            return new vscode.Hover(elementInfo, range);
        }
        
        // Check for CSS properties
        const cssInfo = this.getCSSPropertyInfo(word);
        if (cssInfo) {
            return new vscode.Hover(cssInfo, range);
        }
        
        return null;
    }
    
    private getKeywordInfo(word: string, line: string): vscode.MarkdownString | null {
        const keywords: { [key: string]: string } = {
            'Template': '**[Template]** - Defines a reusable template\n\nUsage: `[Template] @Type Name { ... }`',
            'Custom': '**[Custom]** - Defines a custom element with empty values\n\nUsage: `[Custom] @Type Name { ... }`',
            'Namespace': '**[Namespace]** - Creates a namespace for organizing templates\n\nUsage: `[Namespace] name { ... }`',
            'Import': '**[Import]** - Imports external CHTL files or resources\n\nUsage: `[Import] @Type from "path" as alias;`',
            'except': '**except** - Defines constraints within a namespace\n\nUsage: `except [Template];` or `except @Html div;`',
            'inherit': '**inherit** - Inherits properties from another style\n\nUsage: `inherit BaseStyle;`',
            'from': '**from** - Specifies the source namespace or file\n\nUsage: `@Element Button from ui;`',
            'text': '**text** - Defines text content within an element\n\nUsage: `text { Content }`',
            'style': '**style** - Defines local styles for an element\n\nUsage: `style { property: value; }`',
            'script': '**script** - Embeds JavaScript code with CHTL JS enhancements\n\nUsage: `script { /* JS code */ }`'
        };
        
        if (keywords[word]) {
            const md = new vscode.MarkdownString(keywords[word]);
            md.isTrusted = true;
            return md;
        }
        
        // Check for @-prefixed types
        if (word.startsWith('@')) {
            const types: { [key: string]: string } = {
                '@Element': '**@Element** - References or defines an element template',
                '@Style': '**@Style** - References or defines a style template',
                '@Var': '**@Var** - References or defines a variable group',
                '@Html': '**@Html** - References HTML elements in constraints',
                '@Chtl': '**@Chtl** - Imports CHTL files',
                '@JavaScript': '**@JavaScript** - Imports JavaScript files'
            };
            
            if (types[word]) {
                const md = new vscode.MarkdownString(types[word]);
                md.isTrusted = true;
                return md;
            }
        }
        
        return null;
    }
    
    private getElementInfo(word: string): vscode.MarkdownString | null {
        const elements: { [key: string]: string } = {
            'div': '**<div>** - Generic container element\n\nCommonly used for layout and grouping',
            'span': '**<span>** - Inline container element\n\nUsed for styling inline content',
            'p': '**<p>** - Paragraph element\n\nRepresents a paragraph of text',
            'a': '**<a>** - Anchor/link element\n\nCreates hyperlinks\n\nRequired attribute: `href`',
            'img': '**<img>** - Image element\n\nEmbeds images\n\nRequired attributes: `src`, `alt`',
            'button': '**<button>** - Button element\n\nCreates clickable buttons',
            'input': '**<input>** - Input element\n\nCreates form inputs\n\nCommon attribute: `type`',
            'form': '**<form>** - Form element\n\nCreates an HTML form',
            'h1': '**<h1>** - Heading level 1\n\nMain page heading',
            'h2': '**<h2>** - Heading level 2\n\nSection heading',
            'ul': '**<ul>** - Unordered list\n\nCreates a bulleted list',
            'li': '**<li>** - List item\n\nDefines a list item'
        };
        
        if (elements[word]) {
            const md = new vscode.MarkdownString(elements[word]);
            md.isTrusted = true;
            return md;
        }
        
        return null;
    }
    
    private getCSSPropertyInfo(word: string): vscode.MarkdownString | null {
        const properties: { [key: string]: string } = {
            'color': '**color** - Sets text color\n\nValues: color names, hex, rgb, hsl',
            'background': '**background** - Sets background properties\n\nShorthand for background-color, image, etc.',
            'display': '**display** - Sets display type\n\nValues: `block`, `inline`, `flex`, `grid`, `none`',
            'position': '**position** - Sets positioning method\n\nValues: `static`, `relative`, `absolute`, `fixed`, `sticky`',
            'margin': '**margin** - Sets outer spacing\n\nValues: length, percentage, `auto`',
            'padding': '**padding** - Sets inner spacing\n\nValues: length, percentage',
            'width': '**width** - Sets element width\n\nValues: length, percentage, `auto`',
            'height': '**height** - Sets element height\n\nValues: length, percentage, `auto`',
            'flex': '**flex** - Flex container/item properties\n\nShorthand for flex-grow, flex-shrink, flex-basis',
            'border': '**border** - Sets border properties\n\nShorthand for border-width, border-style, border-color'
        };
        
        if (properties[word]) {
            const md = new vscode.MarkdownString(properties[word]);
            md.isTrusted = true;
            return md;
        }
        
        return null;
    }
}