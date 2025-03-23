#pragma once
#include "common.h"

#include "JabukodParser.h"

#include "Scope.h"
#include "FunctionTable.h"
#include "EnumTable.h"

class SymbolTable {
public:
    SymbolTable(JabukodParser *parser) : parser(parser) {}

    void AddGlobalVariable(
        antlr4::Token *variable,
        JabukodParser::StorageSpecifierContext *storageSpecifier,
        JabukodParser::NonVoidTypeContext *variableType,
        JabukodParser::ExpressionContext *defaultValue
    );
    void AddFunction(antlr4::Token *function, JabukodParser::TypeContext *returnType);
    EnumTableEntry *AddEnum(antlr4::Token *theEnum);
    void AddEnumItem(antlr4::Token *itemName, antlr4::Token *itemValue);

    void CheckIfMainPresent();

    void SetCurrentEnum(EnumTableEntry *theEnum);
    void RemoveCurrentEnum();

    void Print();

private:
    JabukodParser *parser; // for semantic error reporting via notifyErrorListeners()

    Scope globalScope;
    FunctionTable functionTable;
    EnumTable enumTable;

    EnumTableEntry *currentEnum = nullptr; // used for adding entries
    int currentEnumItemvalue = 0;

private:
    bool IsIDAvailable(const string & name, Scope & scope);
    bool IsEnumValueAvailable(const int & value);

    Type ResolveNonVoidType(JabukodParser::NonVoidTypeContext *type);
    Type ResolveType(JabukodParser::TypeContext *type);

    StorageSpecifier ResolveStorageSpecifier(JabukodParser::StorageSpecifierContext *specifier);
    any ResolveDefaultValue(JabukodParser::ExpressionContext *expression, Type type);
    bool IsOnlyLiteral(JabukodParser::ExpressionContext *expression);
    any GetImplicitDefaultValue(Type type); // for declarations
    any ResolveExplicitDefaultValue(
        JabukodParser::LiteralContext *defaultValue,
        Type type    
    ); // for global definitions, also does type checking

    string ReplaceEscapeSequences(const string & str);
};
