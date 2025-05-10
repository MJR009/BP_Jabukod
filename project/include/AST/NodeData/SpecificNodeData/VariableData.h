/**
 * @file VariableData.h
 * @author Martin Jabůrek
 *
 * @brief Data to be associated with a node representing a variable.
 */

#pragma once
#include "common.h"

#include "GenericNodeData.h"
#include "Variable.h"

/**
 * @class VariableData
 * @brief Instances of this class are used to hold a pointer to an existing variable, so it can
 * be easily accessed whenever its data is needed.
 * 
 */
class VariableData : public GenericNodeData {
public:
    /**
     * @brief Constructs this data object and associates a specific variable to it.
     * 
     * The variable associated is a member of a scope. Relocations within the scope are safe, as
     * it is implemented with a linked list, which does not relocate its data.
     */
    VariableData(Variable *variable) : location(variable) {}

    /// @brief Retrieve the variable itself.
    Variable *GetSelf();

    /// @brief Get the variable data type.
    Type GetType();
    /// @brief Get the variable name.
    string GetName();
    /// @brief Get the storage specifier of this objects associated variable.
    StorageSpecifier GetSpecifier();

    /// @brief Generic method, which returns the variables value cast to the desired type.
    template <typename T>
    T GetDefaultValue() {
        if (this->location) {
            return this->location->GetDefaultValue<T>();
        }
        ERR::BadData();
        return T{};
    }

    /// @brief Returns the value of the variable as the actual internal "any" object.
    any GetActualDefaultValue();

    /// @brief Returns the offset at which the variable should be stored on the stack during generated programs runtime, if it is local.
    int GetStackOffset();
    /// @brief Returns the order in functions parameter list, if the variable is a parameter.
    int GetParameterOrder();

    /// @brief Returns whether the variable is global. 
    bool IsGlobal();
    /// @brief Returns whether the variable is a function parameter. 
    bool IsParameter();
    /// @brief Returns the memory location (register or %rbp relative stack offset) where the associated varaible is to be stored during runtime. 
    string GetParamaterLocation();
    /// @brief Returns true, if the variable generated is marked as a foreach iterator.
    bool IsForeachControlVariable();

    /// @brief Updates the variables name.
    void SetName(const string & name);

private:
    Variable *location; ///< Memory location of the associated variable.
};
