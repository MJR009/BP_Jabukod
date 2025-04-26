/**
 * @file Opcodes.h
 * @author Martin Jabůrek
 *
 * @brief File containing macros of each instruction, that can be generated in the target code.
 */

#pragma once
#include "common.h"

/**
 * @defgroup stackManipulation Used stack manipulation instructions.
 * 
 * @{
 */
#define PUSH "push"
#define POP "pop"
#define CALL "call"
#define RET "ret"
/** @} */

/**
 * @defgroup dataTransfers Used data transfer instructions.
 * 
 * @{
 */
#define MOV "mov"
#define MOVQ "movq"
#define MOVSS "movss"
#define LEA "lea"
/** @} */

/**
 * @defgroup OS Used OS interaction instructions.
 * 
 * @{
 */
#define SYSCALL "syscall"
/** @} */

/**
 * @defgroup dataTypeConversions Used type conversion instructions.
 * 
 * @{
 */
#define CQO "cqo"
#define CVTSI2SS "cvtsi2ss"
#define CVTTSS2SI "cvttss2si"
/** @} */

/**
 * @defgroup conditionInstructions Used condition and coditional operation instructions.
 * 
 * @{
 */
#define TEST "test"
#define CMOVNZ "cmovnz"
#define JZ "jz"
#define CMP "cmp"
#define CMPB "cmpb" ///< Used with write system call.
#define COMISS "comiss" // "ucomiss"
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
#define CMOVLEQ "cmovleq"
#define CMOVGEQ "cmovgeq"
#define CMOVGQ "cmovgq"
#define CMOVLQ "cmovlq"
#define CMOVNEQ "cmovneq"
#define CMOVEQ "cmoveq"
#define CMOVBEQ "cmovbeq"
#define CMOVAEQ "cmovaeq"
#define CMOVAQ "cmovaq"
#define CMOVBQ "cmovbq"
/** @} */

/**
 * @defgroup compInstructuins Instruction used for general computation.
 * 
 * @{
 */
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
#define XORPS "xorps" // xorss does not exists

#define ANDi "and"
#define ANDQ "andq"

#define NEGQ "negq"

#define NOTQ "notq"

#define INCQ "incq"
/** @} */

/**
 * @class Opcode
 * @brief Static class used to determine what instruction operation code should be used according to operation data type.
 * 
 */
class Opcode {
public:
    static const map<string, string> GPR; ///< Returns operation to be used in general purpose registers.
    static const map<string, string> SSE; ///< Returns operation to be used in floating point registers.
};
