#include "Scope.h"

Variable *Scope::AddEntry(
    const string & name,
    const StorageSpecifier specifier,
    const Type type,
    const any & defaultValue
) {
    this->variables.emplace_back(name, specifier, type, defaultValue);
    return &this->variables.back();
}



bool Scope::IsVariableNameAvailable(const string & name) const {
    return 
        none_of(this->variables.begin(), this->variables.end(), 
            [ & name ](const Variable & current) {
                return current.GetName() == name;
            }
        );
}



Variable *Scope::GetVariable(const string & name) {
    auto lookup =
        find_if(this->variables.begin(), this->variables.end(),
            [ & name ](const Variable & current) {
                return current.GetName() == name;
            }
        );

    if (lookup == this->variables.end()) {
        return nullptr;
    }

    return &(*lookup);
}

list<Variable> & Scope::GetVariables() {
    return this->variables;
}



void Scope::PrintComplete() const {
    for_each(this->variables.begin(), this->variables.end(),
        [ ](const Variable & current){
            current.Print();
            cout << endl;
        }
    );
}

void Scope::PrintDeclarations() const {
    bool first = true;
    for_each(this->variables.begin(), this->variables.end(),
        [ & first ](const Variable & current){
            cout << (first ? "" : ( DIM ", " DEFAULT ));
            first = false;
            current.PrintDeclaration();
        }
    );
}
