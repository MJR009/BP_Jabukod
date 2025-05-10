/**
 * @file Snippets.h
 * @author Martin Jabůrek
 *
 * @brief Reusable intermediate code sequences.
 */

#pragma once
#include "common.h"

#include "Instruction.h"
#include "Transform.h"
#include "ASTNode.h"
#include "SymbolTable.h"

/**
 * @class Snippets
 * @brief Some code sequences are needed often. This static class makes it easier to generate them repeatedly.
 * 
 */
class Snippets {
public:
    /**
     * @defgroup floatLiterals Names of different literals possibly needed in programs, which are always the same.
     * 
     * @{
     */
    static const string floatDeclaration;
    static const string stringDeclaration;
    static const string signMask;
    static const string bitNotMask;
    static const string notMask;
    /** @} */

public:
    /// @brief Generates a function prologue sequence. 
    static const vector<Instruction> Prolog(int bytesToReserve);
    /// @brief Generates a function epilogue sequence.
    static const vector<Instruction> Epilog();
    /// @brief Generates an exit syscall, using exit code from the provided register,
    static const vector<Instruction> Exit(string reg, bool useRDTSC);

    /// @brief Initializes the target memory location to its default value according to the data type. 
    static const vector<Instruction> DeclareDefault(Type type, string target);

    /// @brief Stores a register on the stack.
    static const vector<Instruction> PushRegister(Type type, string reg);
    /// @brief Puts a variable into a register from the top of the stack and pops.
    static const vector<Instruction> PopRegister(Type type, string reg);

    /// @brief Pushes all scratch registers to the stack. Used with write system call.
    static const vector<Instruction> BackupScratchRegisters();
    /// @brief Restores all scratch registers from the stack in reverse order. Used with write system call.
    static const vector<Instruction> RestoreScratchRegisters();

    /// @brief Evaluetes the length of a string for a write system call. The lenght of a string in %rsi is stored into %rdx.
    static const vector<Instruction> CalculateStringLength();
};
