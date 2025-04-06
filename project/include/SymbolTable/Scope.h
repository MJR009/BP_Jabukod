#pragma once
#include "common.h"

#include "Variable.h"

class Scope {
public:
    Variable *AddEntry(
        const string & name,
        const StorageSpecifier specifier,
        const Type type,
        const any & defaultValue = any{}
    );

    bool IsVariableNameAvailable(const string & name) const;

    Variable *GetVariable(const string & name);
    vector<Variable> GetVariables(); // used for printing

    void Print() const;

private:
    vector<Variable> variables;
};
