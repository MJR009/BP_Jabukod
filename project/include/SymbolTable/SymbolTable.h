#pragma once
#include "common.h"

#include "Scope.h"
#include "FunctionTable.h"
#include "EnumTable.h"

class SymbolTable {
public:
    void AddGlobalVariable(const string & name);
    void AddFunction(const string & name);
    void AddEnum(const string & name);
    
    void Print();

private:
    Scope globalScope;
    FunctionTable functionTable;
    EnumTable enumTable;
};
