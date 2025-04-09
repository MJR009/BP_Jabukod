#pragma once
#include "common.h"

#include "Instruction.h"
#include "Transform.h"
#include "ASTNode.h"

class Snippets {
public:
    static const vector<Instruction> Prolog(int variablesToReserve);
    static const vector<Instruction> Epilog();
    static const vector<Instruction> MainEpilog();

    static const vector<Instruction> Exit(int exitCode);

    // after this, depending on type, it will wither be in %rax or %xmm0
    // if an empty vector is returned, there is a nested expression present
    static const vector<Instruction> PrepareOperand(ASTNode *operand);
    // from %rax or %xmm0 to top of the stack
    static const vector<Instruction> PushPreparedOperand(Type operandType);
    // from top of the stack to either %rax or %xmm0 AND puts the other operand into %rbx or %xmm1
    static const vector<Instruction> PopPreparedOperand(Type operandType);
};
