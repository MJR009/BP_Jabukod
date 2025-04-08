#pragma once

// uses System V ABI
// https://wiki.osdev.org/System_V_ABI

// PRESERVED
#define RBX "%rbx"
#define RSP "%rsp"
#define RBP "%rbp"
#define R12 "%r12"
#define R13 "%r13"
#define R14 "%r14"
#define R15 "%r15"

// SCRATCH
#define RAX "%rax" // return value
#define RDI "%rdi"
#define RSI "%rsi"
#define RDX "%rdx"
#define RCX "%rcx"
#define R8 "%r8"
#define R9 "%r9"
#define R10 "%r10"
#define R11 "%r11"

// FLOATING POINT, SCRATCH
#define XMM0 "%xmm0"
#define XMM1 "%xmm1"
#define XMM2 "%xmm2"
#define XMM3 "%xmm3"
#define XMM4 "%xmm4"
#define XMM5 "%xmm5"
#define XMM6 "%xmm6"
#define XMM7 "%xmm7"
