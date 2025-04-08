#pragma once
#include "common.h"

#include "Instruction.h"

class Snippets {
public:
    static const vector<Instruction> Prolog();
    static const vector<Instruction> Epilog();
    static const vector<Instruction> MainEpilog();

    static const vector<Instruction> ExitSyscall(int exitCode);
};
