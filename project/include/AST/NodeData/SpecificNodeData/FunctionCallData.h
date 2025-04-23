/**
 * @file FunctionCallData.h
 * @author Martin Jabůrek
 *
 * @brief Data associated with a function call node.
 */

#pragma once
#include "common.h"

#include "GenericNodeData.h"
#include "FunctionTableEntry.h"

/**
 * @class FunctionCallData
 * @brief When a function call node is encoutered, the function being called is associated with it for easier access.
 */
class FunctionCallData : public GenericNodeData {
public:
    /**
     * @brief Stores the location of the function being called into the data.
     * 
     * Also stores an exists bool. As functions are allowed to have void return type, but, within
     * expressions, void is used to state a semantic error at a lower level, this bit is needed.
     * It is used to again print semantic errors more accurately, only on correctly defined functions.
     * 
     * @param function Address of the function to be stored.
     * @param exists Definedeness bit.
     */
    FunctionCallData(FunctionTableEntry *function, bool exists) : location(function), exists(exists) {}

    /**
     * @brief Retrieves the name of the called function.
     * 
     * @return The name of the functions.
     */
    string GetName();
    /**
     * @brief Retrieves the return type of the function.
     * 
     * @return The functions return type.
     */
    Type GetReturnType();

    /**
     * @brief Returns the data type of a specific parameter of the function.
     * 
     * By System V ABI, integer and floating point arguments are passed differently. This function is used to
     * resolve this.
     * 
     * @param order The order of the functions parameter from the left.
     * @return Data type of the parameter.
     */
    Type GetArgumentType(int order);
    /**
     * @brief Returns the actual address at which the argument should be stored.
     * 
     * The address is mostly System V ABI compliant. The returned string represents either a register
     * or a location on the stack.
     * 
     * @param order The order of the functions parameter from the left.
     * @return String representing where an argument at this order is to be stored during generated program runtime.
     */
    const string GetArgumentSlot(int order);

    /// @brief Differentiates void functions (true) and functions not defined during semantic analysis (false).
    bool exists;

private:
    /// @brief Address of the called function.
    FunctionTableEntry *location;
};
