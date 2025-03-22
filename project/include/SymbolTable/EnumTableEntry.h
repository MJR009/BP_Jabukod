#pragma once
#include "common.h"

class EnumTableEntry {
public:
    EnumTableEntry(const string & name) : name(name) {}

    void Print();

private:
    string name;
};
