grammar Jabukod;

// Parser rules:

//@parser::members{
//    int foo();
//}

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
    : storageSpecifier? nonVoidType IDENTIFIER LIST_SPECIFIER?
    ;

variableDefinition
    : storageSpecifier? nonVoidType IDENTIFIER LIST_SPECIFIER? '=' expression
    //: variableDeclaration '=' expression
    ;

storageSpecifier
    : 'const'
    | 'static'
    ;

functionDefinition
    : type IDENTIFIER '(' functionParameters? ')' statementBlock
    //: type IDENTIFIER LIST_SPECIFIER? '(' functionParameters? ')' statementBlock
    ;

functionParameters
    : functionParameter ( ',' functionParameter )*
    ;

functionParameter
    : nonVoidType IDENTIFIER LIST_SPECIFIER?
    ;

enumDefinition
    : 'enum' IDENTIFIER enumBlock
    ;

enumBlock
    : '{' enumItem ( ',' enumItem )* '}'
    ;

enumItem
    : IDENTIFIER ( '=' INT_LITERAL )?
    ;

expression
    : ( functionCall | listAccess | list)
    //: ( functionCall | listAccess | list listAccess? )
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
    | literal
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

listAccess
    : IDENTIFIER ( '[' expression ']' )+
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
        | expression // covers functionCall
        // assignment -> IDENTIFIER listAccess? '=' expression
        // functionCall
        | 'return' expression?
        | 'exit' expression?
        | 'suspend'
        | 'resume'
        | 'continue'
        | 'break'
        | 'redo'
        | 'restart'
        | 'read' IDENTIFIER
        | 'write' expression
    ) ';'
    ;

// potential extensions:
//      yield
//      potentially define "assignment" 
// May be better to segment into more rules


forHeader
    : ( expression | variableDefinition )? ';' expression? ';' expression?
    ;

foreachHeader
    : variableDeclaration ':' expression
    ;

list
    : '{' ( expression ( ',' expression )* )? '}' // ARISES NEED FOR DYNAMIC MEMORY ALLOCATION
    ;

literal
    : INT_LITERAL
    | FLOAT_LITERAL
    | BOOL_LITERAL
    | STRING_LITERAL
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



// Lexer rules:

LIST_SPECIFIER
    : '[]'+
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

IDENTIFIER
    : ( ALPHA | UNDERSCORE ) ( ALPHA | UNDERSCORE | DIGIT )*
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
