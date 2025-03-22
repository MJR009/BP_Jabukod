#pragma once
#include "common.h"

#include "FunctionTable.h"
#include "Scope.h"

class SymbolTable {
public:
    void AddFunction(const string & name);
    void AddGlobalVariable(const string & name);
    
    void Print();

private:
    Scope globalScope;
    FunctionTable functionTable;
    // enum scope
    // function scopes
};
