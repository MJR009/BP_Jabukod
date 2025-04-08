#pragma once
#include "common.h"

class Instruction {
public:
    Instruction(string opcode, string arg1 = "", string arg2 = "", string arg3 = "") :
        opcode(opcode), first(arg1), second(arg2), third(arg3)
    {}

    void Output(ofstream & file);

    string GetOpcode();

private:
    string opcode;
    // operands
    string first;
    string second;
    string third;
};
