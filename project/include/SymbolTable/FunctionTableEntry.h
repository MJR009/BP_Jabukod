#pragma once
#include "common.h"

#include "Variable.h"

class FunctionTableEntry {
public:
    FunctionTableEntry(const string & name, const Type returnType) : name(name), returnType(returnType) {}

    void AddParameter(const Type parameterType, const string & parameterName);

    string GetFunctionName() const;
    Type GetReturnType() const;
    list<Variable> & GetParameters();
    Variable *GetParameter(const string & name);

    void Print() const;

private:
    string name;
    Type returnType;
    list<Variable> parameters;
};
