grammar Jabukod;

// Parser rules:

sourceFile // starting nonterminal
    : program? EOF
    ;

program
    : definition+
    ;

definition
    : variableDeclaration ';'
    | variableDefinition ';'
    | functionDefinition
    | enumDefinition
    ;

variableDeclaration
    : storageSpecifier? nonVoidType IDENTIFIER
    ;

variableDefinition
    : storageSpecifier? nonVoidType IDENTIFIER '=' expression
    ;

storageSpecifier
    : 'const'
    | 'static'
    ;

functionDefinition
    : type IDENTIFIER '(' functionParameters? ')' statementBlock
    ;

functionParameters
    : functionParameter ( ',' functionParameter )*
    ;

functionParameter
    : nonVoidType IDENTIFIER
    ;

enumDefinition
    : 'enum' IDENTIFIER enumBlock ';'
    ;

enumBlock
    : '{' enumItem ( ',' enumItem )* '}'
    ;

enumItem
    : IDENTIFIER ( '=' INT_LITERAL )?
    ;

type
    : 'void'
    | nonVoidType
    ;

nonVoidType
    : 'int'
    | 'float'
    | 'bool'
    | 'string'
    | 'enum' IDENTIFIER
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
    | (   variableDeclaration
        | variableDefinition
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
    : variableDeclaration ':' expression
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
    : '-'? ( NUMBER | HEX_NUMBER )
    ;

FLOAT_LITERAL
    : '-'? NUMBER ( EXPONENT | '.' NUMBER EXPONENT? )
    ;

// potential extensions:
//      hex. floats
//      whole or decimal part ommission (.1, 2., ...)
//      binary literals 0b../0B...


BOOL_LITERAL
    : 'true'
    | 'false'
    ;

STRING_LITERAL
    : '"' ( ~["\\] | ESCAPE_SEQUENCE )*? '"'
    ;

// potential extensions:
//      unicode
//      more escape sequences (ascii < 32, ones defined for std. C)


fragment NUMBER
    : NON_ZERO_DIGIT DIGIT*
    | ZERO
    ;

fragment HEX_NUMBER
    : HEX_PREFIX HEX_DIGIT+
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
fragment EXPONENT
    : EXPONENT_SIGN '-'? NUMBER
    ;
fragment EXPONENT_SIGN
    : 'e'
    | 'E'
    ;
fragment HEX_PREFIX
    : '0x'
    | '0X'
    ;
fragment HEX_DIGIT
    : [0-9a-fA-F]
    ;
fragment ZERO
    : '0'
    ;
fragment ESCAPE_SEQUENCE
    : '\\"'
    | '\\\\'
    | '\\n'
    | '\\t'
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
