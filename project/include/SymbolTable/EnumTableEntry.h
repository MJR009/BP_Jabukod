/**
 * @file EnumTableEntry.h
 * @author Martin Jabůrek
 *
 * @brief Internal compiler representitation of a custom enum type.
 */

#pragma once
#include "common.h"

#include "Scope.h"

/**
 * @class EnumTableEntry
 * @brief An object of this class represents a single enum and all its items, which are considered
 * constant global variables.
 * 
 */
class EnumTableEntry {
public:
    /// @brief Initialise the enum by giving it a scope for all its items.
    EnumTableEntry(const string & name) : name(name) {
        this->items = new Scope();
    }

    /// @brief Adds a new item to the enum with the specified name and value.
    void AddItem(string itemName, int itemValue);

    /// @brief Returns the name if this enum.
    string GetEntryName() const;
    /// @brief Returns all items of this enum.
    list<Variable *> *GetEntryItems();

    /// @brief According to the provided name, returns an item, if it exists.
    Variable *GetItem(const string & name);

    /// @brief Prints this enums contents.
    void Print() const;

    /// @brief Deletes the enum and its associated scope.
    ~EnumTableEntry() {
        delete this->items;
    }

private:
    string name; ///< Name of the enum.
    Scope *items; ///< Scope containing all the enums items.
};
