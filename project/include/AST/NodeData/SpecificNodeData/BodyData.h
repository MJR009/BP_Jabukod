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
 * Other specialisations are used for more specific circumstances. This class is used for any code block.
 * That means if, if-else, while, for and foreach statement bodies.
 */
class BodyData : public GenericNodeData {
public:
    /// @brief Constructor of a body. Creates the associated scope.
    BodyData() {
        this->scope = new Scope();
    }

    /**
     * @brief From the provided information, adds a new variable to the associated scope.
     * 
     * The new variable is considered local. No default value is given (representded by an empty any object).
     * 
     * @param name Name of the new variable.
     * @param specifier Storage specifier of the new variable.
     * @param type Data type of the new variable.
     * @param stackOffset Potential offset from %rbp register to be used to access the variable in the geerated assembly.
     * @return Memory location of the newly created variable.
     */
    Variable *AddVariable(
        const string & name,
        StorageSpecifier specifier,
        Type type,
        int stackOffset
    );
    /**
     * @brief Return true if the desired variable name is available. If not, returns false.
     * 
     * @param name The desired name of the new variable.
     */
    bool IsVariableNameAvailable(const string & name) const;
    /**
     * @brief Finds and returns a variable with the provided name located within the associaed scope.
     * 
     * @param name Name of the looked up variable.
     * @return Memory location of the desired variable.
     */
    Variable *GetVariable(const string & name);

    /// @brief Removes all variables with static storage specifier from the scope.
    void RemoveStaticVariables();

    /// @brief Prints all variables within the scope as declarations.
    void PrintScope();

    /// @brief When destroyed deletes the associated scope.
    ~BodyData() {
        delete this->scope;
    }

protected:
    /// @brief The associated internal scope object. Protected used for inheriting by derived classes.
    Scope *scope;
};
