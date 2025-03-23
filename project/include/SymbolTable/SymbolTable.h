#pragma once
#include "common.h"

#include "JabukodParser.h"

#include "Scope.h"
#include "FunctionTable.h"
#include "EnumTable.h"

class SymbolTable {
public:
    SymbolTable(JabukodParser *parser) : parser(parser) {}

    void AddGlobalVariable(antlr4::Token *variable, JabukodParser::StorageSpecifierContext *storageSpecifier);
    void AddFunction(antlr4::Token *function);
    EnumTableEntry *AddEnum(antlr4::Token *theEnum);
    void AddEnumItem(antlr4::Token *itemName, antlr4::Token *itemValue);

    bool IsIDAvailable(const string & name, Scope & scope);
    bool IsEnumValueAvailable(const int & value);
    void CheckIfMainPresent();

    void SetCurrentEnum(EnumTableEntry *theEnum);
    void RemoveCurrentEnum();

    void Print();

private:
    JabukodParser *parser; // for semantic error reporting via notifyErrorListeners()

    Scope globalScope;
    FunctionTable functionTable;
    EnumTable enumTable;
public:
    EnumTableEntry *currentEnum = nullptr; // used for adding entries
    int currentEnumItemvalue = 0;
};
