#include "Opcodes.h"

const map<string, string> Opcode::GPR = {
    {ADD, ADDQ},
    {SUB, SUBQ},
    {IMUL, IMULQ},
    {IDIV, IDIVQ},
    
    {SHL, SHLQ},
    {SHR, SHRQ}
};

const map<string, string> Opcode::SSE = {
    {ADD, ADDSS},
    {SUB, SUBSS},
    {IMUL, MULSS},
    {IDIV, DIVSS}
};
