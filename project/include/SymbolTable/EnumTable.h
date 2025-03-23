#pragma once
#include "common.h"

#include "EnumTableEntry.h"

class EnumTable {
public:
    EnumTableEntry *AddEntry(const string & name);

    bool IsIdTaken(const string & name);
    bool IsItemValueAvailable(const int & value, EnumTableEntry *theEnum);

    void Print();

private:
    vector<EnumTableEntry> enums;
};
