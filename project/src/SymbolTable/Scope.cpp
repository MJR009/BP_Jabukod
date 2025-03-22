#include "Scope.h"

void Scope::AddEntry(const string & name) {
    ScopeEntry variable(name);
    this->variables.push_back(variable);
}



void Scope::Print() {
    for (auto & variable : this->variables) {
        variable.Print();
    }
}
