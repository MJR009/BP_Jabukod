#pragma once
#include "common.h"

enum NODE_KIND {
    PROGRAM, 
    VARIABLE_DECLARATION, VARIABLE_DEFINITION,
    FUNCTION,

    MULTIPLICATION, DIVISION, MODULO,
    POWER,
    LEFT_SHIFT, RIGHT_SHIFT,
    BIT_OR,
    ADDITION, SUBTRACTION,
    BIT_XOR,
    OR,
    AND,
    BIT_AND,
    LESS, LESS_EQUAL, GREATER, GREATER_EQUAL,
    EQUAL, NOT_EQUAL,
    UNARY_MINUS, BIT_NOT, NOT,
    minus, // universal option for unary and binary minus, which one resolved in respective context

    VARIABLE, LITERAL,

    ASSIGNMENT, FUNCTION_CALL,
    IF, WHILE, FOR, FOREACH,
    FOR_HEADER1, FOR_HEADER2, FOR_HEADER3, // needed for diferentiation
    BODY, // general for all structured control flow

    RETURN, EXIT, SUSPEND, RESUME,
    CONTINUE, BREAK, REDO, RESTART,

    READ, WRITE,

    invalid
};
typedef enum NODE_KIND NodeKind;

namespace NodeKindFunctions {

string NodeKindToString(NodeKind kind);
// only defined for signs which are parts of a subrule within one expression expansion
NodeKind SignToNodeKind(string sign);

} // namespace NodeKindFunctions
