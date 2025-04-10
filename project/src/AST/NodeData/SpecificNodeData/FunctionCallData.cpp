#include "FunctionCallData.h"

string FunctionCallData::GetName() {
    if (this->location) {
        return this->location->GetFunctionName();
    }
    return "ERR";
}

Type FunctionCallData::GetReturnType() {
    if (this->location) {
        return this->location->GetReturnType();
    }
    return Type::VOID;
}
