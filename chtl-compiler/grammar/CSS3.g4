grammar CSS3;

// Parser Rules
stylesheet
    : ws* charSet? (ws* imports)* (ws* namespace_)* (ws* nestedStatement)* ws* EOF
    ;

charSet
    : CharSet ws* String_ ws* ';'
    ;

imports
    : Import ws* (String_ | Uri) ws* mediaQueryList? ';' ws*
    ;

namespace_
    : Namespace ws* (namespacePrefix ws*)? (String_ | Uri) ws* ';' ws*
    ;

namespacePrefix
    : ident
    ;

mediaQueryList
    : (mediaQuery (ws* ',' ws* mediaQuery)*)?
    ;

mediaQuery
    : (MediaOnly | Not)? ws* mediaType ws* (And ws* mediaExpression)*
    | mediaExpression (ws* And ws* mediaExpression)*
    ;

mediaType
    : ident
    ;

mediaExpression
    : '(' ws* mediaFeature (ws* ':' ws* expr)? ws* ')'
    ;

mediaFeature
    : ident
    ;

nestedStatement
    : ruleset
    | media
    | page
    | fontFaceRule
    | keyframesRule
    | supportsRule
    | viewport
    | counterStyle
    | fontFeatureValuesRule
    | atRule
    ;

groupRuleBody
    : ws* '{' ws* nestedStatement* '}' ws*
    ;

ruleset
    : selectorGroup ws* '{' ws* declarationList? '}' ws*
    ;

selectorGroup
    : selector (ws* ',' ws* selector)*
    ;

selector
    : simpleSelectorSequence ws* (combinator simpleSelectorSequence ws*)*
    ;

combinator
    : Plus ws* | Greater ws* | Tilde ws* | ws+
    ;

simpleSelectorSequence
    : (typeSelector | universal) (Hash | className | attrib | pseudo | negation)*
    | (Hash | className | attrib | pseudo | negation)+
    ;

typeSelector
    : typeNamespacePrefix? elementName
    ;

typeNamespacePrefix
    : (ident | '*')? '|'
    ;

elementName
    : ident
    ;

universal
    : typeNamespacePrefix? '*'
    ;

className
    : '.' ident
    ;

attrib
    : '[' ws* typeNamespacePrefix? ident ws* (attribOperator ws* (ident | String_) ws*)? ']'
    ;

attribOperator
    : '=' | '~=' | '|=' | '^=' | '$=' | '*='
    ;

pseudo
    : ':' ':'? (ident | functionalPseudo)
    ;

functionalPseudo
    : Function_ ws* expression ')'
    ;

expression
    : (Plus | Minus | Dimension | UnknownDimension | Number | String_ | ident) ws*
    ;

negation
    : PseudoNot ws* negationArg ws* ')'
    ;

negationArg
    : typeSelector | universal | Hash | className | attrib | pseudo
    ;

declarationList
    : (declaration? ';' ws*)* declaration?
    ;

declaration
    : property ':' ws* expr prio?
    | property ':' ws* value
    ;

prio
    : Important ws*
    ;

value
    : (any | block | atKeyword ws*)+
    ;

expr
    : term (operator? term)*
    ;

term
    : number ws*
    | percentage ws*
    | dimension ws*
    | String_ ws*
    | Uri ws*
    | ident ws*
    | var
    | hexcolor
    | calc
    | function_
    ;

function_
    : Function_ ws* expr ')' ws*
    ;

var
    : Var ws* Variable ws* (ws* ',' ws* expr)? ')' ws*
    ;

calc
    : Calc ws* calcSum ')' ws*
    ;

calcSum
    : calcProduct (ws* (Plus | Minus) ws* calcProduct)*
    ;

calcProduct
    : calcValue (ws* ('*' | '/') ws* calcValue)*
    ;

calcValue
    : number ws* | dimension ws* | percentage ws* | '(' ws* calcSum ws* ')' | var | calc
    ;

operator
    : '/' ws* | ',' ws* | ws+
    ;

property
    : (ident | Variable) ws*
    ;

any
    : (ident | number | percentage | dimension | String_ | Uri | Hash | UnicodeRange | Includes | 
       DashMatch | ':' | Function_ ws* (any | unused)* ')' | '(' ws* (any | unused)* ')' | '[' ws* (any | unused)* ']'
      ) ws*
    ;

atRule
    : atKeyword ws* any* (block | ';' ws*)
    ;

unused
    : block | atKeyword ws* | ';' ws* | CDO ws* | CDC ws*
    ;

block
    : '{' ws* ((declarationList | nestedStatement | any | block | atKeyword ws* | ';' ws*) ws*)* '}' ws*
    ;

atKeyword
    : '@' ident
    ;

media
    : Media ws* mediaQueryList groupRuleBody
    ;

page
    : Page ws* pseudoPage? '{' ws* declaration? (';' ws* declaration?)* '}' ws*
    ;

pseudoPage
    : ':' ident ws*
    ;

fontFaceRule
    : FontFace ws* '{' ws* fontFaceDeclaration? (';' ws* fontFaceDeclaration?)* '}' ws*
    ;

fontFaceDeclaration
    : property ':' ws* expr
    ;

keyframesRule
    : Keyframes ws* ident ws* '{' ws* keyframeBlock* '}' ws*
    ;

keyframeBlock
    : keyframeSelector '{' ws* declarationList? '}' ws*
    ;

keyframeSelector
    : (From | To | percentage) (ws* ',' ws* (From | To | percentage))*
    ;

viewport
    : Viewport ws* '{' ws* declarationList? '}' ws*
    ;

counterStyle
    : CounterStyle ws* ident ws* '{' ws* declarationList? '}' ws*
    ;

fontFeatureValuesRule
    : FontFeatureValues ws* fontFamilyNameList ws* '{' ws* featureValueBlock* '}' ws*
    ;

fontFamilyNameList
    : fontFamilyName (ws* ',' ws* fontFamilyName)*
    ;

fontFamilyName
    : String_ | ident+
    ;

featureValueBlock
    : featureType ws* '{' ws* featureValueDefinition? (ws* ';' ws* featureValueDefinition?)* '}' ws*
    ;

featureType
    : atKeyword
    ;

featureValueDefinition
    : ident ws* ':' ws* number (ws+ number)*
    ;

supportsRule
    : Supports ws* supportsCondition ws* groupRuleBody
    ;

supportsCondition
    : supportsConditionInParens (ws+ supportsConjunction)*
    | supportsConditionInParens (ws+ supportsDisjunction)*
    ;

supportsConditionInParens
    : '(' ws* supportsCondition ws* ')'
    | supportsDeclarationCondition
    | generalEnclosed
    ;

supportsConjunction
    : And ws+ supportsConditionInParens
    ;

supportsDisjunction
    : Or ws+ supportsConditionInParens
    ;

supportsDeclarationCondition
    : '(' ws* declaration ws* ')'
    ;

generalEnclosed
    : Function_ (any | unused)* ')'
    | '(' (any | unused)* ')'
    ;

// Numbers and dimensions
number
    : (Plus | Minus)? (Number | Percentage)
    ;

percentage
    : (Plus | Minus)? Percentage
    ;

dimension
    : (Plus | Minus)? Dimension
    ;

// Basic units
ident
    : Ident
    | MediaOnly
    | Not
    | And
    | Or
    | From
    | To
    ;

hexcolor
    : Hash
    ;

// Whitespace handling
ws
    : (Space | Newline)+
    ;

// Lexer Rules
fragment Hex
    : [0-9a-fA-F]
    ;

fragment NonAscii
    : ~[\u0000-\u007F]
    ;

fragment Unicode
    : '\\' Hex Hex? Hex? Hex? Hex? Hex? ('\r\n' | [ \t\r\n\f])?
    ;

fragment Escape
    : Unicode | '\\' ~[\r\n\f0-9a-fA-F]
    ;

fragment Nmstart
    : [_a-zA-Z] | NonAscii | Escape
    ;

fragment Nmchar
    : [_a-zA-Z0-9-] | NonAscii | Escape
    ;

fragment Url
    : ([!#$%&*-~] | NonAscii | Escape)*
    ;

fragment Whitespace
    : Space | '\t' | '\r' | '\n' | '\f'
    ;

Comment
    : '/*' ~[*]* '*'+ (~[/*] ~[*]* '*'+)* '/' -> skip
    ;

CDO
    : '<!--'
    ;

CDC
    : '-->'
    ;

Includes
    : '~='
    ;

DashMatch
    : '|='
    ;

Hash
    : '#' Nmchar+
    ;

Import
    : '@' I M P O R T
    ;

Page
    : '@' P A G E
    ;

Media
    : '@' M E D I A
    ;

Namespace
    : '@' N A M E S P A C E
    ;

CharSet
    : '@charset'
    ;

Important
    : '!' (Space | Comment)* I M P O R T A N T
    ;

FontFace
    : '@' F O N T '-' F A C E
    ;

Supports
    : '@' S U P P O R T S
    ;

Or
    : O R
    ;

Keyframes
    : '@' K E Y F R A M E S
    | '@' '-' W E B K I T '-' K E Y F R A M E S
    | '@' '-' M O Z '-' K E Y F R A M E S
    | '@' '-' M S '-' K E Y F R A M E S
    ;

From
    : F R O M
    ;

To
    : T O
    ;

Calc
    : C A L C
    ;

Viewport
    : '@' V I E W P O R T
    ;

CounterStyle
    : '@' C O U N T E R '-' S T Y L E
    ;

FontFeatureValues
    : '@' F O N T '-' F E A T U R E '-' V A L U E S
    ;

Var
    : V A R
    ;

MediaOnly
    : O N L Y
    ;

Not
    : N O T
    ;

And
    : A N D
    ;

Dimension
    : Number (E M | E X | P X | C M | M M | I N | P T | P C | C H | R E M | V W | V H | V M I N | V M A X | '%')
    ;

UnknownDimension
    : Number Ident
    ;

Percentage
    : Number '%'
    ;

Number
    : [0-9]+ ('.' [0-9]+)?
    | '.' [0-9]+
    ;

Uri
    : U R L '(' Whitespace* (String_ | Url) Whitespace* ')'
    ;

String_
    : '"' (~["\\\r\n\f] | '\\' (Newline | ~[\r\n\f]))* '"'
    | '\'' (~['\\\r\n\f] | '\\' (Newline | ~[\r\n\f]))* '\''
    ;

Function_
    : Ident '('
    ;

UnicodeRange
    : 'u+' (Hex '?'* | Hex Hex '?'* | Hex Hex Hex '?'* | Hex Hex Hex Hex '?'* | Hex Hex Hex Hex Hex '?'* | Hex Hex Hex Hex Hex Hex '?'*)
    | 'u+' Hex+ '-' Hex+
    ;

Ident
    : Nmstart Nmchar*
    ;

Variable
    : '--' Nmchar+
    ;

Plus
    : '+'
    ;

Minus
    : '-'
    ;

Greater
    : '>'
    ;

Tilde
    : '~'
    ;

PseudoNot
    : ':' N O T '('
    ;

Space
    : [ \t]+
    ;

Newline
    : '\r\n' | '\n' | '\r' | '\f'
    ;

// Case-insensitive letters
fragment A : [aA];
fragment B : [bB];
fragment C : [cC];
fragment D : [dD];
fragment E : [eE];
fragment F : [fF];
fragment G : [gG];
fragment H : [hH];
fragment I : [iI];
fragment J : [jJ];
fragment K : [kK];
fragment L : [lL];
fragment M : [mM];
fragment N : [nN];
fragment O : [oO];
fragment P : [pP];
fragment Q : [qQ];
fragment R : [rR];
fragment S : [sS];
fragment T : [tT];
fragment U : [uU];
fragment V : [vV];
fragment W : [wW];
fragment X : [xX];
fragment Y : [yY];
fragment Z : [zZ];