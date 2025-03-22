#pragma once
#include "common.h"

class FunctionTableEntry {
public:
    FunctionTableEntry(const string & name) : name(name) {}

    void Print();

private:
    string name;
};
    