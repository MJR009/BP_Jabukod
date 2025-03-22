#pragma once
#include "common.h"

class FunctionTableEntry {
public:
    FunctionTableEntry(const string & name) : name(name) {}

    string GetFunctionName();

    void Print();

private:
    string name;
};
