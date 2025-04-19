#pragma once
#include "common.h"

#include "Scope.h"

class EnumTableEntry {
public:
    EnumTableEntry(const string & name) : name(name) {}

    void AddItem(string itemName, int itemValue);

    string GetEntryName() const;
    list<Variable> & GetEntryItems();

    void Print() const;

private:
    string name;
    Scope items;
};
