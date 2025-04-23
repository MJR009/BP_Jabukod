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
 * @brief Instances of this class are use to hold a pointer to an existing variable, so it can
 * be easily accessed whenever it's data needs to be resolved.
 * 
 */
class VariableData : public GenericNodeData {
public:
    /**
     * @brief Constructs this data object and associated a specific variable to it.
     * 
     * The variable associated is a member of a scope. Relocations within the scope are safe, as
     * it is implemented with a linked list, which does not relocate its data unlike a vector for example.
     * 
     * @param variable Address of the associated variable.
     */
    VariableData(Variable *variable) : location(variable) {}

    /**
     * @brief Retrieve the variable itself.
     * 
     * This is needed when data of the associated variable needs to be adjusted or changed.
     * 
     * @return The associated variable.
     */
    Variable *GetSelf();

    /**
     * @brief Get the variables data type.
     * 
     * @return Data type of the associated variable.
     */
    Type GetType();
    /**
     * @brief Get the variables name.
     * 
     * @return Name of the associated variable.
     */
    string GetName();
    /**
     * @brief Get the storage specifier of this objects associated variable.
     * 
     * @return Variables storage specifier.
     */
    StorageSpecifier GetSpecifier();

    /**
     * @brief Generic method, which returns the variables value cast to the desired type.
     * 
     * The variable may not have an associated value. This function should only be used on global or static variables.
     * 
     * @tparam Desired type of the variables value.
     * @return The variables value.
     */
    template <typename T>
    T GetDefaultValue() {
        if (this->location) {
            return this->location->GetDefaultValue<T>();
        }
        ERR::BadData();
        return T{};
    }
    /**
     * @brief Returns the value of the variable as the actual internal any object.
     * 
     * This is usefully for example in static variable resolutions.
     * 
     * @return The variables value.
     */
    any GetActualDefaultValue();

    /**
     * @brief Returns the offset at which the variable should be stored on the stack during
     * generated programs runtime.
     * 
     * The result of this method is not valid for global and static variables, as they are not stored on the stack.
     * 
     * @return %rbp relative offset of this variable.
     */
    int GetStackOffset();

    /// @brief Returns whether the variable is global. 
    bool IsGlobal();
    /// @brief Returns whether the variable is actually a function parameter. 
    bool IsParameter();
    /// @brief Returns the memory location (register or stack offset) where the associated varaible is to be stored during runtime. 
    string GetParamaterLocation();

    /**
     * @brief Updates the variables name.
     * 
     * @param name The new name for the variable.
     */
    void SetName(const string & name);

private:
    /// @brief Memory location of the associated variable.
    Variable *location;
};
