#pragma once
#include "common.h"

#include "ScopeEntry.h"

class Scope {
public:
    void AddEntry(
        const string & name,
        const StorageSpecifier specifier,
        const Type type,
        const any & defaultValue
    );

    bool IsVariableInScope(const string & name);

    void Print();

private:
    vector<ScopeEntry> variables;
};
