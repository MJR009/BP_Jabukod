/**
 * @file FunctionTable.h
 * @author Martin Jabůrek
 *
 * @brief Storage for all the functions defined within an input program.
 */

#pragma once
#include "common.h"

#include "FunctionTableEntry.h"

/**
 * @class FunctionTable
 * @brief An object of this class contains all defined functions in the input program.
 * 
 */
class FunctionTable {
public:
    /// @brief Adds a new function to the table, creating it from the provided data.
    FunctionTableEntry *AddEntry(const string & name, const Type returnType);

    /// @brief Returns a function with the given name, if it exists.
    FunctionTableEntry *GetFunctionByName(const string & name);

    /// @brief Returns all functions in the table.
    list<FunctionTableEntry *> *GetFunctions();

    /// @brief Returns true, if a function with the given name does not yet exist.
    bool IsNameAvailable(const string & name) const;
    /// @brief Returns true, if a function does nbot have a parameter with the given name.
    bool IsParameterNameAvailable(const string & name, FunctionTableEntry *function) const;

    /// @brief Prints all the function tables contents.
    void Print() const;

    /// @brief Desctrutor, deleting all the functions at the end of the compilers runtime.
    ~FunctionTable() {
        for_each(this->functions.begin(), this->functions.end(),
            [ ](FunctionTableEntry *current) {
                delete current;
            }
        );
    }

private:
    list<FunctionTableEntry *> functions; ///< A list containing all the function, which can be iterated through.
};
