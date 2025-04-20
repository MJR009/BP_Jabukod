#pragma once
#include "common.h"

#include "NodeKind.h"
#include "Instruction.h"
#include "SpecificNodeData.h"
#include "Registers.h"

class Transform {
public:
    // labels
    static bool IsLabel(Instruction & instruction);
    static string IdentifierToLabel(const string & name);

    // .data, .rodata
    static string TypeToDirective(Type type);
    static string DefaultValueToInitializer(Variable *variable);

    // .text
    static string GlobalToAddress(const string & variableName);
    static string RegisterToAddress(const string & reg);
    static string IntToImmediate(const int & number);
    static string LiteralToImmediate(LiteralData *data);
    static string VariableToLocation(VariableData *data); // either register or memory
    // conditions are negated to jump on false and continue flow on true
    static string ConditionToJump(NodeKind condition, Type comparisonType);
    static string ConditionToCMove(NodeKind condition, Type comparisonType);
};
