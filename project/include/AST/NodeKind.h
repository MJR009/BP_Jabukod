#pragma once
#include "common.h"

enum NODE_KIND {
    PROGRAM, 
    VARIABLE_DECLARATION, VARIABLE_DEFINITION,
    FUNCTION,

    MULTIPLICATION, DIVISION, MODULO,

    invalid
};
typedef enum NODE_KIND NodeKind;

namespace NodeKindFunctions {

string NodeKindToString(NodeKind kind);
NodeKind SignToNodeKind(string sign);

} // namespace NodeKindFunctions
