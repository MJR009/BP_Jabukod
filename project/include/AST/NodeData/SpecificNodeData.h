#pragma once
#include "common.h"

// just one file to greatly simplify includes

#include "GenericNodeData.h"

#include "Scope.h"
#include "Parameter.h"
#include "FunctionTableEntry.h"

class LiteralData : public GenericNodeData {
public:
    LiteralData(Type type, any value) : type(type), value(value) {}

    Type GetType();
    any GetValue();

private:
    Type type;
    any value;
};



class VariableData : public GenericNodeData {
public:
    VariableData(BaseValue *variable) : location(variable) {}

    Type GetType();
    string GetName();

private:
    BaseValue *location;
};



class BodyData : public GenericNodeData {
public:
    Variable *AddVariable(
        const string & name,
        StorageSpecifier specifier,
        Type type
    );
    bool IsVariableNameAvailable(const string & name) const;
    Variable *GetVariable(const string & name);

    void PrintScope();

protected:
    Scope scope;
};



class FunctionData : public BodyData {
public:
    FunctionData(FunctionTableEntry *function) : location(function) {}

    string GetName();

private:
    FunctionTableEntry *location;
};



class ForData : public BodyData {};



class ForeachData : public BodyData {};



class ExpressionData : public GenericNodeData {
public:
    ExpressionData(const Type type) : type(type) {}

    Type GetType();

private:
    Type type;
};



class ReadData : public GenericNodeData {
public:
    ReadData(BaseValue *variable) : location(variable) {}

    string GetTargetName();

private:
    BaseValue *location;
};



class FunctionCallData : public GenericNodeData {
public:
    FunctionCallData(FunctionTableEntry *function, bool exists) : location(function), exists(exists) {}

    string GetName();
    Type GetReturnType();

    bool exists; // differentiates void functions (true) and functions not define during semantic analysis (false)

private:
    FunctionTableEntry *location;
};
