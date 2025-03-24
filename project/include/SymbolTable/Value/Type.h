#pragma once
#include "common.h"

enum TYPE {
    INT, FLOAT, BOOL, STRING,
    VOID
};
typedef enum TYPE Type;

namespace TypeFunctions {

string TypeToString(Type type);
Type StringToType(const string & type);

} // namespace TypeFuntions
