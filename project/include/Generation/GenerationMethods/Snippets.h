#pragma once
#include "common.h"

#include "Instruction.h"
#include "Transform.h"
#include "ASTNode.h"
#include "SymbolTable.h"

class Snippets {
public:
    static const string floatDeclaration;
    static const string stringDeclaration;

public:
    static const vector<Instruction> Prolog(int variablesToReserve);
    static const vector<Instruction> Epilog();
    static const vector<Instruction> MainEpilog();

    static const vector<Instruction> DeclareDefault(Type type, string target);

    static const vector<Instruction> Exit(int exitCode);

    // from %rax or %xmm0 to top of the stack
    static const vector<Instruction> PushPreparedOperand(Type operandType);
    // from top of the stack to either %rax or %xmm0 AND puts the other operand into %rbx or %xmm1
    static const vector<Instruction> PopPreparedOperand(Type operandType);
};
