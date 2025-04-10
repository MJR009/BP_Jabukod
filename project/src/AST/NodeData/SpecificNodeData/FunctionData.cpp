#include "FunctionData.h"

string FunctionData::GetName() {
    if (this->location) {
        return this->location->GetFunctionName();
    }
    return "ERR";
}

int FunctionData::GetVariableCount() {
    return this->location->GetTotalVariables();
}
