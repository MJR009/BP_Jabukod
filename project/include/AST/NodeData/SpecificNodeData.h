#pragma once
#include "common.h"

// just one file to greatly simplify includes

#include "GenericNodeData.h"

class LiteralData : public GenericNodeData {
public:
    LiteralData(Type type, any value) : type(type), value(value) {}

    Type GetType();
    any GetValue();

private:
    Type type;
    any value;
};

class FunctionData : public GenericNodeData {
public:
    FunctionData(const string & name) : name(name) {}

    string GetName();

private:
    string name;
};
