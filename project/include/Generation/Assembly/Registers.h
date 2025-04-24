/**
 * @file Registers.h
 * @author Martin Jabůrek
 *
 * @brief File containing macros of x86 and SSE registers used in generated programs.
 */

#pragma once
#include "common.h"

/**
 * @name Preseved registers.
 * 
 * @{
 */
#define RBX "%rbx" ///< operand
#define RSP "%rsp"
#define RBP "%rbp"
#define R12 "%r12"
#define R13 "%r13"
#define R14 "%r14"
#define R15 "%r15"
/** @} */

/**
 * @name Scratch registers.
 * 
 * @{
 */
#define RAX "%rax" ///< return value, operand
#define RDI "%rdi" ///< function argument 1
#define RSI "%rsi" ///< function argument 2
#define RDX "%rdx" ///< function argument 3
#define RCX "%rcx" ///< function argument 4
#define R8 "%r8" ///< function argument 5
#define R9 "%r9" ///< function argument 6
#define R10 "%r10"
#define R11 "%r11"
/** @} */

/**
 * @name Registers needed for bit shift operations.
 * 
 * @{
 */
#define CL "%cl"
#define BL "%bl"
/** @} */

/**
 * @name Floating point SSE scratch registers.
 * 
 * @{
 */
#define XMM0 "%xmm0" ///< function argument 1
#define XMM1 "%xmm1" ///< function argument 2
#define XMM2 "%xmm2" ///< function argument 3
#define XMM3 "%xmm3" ///< function argument 4
#define XMM4 "%xmm4" ///< function argument 5
#define XMM5 "%xmm5" ///< function argument 6
#define XMM6 "%xmm6" ///< operand
#define XMM7 "%xmm7" ///< operand
/** @} */

#define RIP "%rip"
/** @} */

/**
 * @class Registers
 * @brief Static class for retrieving what location is a function argument stored.
 * 
 */
class Registers {
public:
    static string NormalParameter(int order); ///< Returns what memory location are general arguments stored.
    static string FloatParameter(int order); ///< Returns what memory location are floating point arguments stored.
};
