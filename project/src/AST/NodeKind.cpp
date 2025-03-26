#include "NodeKind.h"

#define Conversion_case(item) case NodeKind::item: return #item

string NodeKindFunctions::NodeKindToString(NodeKind kind) {
    switch (kind) {
        Conversion_case(PROGRAM);
        Conversion_case(VARIABLE_DECLARATION);
        Conversion_case(FUNCTION);
        
    }

    return "";
}
