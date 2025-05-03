/**
 * @file Instruction.h
 * @author Martin Jabůrek
 *
 * @brief Internal representation of one instruction.
 */

#pragma once
#include "common.h"

#include "Opcodes.h"
#include "Registers.h"
#include "System.h"

/**
 * @class Instruction
 * @brief Represents one intermediate, three address code instruction, that is later
 * transformed into the target code.
 * 
 */
class Instruction {
public:
    /// @brief Constructor, storing all instruction parts.
    Instruction(string opcode, string arg1 = "", string arg2 = "", string arg3 = "") :
        opcode(opcode), arg1(arg1), arg2(arg2), arg3(arg3)
    {}

    /// @brief Prints out the instruction to the given stream.
    void Output(ofstream & file);

    /// @brief Returns instructions operation code.
    string GetOpcode();

    /// @brief Appends vector2 to vector1, changing vector1
    static void ConnectSequences(vector<Instruction> & vector1, const vector<Instruction> & vector2);

    /// @brief Helper function adding a comment after an instruction into arg3, which is otherwise unused
    void AddComment(string comment);

private:
    string opcode; ///< Instructions operation code.
    string arg1; ///< First argument, typically source address.
    string arg2; ///< Second argument typically destination address.
    string arg3; ///< Third argument, a.
};
