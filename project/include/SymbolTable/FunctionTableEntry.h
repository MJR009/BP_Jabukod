#pragma once
#include "common.h"

#include "type.h"

class FunctionTableEntry {
public:
    FunctionTableEntry(const string & name, const Type returnType) : name(name), returnType(returnType) {}

    string GetFunctionName();

    void Print();

private:
    string name;
    Type returnType;
};
