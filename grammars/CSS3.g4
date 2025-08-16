/*
 * CSS3 ANTLR Grammar
 * Based on CSS3 specification
 */

grammar CSS3;

// Parser Rules

stylesheet
    : ( statement )*
    ;

statement
    : ruleset
    | atRule
    | comment
    ;

atRule
    : AT_RULE_NAME any* ( block | SEMICOLON )
    ;

block
    : LBRACE declaration* RBRACE
    ;

ruleset
    : selector ( COMMA selector )* LBRACE declaration* RBRACE
    ;

selector
    : simpleSelector ( combinator simpleSelector )*
    ;

combinator
    : PLUS
    | GREATER
    | TILDE
    | WS
    ;

simpleSelector
    : elementName
    | HASH
    | DOT IDENT
    | LBRACKET IDENT ( ( PREFIXMATCH | SUFFIXMATCH | SUBSTRINGMATCH | '=' | INCLUDES | DASHMATCH ) ( IDENT | STRING ) )? RBRACKET
    | COLON IDENT ( LPAREN ( IDENT | STRING | NUMBER ) RPAREN )?
    | COLON COLON IDENT
    ;

elementName
    : IDENT
    | '*'
    ;

declaration
    : property COLON expr prio? SEMICOLON?
    ;

prio
    : IMPORTANT_SYM
    ;

expr
    : term ( cssOperator term )*
    ;

term
    : unaryOperator?
    ( NUMBER ( IDENT | PERCENTAGE | LENGTH | EMS | EXS | ANGLE | TIME | FREQ )?
    | STRING
    | IDENT
    | URI
    | HASH
    | unicodeRange
    | hexcolor
    | function
    )
    ;

function
    : FUNCTION expr RPAREN
    ;

unicodeRange
    : 'U+' HEX_DIGIT ( HEX_DIGIT | '?' )*
    ;

hexcolor
    : HASH HEX_DIGIT HEX_DIGIT HEX_DIGIT ( HEX_DIGIT HEX_DIGIT HEX_DIGIT )?
    ;

cssOperator
    : '/' | ','
    ;

unaryOperator
    : '-' | '+'
    ;

property
    : IDENT
    ;

any
    : IDENT
    | NUMBER
    | PERCENTAGE
    | LENGTH
    | EMS
    | EXS
    | ANGLE
    | TIME
    | FREQ
    | STRING
    | URI
    | HASH
    | PLUS
    | GREATER
    | COMMA
    | TILDE
    | COLON
    | SEMICOLON
    | LBRACE
    | RBRACE
    | LPAREN
    | RPAREN
    | LBRACKET
    | RBRACKET
    | function
    | INCLUDES
    | DASHMATCH
    | PREFIXMATCH
    | SUFFIXMATCH
    | SUBSTRINGMATCH
    ;

comment
    : COMMENT
    ;

// Lexer Rules

COMMENT
    : '/*' .*? '*/'
    ;

WS
    : [ \t\r\n]+ -> skip
    ;

LBRACE : '{' ;
RBRACE : '}' ;
LPAREN : '(' ;
RPAREN : ')' ;
LBRACKET : '[' ;
RBRACKET : ']' ;
SEMICOLON : ';' ;
COLON : ':' ;
DOT : '.' ;
COMMA : ',' ;
PLUS : '+' ;
GREATER : '>' ;
TILDE : '~' ;

INCLUDES : '~=' ;
DASHMATCH : '|=' ;
PREFIXMATCH : '^=' ;
SUFFIXMATCH : '$=' ;
SUBSTRINGMATCH : '*=' ;

IMPORTANT_SYM : '!important' ;

AT_RULE_NAME
    : '@' IDENT
    ;

FUNCTION
    : IDENT '('
    ;

URI
    : 'url(' WS* STRING WS* ')'
    | 'url(' WS* URL WS* ')'
    ;

HASH
    : '#' NAME
    ;

NUMBER
    : [0-9]*'.'?[0-9]+
    ;

PERCENTAGE : NUMBER '%' ;
LENGTH : NUMBER ( 'px' | 'cm' | 'mm' | 'in' | 'pt' | 'pc' | 'em' | 'ex' | 'ch' | 'rem' | 'vw' | 'vh' | 'vmin' | 'vmax' ) ;
EMS : NUMBER 'em' ;
EXS : NUMBER 'ex' ;
ANGLE : NUMBER ( 'deg' | 'rad' | 'grad' | 'turn' ) ;
TIME : NUMBER ( 'ms' | 's' ) ;
FREQ : NUMBER ( 'Hz' | 'kHz' ) ;

STRING
    : '"' ( ~["\r\n\\] | ESCAPE )* '"'
    | '\'' ( ~['\r\n\\] | ESCAPE )* '\''
    ;

IDENT
    : '-'? NMSTART NMCHAR*
    ;

HEX_DIGIT
    : [0-9a-fA-F]
    ;

fragment
NAME
    : NMCHAR+
    ;

fragment
NMSTART
    : [a-zA-Z_]
    | NONASCII
    | ESCAPE
    ;

fragment
NMCHAR
    : [a-zA-Z0-9_-]
    | NONASCII
    | ESCAPE
    ;

fragment
NONASCII
    : ~[\u0000-\u007F]
    ;

fragment
ESCAPE
    : '\\' ( [0-9a-fA-F]+ WS? | ~[\r\n\f0-9a-fA-F] )
    ;

fragment
URL
    : ( [!#$%&*-~] | NONASCII | ESCAPE )*
    ;