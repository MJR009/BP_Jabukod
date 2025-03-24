#pragma once
#include "common.h"

#include "Parameter.h"

class FunctionTableEntry {
public:
    FunctionTableEntry(const string & name, const Type returnType) : name(name), returnType(returnType) {}

    void AddParameter(const Type parameterType, const string & parameterName);

    string GetFunctionName() const;
    Type GetReturnType() const;
    vector<Parameter> GetParameters() const;

    void Print() const;

private:
    string name;
    Type returnType;
    vector<Parameter> parameters;
};
