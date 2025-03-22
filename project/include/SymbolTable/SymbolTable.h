#pragma once
#include "common.h"

#include "FunctionTable.h"

class SymbolTable {
public:
    void AddFunction(const string & name);
    
    void Print();

private:
    FunctionTable functionTable;
    // global scope
    // enum scope
    // function scope
};
