#pragma once
#include "common.h"

#include "JabukodParser.h"

#include "Scope.h"
#include "FunctionTable.h"
#include "EnumTable.h"

class SymbolTable {
public:
    SymbolTable(JabukodParser *parser) : parser(parser) {}

    void AddGlobalVariable(antlr4::Token *variable);
    void AddFunction(const string & name);
    void AddEnum(const string & name);
    
    bool IsVariableVisible(const string & name, Scope *from);
    bool IsVariableInGlobalScope(const string & name);

    void Print();

private:
    JabukodParser *parser; // for semantic error reporting via notifyErrorListeners

    Scope globalScope;
    FunctionTable functionTable;
    EnumTable enumTable;
};
