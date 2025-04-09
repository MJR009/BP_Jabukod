#pragma once
#include "common.h"

#include "Instruction.h"
#include "SpecificNodeData.h"

class Transform {
public:
    static bool IsLabel(Instruction & instruction);
    static string IdentifierToLabel(const string & name);

    static string TypeToDirective(Type type);
    static string DefaultValueToInitializer(Variable & variable);
    static string GlobalToAddress(const string & variableName);
    static string IntToImmediate(const int & number);

    static string LiteralToImmediate(LiteralData *data);

    static string VariableToStackAddress(VariableData *data);
};
