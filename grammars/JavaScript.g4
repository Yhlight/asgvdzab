/*
 * Simplified JavaScript ANTLR Grammar
 * Compatible with ANTLR4 and supports basic JavaScript syntax
 */

grammar JavaScript;

// Parser Rules

program
    : sourceElements? EOF
    ;

sourceElements
    : sourceElement+
    ;

sourceElement
    : statement
    | functionDeclaration
    ;

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
    | tryStatement
    ;

block
    : LBRACE statementList? RBRACE
    ;

statementList
    : statement+
    ;

variableStatement
    : VAR variableDeclarationList SEMICOLON
    | LET variableDeclarationList SEMICOLON
    | CONST variableDeclarationList SEMICOLON
    ;

variableDeclarationList
    : variableDeclaration (COMMA variableDeclaration)*
    ;

variableDeclaration
    : Identifier (ASSIGN assignmentExpression)?
    ;

emptyStatement
    : SEMICOLON
    ;

expressionStatement
    : expression SEMICOLON
    ;

ifStatement
    : IF LPAREN expression RPAREN statement (ELSE statement)?
    ;

iterationStatement
    : WHILE LPAREN expression RPAREN statement                                          # WhileStatement
    | FOR LPAREN forInit? SEMICOLON expression? SEMICOLON expression? RPAREN statement # ForStatement
    ;

forInit
    : variableDeclarationList
    | expression
    ;

continueStatement
    : CONTINUE Identifier? SEMICOLON
    ;

breakStatement
    : BREAK Identifier? SEMICOLON
    ;

returnStatement
    : RETURN expression? SEMICOLON
    ;

tryStatement
    : TRY block (catchClause finallyClause? | finallyClause)
    ;

catchClause
    : CATCH LPAREN Identifier RPAREN block
    ;

finallyClause
    : FINALLY block
    ;

functionDeclaration
    : FUNCTION Identifier LPAREN formalParameterList? RPAREN LBRACE functionBody RBRACE
    ;

formalParameterList
    : Identifier (COMMA Identifier)*
    ;

functionBody
    : sourceElements?
    ;

expression
    : assignmentExpression
    ;

assignmentExpression
    : conditionalExpression
    | leftHandSideExpression assignmentOperator assignmentExpression
    ;

assignmentOperator
    : ASSIGN | MUL_ASSIGN | DIV_ASSIGN | MOD_ASSIGN | PLUS_ASSIGN | MINUS_ASSIGN
    ;

conditionalExpression
    : logicalORExpression (QUESTION assignmentExpression COLON assignmentExpression)?
    ;

logicalORExpression
    : logicalANDExpression (OR logicalANDExpression)*
    ;

logicalANDExpression
    : bitwiseORExpression (AND bitwiseORExpression)*
    ;

bitwiseORExpression
    : bitwiseXORExpression (BIT_OR bitwiseXORExpression)*
    ;

bitwiseXORExpression
    : bitwiseANDExpression (BIT_XOR bitwiseANDExpression)*
    ;

bitwiseANDExpression
    : equalityExpression (BIT_AND equalityExpression)*
    ;

equalityExpression
    : relationalExpression ((EQUALS | NOT_EQUALS | STRICT_EQUALS | STRICT_NOT_EQUALS) relationalExpression)*
    ;

relationalExpression
    : shiftExpression ((LT | GT | LE | GE | INSTANCEOF | IN) shiftExpression)*
    ;

shiftExpression
    : additiveExpression ((LEFT_SHIFT | RIGHT_SHIFT | UNSIGNED_RIGHT_SHIFT) additiveExpression)*
    ;

additiveExpression
    : multiplicativeExpression ((PLUS | MINUS) multiplicativeExpression)*
    ;

multiplicativeExpression
    : unaryExpression ((MULTIPLY | DIVIDE | MODULUS) unaryExpression)*
    ;

unaryExpression
    : postfixExpression
    | (DELETE | VOID | TYPEOF | INCREMENT | DECREMENT | PLUS | MINUS | BIT_NOT | NOT) unaryExpression
    ;

postfixExpression
    : leftHandSideExpression (INCREMENT | DECREMENT)?
    ;

leftHandSideExpression
    : callExpression
    | memberExpression
    ;

callExpression
    : memberExpression arguments
    | callExpression arguments
    | callExpression LBRACKET expression RBRACKET
    | callExpression DOT Identifier
    ;

memberExpression
    : primaryExpression
    | functionExpression
    | memberExpression LBRACKET expression RBRACKET
    | memberExpression DOT Identifier
    | NEW memberExpression arguments
    ;

primaryExpression
    : THIS
    | Identifier
    | literal
    | arrayLiteral
    | objectLiteral
    | LPAREN expression RPAREN
    ;

arrayLiteral
    : LBRACKET elementList? RBRACKET
    ;

elementList
    : assignmentExpression (COMMA assignmentExpression)*
    ;

objectLiteral
    : LBRACE propertyNameAndValueList? RBRACE
    ;

propertyNameAndValueList
    : propertyAssignment (COMMA propertyAssignment)*
    ;

propertyAssignment
    : propertyName COLON assignmentExpression
    ;

propertyName
    : Identifier
    | StringLiteral
    | NumericLiteral
    ;

arguments
    : LPAREN argumentList? RPAREN
    ;

argumentList
    : assignmentExpression (COMMA assignmentExpression)*
    ;

functionExpression
    : FUNCTION Identifier? LPAREN formalParameterList? RPAREN LBRACE functionBody RBRACE
    ;

literal
    : NullLiteral
    | BooleanLiteral
    | NumericLiteral
    | StringLiteral
    ;

// Lexer Rules

// Keywords
BREAK: 'break';
CASE: 'case';
CATCH: 'catch';
CONTINUE: 'continue';
DEFAULT: 'default';
DELETE: 'delete';
DO: 'do';
ELSE: 'else';
FINALLY: 'finally';
FOR: 'for';
FUNCTION: 'function';
IF: 'if';
IN: 'in';
INSTANCEOF: 'instanceof';
NEW: 'new';
RETURN: 'return';
SWITCH: 'switch';
THIS: 'this';
THROW: 'throw';
TRY: 'try';
TYPEOF: 'typeof';
VAR: 'var';
VOID: 'void';
WHILE: 'while';
WITH: 'with';
LET: 'let';
CONST: 'const';

// Operators
ASSIGN: '=';
MUL_ASSIGN: '*=';
DIV_ASSIGN: '/=';
MOD_ASSIGN: '%=';
PLUS_ASSIGN: '+=';
MINUS_ASSIGN: '-=';

QUESTION: '?';
COLON: ':';
OR: '||';
AND: '&&';
BIT_OR: '|';
BIT_XOR: '^';
BIT_AND: '&';
EQUALS: '==';
NOT_EQUALS: '!=';
STRICT_EQUALS: '===';
STRICT_NOT_EQUALS: '!==';
LT: '<';
GT: '>';
LE: '<=';
GE: '>=';
LEFT_SHIFT: '<<';
RIGHT_SHIFT: '>>';
UNSIGNED_RIGHT_SHIFT: '>>>';
PLUS: '+';
MINUS: '-';
MULTIPLY: '*';
DIVIDE: '/';
MODULUS: '%';
INCREMENT: '++';
DECREMENT: '--';
BIT_NOT: '~';
NOT: '!';

// Punctuation
LBRACE: '{';
RBRACE: '}';
LPAREN: '(';
RPAREN: ')';
LBRACKET: '[';
RBRACKET: ']';
SEMICOLON: ';';
COMMA: ',';
DOT: '.';

// Literals
NullLiteral: 'null';
BooleanLiteral: 'true' | 'false';

NumericLiteral
    : DecimalLiteral
    | HexIntegerLiteral
    ;

fragment
DecimalLiteral
    : DecimalIntegerLiteral '.' DecimalDigits? ExponentPart?
    | '.' DecimalDigits ExponentPart?
    | DecimalIntegerLiteral ExponentPart?
    ;

fragment
DecimalIntegerLiteral
    : '0'
    | NonZeroDigit DecimalDigits?
    ;

fragment
DecimalDigits
    : DecimalDigit+
    ;

fragment
DecimalDigit
    : [0-9]
    ;

fragment
NonZeroDigit
    : [1-9]
    ;

fragment
ExponentPart
    : ExponentIndicator SignedInteger
    ;

fragment
ExponentIndicator
    : [eE]
    ;

fragment
SignedInteger
    : [+-]? DecimalDigits
    ;

fragment
HexIntegerLiteral
    : '0' [xX] HexDigit+
    ;

fragment
HexDigit
    : [0-9a-fA-F]
    ;

StringLiteral
    : '"' DoubleStringCharacters? '"'
    | '\'' SingleStringCharacters? '\''
    ;

fragment
DoubleStringCharacters
    : DoubleStringCharacter+
    ;

fragment
SingleStringCharacters
    : SingleStringCharacter+
    ;

fragment
DoubleStringCharacter
    : ~["\\\r\n]
    | EscapeSequence
    ;

fragment
SingleStringCharacter
    : ~['\\\r\n]
    | EscapeSequence
    ;

fragment
EscapeSequence
    : '\\' EscapeCharacter
    ;

fragment
EscapeCharacter
    : SingleEscapeCharacter
    | DecimalDigit
    | HexEscapeSequence
    | UnicodeEscapeSequence
    ;

fragment
SingleEscapeCharacter
    : ['"\\bfnrtv]
    ;

fragment
HexEscapeSequence
    : 'x' HexDigit HexDigit
    ;

fragment
UnicodeEscapeSequence
    : 'u' HexDigit HexDigit HexDigit HexDigit
    ;

Identifier
    : IdentifierStart IdentifierPart*
    ;

fragment
IdentifierStart
    : [a-zA-Z$_]
    ;

fragment
IdentifierPart
    : IdentifierStart
    | DecimalDigit
    ;

// Whitespace and Comments
WS: [ \t\r\n\u000C]+ -> skip;
COMMENT: '/*' .*? '*/' -> skip;
LINE_COMMENT: '//' ~[\r\n]* -> skip;