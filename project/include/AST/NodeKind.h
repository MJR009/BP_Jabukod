#pragma once
#include "common.h"

enum NODE_KIND {
    PROGRAM, FUNCTION
};
typedef enum NODE_KIND NodeKind;

namespace NodeKindFunctions {

string NodeKindToString(NodeKind kind);

} // namespace NodeKindFunctions
