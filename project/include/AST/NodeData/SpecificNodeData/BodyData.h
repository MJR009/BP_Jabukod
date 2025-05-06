/**
 * @file BodyData.h
 * @author Martin Jabůrek
 *
 * @brief Data associated with scope having bodies within the abstract syntax tree.
 */

#pragma once
#include "common.h"

#include "GenericNodeData.h"
#include "Scope.h"

/**
 * @class BodyData
 * @brief This class is used to hold and access a scope of variables associated with a specific body.
 * 
 * Other specialisations are used in more specific circumstances. This class is used for any code block.
 * That means if, if-else, while, for and foreach statement bodies.
 */
class BodyData : public GenericNodeData {
public:
    /// @brief Constructor of a body. Creates the associated scope.
    BodyData() {
        this->scope = new Scope();
    }

    /// @brief From the provided information, adds a new variable to the associated scope.
    Variable *AddVariable(
        const string & name,
        StorageSpecifier specifier,
        Type type,
        int stackOffset
    );
    /// @brief Return true if the desired variable name is available. If not, returns false.
    bool IsVariableNameAvailable(const string & name) const;
    /// @brief Finds and returns a variable with the provided name located within the associated scope.
    Variable *GetVariable(const string & name);

    /// @brief Returns all variables withing the associated scope. 
    list<Variable *> *GetVariables();

    /// @brief Removes all variables with static storage specifier from the scope.
    void RemoveStaticVariables();

    /// @brief Prints all variables within the scope as declarations.
    void PrintScope();

    /// @brief When destroyed deletes the associated scope.
    ~BodyData() {
        delete this->scope;
    }

public: // used with obfuscation
    /// @brief Returns a variable, if there is one in the scope; the caller does not need to know what variable it is, just that it is in this scope
    Variable *GetVariableForOpaquePredicate();
    /// @brief Shifts all variables stack offsets to accomodate for changed array size.
    void AdjustForRestructuring(int stackOffsetOfRestructured, int offsetAdjustment);

protected:
    Scope *scope; ///< The associated internal scope object. Protected used for inheriting by derived classes.
};
