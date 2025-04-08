#pragma once
#include "common.h"

#include "Instruction.h"
#include "Type.h"
#include "Variable.h"

class GenMethods {
public:
    static bool IsLabel(Instruction & instruction);
    static string FunctionNameToLabel(const string & name);
    static string VariableNameToLabel(const string & name);

    static string VariableTypeToString(Type type);
    static string DefaultValueToString(Variable & variable);

    static void Append(vector<Instruction> & vector1, const vector<Instruction> & vector2);

    static const vector<Instruction> GetProlog();
    static const vector<Instruction> GetEpilog(const string & inFunction);

    static const vector<Instruction> GetExit(int exitCode);
};
