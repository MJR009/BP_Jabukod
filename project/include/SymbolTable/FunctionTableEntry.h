/**
 * @file FunctionTableEntry.h
 * @author Martin Jabůrek
 *
 * @brief A representation of one function from the compiled program.
 */

#pragma once
#include "common.h"

#include "Variable.h"

/**
 * @class FunctionTableEntry
 * @brief This class represents all data needed to define a function. The data is encapsulated and
 * can be accessed or modified with the provided methods.
 * 
 */
class FunctionTableEntry {
public:
    /// @brief Creates a new function according to the provided data.
    FunctionTableEntry(const string & name, const Type returnType) : name(name), returnType(returnType) {}

    /// @brief Gives the function a new parameter. It is added as the last one.
    void AddParameter(const Type parameterType, const string & parameterName);

    /// @brief Returns the name of the function.
    string GetFunctionName() const;
    /// @brief Returns the return type of the function.
    Type GetReturnType() const;

    /// @brief Sets the function name according to parameter.
    void SetFunctionName(const string & name);

    /// @brief Returns how many parameters this function has.
    int GetParameterCount();
    /// @brief Return all the parameters of the function.
    list<Variable *> *GetParameters();
    /// @brief If the function has a parameter with the given name, it is returned. Otherwise nullptr is returned.
    Variable *GetParameter(const string & name);

    /// @brief Returns at what memory location will the parameter at the given order be stored during runtime.
    const string GetParameterSlot(int order);
    /// @brief Returns the data type of parameter with the specifed order.
    Type GetParameterType(int order);

    /// @brief Sets the number of variables within this function to a new value.
    void SetVariableCount(int variableCount);
    /// @brief Returns how many variables are currently located within this function.
    int GetVariableCount();
    /// @brief Sets mow many bytes are needed to be allocated on stack at the start of this function.
    void SetVariableStackSpace(int variableStackSpace);
    /// @brief Returns how many bytes this function needs to have allocated on stack at its start. 
    int GetVariableStackSpace();

    /// @brief Prints out all data regarding this function.
    void Print() const;

    /// @brief Deletes this function and all its parameters.
    ~FunctionTableEntry() {
        for_each(this->parameters.begin(), this->parameters.end(),
            [ ](Variable *current) {
                delete current;
            }
        );
    }

private:
    string name; ///< The name of the function.
    Type returnType; ///< The return type of the function.
    list<Variable *> parameters; ///< A list of all this functions parameters.

    int parameterCount = 0; ///< How many parameters does this function have.

    int variableCount = 0; ///< How many variables are defined within this function.
    int variableStackSpace = 0; ///< How much stack space is needed to allocate for variables.

private:
    /// @brief Returns a string representing the memory location that needs to be used for the next parameter.
    string AllocateNewParameter(Type type);
    int nextStackOffset = 16; ///< Stack offset at which a new parameter should be allocated.
    int floatCount = 0; ///< Returns how many parameters of type float does this function have.
    int nonFloatCount = 0; ///< Returns how many parameters of a non-float type does this function have.
};
