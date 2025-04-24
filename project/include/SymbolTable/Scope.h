/**
 * @file Scope.h
 * @author Martin Jabůrek
 *
 * @brief A container for variables defined at a specific part of the source program.
 */

#pragma once
#include "common.h"

#include "Variable.h"

/**
 * @class Scope
 * @brief Each variable used in a program is associated with a scope object. That on the other
 * hand is associated with the rest of the internal program representation.
 * 
 * For ease of access, since variables are always referenced from within a scope, they are stored
 * in a linked list. Like that they cannot be reallocated during compilation and their addresses can be used
 * safely as shortcuts to their data.
 * 
 */
class Scope {
public:
    /// @brief Adds a new variable to the scope.
    Variable *AddEntry(
        const string & name,
        const StorageSpecifier specifier,
        const Type type,
        const any & defaultValue,
        int stackOffset,
        bool isGlobal,
        bool isParameter
    );
    /// @brief Adds a new variable, that has be constructed earlier.
    void AddExistingEntry(Variable *variable);

    /// @brief Returns true, if the variable with the specified name is not within this scope.
    bool IsVariableNameAvailable(const string & name) const;

    /// @brief Returns the variable searched for by its name, if it exists.
    Variable *GetVariable(const string & name);
    /// @brief Returns all variables associated with this scope. Used for printing.
    list<Variable *> *GetVariables();

    /// @brief Prints all the variables within this scope.
    void PrintComplete() const;
    /// @brief Prints all the variables within this scope, but only as declarations.
    void PrintDeclarations() const;
    /// @brief Prints all the variables within this scope, but only as enum items.
    void PrintAsEnum() const;

    /// @brief Deletes the scope object, also deleting all variables within it.
    ~Scope() {
        for_each(this->variables.begin(), this->variables.end(),
            [ ](Variable *current) {
                delete current;
            }
        );
    }

public:
    /// @brief Removes all entries with static storage specifier from the scope.
    void PurgeStaticVariables();

private:
    list<Variable *> variables; ///< A list of all variables within this scope.
};
