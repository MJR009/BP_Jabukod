/**
 * @file VariableData.cpp
 * @author Martin Jabůrek
 * 
 * Implementation of
 * @link VariableData.h
 */


#include "VariableData.h"

Variable *VariableData::GetSelf() {
    return this->location;
}



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



any VariableData::GetActualDefaultValue() {
    if (this->location) {
        return this->location->GetActualDefaultValue();
    }
    return any{};
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



void VariableData::SetName(const string & name) {
    this->location->SetName(name);
}
