#pragma once
#include "common.h"

#include "JabukodParser.h"

#include "Scope.h"
#include "FunctionTable.h"
#include "EnumTable.h"

class SymbolTable {
public:
    SymbolTable(JabukodParser *parser) : parser(parser) {
        this->globalScope = new Scope();
        this->functionTable = new FunctionTable();
        this->enumTable = new EnumTable();
    }

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

    Variable *AddGlobalLiteral(const string & name, Type type, any value);

    void SetCurrentEnum(EnumTableEntry *theEnum);
    void ResetCurrentEnum();
    void SetCurrentFunction(FunctionTableEntry *function);
    void ResetCurrentFunction();

    void CheckIfIntMainPresent();

    bool IsIdFunctionParameter(const string & functionName, const string & identifier);
    Variable *IsIdGlobalVariable(const string & name);
    Variable *IsIdEnumItem(const string & name);
    FunctionTableEntry *IsIdFunction(const string & name);
    bool IsIdEnumName(const string & name);

    Scope *GetGlobalVariables();
    list<EnumTableEntry *> *GetAllEnums();

    bool IsIdentifierAllowed(const string & identifier) const;

    void Print() const;

    ~SymbolTable() {
        delete this->globalScope;
        delete this->functionTable;
        delete this->enumTable;
    }

public:
    static const int defaultINT;
    static const float defaultFLOAT;
    static const bool defaultBOOL;
    static const string defaultSTRING;

private:
    JabukodParser *parser; // for semantic error reporting via notifyErrorListeners()

    Scope *globalScope;
    FunctionTable *functionTable;
    EnumTable *enumTable;

    EnumTableEntry *currentEnum = nullptr; // used for adding entries
    int currentEnumItemValue = 0;

    FunctionTableEntry *currentFunction = nullptr; // used for storing parameters

private:
    bool IsGlobalVariableNameAvailable(const string & name) const;
    bool IsFunctionNameAvailable(const string & name) const;
    bool IsFunctionParameterNameAvailable(const string & name) const; // checks in currentFunction
    bool IsEnumNameAvailable(const string & name) const;
    bool IsEnumItemNameAvailable(const string & name) const; // checks in currentEnum
    bool IsEnumItemValueAvailable(const int & value) const;

    StorageSpecifier ResolveStorageSpecifier(JabukodParser::StorageSpecifierContext *specifier) const;
    bool IsFromDeclaration(JabukodParser::StorageSpecifierContext *specifier) const;

    any ResolveDefaultValue(JabukodParser::ExpressionContext *expression, Type type) const;
    bool IsLiteralExpression(JabukodParser::ExpressionContext *expression) const;
    any ResolveExplicitDefaultValue(JabukodParser::LiteralContext *defaultValue, Type variableType) const; // for definitions, also does conversions
    any GetImplicitDefaultValue(Type type) const; // for declarations
    any ConvertLiteralByType(JabukodParser::LiteralContext *defaultValue, Type literalType, Type variableType) const;
};
