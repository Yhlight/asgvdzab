grammar ECMAScript;

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
    | withStatement
    | labelledStatement
    | switchStatement
    | throwStatement
    | tryStatement
    | debuggerStatement
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

variableDeclarationList
    : variableDeclaration (',' variableDeclaration)*
    ;

variableDeclaration
    : (Identifier | bindingPattern) ('=' singleExpression)?
    ;

bindingPattern
    : '[' (Identifier (',' Identifier)*)? ']'  // Array pattern
    | '{' (propertyName ':' Identifier (',' propertyName ':' Identifier)*)? '}'  // Object pattern
    ;

emptyStatement
    : SemiColon
    ;

expressionStatement
    : {notOpenBraceAndNotFunction()}? expressionSequence eos
    ;

ifStatement
    : If '(' expressionSequence ')' statement (Else statement)?
    ;

iterationStatement
    : Do statement While '(' expressionSequence ')' eos                                                         # DoStatement
    | While '(' expressionSequence ')' statement                                                                # WhileStatement
    | For '(' (expressionSequence | variableDeclarationList)? ';' expressionSequence? ';' expressionSequence? ')' statement # ForStatement
    | For '(' (singleExpression | variableDeclarationList) In expressionSequence ')' statement                  # ForInStatement
    | For '(' (singleExpression | variableDeclarationList) Of expressionSequence ')' statement                  # ForOfStatement
    ;

varModifier
    : Var
    | Let
    | Const
    ;

continueStatement
    : Continue ({notLineTerminator()}? Identifier)? eos
    ;

breakStatement
    : Break ({notLineTerminator()}? Identifier)? eos
    ;

returnStatement
    : Return ({notLineTerminator()}? expressionSequence)? eos
    ;

withStatement
    : With '(' expressionSequence ')' statement
    ;

switchStatement
    : Switch '(' expressionSequence ')' caseBlock
    ;

caseBlock
    : '{' caseClauses? (defaultClause caseClauses?)? '}'
    ;

caseClauses
    : caseClause+
    ;

caseClause
    : Case expressionSequence ':' statementList?
    ;

defaultClause
    : Default ':' statementList?
    ;

labelledStatement
    : Identifier ':' statement
    ;

throwStatement
    : Throw {notLineTerminator()}? expressionSequence eos
    ;

tryStatement
    : Try block (catchProduction finallyProduction? | finallyProduction)
    ;

catchProduction
    : Catch '(' Identifier ')' block
    ;

finallyProduction
    : Finally block
    ;

debuggerStatement
    : Debugger eos
    ;

functionDeclaration
    : Function Identifier '(' formalParameterList? ')' '{' functionBody '}'
    ;

formalParameterList
    : formalParameterArg (',' formalParameterArg)*
    ;

formalParameterArg
    : Identifier ('=' singleExpression)?
    ;

functionBody
    : sourceElements?
    ;

expressionSequence
    : singleExpression (',' singleExpression)*
    ;

singleExpression
    : Function Identifier? '(' formalParameterList? ')' '{' functionBody '}'                       # FunctionExpression
    | singleExpression '[' expressionSequence ']'                                                  # MemberIndexExpression
    | singleExpression '.' identifierName                                                          # MemberDotExpression
    | singleExpression arguments                                                                   # ArgumentsExpression
    | New singleExpression                                                                         # NewExpression
    | singleExpression {notLineTerminator()}? '++'                                                 # PostIncrementExpression
    | singleExpression {notLineTerminator()}? '--'                                                 # PostDecreaseExpression
    | Delete singleExpression                                                                      # DeleteExpression
    | Void singleExpression                                                                        # VoidExpression
    | Typeof singleExpression                                                                      # TypeofExpression
    | '++' singleExpression                                                                        # PreIncrementExpression
    | '--' singleExpression                                                                        # PreDecreaseExpression
    | '+' singleExpression                                                                         # UnaryPlusExpression
    | '-' singleExpression                                                                         # UnaryMinusExpression
    | '~' singleExpression                                                                         # BitNotExpression
    | '!' singleExpression                                                                         # NotExpression
    | <assoc=right> singleExpression '**' singleExpression                                         # PowerExpression
    | singleExpression ('*' | '/' | '%') singleExpression                                         # MultiplicativeExpression
    | singleExpression ('+' | '-') singleExpression                                               # AdditiveExpression
    | singleExpression ('<<' | '>>' | '>>>') singleExpression                                     # BitShiftExpression
    | singleExpression ('<' | '>' | '<=' | '>=') singleExpression                                 # RelationalExpression
    | singleExpression Instanceof singleExpression                                                 # InstanceofExpression
    | singleExpression In singleExpression                                                         # InExpression
    | singleExpression ('==' | '!=' | '===' | '!==') singleExpression                            # EqualityExpression
    | singleExpression '&' singleExpression                                                        # BitAndExpression
    | singleExpression '^' singleExpression                                                        # BitXOrExpression
    | singleExpression '|' singleExpression                                                        # BitOrExpression
    | singleExpression '&&' singleExpression                                                       # LogicalAndExpression
    | singleExpression '||' singleExpression                                                       # LogicalOrExpression
    | singleExpression '?' singleExpression ':' singleExpression                                   # TernaryExpression
    | <assoc=right> singleExpression '=' singleExpression                                          # AssignmentExpression
    | <assoc=right> singleExpression assignmentOperator singleExpression                           # AssignmentOperatorExpression
    | This                                                                                         # ThisExpression
    | Identifier                                                                                   # IdentifierExpression
    | literal                                                                                      # LiteralExpression
    | arrayLiteral                                                                                 # ArrayLiteralExpression
    | objectLiteral                                                                                # ObjectLiteralExpression
    | '(' expressionSequence ')'                                                                   # ParenthesizedExpression
    | arrowFunctionParameters '=>' arrowFunctionBody                                               # ArrowFunctionExpression
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
    | '**='
    ;

literal
    : NullLiteral
    | BooleanLiteral
    | StringLiteral
    | TemplateStringLiteral
    | RegularExpressionLiteral
    | numericLiteral
    ;

numericLiteral
    : DecimalLiteral
    | HexIntegerLiteral
    | OctalIntegerLiteral
    | OctalIntegerLiteral2
    | BinaryIntegerLiteral
    ;

identifierName
    : Identifier
    | reservedWord
    ;

reservedWord
    : keyword
    | NullLiteral
    | BooleanLiteral
    ;

keyword
    : Break
    | Do
    | Instanceof
    | Typeof
    | Case
    | Else
    | New
    | Var
    | Catch
    | Finally
    | Return
    | Void
    | Continue
    | For
    | Switch
    | While
    | Debugger
    | Function
    | This
    | With
    | Default
    | If
    | Throw
    | Delete
    | In
    | Try
    | Class
    | Enum
    | Extends
    | Super
    | Const
    | Export
    | Import
    | Implements
    | Let
    | Private
    | Public
    | Interface
    | Package
    | Protected
    | Static
    | Yield
    | Of
    ;

arguments
    : '(' argumentList? ')'
    ;

argumentList
    : argument (',' argument)*
    ;

argument
    : '...'? singleExpression
    ;

arrayLiteral
    : '[' elementList? ']'
    ;

elementList
    : arrayElement (',' arrayElement)*
    ;

arrayElement
    : '...'? singleExpression
    ;

objectLiteral
    : '{' (propertyAssignment (',' propertyAssignment)*)? ','? '}'
    ;

propertyAssignment
    : propertyName ':' singleExpression                                             # PropertyExpressionAssignment
    | '[' singleExpression ']' ':' singleExpression                                # ComputedPropertyExpressionAssignment
    | Function Identifier? '(' formalParameterList? ')' '{' functionBody '}'        # PropertyFunctionAssignment
    | getter '(' ')' '{' functionBody '}'                                          # PropertyGetter
    | setter '(' formalParameterArg ')' '{' functionBody '}'                       # PropertySetter
    | '...' singleExpression                                                        # PropertySpread
    | Identifier                                                                    # PropertyShorthand
    ;

propertyName
    : identifierName
    | StringLiteral
    | numericLiteral
    ;

getter
    : {p("get")}? Identifier
    ;

setter
    : {p("set")}? Identifier
    ;

arrowFunctionParameters
    : Identifier
    | '(' formalParameterList? ')'
    ;

arrowFunctionBody
    : singleExpression
    | '{' functionBody '}'
    ;

eos
    : SemiColon
    | EOF
    | {lineTerminatorAhead()}?
    ;

// Lexer Rules
RegularExpressionLiteral
    : '/' RegularExpressionFirstChar RegularExpressionChar* '/' IdentifierPart*
    ;

NullLiteral
    : 'null'
    ;

BooleanLiteral
    : 'true'
    | 'false'
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
    : '0' OctalDigit+
    ;

OctalIntegerLiteral2
    : '0' [oO] OctalDigit+
    ;

BinaryIntegerLiteral
    : '0' [bB] BinaryDigit+
    ;

Break : 'break';
Do : 'do';
Instanceof : 'instanceof';
Typeof : 'typeof';
Case : 'case';
Else : 'else';
New : 'new';
Var : 'var';
Catch : 'catch';
Finally : 'finally';
Return : 'return';
Void : 'void';
Continue : 'continue';
For : 'for';
Switch : 'switch';
While : 'while';
Debugger : 'debugger';
Function : 'function';
This : 'this';
With : 'with';
Default : 'default';
If : 'if';
Throw : 'throw';
Delete : 'delete';
In : 'in';
Try : 'try';
Class : 'class';
Enum : 'enum';
Extends : 'extends';
Super : 'super';
Const : 'const';
Export : 'export';
Import : 'import';
Implements : 'implements';
Let : 'let';
Private : 'private';
Public : 'public';
Interface : 'interface';
Package : 'package';
Protected : 'protected';
Static : 'static';
Yield : 'yield';
Of : 'of';

Identifier
    : IdentifierStart IdentifierPart*
    ;

StringLiteral
    : '"' DoubleStringCharacter* '"'
    | '\'' SingleStringCharacter* '\''
    ;

TemplateStringLiteral
    : '`' ('\\`' | ~'`')* '`'
    ;

WhiteSpaces
    : [\t\u000B\u000C\u0020\u00A0]+ -> skip
    ;

LineTerminator
    : [\r\n\u2028\u2029] -> skip
    ;

MultiLineComment
    : '/*' .*? '*/' -> skip
    ;

SingleLineComment
    : '//' ~[\r\n\u2028\u2029]* -> skip
    ;

UnexpectedCharacter
    : .
    ;

SemiColon : ';';

// Fragments
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

fragment HexEscapeSequence
    : 'x' HexDigit HexDigit
    ;

fragment UnicodeEscapeSequence
    : 'u' HexDigit HexDigit HexDigit HexDigit
    ;

fragment HexDigit
    : [0-9a-fA-F]
    ;

fragment DecimalIntegerLiteral
    : '0'
    | [1-9] DecimalDigit*
    ;

fragment DecimalDigit
    : [0-9]
    ;

fragment ExponentPart
    : [eE] [+-]? DecimalDigit+
    ;

fragment IdentifierStart
    : [\p{L}]
    | [$_]
    | '\\' UnicodeEscapeSequence
    ;

fragment IdentifierPart
    : IdentifierStart
    | [\p{Mn}]
    | [\p{Nd}]
    | [\p{Pc}]
    | '\u200C'
    | '\u200D'
    ;

fragment RegularExpressionFirstChar
    : ~[*\\/\r\n]
    | '\\' ~[\r\n]
    ;

fragment RegularExpressionChar
    : ~[\\/\r\n]
    | '\\' ~[\r\n]
    ;

fragment LineContinuation
    : '\\' [\r\n\u2028\u2029]
    ;

fragment OctalDigit
    : [0-7]
    ;

fragment BinaryDigit
    : [01]
    ;