grammar CSS3;

// Parser Rules
stylesheet
    : ws? charsetRule? (importRule ws?)* (namespaceRule ws?)* (cdoCdc ws? | statement ws?)* EOF
    ;

charsetRule
    : '@charset' ws? STRING ws? ';'
    ;

importRule
    : '@import' ws? (STRING | url) ws? mediaQueryList? ';'
    ;

namespaceRule
    : '@namespace' ws? (namespacePrefix ws?)? (STRING | url) ws? ';'
    ;

namespacePrefix
    : IDENT
    ;

mediaQueryList
    : ws? mediaQuery (ws? ',' ws? mediaQuery)*
    ;

mediaQuery
    : (ONLY | NOT)? ws? mediaType ws? (AND ws? mediaExpression)*
    | mediaExpression (ws? AND ws? mediaExpression)*
    ;

mediaType
    : IDENT
    ;

mediaExpression
    : '(' ws? mediaFeature ws? (':' ws? expr)? ws? ')'
    ;

mediaFeature
    : IDENT
    ;

statement
    : ruleset
    | media
    | page
    | fontFace
    | keyframes
    | supports
    | viewport
    | counterStyle
    | fontFeatureValues
    | unknownRule
    ;

ruleset
    : selectors ws? '{' ws? declarationList? '}' ws?
    ;

selectors
    : selector (ws? ',' ws? selector)*
    ;

selector
    : simpleSelectorSequence ws? (combinator simpleSelectorSequence ws?)*
    ;

combinator
    : ws? ('+' | '>' | '~' | '//')  // // is for deep combinator
    | ws  // descendant combinator
    ;

simpleSelectorSequence
    : (typeSelector | universal) (HASH | classSelector | attrib | pseudo | negation)*
    | (HASH | classSelector | attrib | pseudo | negation)+
    ;

typeSelector
    : typeNamespacePrefix? elementName
    ;

typeNamespacePrefix
    : (namespacePrefix | '*')? '|'
    ;

elementName
    : IDENT
    ;

universal
    : typeNamespacePrefix? '*'
    ;

classSelector
    : '.' IDENT
    ;

attrib
    : '[' ws? typeNamespacePrefix? IDENT ws? (attribOperator ws? (IDENT | STRING) ws?)? ']'
    ;

attribOperator
    : '=' | '~=' | '|=' | '^=' | '$=' | '*='
    ;

pseudo
    : ':' ':'? (IDENT | functionalPseudo)
    ;

functionalPseudo
    : FUNCTION ws? expression ')'
    ;

expression
    : (term ws?)+
    ;

negation
    : ':not(' ws? negationArg ws? ')'
    ;

negationArg
    : typeSelector | universal | HASH | classSelector | attrib | pseudo
    ;

declarationList
    : (declaration ';' ws?)* declaration?
    ;

declaration
    : property ws? ':' ws? expr important?
    ;

property
    : IDENT
    ;

important
    : ws? '!' ws? 'important'
    ;

expr
    : term (operator? term)*
    ;

term
    : unaryOperator? (NUMBER | PERCENTAGE | LENGTH | EMS | EXS | ANGLE | TIME | FREQ | RESOLUTION)
    | STRING | IDENT | url | HASH | UNICODE_RANGE | INCLUDES | DASHMATCH
    | ':' IDENT  // for IE filter
    | FUNCTION ws? expr ')'
    ;

operator
    : '/' | ',' | ws
    ;

unaryOperator
    : '+' | '-'
    ;

url
    : 'url(' ws? (STRING | URL) ws? ')'
    ;

// Media rules
media
    : '@media' mediaQueryList '{' ws? groupRuleBody '}' ws?
    ;

// Page rules
page
    : '@page' ws? pseudoPage? '{' ws? declarationList? '}' ws?
    ;

pseudoPage
    : ':' IDENT
    ;

// Font face rules
fontFace
    : '@font-face' ws? '{' ws? declarationList? '}' ws?
    ;

// Keyframes rules
keyframes
    : '@' KEYFRAMES ws? IDENT ws? '{' ws? keyframesBlocks '}' ws?
    ;

keyframesBlocks
    : keyframeSelector ws? '{' ws? declarationList? '}' ws? (keyframeSelector ws? '{' ws? declarationList? '}' ws?)*
    ;

keyframeSelector
    : (FROM | TO | PERCENTAGE) (ws? ',' ws? (FROM | TO | PERCENTAGE))*
    ;

// Supports rules
supports
    : '@supports' ws? supportsCondition ws? '{' ws? groupRuleBody '}' ws?
    ;

supportsCondition
    : supportsNegation | supportsConjunction | supportsDisjunction | supportsConditionInParens
    ;

supportsConditionInParens
    : '(' ws? supportsCondition ws? ')' | supportsDeclarationCondition | generalEnclosed
    ;

supportsNegation
    : NOT ws supportsConditionInParens
    ;

supportsConjunction
    : supportsConditionInParens (ws AND ws supportsConditionInParens)+
    ;

supportsDisjunction
    : supportsConditionInParens (ws OR ws supportsConditionInParens)+
    ;

supportsDeclarationCondition
    : '(' ws? declaration ws? ')'
    ;

generalEnclosed
    : FUNCTION (any | generalEnclosed)* ')'
    | '(' (any | generalEnclosed)* ')'
    ;

any
    : IDENT | NUMBER | PERCENTAGE | LENGTH | EMS | EXS | ANGLE | TIME | FREQ | RESOLUTION
    | STRING | URL | HASH | UNICODE_RANGE | INCLUDES | DASHMATCH | ':' | FUNCTION
    | '(' any* ')' | '[' any* ']' | ws
    ;

// Viewport rules
viewport
    : '@viewport' ws? '{' ws? declarationList? '}' ws?
    ;

// Counter style rules
counterStyle
    : '@counter-style' ws? IDENT ws? '{' ws? declarationList? '}' ws?
    ;

// Font feature values
fontFeatureValues
    : '@font-feature-values' ws? fontFamilyNameList ws? '{' ws? fontFeatureValueBlock* '}' ws?
    ;

fontFamilyNameList
    : fontFamilyName (ws? ',' ws? fontFamilyName)*
    ;

fontFamilyName
    : STRING | (IDENT ws?)+
    ;

fontFeatureValueBlock
    : fontFeatureType ws? '{' ws? fontFeatureValueList? '}' ws?
    ;

fontFeatureType
    : '@stylistic' | '@historical-forms' | '@styleset' | '@character-variant'
    | '@swash' | '@ornaments' | '@annotation'
    ;

fontFeatureValueList
    : fontFeatureValue (ws? ';' ws? fontFeatureValue)* ';'?
    ;

fontFeatureValue
    : IDENT ws? ':' ws? NUMBER (ws? NUMBER)*
    ;

// Group rule body
groupRuleBody
    : (ruleset | media | page | fontFace | keyframes | supports | viewport 
      | counterStyle | fontFeatureValues | unknownRule)*
    ;

// Unknown rules
unknownRule
    : unknownAtRule
    ;

unknownAtRule
    : '@' IDENT (any | ';' | '{' unknownBlock '}')*
    ;

unknownBlock
    : (any | ';')*
    ;

// CDO/CDC
cdoCdc
    : CDO | CDC
    ;

// Whitespace
ws
    : (COMMENT | WS)+
    ;

// Lexer Rules
CDO : '<!--';
CDC : '-->';

INCLUDES : '~=';
DASHMATCH : '|=';

FROM : 'from';
TO : 'to';

KEYFRAMES
    : '@keyframes'
    | '@-webkit-keyframes'
    | '@-moz-keyframes'
    | '@-o-keyframes'
    ;

ONLY : 'only';
NOT : 'not';
AND : 'and';
OR : 'or';

HASH : '#' NAME;

STRING
    : '"' (~["\r\n\\] | '\\' .)* '"'
    | '\'' (~['\r\n\\] | '\\' .)* '\''
    ;

NUMBER
    : [0-9]+ ('.' [0-9]+)?
    ;

PERCENTAGE : NUMBER '%';

LENGTH
    : NUMBER ('px' | 'cm' | 'mm' | 'in' | 'pt' | 'pc' | 'ch' | 'rem' | 'vw' | 'vh' | 'vmin' | 'vmax')
    ;

EMS : NUMBER 'em';
EXS : NUMBER 'ex';

ANGLE
    : NUMBER ('deg' | 'rad' | 'grad' | 'turn')
    ;

TIME
    : NUMBER ('s' | 'ms')
    ;

FREQ
    : NUMBER ('hz' | 'khz')
    ;

RESOLUTION
    : NUMBER ('dpi' | 'dpcm' | 'dppx')
    ;

UNICODE_RANGE
    : 'u+' [0-9a-fA-F?]+ ('-' [0-9a-fA-F]+)?
    ;

IDENT
    : '-'? NMSTART NMCHAR*
    ;

FUNCTION
    : IDENT '('
    ;

URL
    : (~['")\r\n\t ])+
    ;

COMMENT
    : '/*' .*? '*/' -> skip
    ;

WS
    : [ \t\r\n]+ -> skip
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

fragment NONASCII
    : ~[\u0000-\u007F]
    ;

fragment ESCAPE
    : '\\' ([0-9a-fA-F]{1,6} ' '? | ~[\r\n\f0-9a-fA-F])
    ;

fragment NAME
    : NMCHAR+
    ;