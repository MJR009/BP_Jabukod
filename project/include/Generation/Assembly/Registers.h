/**
 * @file Registers.h
 * @author Martin Jabůrek
 *
 * @brief File containing macros of x86 and SSE registers used in generated programs.
 */

#pragma once
#include "common.h"

/**
 * @defgroup presevedRegisters Used preserved registers.
 * 
 * @{
 */
#define RBX "%rbx" ///< operand
#define RSP "%rsp"
#define RBP "%rbp"
#define R12 "%r12" ///< index of the control variable inside a foreach loop
#define R13 "%r13"
#define R14 "%r14"
#define R15 "%r15"
/** @} */

/**
 * @defgroup scratchRegisters Used scratch registers.
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

/// @brief hard set to 1 at the start of each program, used for bool conversions, as conditional move instructions do not work with immediate values
#define R10 "%r10"
#define R11 "%r11"
/** @} */

/**
 * @defgroup bitShiftRegister Register parts needed for bit shift operations.
 * 
 * @{
 */
#define CL "%cl"
#define BL "%bl"
/** @} */

/**
 * @defgroup sseRegisters Used floating point SSE scratch registers.
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

/**
 * @brief ripRegister instruction pointer register.
 * 
 * @{
 */
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
