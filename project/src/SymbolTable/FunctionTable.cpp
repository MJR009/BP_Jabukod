/**
 * @file FunctionTable.cpp
 * @author Martin Jabůrek
 * 
 * Implementation of
 * @link FunctionTable.h
 */

#include "FunctionTable.h"

FunctionTableEntry *FunctionTable::AddEntry(const string & name, const Type returnType) {
    FunctionTableEntry *newFunction = new FunctionTableEntry(name, returnType);

    this->functions.push_back(newFunction);

    return newFunction;
}



FunctionTableEntry *FunctionTable::GetFunctionByName(const string & name) {
    auto lookup =
        find_if(this->functions.begin(), this->functions.end(),
            [ & name ](const FunctionTableEntry *current) {
                return current->GetFunctionName() == name;
            }
        );

    if (lookup == this->functions.end()) {
        return nullptr;
    }

    return *lookup;
}



bool FunctionTable::IsNameAvailable(const string & name) const {
    return
        none_of(this->functions.begin(), this->functions.end(),
            [ & name ](const FunctionTableEntry *current) {
                return current->GetFunctionName() == name;
            }
        );
}

bool FunctionTable::IsParameterNameAvailable(const string & name, FunctionTableEntry *function) const {
    list<Variable *> *parameters = function->GetParameters();

    return
        none_of(parameters->begin(), parameters->end(),
            [ & name ](const Variable *current) {
                return current->GetName() == name;
            }
        );
}



void FunctionTable::Print() const {
    for_each(this->functions.begin(), this->functions.end(),
        [ ](const FunctionTableEntry *current) {
            current->Print();
        }
    );
}
