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
 * @brief Data associated with a function definition node for quick access to that function.
 * 
 * The existence of the function is resolved When doing global symbol analysis.
 */
class FunctionData : public BodyData {
public:
    /// @brief Associates the function that is currently defined.
    FunctionData(FunctionTableEntry *function) : location(function) {}

    /// @brief Gets the name of the function.
    string GetName();
    /// @brief Retrieves how many bytes the associated function needs to allocate for its local variables on stack.
    int GetNeededStackSpace();

    /// @brief Returns how many parameters the associated function has.
    int GetParameterCount();
    /// @brief Returns the location of the associated functions parameter at the specified order.
    const string GetParameterSlot(int order);
    /// @brief Returns the data type of a parameter at the spefied order of the associated function.
    Type GetParameterType(int order);

private:
    FunctionTableEntry *location; ///< Reference to the actual function.
};
