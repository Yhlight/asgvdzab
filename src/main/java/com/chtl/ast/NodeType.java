package com.chtl.ast;

import com.chtl.ast.AbstractCHTLASTNode;
import com.chtl.ast.NodeType;

public class NodeType extends AbstractCHTLASTNode {
public enum NodeType {
    // Element types
    ELEMENT,
    TEXT,
    ATTRIBUTE,
    COMMENT,

    // Template types
    TEMPLATE_DEFINITION,
    TEMPLATE_USAGE,

    // Style types
    STYLE_BLOCK,
    STYLE_RULE,
    STYLE_PROPERTY,
    STYLE_SELECTOR,

    // Script types
    SCRIPT_BLOCK,

    // Custom types
    CUSTOM_DEFINITION,
    CUSTOM_USAGE,
    CUSTOM_OPERATION,
    SPECIALIZATION,

    // Origin types
    ORIGIN_BLOCK,

    // Import/Namespace types
    IMPORT_STATEMENT,
    NAMESPACE,

    // Configuration types
    CONFIGURATION,
    CONSTRAINT,

    // Variable types
    VAR_GROUP_DEFINITION,
    VAR_GROUP_USAGE,

    // Literal types
    STRING_LITERAL,
    NUMBER_LITERAL,
    UNQUOTED_LITERAL,

    // Other types
    ROOT,
    INHERIT_STATEMENT,

    // CHTL JS types
    CHTLJS_SELECTOR,
    CHTLJS_METHOD_CALL,
    CHTLJS_ANIMATE,
    CHTLJS_OBJECT,
    CHTLJS_ARRAY,
    CHTLJS_FUNCTION,
    CHTLJS_IDENTIFIER,
    ANIMATION_CONTROLLER,
    CHTLJS_EXPRESSION
}

}