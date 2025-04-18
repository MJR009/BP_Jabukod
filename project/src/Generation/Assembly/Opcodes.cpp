#include "Opcodes.h"

const map<string, string> Opcode::GPR = {
    {ADD, ADDQ},
    {SUB, SUBQ},
    {IMUL, IMULQ},
    {IDIV, IDIVQ},

    {ORi, ORQ},
    {ANDi, ANDQ},
    {SHL, SHLQ},
    {SHR, SHRQ},
    {XOR, XORQ}
};

const map<string, string> Opcode::SSE = {
    {ADD, ADDSS},
    {SUB, SUBSS},
    {IMUL, MULSS},
    {IDIV, DIVSS},
};
