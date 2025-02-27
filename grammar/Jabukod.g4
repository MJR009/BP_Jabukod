grammar Jabukod;

// Parser rules:

sourceFile // starting nonterminal
    : program? EOF
    ;

program
    : definition+
    ;

definition
    : variableDefinition ';'
    | functionDefinition
    ;

variableDefinition
    : nonVoidType IDENTIFIER '=' expression
    ;

functionDefinition
    : type IDENTIFIER '(' functionParameters? ')' statementBlock
    ;

functionParameters
    : functionParameter ( ',' functionParameter )*
    ;

functionParameter
    : type IDENTIFIER
    ;

nonVoidType
    : 'int'
    | 'float'
    | 'bool'
    | 'string'
    ;

type
    : 'void'
    | 'int'
    | 'float'
    | 'bool'
    | 'string'
    ;

expression
    : functionCall
    | <assoc=right> expression '**' expression
    | <assoc=right> ( '-' | '~' | '!' ) expression
    | expression ( '*' | '/' | '%' ) expression
    | expression ( '+' | '-' ) expression
    | expression ( '<<' | '>>' ) expression
    | expression ( '<' | '<=' | '>' | '>=' ) expression
    | expression ( '==' | '!=' ) expression
    | expression '&' expression
    | expression '^' expression
    | expression '|' expression
    | expression '&&' expression
    | expression '||' expression
    | <assoc=right> expression '=' expression
    | IDENTIFIER
    | LITERAL
    | '(' expression ')'
    ;

// potential extensions:
//      pre/postfix unary ++, --
//      prefix unary +
//      bit rotations ?
//      ternary ?:
//      opeartion assignments (+=, -=, ...)


functionCall
    : IDENTIFIER '(' functionArguments? ')'
    ;

functionArguments
    : functionArgument ( ',' functionArgument )*
    ;

functionArgument
    : expression
    ;

statementBlock
    : '{' statement+ '}'
    | statement
    ;

statement
    : 'if' '(' expression ')' statementBlock ( 'else' statementBlock )?
    | 'while' '(' expression ')' statementBlock
    | 'for' '(' forHeader ')' statementBlock
    | 'foreach' '(' foreachHeader ')' statementBlock
    | (   variableDefinition
        | expression // TODO or define "assignment" ? this covers functionCall also
        | 'return' expression?
        | 'exit' expression?
        | 'suspend'
        | 'resume'
        | 'continue'
        | 'break'
        | 'redo'
        | 'restart'
    ) ';'
    ;

// potential extensions:
//      yield


forHeader
    : ( expression | variableDefinition )? ';' expression? ';' expression?
    ;

foreachHeader
    : expression ':' expression
    ;



// Lexer rules:

IDENTIFIER
    : ( ALPHA | UNDERSCORE ) ( ALPHA | UNDERSCORE | DIGIT )*
    ;

LITERAL
    : INT_LITERAL
    | FLOAT_LITERAL
    | BOOL_LITERAL
    | STRING_LITERAL
    ;

INT_LITERAL
    : '-'? NUMBER
    ;

FLOAT_LITERAL
    : '-'? NUMBER? '.' NUMBER
    ;

BOOL_LITERAL
    : 'true'
    | 'false'
    ;

STRING_LITERAL
    : '"' ( ~["\\] | ESCAPE_SEQUENCE )*? '"'
    ;

NUMBER
    : NON_ZERO_DIGIT ( DIGIT )*
    | ZERO
    ;

fragment ALPHA
    : [a-zA-Z]
    ;
fragment UNDERSCORE
    : '_'
    ;
fragment DIGIT
    : [0-9]
    ;
fragment NON_ZERO_DIGIT
    : [1-9]
    ;
fragment ZERO
    : '0'
    ;
fragment ESCAPE_SEQUENCE
    : '\\"'
    | '\\\\'
    ;

LINE_COMMENT
    : '//' .*? NEWLINE -> skip
    ;
BLOCK_COMMENT
    : '/*' .*? '*/' -> skip
    ;
fragment NEWLINE
    : '\r'? '\n'
    ;

WS
    : [ \t\r\n]+ -> skip
    ;
