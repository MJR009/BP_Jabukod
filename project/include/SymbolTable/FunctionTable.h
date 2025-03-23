#pragma once
#include "common.h"

#include "FunctionTableEntry.h"

class FunctionTable {
public:
    void AddEntry(const string & name, const Type returnType);

    bool IsIdTaken(const string & name);

    void Print();

private:
    vector<FunctionTableEntry> functions;
};
