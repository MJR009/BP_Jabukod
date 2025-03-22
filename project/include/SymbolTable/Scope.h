#pragma once
#include "common.h"

#include "ScopeEntry.h"

class Scope {
public:
    void AddEntry(const string & name);

    bool IsVariableInScope(const string & name);

    void Print();

private:
    vector<ScopeEntry> variables;
};
