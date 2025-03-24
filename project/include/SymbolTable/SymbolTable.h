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
    FunctionTableEntry *AddFunction(
        antlr4::Token *function,
        JabukodParser::TypeContext *returnType
    );
    void AddFunctionParameter(
        JabukodParser::NonVoidTypeContext *parameterType,
        antlr4::Token *parameterName
    );
    EnumTableEntry *AddEnum(
        antlr4::Token *theEnum
    );
    void AddEnumItem(
        antlr4::Token *itemName,
        antlr4::Token *itemValue
    );

    void SetCurrentEnum(EnumTableEntry *theEnum);
    void ResetCurrentEnum();
    void SetCurrentFunction(FunctionTableEntry *function);
    void ResetCurrentFunction();

    void CheckIfIntMainPresent();

    void Print();

private:
    JabukodParser *parser; // for semantic error reporting via notifyErrorListeners()

    Scope globalScope;
    FunctionTable functionTable;
    EnumTable enumTable;

    EnumTableEntry *currentEnum = nullptr; // used for adding entries
    int currentEnumItemValue = 0;

    FunctionTableEntry *currentFunction = nullptr; // used for storing parameters

private:
    bool IsVariableNameAvailable(const string & name, Scope & scope);
    bool IsFunctionNameAvailable(const string & name);
    bool IsFunctionParameterNameAvailable(const string & name); // checks in currentFunction
    bool IsEnumNameAvailable(const string & name);
    bool IsEnumItemNameAvailable(const string & name); // checks in currentEnum
    bool IsEnumItemValueAvailable(const int & value);

    StorageSpecifier ResolveStorageSpecifier(JabukodParser::StorageSpecifierContext *specifier);
    bool IsFromDeclaration(JabukodParser::StorageSpecifierContext *specifier);

    any ResolveDefaultValue(JabukodParser::ExpressionContext *expression, Type type);
    bool IsExpressionOnlyLiteral(JabukodParser::ExpressionContext *expression);
    any ResolveExplicitDefaultValue(JabukodParser::LiteralContext *defaultValue, Type type); // for definitions, also does type checking
    any GetImplicitDefaultValue(Type type); // for declarations

    string ReplaceEscapeSequences(const string & str);
};
