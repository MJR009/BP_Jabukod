#pragma once
#include "common.h"

#include "Variable.h"

class FunctionTableEntry {
public:
    FunctionTableEntry(const string & name, const Type returnType) : name(name), returnType(returnType) {}

    void AddParameter(const Type parameterType, const string & parameterName);

    string GetFunctionName() const;
    Type GetReturnType() const;
    list<Variable *> *GetParameters();
    Variable *GetParameter(const string & name);

    const string GetParameterSlot(int order);
    Type GetParameterType(int order);

    void SetTotalVariables(int variableCount);
    int GetTotalVariables();

    void Print() const;

    ~FunctionTableEntry() {
        for_each(this->parameters.begin(), this->parameters.end(),
            [ ](Variable *current) {
                delete current;
            }
        );
    }

private:
    string name;
    Type returnType;
    list<Variable *> parameters;

    int parameterCount = 0;
    int totalVariables = 0;

private:
    string AllocateNewParameter(Type type);
    int nextStackOffset = 16;
    int floatCount = 0;
    int nonFloatCount = 0;
};
