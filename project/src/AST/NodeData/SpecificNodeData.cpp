#include "SpecificNodeData.h"

Type LiteralData::GetType() {
    return this->type;
}

any LiteralData::GetValue() {
    return this->value;
}



string FunctionData::GetName() {
    return this->name;
}



string VariableData::GetName() {
    return this->name;
}
