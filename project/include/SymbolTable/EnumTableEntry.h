#pragma once
#include "common.h"

#include "EnumItem.h"

class EnumTableEntry {
public:
    EnumTableEntry(const string & name) : name(name) {}

    void AddItem(string itemName, int itemValue);

    string GetEntryName();
    vector<EnumItem> GetEntryItems();

    void Print();

private:
    string name;
    vector<EnumItem> items;
};
