/**
 * @file Scope.cpp
 * @author Martin Jabůrek
 * 
 * Implementation of
 * @link Scope.h
 */

#include "Scope.h"

Variable *Scope::AddEntry(
    const string & name,
    const StorageSpecifier specifier,
    const Type type,
    const any & defaultValue,
    int stackOffset,
    bool isGlobal,
    bool isParameter
) {
    Variable *newVariable = new Variable(
        name,
        specifier,
        type,
        defaultValue,
        stackOffset,
        isGlobal,
        isParameter
    );

    this->variables.push_back(newVariable);

    return newVariable;
}

void Scope::AddExistingEntry(Variable *variable) {
    this->variables.push_back(variable);
}



bool Scope::IsVariableNameAvailable(const string & name) const {
    return 
        none_of(this->variables.begin(), this->variables.end(), 
            [ & name ](const Variable *current) {
                return current->GetName() == name;
            }
        );
}



Variable *Scope::GetVariable(const string & name) {
    auto lookup =
        find_if(this->variables.begin(), this->variables.end(),
            [ & name ](const Variable *current) {
                return current->GetName() == name;
            }
        );

    if (lookup == this->variables.end()) {
        return nullptr;
    }

    return *lookup;
}

list<Variable *> *Scope::GetVariables() {
    return &this->variables;
}



void Scope::PrintComplete() const {
    for_each(this->variables.begin(), this->variables.end(),
        [ ](const Variable *current){
            current->Print();
            cout << endl;
        }
    );
}

void Scope::PrintDeclarations() const {
    bool first = true;
    for_each(this->variables.begin(), this->variables.end(),
        [ & first ](const Variable *current){
            cout << (first ? "" : ( DIM ", " DEFAULT ));
            first = false;
            current->PrintDeclaration();
        }
    );
}

void Scope::PrintAsEnum() const {
    bool first = true;
    for_each(this->variables.begin(), this->variables.end(),
        [ & first ](const Variable *current) {
            cout << DIM << (first ? "" : ", ") << DEFAULT;
            cout << current->GetName();
            cout << DIM << " = " << current->GetDefaultValue<int>() << DEFAULT;
            first = false;
        }
    );
}



void Scope::PurgeStaticVariables() {
    auto variable = this->variables.begin();

    while (variable != this->variables.end()) {
        if ((*variable)->GetSpecifier() == StorageSpecifier::STATIC) {
            variable = this->variables.erase(variable);
        } else {
            variable++;
        }
    }
}
