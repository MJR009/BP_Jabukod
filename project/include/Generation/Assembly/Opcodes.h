#pragma once
#include "common.h"

#define PUSH "push"
#define POP "pop"
#define RET "ret"

#define MOV "mov"
#define MOVQ "movq"
#define MOVSS "movss"
#define LEA "lea"

#define SYSCALL "syscall"

#define ADD "add"
#define ADDQ "addq"
#define ADDSS "addss"

#define SUB "sub"
#define SUBQ "subq"
#define SUBSS "subss"

class Opcode {
public:
    static const map<string, string> GPR;
    static const map<string, string> SSE;
};
