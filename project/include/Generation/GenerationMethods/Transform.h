#pragma once
#include "common.h"

#include "Instruction.h"
#include "Type.h"
#include "Variable.h"

class Transform {
public:
    static bool IsLabel(Instruction & instruction);
    static string IdentifierToLabel(const string & name);

    static string TypeToDirective(Type type);
    static string DefaultValueToInitializer(Variable & variable);
    static string GlobalToAddress(const string & variableName);
    static string IntToImmediate(const int & number);
};
