/**
 * @file BodyData.cpp
 * @author Martin Jabůrek
 * 
 * Implementation of
 * @link BodyData.h
 */

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



list<Variable *> *BodyData::GetVariables() {
    return this->scope->GetVariables();
}



void BodyData::RemoveStaticVariables() {
    this->scope->PurgeStaticVariables();
}



void BodyData::PrintScope() {
    this->scope->PrintDeclarations();
}



Variable *BodyData::GetVariableForOpaquePredicate() {
    Variable *chosen = nullptr;
    auto available = this->scope->GetVariables();

    for (auto variable : *available) {
        if (variable->GetType() != Type::STRING) {
            chosen = variable;
            break;
        }
    }

    return chosen;
}
