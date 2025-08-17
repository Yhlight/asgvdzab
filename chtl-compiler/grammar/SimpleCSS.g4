grammar SimpleCSS;

// 简化的CSS语法，足够处理CHTL生成的CSS

stylesheet
    : statement* EOF
    ;

statement
    : ruleSet
    | atRule
    ;

ruleSet
    : selector (',' selector)* '{' declaration* '}'
    ;

selector
    : simpleSelector+
    ;

simpleSelector
    : elementName
    | '.' className
    | '#' idName
    | '*'
    | ':' pseudoClass
    | '::' pseudoElement
    | '[' attribute ']'
    ;

elementName
    : IDENT
    ;

className
    : IDENT
    ;

idName
    : IDENT
    ;

pseudoClass
    : IDENT ('(' expression ')')?
    ;

pseudoElement
    : IDENT
    ;

attribute
    : IDENT (attributeOperator (IDENT | STRING))?
    ;

attributeOperator
    : '=' | '~=' | '|=' | '^=' | '$=' | '*='
    ;

declaration
    : property ':' value+ ';'?
    ;

property
    : IDENT
    ;

value
    : term
    | value ',' value
    ;

term
    : NUMBER unit?
    | PERCENTAGE
    | COLOR
    | STRING
    | IDENT
    | URI
    | function
    ;

unit
    : 'px' | 'em' | 'rem' | '%' | 'vh' | 'vw' | 'pt' | 'cm' | 'mm' | 'in'
    ;

function
    : IDENT '(' expression ')'
    ;

expression
    : term+
    ;

atRule
    : '@' IDENT any* (block | ';')
    ;

block
    : '{' any* '}'
    ;

any
    : IDENT | NUMBER | STRING | COLOR | ',' | ':' | ';' | block
    ;

// Lexer rules
COMMENT
    : '/*' .*? '*/' -> skip
    ;

WS
    : [ \t\r\n]+ -> skip
    ;

COLOR
    : '#' [0-9a-fA-F]+
    ;

NUMBER
    : [0-9]+ ('.' [0-9]+)?
    ;

PERCENTAGE
    : NUMBER '%'
    ;

STRING
    : '"' (~["\r\n] | '""')* '"'
    | '\'' (~['\r\n] | '\'\'')* '\''
    ;

URI
    : 'url(' (~[)]+ | STRING) ')'
    ;

IDENT
    : [a-zA-Z_] [a-zA-Z0-9_-]*
    ;

// 捕获所有其他字符
ANY_CHAR
    : .
    ;