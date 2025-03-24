#pragma once
#include "common.h"

#include "EnumTableEntry.h"

class EnumTable {
public:
    EnumTableEntry *AddEntry(const string & name);

    EnumTableEntry *GetEntryByName(const string & name);

    bool IsNameAvailable(const string & name);
    bool IsItemNameAvailable(const string & name, EnumTableEntry *theEnum);
    bool IsItemValueAvailable(const int & value, EnumTableEntry *theEnum);

    void Print();

private:
    vector<EnumTableEntry> enums;
};
