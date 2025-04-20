#include "BodyData.h"

Variable *BodyData::AddVariable(
    const string & name,
    StorageSpecifier specifier,
    Type type,
    int stackOffset
) {
    return this->scope->AddEntry(name, specifier, type, any{}, stackOffset, false, false);
}

bool BodyData::IsVariableNameAvailable(const string & name) const {
    return this->scope->IsVariableNameAvailable(name);
}

Variable *BodyData::GetVariable(const string & name) {
    return this->scope->GetVariable(name);
}



void BodyData::RemoveStaticVariables() {
    this->scope->PurgeStaticVariables();
}



void BodyData::PrintScope() {
    this->scope->PrintDeclarations();
}
