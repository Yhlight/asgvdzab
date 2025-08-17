grammar ECMAScript;

// ECMAScript Grammar with CHTL JS Support

program
    : sourceElements? EOF
    ;

sourceElements
    : sourceElement+
    ;

sourceElement
    : statement
    | functionDeclaration
    | chtlJsElement
    ;

// CHTL JS Extensions
chtlJsElement
    : chtlEnhancedSelector
    | chtlArrowAccess
    | chtlListenFunction
    | chtlDelegateFunction
    | chtlAnimateFunction
    | chtlVirDeclaration
    | chtlVirAccess
    | chtlVariableUsage
    | chtlOriginEmbed
    ;

chtlEnhancedSelector
    : '{{' selectorExpression '}}'
    ;

selectorExpression
    : SELECTOR_CONTENT
    ;

chtlArrowAccess
    : primaryExpression '->' identifier
    ;

chtlListenFunction
    : 'listen' '(' arguments ')'
    ;

chtlDelegateFunction
    : 'delegate' '(' arguments ')'
    ;

chtlAnimateFunction
    : 'animate' '(' arguments ')'
    ;

chtlVirDeclaration
    : 'vir' identifier '=' objectLiteral
    ;

chtlVirAccess
    : identifier '.' identifier
    ;

chtlVariableUsage
    : identifier '(' identifier ')'
    ;

chtlOriginEmbed
    : '[Origin]' '@JavaScript' identifier ';'
    ;

// Statements
statement
    : block
    | variableStatement
    | emptyStatement
    | expressionStatement
    | ifStatement
    | iterationStatement
    | continueStatement
    | breakStatement
    | returnStatement
    | withStatement
    | switchStatement
    | throwStatement
    | tryStatement
    | debuggerStatement
    | labelledStatement
    ;

block
    : '{' statementList? '}'
    ;

statementList
    : statement+
    ;

variableStatement
    : varModifier variableDeclarationList eos
    ;

varModifier
    : 'var'
    | 'let'
    | 'const'
    ;

variableDeclarationList
    : variableDeclaration (',' variableDeclaration)*
    ;

variableDeclaration
    : identifier ('=' singleExpression)?
    ;

emptyStatement
    : ';'
    ;

expressionStatement
    : expressionSequence eos
    ;

ifStatement
    : 'if' '(' expressionSequence ')' statement ('else' statement)?
    ;

iterationStatement
    : 'do' statement 'while' '(' expressionSequence ')' eos
    | 'while' '(' expressionSequence ')' statement
    | 'for' '(' forControl ')' statement
    ;

forControl
    : expressionSequence? ';' expressionSequence? ';' expressionSequence?
    | 'var' variableDeclarationList ';' expressionSequence? ';' expressionSequence?
    | singleExpression 'in' expressionSequence
    | 'var' variableDeclaration 'in' expressionSequence
    | singleExpression 'of' expressionSequence
    | 'var' variableDeclaration 'of' expressionSequence
    ;

continueStatement
    : 'continue' identifier? eos
    ;

breakStatement
    : 'break' identifier? eos
    ;

returnStatement
    : 'return' expressionSequence? eos
    ;

withStatement
    : 'with' '(' expressionSequence ')' statement
    ;

switchStatement
    : 'switch' '(' expressionSequence ')' caseBlock
    ;

caseBlock
    : '{' caseClauses? '}'
    ;

caseClauses
    : caseClause+
    ;

caseClause
    : 'case' expressionSequence ':' statementList?
    | 'default' ':' statementList?
    ;

throwStatement
    : 'throw' expressionSequence eos
    ;

tryStatement
    : 'try' block (catchProduction finallyProduction? | finallyProduction)
    ;

catchProduction
    : 'catch' '(' identifier ')' block
    ;

finallyProduction
    : 'finally' block
    ;

debuggerStatement
    : 'debugger' eos
    ;

labelledStatement
    : identifier ':' statement
    ;

// Functions
functionDeclaration
    : 'function' identifier '(' formalParameterList? ')' '{' functionBody '}'
    ;

formalParameterList
    : identifier (',' identifier)*
    ;

functionBody
    : sourceElements?
    ;

// Expressions
expressionSequence
    : singleExpression (',' singleExpression)*
    ;

singleExpression
    : functionExpression
    | arrowFunctionExpression
    | singleExpression '[' expressionSequence ']'
    | singleExpression '.' identifier
    | singleExpression arguments
    | 'new' singleExpression arguments?
    | singleExpression '++'
    | singleExpression '--'
    | 'delete' singleExpression
    | 'void' singleExpression
    | 'typeof' singleExpression
    | '++' singleExpression
    | '--' singleExpression
    | '+' singleExpression
    | '-' singleExpression
    | '~' singleExpression
    | '!' singleExpression
    | singleExpression ('*' | '/' | '%') singleExpression
    | singleExpression ('+' | '-') singleExpression
    | singleExpression ('<<' | '>>' | '>>>') singleExpression
    | singleExpression ('<' | '>' | '<=' | '>=') singleExpression
    | singleExpression 'instanceof' singleExpression
    | singleExpression 'in' singleExpression
    | singleExpression ('==' | '!=' | '===' | '!==') singleExpression
    | singleExpression '&' singleExpression
    | singleExpression '^' singleExpression
    | singleExpression '|' singleExpression
    | singleExpression '&&' singleExpression
    | singleExpression '||' singleExpression
    | singleExpression '?' singleExpression ':' singleExpression
    | singleExpression '=' singleExpression
    | singleExpression assignmentOperator singleExpression
    | primaryExpression
    | chtlArrowAccess
    ;

assignmentOperator
    : '*='
    | '/='
    | '%='
    | '+='
    | '-='
    | '<<='
    | '>>='
    | '>>>='
    | '&='
    | '^='
    | '|='
    ;

primaryExpression
    : 'this'
    | identifier
    | literal
    | arrayLiteral
    | objectLiteral
    | '(' expressionSequence ')'
    | chtlEnhancedSelector
    | chtlListenFunction
    | chtlDelegateFunction
    | chtlAnimateFunction
    | chtlVirAccess
    | chtlVariableUsage
    ;

functionExpression
    : 'function' identifier? '(' formalParameterList? ')' '{' functionBody '}'
    ;

arrowFunctionExpression
    : arrowFunctionParameters '=>' arrowFunctionBody
    ;

arrowFunctionParameters
    : identifier
    | '(' formalParameterList? ')'
    ;

arrowFunctionBody
    : singleExpression
    | '{' functionBody '}'
    ;

arguments
    : '(' argumentList? ')'
    ;

argumentList
    : singleExpression (',' singleExpression)*
    ;

// Literals
literal
    : nullLiteral
    | booleanLiteral
    | stringLiteral
    | templateLiteral
    | regularExpressionLiteral
    | numericLiteral
    ;

nullLiteral
    : 'null'
    ;

booleanLiteral
    : 'true'
    | 'false'
    ;

stringLiteral
    : StringLiteral
    ;

templateLiteral
    : '`' templateStringAtom* '`'
    ;

templateStringAtom
    : TemplateStringAtom
    | '${' singleExpression '}'
    ;

regularExpressionLiteral
    : RegularExpressionLiteral
    ;

numericLiteral
    : DecimalLiteral
    | HexIntegerLiteral
    | OctalIntegerLiteral
    | BinaryIntegerLiteral
    ;

arrayLiteral
    : '[' elementList? ']'
    ;

elementList
    : ','* arrayElement? (','+ arrayElement)* ','*
    ;

arrayElement
    : singleExpression
    ;

objectLiteral
    : '{' propertyAssignment? (',' propertyAssignment)* ','? '}'
    ;

propertyAssignment
    : propertyName ':' singleExpression
    | '[' singleExpression ']' ':' singleExpression
    | identifier
    | getter
    | setter
    ;

propertyName
    : identifier
    | stringLiteral
    | numericLiteral
    ;

getter
    : 'get' propertyName '(' ')' '{' functionBody '}'
    ;

setter
    : 'set' propertyName '(' identifier ')' '{' functionBody '}'
    ;

// Identifiers
identifier
    : Identifier
    | 'var'
    | 'let'
    | 'const'
    | 'function'
    | 'if'
    | 'else'
    | 'for'
    | 'while'
    | 'do'
    | 'switch'
    | 'case'
    | 'default'
    | 'break'
    | 'continue'
    | 'return'
    | 'throw'
    | 'try'
    | 'catch'
    | 'finally'
    | 'new'
    | 'delete'
    | 'void'
    | 'typeof'
    | 'instanceof'
    | 'in'
    | 'of'
    | 'this'
    | 'with'
    | 'debugger'
    | 'null'
    | 'true'
    | 'false'
    | 'get'
    | 'set'
    ;

// End of statement
eos
    : ';'
    | EOF
    | {lineTerminatorAhead()}?
    ;

// Lexer Rules
StringLiteral
    : '"' DoubleStringCharacter* '"'
    | '\'' SingleStringCharacter* '\''
    ;

TemplateStringAtom
    : ~[`$]+
    ;

RegularExpressionLiteral
    : '/' RegularExpressionBody '/' RegularExpressionFlags
    ;

fragment RegularExpressionBody
    : RegularExpressionFirstChar RegularExpressionChar*
    ;

fragment RegularExpressionFirstChar
    : ~[\r\n\u2028\u2029*/\\[]
    | RegularExpressionBackslashSequence
    | '[' RegularExpressionClassChar* ']'
    ;

fragment RegularExpressionChar
    : ~[\r\n\u2028\u2029/\\[]
    | RegularExpressionBackslashSequence
    | '[' RegularExpressionClassChar* ']'
    ;

fragment RegularExpressionClassChar
    : ~[\r\n\u2028\u2029\]\\]
    | RegularExpressionBackslashSequence
    ;

fragment RegularExpressionBackslashSequence
    : '\\' ~[\r\n\u2028\u2029]
    ;

fragment RegularExpressionFlags
    : IdentifierPart*
    ;

DecimalLiteral
    : DecimalIntegerLiteral '.' DecimalDigit* ExponentPart?
    | '.' DecimalDigit+ ExponentPart?
    | DecimalIntegerLiteral ExponentPart?
    ;

HexIntegerLiteral
    : '0' [xX] HexDigit+
    ;

OctalIntegerLiteral
    : '0' [oO] OctalDigit+
    ;

BinaryIntegerLiteral
    : '0' [bB] BinaryDigit+
    ;

fragment DecimalIntegerLiteral
    : '0'
    | [1-9] DecimalDigit*
    ;

fragment ExponentPart
    : [eE] [+-]? DecimalDigit+
    ;

fragment DecimalDigit
    : [0-9]
    ;

fragment HexDigit
    : [0-9a-fA-F]
    ;

fragment OctalDigit
    : [0-7]
    ;

fragment BinaryDigit
    : [01]
    ;

Identifier
    : IdentifierStart IdentifierPart*
    ;

fragment IdentifierStart
    : [a-zA-Z]
    | '_'
    | '$'
    | '\\' UnicodeEscapeSequence
    ;

fragment IdentifierPart
    : IdentifierStart
    | [0-9]
    ;

fragment UnicodeEscapeSequence
    : 'u' HexDigit HexDigit HexDigit HexDigit
    ;

fragment DoubleStringCharacter
    : ~["\\\r\n]
    | '\\' EscapeSequence
    | LineContinuation
    ;

fragment SingleStringCharacter
    : ~['\\\r\n]
    | '\\' EscapeSequence
    | LineContinuation
    ;

fragment EscapeSequence
    : CharacterEscapeSequence
    | '0'
    | HexEscapeSequence
    | UnicodeEscapeSequence
    ;

fragment CharacterEscapeSequence
    : SingleEscapeCharacter
    | NonEscapeCharacter
    ;

fragment SingleEscapeCharacter
    : ['"\\bfnrtv]
    ;

fragment NonEscapeCharacter
    : ~['"\\bfnrtv0-9xu\r\n]
    ;

fragment EscapeCharacter
    : SingleEscapeCharacter
    | [0-9]
    | [xu]
    ;

fragment HexEscapeSequence
    : 'x' HexDigit HexDigit
    ;

fragment LineContinuation
    : '\\' LineTerminatorSequence
    ;

fragment LineTerminatorSequence
    : '\r\n'
    | LineTerminator
    ;

fragment LineTerminator
    : [\r\n\u2028\u2029]
    ;

WhiteSpaces
    : [\t\u000B\u000C\u0020\u00A0]+ -> skip
    ;

LineTerminatorHidden
    : LineTerminatorSequence -> skip
    ;

MultiLineComment
    : '/*' .*? '*/' -> skip
    ;

SingleLineComment
    : '//' ~[\r\n\u2028\u2029]* -> skip
    ;

GeneratorComment
    : '--' ~[\r\n]* -> channel(HIDDEN)
    ;

SELECTOR_CONTENT
    : (~[{}])+
    ;