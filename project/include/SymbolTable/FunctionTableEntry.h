#pragma once
#include "common.h"

#include "Type.h"

class FunctionTableEntry {
public:
    FunctionTableEntry(const string & name, const Type returnType) : name(name), returnType(returnType) {}

    void AddParameter(const Type parameterType, const string & parameterName);

    string GetFunctionName();
    Type GetReturnType();
    vector< pair<Type, string> > GetParameters();

    void Print();

private:
    string name;
    Type returnType;
    vector< pair<Type, string> > parameters;
};
