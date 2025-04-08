#pragma once
#include "common.h"

#include "Instruction.h"
#include "Type.h"
#include "Variable.h"

class Transform {
public:
    static bool IsLabel(Instruction & instruction);
    static string FunctionNameToLabel(const string & name);
    static string VariableNameToLabel(const string & name);

    static string TypeToDirective(Type type);
    static string DefaultValueToInitializer(Variable & variable);
};
