#include "NodeKind.h"

#define Conversion_case(item) case NodeKind::item: return #item

string NodeKindFunctions::NodeKindToString(NodeKind kind) {
    switch (kind) {
        Conversion_case(PROGRAM);
        Conversion_case(VARIABLE_DECLARATION);
        Conversion_case(VARIABLE_DEFINITION);
        Conversion_case(FUNCTION);
        Conversion_case(MULTIPLICATION);
        Conversion_case(DIVISION);
        Conversion_case(MODULO);

        Conversion_case(invalid);        
    }

    return "";
}

NodeKind NodeKindFunctions::SignToNodeKind(string sign) {
    if (sign == "*") return NodeKind::MULTIPLICATION;
    if (sign == "/") return NodeKind::DIVISION;
    if (sign == "%") return NodeKind::MODULO;

    return NodeKind::invalid;
}
