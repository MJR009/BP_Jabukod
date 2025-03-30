#pragma once
#include "common.h"

// just one file to greatly simplify includes

#include "GenericNodeData.h"

#include "Scope.h"

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
    VariableData(Type type, const string & name) : type(type), name(name) {}

    Type GetType();
    string GetName();

private:
    Type type;
    string name;
};

class BodyData : public GenericNodeData {
public:
    void AddVariable(
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
    FunctionData(const string & name) : name(name) {}

    string GetName();

private:
    string name;
};

class ForData : public BodyData {};

class ForeachData : public BodyData {};
