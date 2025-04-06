#pragma once
#include "common.h"

#include "Instruction.h"
#include "Type.h"
#include "Variable.h"

class GenMethods {
public:
    static bool IsLabel(Instruction *instruction);

    static string FunctionNameToLabel(const string & name);

    static string VariableNameToLabel(const string & name);
    static string VariableTypeToString(Type type);
    static string ProduceDefaultValue(Variable *variable);
};
