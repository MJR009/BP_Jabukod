#pragma once
#include "common.h"

#include "FunctionTableEntry.h"

class FunctionTable {
public:
    FunctionTableEntry *AddEntry(const string & name, const Type returnType);

    bool IsIdTaken(const string & name);
    bool IsParameterNameAvailable(const string & name, FunctionTableEntry *function);

    void Print();

private:
    vector<FunctionTableEntry> functions;
};
