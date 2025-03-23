#pragma once
#include "common.h"

class EnumTableEntry {
public:
    EnumTableEntry(const string & name) : name(name) {}

    void AddItem(string itemName, int itemValue);

    string GetEntryName();
    vector<pair<string, int>> GetEntryItems();

    void Print();

private:
    string name;
    vector<pair<string, int>> items;
};
