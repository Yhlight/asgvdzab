package com.chtl.compiler.namespace;

public class NamespaceDefinition {
    private String name;
    private String source;

    public NamespaceDefinition(String name, String source) {
        this.name = name;
        this.source = source;
    }

    public String getName() { return name; }
    public String getSource() { return source; }
}
