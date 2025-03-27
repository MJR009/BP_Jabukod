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

    minus, // universal option for unary and binary minus, specific to be resolved later

    invalid
};
typedef enum NODE_KIND NodeKind;

namespace NodeKindFunctions {

string NodeKindToString(NodeKind kind);
// only defined for signs which are parts of a subrule in a single expression expansion
NodeKind SignToNodeKind(string sign);

} // namespace NodeKindFunctions
