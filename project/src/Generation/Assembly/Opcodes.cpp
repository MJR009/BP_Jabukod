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
    const vector<string> jumps = {
        JZ, JMP, JLE, JGE, JG, JL, JNE, JE, JBE, JAE, JA, JB
    };

    auto position = find(jumps.begin(), jumps.end(), opcode);

    if (position != jumps.end()) {
        return true;
    }

    return false;
}

const string Opcode::FlipJumpSign(string opcode) {
    const map<string, string> jumps = {
        {JZ, JZ},
        {JMP, JMP},
        {JLE, JBE},
        {JGE, JAE},
        {JG, JA},
        {JL, JB},
        {JNE, JNE},
        {JE, JE},
        {JBE, JLE},
        {JAE, JGE},
        {JA, JG},
        {JB, JL}
    };

    return jumps.at(opcode);
}
