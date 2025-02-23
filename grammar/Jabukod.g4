grammar Jabukod;

// Parser rules:

sourceFile // starting nonterminal
    : program? EOF
    ;

program
    : ( IDENTIFIER | NUMBER )+
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
WS
    : [ \t\r\n]+ -> skip
    ;

fragment NEWLINE
    : '\r'? '\n'
    ;
