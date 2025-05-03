/**
 * @file Opcodes.cpp
 * @author Martin Jabůrek
 * 
 * Implementation of
 * @link Opcodes.h
 */

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



bool Opcode::IsJump(string opcode) {
    vector<string> jumps = {
        CALL, RET,
        JZ, JMP, JLE, JGE, JG, JL, JNE, JE, JBE, JAE, JA, JB
    };

    auto position = find(jumps.begin(), jumps.end(), opcode);

    if (position != jumps.end()) {
        return true;
    }

    return false;
}
