/**
 * @file Jabukod.g4
 * @author Martin Jabůrek
 *
 * @brief ANTLR4 grammar describing the Jabukód programming language syntax and lexeme
 */

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
    : storageSpecifier? nonVoidType IDENTIFIER listSpecifier?
    ;

variableDefinition
    : storageSpecifier? nonVoidType IDENTIFIER listSpecifier? '=' expression
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
    : 'enum' IDENTIFIER enumBlock
    ;

enumBlock
    : '{' enumItem ( ',' enumItem )* '}'
    ;

enumItem
    : IDENTIFIER ( '=' INT_LITERAL )?
    ;

expression
    : functionCall                                              # functionCallExpression
    | IDENTIFIER listAccess                                     # listAccessExpression
    | list                                                      # listExpression
    | <assoc=right> expression '**' expression                  # exponentExpression
    | <assoc=right> sign=( '-' | '~' | '!' ) expression         # prefixUnaryExpression
    | expression sign=( '*' | '/' | '%' ) expression            # mulDivModExpression
    | expression sign=( '+' | '-' ) expression                  # addSubExpression
    | expression sign=( '<<' | '>>' ) expression                # shiftExpression
    | expression sign=( '<' | '<=' | '>' | '>=' ) expression    # lessMoreExpression
    | expression sign=( '==' | '!=' ) expression                # equalityExpression
    | expression '&' expression                                 # bitAndExpression
    | expression '^' expression                                 # bitXorExpression
    | expression '|' expression                                 # bitOrExpression
    | expression '&&' expression                                # andExpression
    | expression '||' expression                                # orExpression
    | <assoc=right> expression '=' expression                   # assignExpression
    | IDENTIFIER                                                # identifierExpression
    | literal                                                   # literalExpression
    | '(' expression ')'                                        # parenthesisExpression
    ;

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
    : 'if' '(' expression ')' statementBlock ( 'else' statementBlock )? # ifStatement
    | 'while' '(' expression ')' statementBlock                         # whileStatement
    | 'for' '(' forHeader ')' statementBlock                            # forStatement
    | 'foreach' '(' foreachHeader ')' statementBlock                    # foreachStatement
    | simpleStatement ';'                                               # simpleStatementStatement
    ;

simpleStatement
    : variableDeclaration                                           # variableDeclarationStatement
    | variableDefinition                                            # variableDefinitionStatement
    | assignment                                                    # assignmentStatement
    | functionCall                                                  # functionCallStatement
    | 'return' expression?                                          # returnStatement
    | 'exit' expression                                             # exitStatement
    | 'suspend'                                                     # suspendStatement
    | 'resume'                                                      # resumeStatement
    | 'continue'                                                    # continueStatement
    | 'break'                                                       # breakStatement
    | 'redo'                                                        # redoStatement
    | 'restart'                                                     # restartStatement
    | 'read' IDENTIFIER                                             # readStatement
    | 'write' expression                                            # writeStatement
    ;

assignment
    : IDENTIFIER listAccess? '=' expression
    ;

forHeader
    : init=forInitializer? ';' condition=expression? ';' update=expression?
    ;

forInitializer
    : expression
    | variableDefinition
    ;

foreachHeader
    : variableDeclaration ':' expression
    ;

list
    : '{' ( expression ( ',' expression )* )? '}'
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

listSpecifier
    : '[' INT_LITERAL ']'
    ;

listAccess
    : '[' expression ']'
    ;



// Lexer rules:

INT_LITERAL
    : '-'? ( NUMBER | HEX_NUMBER )
    ;

FLOAT_LITERAL
    : '-'? NUMBER ( EXPONENT | '.' DIGIT+ EXPONENT? )
    ;

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
