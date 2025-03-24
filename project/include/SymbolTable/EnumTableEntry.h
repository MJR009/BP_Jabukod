#pragma once
#include "common.h"

#include "EnumItem.h"

class EnumTableEntry {
public:
    EnumTableEntry(const string & name) : name(name) {}

    void AddItem(string itemName, int itemValue);

    string GetEntryName() const;
    vector<EnumItem> GetEntryItems() const;

    void Print() const;

private:
    string name;
    vector<EnumItem> items;
};
