grammar CSS3;

// CSS3 Grammar with CHTL Support

stylesheet
    : ws* styleRule* ws* EOF
    ;

styleRule
    : ruleSet
    | atRule
    | chtlTemplateUsage
    | chtlCustomUsage
    | chtlVariableUsage
    | chtlOriginEmbed
    ;

ruleSet
    : selectorList ws* '{' ws* declarationList? ws* '}'
    ;

selectorList
    : selector (ws* ',' ws* selector)*
    ;

selector
    : simpleSelector (combinator simpleSelector)*
    ;

combinator
    : ws* ('+' | '>' | '~' | '||') ws*
    | ws+
    ;

simpleSelector
    : typeSelector qualifiers*
    | qualifiers+
    ;

typeSelector
    : tagName
    | '*'
    ;

tagName
    : ident
    ;

qualifiers
    : hashSelector
    | classSelector
    | attributeSelector
    | pseudoClass
    | pseudoElement
    ;

hashSelector
    : '#' ident
    ;

classSelector
    : '.' ident
    ;

attributeSelector
    : '[' ws* ident ws* (attributeOperator ws* attributeValue ws*)? ']'
    ;

attributeOperator
    : '='
    | '~='
    | '|='
    | '^='
    | '$='
    | '*='
    ;

attributeValue
    : ident
    | string
    ;

pseudoClass
    : ':' ident ('(' ws* expression ws* ')')?
    ;

pseudoElement
    : '::' ident
    ;

declarationList
    : declaration (ws* ';' ws* declaration)* ws* ';'?
    ;

declaration
    : property ws* ':' ws* valueList prio?
    | chtlStyleGroupUsage
    | chtlDeleteProperty
    | chtlInherit
    ;

property
    : ident
    | var
    ;

valueList
    : value (ws+ value)*
    ;

value
    : number unit?
    | percentage
    | dimension
    | string
    | ident
    | uri
    | hexcolor
    | calc
    | var
    | chtlVariableReference
    | function_
    ;

calc
    : 'calc(' ws* expression ws* ')'
    ;

function_
    : ident '(' ws* expressionList? ws* ')'
    ;

expressionList
    : expression (ws* ',' ws* expression)*
    ;

expression
    : term (('+' | '-') ws* term)*
    ;

term
    : factor (('*' | '/') ws* factor)*
    ;

factor
    : number unit?
    | percentage
    | dimension
    | '(' ws* expression ws* ')'
    | value
    ;

prio
    : ws* '!' ws* 'important' ws*
    ;

// CHTL Extensions
chtlTemplateUsage
    : '@' ('Style' | 'style' | 'CSS' | 'Css' | 'css') ws+ ident ws* chtlSpecialization? ws* ';'
    ;

chtlCustomUsage
    : '@' ('Style' | 'style' | 'CSS' | 'Css' | 'css') ws+ ident ws* chtlSpecialization? ws* ';'
    ;

chtlSpecialization
    : '{' ws* declarationList? ws* '}'
    ;

chtlVariableUsage
    : ident '(' ident ')' ws* ';'
    ;

chtlVariableReference
    : ident '(' ident ')'
    ;

chtlStyleGroupUsage
    : '@' ('Style' | 'style' | 'CSS' | 'Css' | 'css') ws+ ident ws* chtlSpecialization?
    ;

chtlDeleteProperty
    : 'delete' ws+ property ws*
    ;

chtlInherit
    : 'inherit' ws+ '@' ('Style' | 'style' | 'CSS' | 'Css' | 'css') ws+ ident ws*
    ;

chtlOriginEmbed
    : '[Origin]' ws* '@' ('Style' | 'style' | 'CSS' | 'Css' | 'css') ws+ ident ws* ';'
    ;

// At-rules
atRule
    : '@' ident ws* (atRuleBlock | ';')
    ;

atRuleBlock
    : '{' ws* (styleRule | declaration)* ws* '}'
    | valueList '{' ws* styleRule* ws* '}'
    ;

// Tokens
var
    : 'var(' ws* '--' ident ws* (',' ws* value)? ws* ')'
    ;

hexcolor
    : '#' hexdigit+
    ;

number
    : NUMBER
    ;

percentage
    : number '%'
    ;

dimension
    : number unit
    ;

unit
    : '%'
    | 'em' | 'ex' | 'ch' | 'rem' 
    | 'vw' | 'vh' | 'vmin' | 'vmax'
    | 'cm' | 'mm' | 'in' | 'px' | 'pt' | 'pc'
    | 'deg' | 'rad' | 'grad' | 'turn'
    | 's' | 'ms'
    | 'Hz' | 'kHz'
    | 'dpi' | 'dpcm' | 'dppx'
    ;

uri
    : 'url(' ws* (string | URL_CONTENT) ws* ')'
    ;

string
    : STRING
    ;

ident
    : IDENT
    ;

hexdigit
    : HEXDIGIT
    ;

ws
    : WS
    ;

// Lexer rules
NUMBER
    : [0-9]+ ('.' [0-9]+)?
    | '.' [0-9]+
    ;

STRING
    : '"' (~["\n\r])* '"'
    | '\'' (~['\n\r])* '\''
    ;

IDENT
    : '-'? NMSTART NMCHAR*
    ;

fragment NMSTART
    : [a-zA-Z_]
    | NONASCII
    | ESCAPE
    ;

fragment NMCHAR
    : [a-zA-Z0-9_-]
    | NONASCII
    | ESCAPE
    ;

HEXDIGIT
    : [0-9a-fA-F]
    ;

WS
    : [ \t\r\n\f]+
    ;

NONASCII
    : ~[\u0000-\u007F]
    ;

ESCAPE
    : '\\' (~[\r\n\f] | HEXDIGIT HEXDIGIT? HEXDIGIT? HEXDIGIT? HEXDIGIT? HEXDIGIT? WS?)
    ;

URL_CONTENT
    : (~[)])+
    ;

COMMENT
    : '/*' .*? '*/' -> skip
    ;

LINE_COMMENT
    : '//' ~[\r\n]* -> skip
    ;