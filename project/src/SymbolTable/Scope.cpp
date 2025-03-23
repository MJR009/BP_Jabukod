#include "Scope.h"

void Scope::AddEntry(
    const string & name,
    const StorageSpecifier specifier,
    const Type type,
    const any & defaultValue
) {
    ScopeEntry variable(name, specifier, type, defaultValue);
    this->variables.push_back(variable);
}



bool Scope::IsVariableInScope(const string & name) {
    for (auto & entry : this->variables) {
        if (entry.GetEntryName() == name) {
            return true;
        }
    }

    return false;
}



void Scope::Print() {
    for (auto & variable : this->variables) {
        variable.Print();
    }
}
