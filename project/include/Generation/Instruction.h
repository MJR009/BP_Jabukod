#pragma once
#include "common.h"

#include "Opcodes.h"
#include "Registers.h"
#include "GeneralMacros.h"

class Instruction {
public:
    Instruction(string opcode, string arg1 = "", string arg2 = "", string arg3 = "") :
        opcode(opcode), arg1(arg1), arg2(arg2), arg3(arg3)
    {}

    void Output(ofstream & file);

    string GetOpcode();

    static void ConnectSequences(vector<Instruction> & vector1, const vector<Instruction> & vector2);

private:
    string opcode;
    string arg1;
    string arg2;
    string arg3;
};
