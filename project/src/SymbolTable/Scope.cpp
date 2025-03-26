#include "Scope.h"

void Scope::AddEntry(
    const string & name,
    const StorageSpecifier specifier,
    const Type type,
    const any & defaultValue
) {
    Variable newVariable(name, specifier, type, defaultValue);
    this->variables.push_back(newVariable);
}



bool Scope::IsVariableNameAvailable(const string & name) const {
    for (auto & variable : this->variables) {
        if (variable.GetName() == name) {
            return false;
        }
    }

    return true;
}



void Scope::Print() const {
    for (auto & variable : this->variables) {
        cout << YELLOW << variable.GetName() << DEFAULT;

        cout << DIM << " < " << DEFAULT;
        variable.Print();
        cout << DIM << " >" << DEFAULT << endl;
    }
}
