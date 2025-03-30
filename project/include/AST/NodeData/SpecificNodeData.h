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

// data regarding variable is stored in closest scope, here is just name mainly for debugging
class VariableData : public GenericNodeData {
public:
    VariableData(const string & name) : name(name) {}

    string GetName();

private:
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
