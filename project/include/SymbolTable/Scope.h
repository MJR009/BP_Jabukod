#pragma once
#include "common.h"

#include "Variable.h"

class Scope {
public:
    void AddEntry(
        const string & name,
        const StorageSpecifier specifier,
        const Type type,
        const any & defaultValue = any{}
    );

    bool IsVariableNameAvailable(const string & name) const;

    vector<Variable> GetVariables(); // used for printing

    void Print() const;

private:
    vector<Variable> variables;
};
