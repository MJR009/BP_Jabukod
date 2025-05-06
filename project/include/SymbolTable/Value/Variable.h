/**
 * @file Variable.h
 * @author Martin Jabůrek
 *
 * @brief Internal representation of the generated programs runtime values.
 */

#pragma once
#include "common.h"

#include "BaseValue.h"
#include "Type.h"
#include "Specifier.h"
#include "Registers.h" // for paramaters

/**
 * @class Variable
 * @brief Class used to represent all values during runtime, their types, storage and all other behaviour.
 * 
 */
class Variable : public BaseValue {
public:
    /// @brief According to the provided data, a new variable is constructed.
    Variable(
        const string & name,
        const StorageSpecifier storage,
        const Type type,
        const any & defaultValue,
        int stackOffset,
        bool isGlobal,
        bool isParameter,
        string parameterStorage = ""
    ) :
        BaseValue(name),
        storage(storage),
        type(type),
        defaultValue(defaultValue),
        stackOffset(stackOffset),
        isGlobal(isGlobal),
        isParameter(isParameter),
        parameterStorage(parameterStorage)
    {}

    /// @brief Returns a variables data type.
    Type GetType();
    /// @brief Returns a variables storage specifier.
    StorageSpecifier GetSpecifier();
    /// @brief Generic method to return variables value according to the specified type.
    template <typename T>
    T GetDefaultValue() const {
        return any_cast<T>( this->defaultValue );
    }
    /// @brief Returns the default value as the actualy internal "any" object
    any GetActualDefaultValue();

    /// @brief Returns the variables location on stack, if it is local.
    int GetStackOffset();
    /// @brief Returns the order of the variable, if it is a parameter.
    int GetParameterOrder();

    /// @brief If true, the variable is global, otherwise it is local or a parameter.
    bool IsGlobal();
    /// @brief  If true, the variable is actually a parameter.
    bool IsParameter();
    /// @brief If the variable is a parameter, returns where it is located for access as an argument.
    string GetParameterLocation();

    /// @brief Makes the variable global.
    void SetGlobalFlag();

    /// @brief Changes the variables name.
    void SetName(const string & name);
    /// @brief Sets default value. Important mainly with static variables and array restructuring obfuscation.
    void SetDefaultValue(any value);
    /// @brief Gives variable a new type. Used for array restructuring.
    void SetType(Type type);

    /// @brief Gives the variable all data needed to act as a foreach control variable.
    void MakeForeachControlVariable(Variable *iteratedArray);
    /// @brief Returns true, if this variable is set as a foreach variable.
    bool IsControlVariable();
    /// @brief Returns the array variable that foreach control variable iterates over, if this is one.
    Variable *GetIteratedArray();

    /// @brief Prints all data about the variable
    void Print() const override;
    /// @brief Prints the variable as local.
    void PrintDeclaration() const;
    /// @brief Prints the variable as a parameter.
    void PrintAsParameter() const;
    
    /// @brief Prints the variable associated default value.
    void PrintDefaultValue() const;

private:
    StorageSpecifier storage; ///< Variables storage specifier.
    Type type; ///< Variables data type.
    any defaultValue; ///< Optional default value, associated with global and static variables.

    /**
     * @brief Value used to determine its location on stack.
     * 
     * The exact offset if it is local.
     * An order from the left, if it is a parameter.
     */
    int stackOffset;

    bool isGlobal; ///< Global flag.

    bool isParameter; ///< Parameter flag.
    string parameterStorage; ///< If the variable is a parameter, this string contains where it is stored.

    bool isForeachControlVariable = false; ///< foreach loop control variable acts as a kind of fake variable, the array needs to be accessed.
    Variable *iteratedArray = nullptr; ///< foreach loop control variable will iterate over this arrray.

public: // obfuscation
    /**
     * @brief The varaible (an array) is marked for restructuring.
     * 
     * Restructuring must actually be marked inside the variable. Without this, foreach control variables
     * have no way of changing their generated index when accessing the array. Usual array access with []
     * is done withing AST before transforming to 3AC.
     */
    bool restructure = false;

    /// @brief Adjusts the variables stack offset to accomodate for array restructuring.
    void SetStackOffset(int newOffset);
};
