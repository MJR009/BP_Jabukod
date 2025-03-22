#pragma once
#include "common.h"

#include "EnumTableEntry.h"

class EnumTable {
public:
    void AddEntry(const string & name);

    void Print();

private:
    vector<EnumTableEntry> enums;
};
