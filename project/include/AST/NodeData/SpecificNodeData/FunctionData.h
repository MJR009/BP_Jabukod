/**
 * @file FunctionData.h
 * @author Martin Jabůrek
 *
 * @brief Data associated with a function definition node.
 */

#pragma once
#include "common.h"

#include "BodyData.h"
#include "FunctionTableEntry.h"

/**
 * @class FunctionData
 * @brief Is used to be associated with a function definition node, for quick access to that function.
 * 
 * The existence of the function is resolved When doing global symbol analysis.
 */
class FunctionData : public BodyData {
public:
    /**
     * @brief Associates the function that is currently defined.
     * 
     * @param function 
     */
    FunctionData(FunctionTableEntry *function) : location(function) {}

    /**
     * @brief Get the name of the function.
     * 
     * @return Function name.
     */
    string GetName();
    /**
     * @brief Retrieves how many variables are located within the function.
     * 
     * @return The variable count.
     */
    int GetVariableCount();

private:
    /// @brief Memory location of the defined function.
    FunctionTableEntry *location;
};
