#pragma once
#include "common.h"

#include "FunctionTableEntry.h"

class FunctionTable {
public:
    void AddEntry(const string & name);

    void Print();

private:
    vector<FunctionTableEntry> functions;
};
