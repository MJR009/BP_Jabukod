#include "Scope.h"

Variable *Scope::AddEntry(
    const string & name,
    const StorageSpecifier specifier,
    const Type type,
    const any & defaultValue
) {
    Variable newVariable(name, specifier, type, defaultValue);
    this->variables.push_back(newVariable);

    return &this->variables.back();
}



bool Scope::IsVariableNameAvailable(const string & name) const {
    for (auto & variable : this->variables) {
        if (variable.GetName() == name) {
            return false;
        }
    }

    return true;
}



Variable *Scope::GetVariable(const string & name) {
    for (auto & variable : this->variables) {
        if (variable.GetName() == name) {
            return &variable;
        }
    }

    return nullptr;
}

vector<Variable> Scope::GetVariables() {
    return this->variables;
}



void Scope::Print() const {
    for (auto & variable : this->variables) {
        variable.Print();
        cout << endl;
    }
}
