#include "Opcodes.h"

const map<string, string> Opcode::GPR = {
    {ADD, ADDQ},
    {SUB, SUBQ}
};

const map<string, string> Opcode::SSE = {
    {ADD, ADDSS},
    {SUB, SUBSS}
};
