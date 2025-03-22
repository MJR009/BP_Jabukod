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
    void AddFunction(antlr4::Token *function);
    void AddEnum(antlr4::Token *theEnum);
    
    bool IsIDAvailable(const string & name, Scope & scope);

    void Print();

private:
    JabukodParser *parser; // for semantic error reporting via notifyErrorListeners

    Scope globalScope;
    FunctionTable functionTable;
    EnumTable enumTable;
};
