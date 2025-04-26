/**
 * @file Transform.h
 * @author Martin Jabůrek
 *
 * @brief Creates valid instruction components from internal compiler values.
 */

#pragma once
#include "common.h"

#include "ASTNode.h"
#include "Instruction.h"
#include "Registers.h"

/**
 * @class Transform
 * @brief To comply with the target assembly form, internal represantiation is adjusted to reflect it.
 * 
 */
class Transform {
public:
    /**
     * @name Labels
     * 
     * @{ 
     */
    static bool IsLabel(Instruction & instruction); ///< Returns true if given instruction is a label.
    static string IdentifierToLabel(const string & name); ///< Transforms a string to a correct label.
    /** @} */

    /**
     * @name .data and .rodata sections
     * 
     * @{
     */
    static string TypeToDirective(Type type); ///< Transforms a data type to its respective data directive.
    static string DefaultValueToInitializer(Variable *variable); ///< Transforms a value to how it is represented in the generated program.
    /** @} */

    /**
     * @name .text section
     * 
     * @{
     */
    static bool IsRegister(const string & memory); ///< Returns true, if the given string contains a register.
    static string GlobalToAddress(const string & variableName); ///< Transforms a global variable to its memory location.
    static string RegisterToAddress(const string & reg); ///< Transforms a register to address for loading an effective address of the stored value.
    static string IntToImmediate(const int & number); ///< Transforms an integer into an immediate value of the instruction.
    static string LiteralToImmediate(LiteralData *data); ///< Makes an immediate value straight from the literal data.
    static string VariableToLocation(VariableData *data); ///< Returns access to either a register or memory.
    static string ListAccessToLocation(Variable *array, string indexRegister = RAX); ///< Returns access to memory with indexed addressing.
    static string ConditionToJump(NodeKind condition, Type comparisonType); ///< Returns the jump instruction used with a specific condition.
    static string ConditionToCMove(NodeKind condition, Type comparisonType); ///< Returns the conditional move instruction used with a specific condition.
    /** @} */
};
