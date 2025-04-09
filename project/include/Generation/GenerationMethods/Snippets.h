#pragma once
#include "common.h"

#include "Instruction.h"
#include "Transform.h"

class Snippets {
public:
    static const vector<Instruction> Prolog(int variablesToReserve);
    static const vector<Instruction> Epilog();
    static const vector<Instruction> MainEpilog();

    static const vector<Instruction> Exit(int exitCode);
};
