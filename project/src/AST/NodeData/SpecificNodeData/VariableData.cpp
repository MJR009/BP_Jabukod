#include "VariableData.h"

Type VariableData::GetType() {
    if (this->location) {
        return this->location->GetType();
    }
    return Type::VOID;
}

string VariableData::GetName() {
    if (this->location) {
        return this->location->GetName();
    }
    return "ERR";
}

StorageSpecifier VariableData::GetSpecifier() {
    if (this->location) {
        return this->location->GetSpecifier();
    }
    return StorageSpecifier::NONE;
}



int VariableData::GetStackOffset() {
    return this->location->GetStackOffset();
}



bool VariableData::IsGlobal() {
    return this->location->IsGlobal();
}

bool VariableData::IsParameter() {
    return this->location->IsParameter();
}

string VariableData::GetParamaterLocation() {
    return this->location->GetParameterLocation();
}
