#pragma once
#include "common.h"

#define PUSH "push"
#define POP "pop"
#define RET "ret"

#define MOV "mov"
#define MOVQ "movq"
#define MOVSS "movss"
#define LEA "lea"

#define CALL "call"
#define SYSCALL "syscall"

#define CQO "cqo"
#define CVTSI2SS "cvtsi2ss"
#define CVTTSS2SI "cvttss2si"

#define TEST "test"
#define CMOVNZ "cmovnz"
#define CMP "cmp"
#define COMISS "comiss"
#define JMP "jmp"
#define JLE "jle"
#define JGE "jge"
#define JG "jg"
#define JL "jl"
#define JNE "jne"
#define JE "je"
#define JBE "jbe"
#define JAE "jae"
#define JA "ja"
#define JB "jb"

#define ADD "add"
#define ADDQ "addq"
#define ADDSS "addss"

#define SUB "sub"
#define SUBQ "subq"
#define SUBSS "subss"

#define IMUL "imul"
#define IMULQ "imulq"
#define MULSS "mulss"

#define IDIV "idiv"
#define IDIVQ "idivq"
#define DIVSS "divss"

#define SHL "shl"
#define SHLQ "shlq"

#define SHR "shr"
#define SHRQ "shrq"

#define ORi "or"
#define ORQ "orq"
#define ORSS "orss"

#define XOR "xor"
#define XORQ "xorq"

#define ANDi "and"
#define ANDQ "andq"
#define ANDSS "andss"

class Opcode {
public:
    static const map<string, string> GPR;
    static const map<string, string> SSE;
};
