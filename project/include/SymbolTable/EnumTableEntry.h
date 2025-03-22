#pragma once
#include "common.h"

class EnumTableEntry {
public:
    EnumTableEntry(const string & name) : name(name) {}

    string GetEntryName();

    void Print();

private:
    string name;
};
