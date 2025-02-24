grammar Jabukod;

// Parser rules:

sourceFile // starting nonterminal
    : program? EOF
    ;

program
    : ( definition )+
    ;

definition
    : variableDefinition
    | functionDefinition
    ;

variableDefinition
    : nonVoidType IDENTIFIER '=' expression ';'
    ;

functionDefinition
    : type IDENTIFIER '(' functionParameters? ')' functionBody
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
    : '...'
    ;

functionParameters
    : functionParameter ( ',' functionParameter )*
    ;

functionParameter
    : type IDENTIFIER
    ;

functionBody
    : '{' statement+ '}'
    | statement
    ;

statement
    : variableDefinition
    //| // if, while, for, foreach, return, exit, suspend, resume, continue, break, redo, restart
    ;



// Lexer rules:

IDENTIFIER
    : ( ALPHA | UNDERSCORE ) ( ALPHA | UNDERSCORE | DIGIT )*
    ;

NUMBER
    : NON_ZERO_DIGIT ( DIGIT )*
    | ZERO
    ;

STRING_LITERAL
    : '"' ( . | ESCAPE_SEQUENCE )*? '"'
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
