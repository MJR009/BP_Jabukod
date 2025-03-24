#pragma once
#include "common.h"

#include "Parameter.h"

class FunctionTableEntry {
public:
    FunctionTableEntry(const string & name, const Type returnType) : name(name), returnType(returnType) {}

    void AddParameter(const Type parameterType, const string & parameterName);

    string GetFunctionName();
    Type GetReturnType();
    vector<Parameter> GetParameters();

    void Print();

private:
    string name;
    Type returnType;
    vector<Parameter> parameters;
};
