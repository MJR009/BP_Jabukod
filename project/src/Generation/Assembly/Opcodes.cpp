#include "Opcodes.h"

const map<string, string> Opcode::GPR = {
    {ADD, ADDQ}
};

const map<string, string> Opcode::SSE = {
    {ADD, ADDSS}
};
