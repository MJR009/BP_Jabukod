#include "Scope.h"

void Scope::AddEntry(const string & name) {
    ScopeEntry variable(name);
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
