#include "SpecificNodeData.h"

Type LiteralData::GetType() {
    return this->type;
}

any LiteralData::GetValue() {
    return this->value;
}
