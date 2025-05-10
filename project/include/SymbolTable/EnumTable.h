/**
 * @file EnumTable.h
 * @author Martin Jabůrek
 *
 * @brief Used to contain all defined enum types in the source program.
 */

#pragma once
#include "common.h"

#include "EnumTableEntry.h"

/**
 * @class EnumTable
 * @brief During compilation, addresses of all enum types are stored here.
 * 
 */
class EnumTable {
public:
    /// @brief Adds a new enum with the specified name. 
    EnumTableEntry *AddEntry(const string & name);

    /// @brief Retrieves an enum with the given name, if it exists.
    EnumTableEntry *GetEntryByName(const string & name);
    /// @brief Returns all stored enums.
    list<EnumTableEntry *> *GetEnums();

    /// @brief Finds an enum item located in any of the defined enums.
    Variable *GetItemFromAcrossAll(const string & name);

    /// @brief Returns true if the provided name is currently not being used for an enum.
    bool IsNameAvailable(const string & name) const;
    /// @brief Returns true if the provided name is not used for any enum item across all enums.
    bool IsItemNameAvailableAcrossAll(const string & name) const;
    /// @brief Returns true if the provided enum does not contain the item with the provided name.
    bool IsItemNameAvailable(const string & name, EnumTableEntry *theEnum) const;
    /// @brief Returns true if the provided enum does not contain the item with the provided value. Values must not be repeated across enum items.
    bool IsItemValueAvailable(const int & value, EnumTableEntry *theEnum) const;

    /// @brief Prints the contents of the enum table.
    void Print() const;

    /// @brief Destructor removing all dynamically allocated enums.
    ~EnumTable() {
        for_each(this->enums.begin(), this->enums.end(),
            [ ](EnumTableEntry *current) {
                delete current;
            }
        );
    }

private:
    list<EnumTableEntry *> enums; ///< A list of all defined enums, which can be iterated through.
};
