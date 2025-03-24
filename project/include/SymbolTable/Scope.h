#pragma once
#include "common.h"

#include "Variable.h"

class Scope {
public:
    void AddEntry(
        const string & name,
        const StorageSpecifier specifier,
        const Type type,
        const any & defaultValue
    );

    bool IsVariableNameAvailable(const string & name);

    void Print();

private:
    vector<Variable> variables;
};
